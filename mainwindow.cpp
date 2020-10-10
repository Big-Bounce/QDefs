#include "mainwindow.h"
#include <QDockWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMenuBar>
#include <QApplication>
#include <QTextStream>
#include <cassert>
#include <QHeaderView>
#include <QLabel>
#include <QCheckBox>
#include <QToolBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QShortcut>
#include "delimiterWidget.h"
#include <QtGlobal>
#include <QGridLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      _varsTable(new QTableWidget),
      _codesTable(new QTableWidget),
      _mainDefs(new QTextEdit),
      _helpDefs(new QTextEdit),
      _current_range(1,0),
      _infos(delimiter("_", 0), delimiter("_", 1), delimiter(":", 0), delimiter(":", 1), 1, false, true, false),
      _searchingLine(new QLineEdit),
      _blocksSpinBox(new QSpinBox),
      _clipboard(QGuiApplication::clipboard())
{

    QMenu* menu;
    QAction* action;
    QCheckBox* checkBox;
    QLabel* label;
    QHeaderView* headerView;
    QHBoxLayout* hBoxLayout;
    QVBoxLayout* vBoxLayout;
    QPushButton* pushButton;
    QToolBar* toolBar;
    QFrame* frame;
    delimiterWidget* separator;
    QGridLayout* gridLayout;

    //MENUBAR ===================================================================
    setMenuBar(new QMenuBar);

    menu = menuBar() -> addMenu(tr("Program"));
    action = menu -> addAction(tr("Wczytaj słownik"));
    connect(action, SIGNAL(triggered()), this, SLOT(load()));
    action = menu -> addAction(tr("Wyjdź"));
    connect(action, SIGNAL(triggered()), QApplication::instance(), SLOT(quit()));

    //TOOLBARS ==================================================================

    //top toolbar ===============================================================
    toolBar = new QToolBar(this);

    //append mode
    checkBox = new QCheckBox(tr("Dopisuj definicje"));
    checkBox -> setChecked(_infos.append_mode);
    connect(checkBox, SIGNAL(toggled(bool)), this, SLOT(append_mode(bool)));
    toolBar -> addWidget(checkBox);
    toolBar -> addSeparator();

    //create files
    checkBox = new QCheckBox(tr("Twórz pliki"));    
    checkBox -> setChecked(_infos.create_files);
    connect(checkBox, SIGNAL(toggled(bool)), this, SLOT(create_files(bool)));
    toolBar -> addWidget(checkBox);
    toolBar -> addSeparator();

    //blocks mode
    QCheckBox* blocksBox = new QCheckBox(tr("Bloki zmiennych"));
    blocksBox -> setChecked(_infos.blocks_mode);
    connect(blocksBox, SIGNAL(toggled(bool)), this, SLOT(blocks_mode(bool)));
    toolBar -> addWidget(blocksBox);

    Q_ASSERT(_infos.block_size >= 1);
    _blocksSpinBox -> setMinimum(1);
    _blocksSpinBox -> setValue(_infos.block_size);
    _blocksSpinBox -> setEnabled(_infos.blocks_mode);
    connect(blocksBox, SIGNAL(toggled(bool)), _blocksSpinBox, SLOT(setEnabled(bool)));
    connect(_blocksSpinBox, SIGNAL(valueChanged(int)), this, SLOT(block_size(int)));
    toolBar -> addWidget(_blocksSpinBox);
    toolBar -> addSeparator();

    //searching line
    label = new QLabel(tr("Szukaj zmiennej: "));
    toolBar -> addWidget(label);
    connect(_searchingLine, SIGNAL(returnPressed()), this, SLOT(search()));
    connect(new QShortcut(QKeySequence(QKeySequence::Find), this), SIGNAL(activated()), _searchingLine, SLOT(setFocus()));
    toolBar -> addWidget(_searchingLine);

    addToolBar(Qt::TopToolBarArea, toolBar);

    //bottom toolbar ===============================================================
    toolBar = new QToolBar(this);
    gridLayout = new QGridLayout;

    // -------------- nonblocks

    //delimiter (names)
    Q_ASSERT(_infos.name_right.is_valid() && !_infos.name_right.zero_length);
    separator = new delimiterWidget("Separator (nazwy) ", _infos.name_right);
    connect(separator, SIGNAL(delimiter_changed(const QString&)), this, SLOT(name_right_delimiter(const QString&)));
    connect(separator, SIGNAL(consec_changed(int)), this, SLOT(name_right_index(int)));
    gridLayout -> addWidget(separator, 0, 0);

    //delimiter (labels)
    Q_ASSERT(_infos.label_right.is_valid() && !_infos.label_right.zero_length);
    separator = new delimiterWidget("Separator (etykiety) ", _infos.label_right);
    connect(separator, SIGNAL(delimiter_changed(const QString&)), this, SLOT(label_right_delimiter(const QString&)));
    connect(separator, SIGNAL(consec_changed(int)), this, SLOT(label_right_index(int)));
    gridLayout -> addWidget(separator, 0, 1);

    //denotement
    label = new QLabel(tr("[Prawy]"));
    label -> setVisible(_infos.blocks_mode);
    connect(blocksBox, SIGNAL(toggled(bool)), label, SLOT(setVisible(bool)));
    gridLayout -> addWidget(label, 0, 2);

    // ------------ blocks mode

    //delimiter (names)
    Q_ASSERT(_infos.name_left.is_valid() && !_infos.name_left.zero_length);
    separator = new delimiterWidget("Separator (nazwy) ", _infos.name_left);
    separator -> set_state(_infos.blocks_mode);
    connect(separator, SIGNAL(delimiter_changed(const QString&)), this, SLOT(name_left_delimiter(const QString&)));
    connect(separator, SIGNAL(consec_changed(int)), this, SLOT(name_left_index(int)));
    connect(blocksBox, SIGNAL(toggled(bool)), separator, SLOT(set_state(bool)));
    gridLayout -> addWidget(separator, 1, 0);

    //delimiter (labels)
    Q_ASSERT(_infos.label_left.is_valid() && !_infos.label_left.zero_length);
    separator = new delimiterWidget("Separator (etykiety) ", _infos.label_left);
    separator -> set_state(_infos.blocks_mode);
    connect(separator, SIGNAL(delimiter_changed(const QString&)), this, SLOT(label_left_delimiter(const QString&)));
    connect(separator, SIGNAL(consec_changed(int)), this, SLOT(label_left_index(int)));
    connect(blocksBox, SIGNAL(toggled(bool)), separator, SLOT(set_state(bool)));
    gridLayout -> addWidget(separator, 1, 1);

    //denotement
    label = new QLabel(tr("[Lewy]"));
    label -> setVisible(_infos.blocks_mode);
    connect(blocksBox, SIGNAL(toggled(bool)), label, SLOT(setVisible(bool)));
    gridLayout -> addWidget(label, 1, 2);

    // ------------ additional buttons
    pushButton = new QPushButton(tr("Kopiuj osi&e"));
    connect(pushButton, SIGNAL(clicked()), this, SLOT(copy_maindefs()));
    gridLayout -> addWidget(pushButton, 0, 3);
    pushButton = new QPushButton(tr("Kopiuj &tabele"));
    connect(pushButton, SIGNAL(clicked()), this, SLOT(copy_helpdefs()));
    gridLayout -> addWidget(pushButton, 0, 4);
    pushButton = new QPushButton(tr("Wy&czyść osie"));
    connect(pushButton, SIGNAL(clicked()), this, SLOT(clear_maindefs()));
    gridLayout -> addWidget(pushButton, 1, 3);
    pushButton = new QPushButton(tr("Wyczyść ta&bele"));
    connect(pushButton, SIGNAL(clicked()), this, SLOT(clear_helpdefs()));
    gridLayout -> addWidget(pushButton, 1, 4);

    frame = new QFrame;
    frame -> setLayout(gridLayout);
    toolBar -> addWidget(frame);
    addToolBar(Qt::BottomToolBarArea, toolBar);

    //MAIN PANE ===============================================================
    headerView = new QHeaderView(Qt::Horizontal, _varsTable);
    _varsTable -> setSelectionBehavior(QAbstractItemView::SelectRows);
    _varsTable -> setColumnCount(5);
    _varsTable -> setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Nazwa")));
    _varsTable -> setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Start")));
    _varsTable -> setHorizontalHeaderItem(2, new QTableWidgetItem(tr("Stop")));
    _varsTable -> setHorizontalHeaderItem(3, new QTableWidgetItem(tr("Format")));
    _varsTable -> setHorizontalHeaderItem(4, new QTableWidgetItem(tr("Etykieta")));    
    _varsTable -> setHorizontalHeader(headerView);
    headerView -> setSectionResizeMode(0, QHeaderView::ResizeToContents);
    headerView -> setSectionResizeMode(1, QHeaderView::ResizeToContents);
    headerView -> setSectionResizeMode(2, QHeaderView::ResizeToContents);
    headerView -> setSectionResizeMode(3, QHeaderView::ResizeToContents);
    headerView -> setSectionResizeMode(4, QHeaderView::ResizeToContents);    

    connect(_varsTable, SIGNAL(itemSelectionChanged()), this, SLOT(show_codes()));
    setCentralWidget(_varsTable);

    //bottom pane ===============================================================
    headerView = new QHeaderView(Qt::Horizontal, _codesTable);
    _codesTable -> verticalHeader() -> setVisible(false);
    _codesTable -> setColumnCount(2);
    _codesTable -> setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Kod")));
    _codesTable -> setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Etykieta")));
    _codesTable -> setHorizontalHeader(headerView);
    headerView -> setSectionResizeMode(0, QHeaderView::ResizeToContents);
    headerView -> setSectionResizeMode(1, QHeaderView::Stretch);

    vBoxLayout = new QVBoxLayout;

    hBoxLayout = new QHBoxLayout;    

    pushButton = new QPushButton(tr("Oś (&1)"));
    pushButton -> setDisabled(_infos.blocks_mode);
    connect(blocksBox, SIGNAL(toggled(bool)), pushButton, SLOT(setDisabled(bool)));
    connect(pushButton, SIGNAL(clicked()), this, SLOT(axis()));
    hBoxLayout -> addWidget(pushButton);

    pushButton = new QPushButton(tr("Oś - skala (&2)"));
    pushButton -> setDisabled(_infos.blocks_mode);
    connect(blocksBox, SIGNAL(toggled(bool)), pushButton, SLOT(setDisabled(bool)));
    connect(pushButton, SIGNAL(clicked()), this, SLOT(axis_scale()));
    hBoxLayout -> addWidget(pushButton);

    pushButton = new QPushButton(tr("Multi - col (&3)"));
    //pushButton -> setDisabled(_infos.blocks_mode);
    //connect(blocksBox, SIGNAL(toggled(bool)), pushButton, SLOT(setDisabled(bool)));
    connect(pushButton, SIGNAL(clicked()), this, SLOT(multi()));
    hBoxLayout -> addWidget(pushButton);

    pushButton = new QPushButton(tr("Multi - fld (&4)"));
    //pushButton -> setDisabled(_infos.blocks_mode);
    //connect(blocksBox, SIGNAL(toggled(bool)), pushButton, SLOT(setDisabled(bool)));
    connect(pushButton, SIGNAL(clicked()), this, SLOT(multi_codes()));
    hBoxLayout -> addWidget(pushButton);

    pushButton = new QPushButton(tr("#include (&5)"));
    //pushButton -> setDisabled(_infos.blocks_mode);
    //connect(blocksBox, SIGNAL(toggled(bool)), pushButton, SLOT(setDisabled(bool)));
    connect(pushButton, SIGNAL(clicked()), this, SLOT(include()));
    hBoxLayout -> addWidget(pushButton);

    pushButton = new QPushButton(tr("#include - skala (&6)"));
    //pushButton -> setDisabled(_infos.blocks_mode);
    //connect(blocksBox, SIGNAL(toggled(bool)), pushButton, SLOT(setDisabled(bool)));
    connect(pushButton, SIGNAL(clicked()), this, SLOT(include_scale()));
    hBoxLayout -> addWidget(pushButton);

    pushButton = new QPushButton(tr("Grid (&7)"));
    pushButton -> setDisabled(_infos.blocks_mode);
    connect(blocksBox, SIGNAL(toggled(bool)), pushButton, SLOT(setDisabled(bool)));
    connect(pushButton, SIGNAL(clicked()), this, SLOT(grid()));
    hBoxLayout -> addWidget(pushButton);

    pushButton = new QPushButton(tr("Tabela średnich (&8)"));
    pushButton -> setDisabled(_infos.blocks_mode);
    connect(blocksBox, SIGNAL(toggled(bool)), pushButton, SLOT(setDisabled(bool)));
    connect(pushButton, SIGNAL(clicked()), this, SLOT(means()));
    hBoxLayout -> addWidget(pushButton);

    pushButton = new QPushButton(tr("Tabela TB/LB (&9)"));
    pushButton -> setDisabled(_infos.blocks_mode);
    connect(blocksBox, SIGNAL(toggled(bool)), pushButton, SLOT(setDisabled(bool)));
    connect(pushButton, SIGNAL(clicked()), this, SLOT(boxes()));
    hBoxLayout -> addWidget(pushButton);

    vBoxLayout -> addLayout(hBoxLayout);

    hBoxLayout = new QHBoxLayout;
    hBoxLayout -> addWidget(_codesTable);
    hBoxLayout -> setStretchFactor(_codesTable, 32);
    hBoxLayout -> addWidget(_mainDefs);
    hBoxLayout -> setStretchFactor(_mainDefs, 64);
    hBoxLayout -> addWidget(_helpDefs);
    hBoxLayout -> setStretchFactor(_helpDefs, 31);
    vBoxLayout -> addLayout(hBoxLayout);

    QDockWidget* dockWidget = new QDockWidget(this);
    dockWidget -> setFeatures(QDockWidget::DockWidgetFeatures());

    frame = new QFrame(dockWidget);
    frame -> setLayout(vBoxLayout);    
    label = new QLabel(tr("Definicje"), dockWidget);
    label -> setAlignment(Qt::AlignHCenter);

    dockWidget -> setWidget(frame);
    dockWidget -> setTitleBarWidget(label);
    addDockWidget(Qt::BottomDockWidgetArea, dockWidget);
}

MainWindow::~MainWindow()
{
}

void MainWindow::load() {

    QMessageBox box;

    QFile input(QFileDialog::getOpenFileName());

    if (!input.open(QIODevice::ReadOnly | QIODevice::Text | QIODevice::ExistingOnly)) {
        box.setText(tr("Nie można otworzyć pliku"));
        box.exec();
        return;
    }

    QTextStream stream(&input);
    stream.setCodec("Windows-1250");
    std::vector<QString> dictionary(5);
    size_t next = 0;
    QString line;

    if (stream.atEnd()) {
        box.setText(tr("Plik jest pusty"));
        box.exec();
        return;
    }

    line = stream.readLine();
    bool ok;
    unsigned declared_size = line.toUInt(&ok);
    if (!ok) {
        box.setText(tr("Nieprawidłowy format pliku"));
        box.exec();
        return;
    }

    _clear_contents();
    _path = input.fileName().left(input.fileName().lastIndexOf("/"));
    _variables.reserve(declared_size);
    _varsTable -> setRowCount(declared_size);

    while (!stream.atEnd()) {
        line = stream.readLine();

        if (next < dictionary.size()) {
            dictionary[next] = line;
            ++next;
            continue;
        }

        if (next == dictionary.size()) {
            try {
                _variables.emplace_back(dictionary[0], dictionary[1], dictionary[2], dictionary[3], dictionary[4]);
            }
            catch (const variable::error& err) {
                box.setText(err.what());
                box.exec();
                _clear_contents();
                return;
            }
            assert(_variables.size() <= declared_size);

            //populate table
            for (size_t column = 0; column < dictionary.size(); ++column) {
                assert(_variables.size() > 0);
                size_t row = _variables.size() - 1;
                QString info = _variables.back().get_info(column);
                _varsTable -> setItem(row, column, new QTableWidgetItem(info));
            }
            ++next;

        }

        if (line.isEmpty())
            next = 0;
        else {
            try {
                _variables.back().add_code(line);
            }
            catch (const variable::error& err) {
                box.setText(err.what());
                box.exec();
                _clear_contents();
                return;
            }
        }
    }

    structure();
    axes();
}

void MainWindow::_clear_contents() {
    _variables.clear();
    _clear_table(_varsTable);
    _clear_table(_codesTable);
    _mainDefs -> clear();
    _helpDefs -> clear();
    _current_range = range_type(1,0);
    _path.clear();
}

void MainWindow::_clear_table(QTableWidget* table) {
    table -> clearContents();
    table -> setRowCount(0);
}

void MainWindow::show_codes() {

    QList<QTableWidgetSelectionRange> selections = _varsTable -> selectedRanges();
    if (selections.empty()) {
        _clear_table(_codesTable);
        _current_range = range_type(1,0);
        return;
    }

    QTableWidgetSelectionRange selection = selections.front();
    int topVar = selection.topRow();
    int bottomVar = selection.bottomRow();
    int oldTopVar = _current_range.first;
    _current_range = range_type(topVar, bottomVar);
    if (topVar == oldTopVar)
        return;
    _clear_table(_codesTable);

    size_t codes_count = _variables[topVar].codes_count();
    _codesTable -> setRowCount(codes_count);

    variable::codes_type codes = _variables[topVar].codes();
    size_t row = 0;
    for (variable::codes_type::iterator iter = codes.begin(); iter != codes.end(); ++iter) {
        assert(row < codes_count);
        _codesTable -> setItem(row, 0, new QTableWidgetItem(iter -> first));
        _codesTable -> setItem(row, 1, new QTableWidgetItem(iter -> second));
        ++row;
    }

}

void MainWindow::name_right_delimiter(const QString& separator) {
    _infos.name_right.separator = separator;
}

void MainWindow::name_right_index(int consec) {
    _infos.name_right.consec = consec;
}

void MainWindow::name_left_delimiter(const QString& separator) {
    _infos.name_left.separator = separator;
}

void MainWindow::name_left_index(int consec) {
    _infos.name_left.consec = consec;
}

void MainWindow::label_right_delimiter(const QString& separator) {
    _infos.label_right.separator = separator;
}

void MainWindow::label_right_index(int consec) {
    _infos.label_right.consec = consec;
}

void MainWindow::label_left_delimiter(const QString& separator) {
    _infos.label_left.separator = separator;
}

void MainWindow::label_left_index(int consec) {
    _infos.label_left.consec = consec;
}

void MainWindow::append_mode(bool checked) {
    _infos.append_mode = checked;
}

void MainWindow::blocks_mode(bool checked) {
    _infos.blocks_mode = checked;
    block_size(_blocksSpinBox -> value());
}

void MainWindow::create_files(bool checked) {
    _infos.create_files = checked;
}

void MainWindow::_print_mainDefs(const QString& text) {
    if (_infos.append_mode) {
        _mainDefs -> moveCursor(QTextCursor::End);
        _mainDefs -> insertPlainText(text);
    }
    else {
        _mainDefs -> clear();
        _mainDefs -> setText(text);
    }
}

void MainWindow::_print_helpDefs(const QString& text) {
    if (_infos.append_mode) {
        _helpDefs -> moveCursor(QTextCursor::End);
        _helpDefs -> insertPlainText(text);
    }
    else {
        _helpDefs -> clear();
        _helpDefs -> setText(text);
    }
}

void MainWindow::_outwrite(const QString& filename, const QString& contents) {
    if (!_infos.create_files)
        return;

    QMessageBox box;
    QFile output(QString("%1/%2").arg(_path).arg(filename));
    bool is_correct = true;

    if (_infos.append_mode && (filename == "Qtables.rn" || filename == "Qside.rn"))
         is_correct = output.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
    else
        is_correct = output.open(QIODevice::WriteOnly | QIODevice::Text);

    if (!is_correct) {
            box.setText(tr("Nie można stworzyć pliku"));
            box.exec();
            return;
    }

    QTextStream stream(&output);
    stream.setCodec(&_codec);
    stream << contents.trimmed().append("\n");

}

void MainWindow::axis() {

    if (_current_range == range_type(1,0))
        return;

    definitions def(_variables, _current_range, _infos);
    def.axis();

    _prepare_output(def);

}

void MainWindow::axis_scale() {

    if (_current_range == range_type(1,0))
        return;

    definitions def(_variables, _current_range, _infos);
    def.axis_scale();

    _prepare_output(def);
}

void MainWindow::multi() {

    if (_current_range == range_type(1,0))
        return;

    definitions def(_variables, _current_range, _infos);
    def.multi();

    _prepare_output(def);
}

void MainWindow::multi_codes() {

    if (_current_range == range_type(1,0))
        return;

    definitions def(_variables, _current_range, _infos);
    def.multi_codes();

    _prepare_output(def);
}

void MainWindow::include() {

    if (_current_range == range_type(1,0))
        return;

    definitions def(_variables, _current_range, _infos);
    def.include();

    _prepare_output(def);
}

void MainWindow::include_scale() {

    if (_current_range == range_type(1,0))
        return;

    definitions def(_variables, _current_range, _infos);
    def.include_scale();

    _prepare_output(def);
}

void MainWindow::boxes() {

    if (_current_range == range_type(1,0))
        return;

    definitions def(_variables, _current_range, _infos);
    def.boxes();

    _prepare_output(def);

}

void MainWindow::means() {

    if (_current_range == range_type(1,0))
        return;

    definitions def(_variables, _current_range, _infos);
    def.means();

    _prepare_output(def);

}

void MainWindow::grid() {

    if (_current_range == range_type(1,0))
        return;

    definitions def(_variables, _current_range, _infos);
    def.grid();

    _prepare_output(def);

}

void MainWindow::structure() {

    range_type whole_range = (_variables.size() ? range_type(0, _variables.size() - 1) : range_type(1,0));
    definitions def(_variables, whole_range, _infos);
    def.structure();

    _outwrite("QDefs_struktura.txt", def.side());

}

void MainWindow::axes() {

    range_type whole_range = (_variables.size() ? range_type(0, _variables.size() - 1) : range_type(1,0));
    definitions def(_variables, whole_range, _infos);
    def.axis();

    _outwrite("QDefs_axes.rn", def.side());
}

void MainWindow::search() {
    size_t i = 0;
    while (i < _variables.size()) {
        if (_variables[i].has_text(_searchingLine -> text()))
            break;
        ++i;
    }

    if (i < _variables.size()) {
        _varsTable -> selectRow(i);
        _varsTable -> setFocus();
    }
    else {
        _searchingLine -> setText(tr("Nie znaleziono"));
        _searchingLine -> selectAll();
    }

}

void MainWindow::_prepare_output (definitions& def) {

    if (_infos.blocks_mode) {
        QMessageBox box;
        Q_ASSERT(_current_range.first <= _current_range.second);
        Q_ASSERT(_infos.block_size >= 0);
        unsigned block_size = _infos.block_size;
        if ((_current_range.second - _current_range.first + 1u) % block_size) {
            box.setText(tr("Nieprawidłowy rozmiar bloku"));
            box.exec();
            return;
        }
    }

    _print_mainDefs(def.side());
    _outwrite("Qside.rn", def.side());

    _print_helpDefs(def.tables());
    _outwrite("Qtables.rn", def.tables());

    if (!def.is_out_empty()) {
        Q_ASSERT(!def.is_filename_empty());
        _outwrite(def.filename(), def.out());
    }

    if (!def.is_out_blocks_empty()) {
        Q_ASSERT(!def.is_filename_blocks_empty());
        _outwrite(def.filename_blocks(), def.out_blocks());
    }

}

void MainWindow::block_size(int) {
    _infos.block_size = _blocksSpinBox -> value();
}

void MainWindow::copy_maindefs() {
    _mainDefs -> selectAll();
    _clipboard -> setText(_mainDefs -> toPlainText());
}

void MainWindow::clear_maindefs() {
    _mainDefs -> clear();
}

void MainWindow::copy_helpdefs() {
    _helpDefs -> selectAll();
    _clipboard -> setText(_helpDefs -> toPlainText());
}

void MainWindow::clear_helpdefs() {
    _helpDefs -> clear();
}

