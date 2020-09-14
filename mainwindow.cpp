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
#include <QLineEdit>
#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), _varsTable(new QTableWidget), _codesTable(new QTableWidget),
      _mainDefs(new QTextEdit), _helpDefs(new QTextEdit),
      _current_range(1,0),
      _delimiter(":"), _append_mode(false), _create_files(false)
{

    QMenu* menu;
    QAction* action;
    QCheckBox* checkBox;
    QLabel* label;
    QHeaderView* headerView;
    QHBoxLayout* hBoxLayout;
    QPushButton* pushButton;
    QFrame* frame;

    //menubar ===============================================================
    setMenuBar(new QMenuBar);

    menu = menuBar() -> addMenu(tr("Program"));
    action = menu -> addAction(tr("Wczytaj słownik"));
    connect(action, SIGNAL(triggered()), this, SLOT(load()));
    action = menu -> addAction(tr("Wyjdź"));
    connect(action, SIGNAL(triggered()), QApplication::instance(), SLOT(quit()));

    //toolbar ===============================================================
    QToolBar* toolBar = new QToolBar(this);

    checkBox = new QCheckBox(tr("Dopisuj definicje"));
    connect(checkBox, SIGNAL(stateChanged(int)), this, SLOT(append_mode(int)));
    toolBar -> addWidget(checkBox);
    toolBar -> addSeparator();

    checkBox = new QCheckBox(tr("Twórz pliki"));    
    connect(checkBox, SIGNAL(stateChanged(int)), this, SLOT(create_files(int)));
    checkBox -> setChecked(true);
    toolBar -> addWidget(checkBox);
    toolBar -> addSeparator();

    label = new QLabel("Separator ");
    toolBar -> addWidget(label);

    QLineEdit* lineEdit = new QLineEdit(":");
    QSizePolicy sizePolicy(lineEdit->sizePolicy());
    sizePolicy.setHorizontalPolicy(QSizePolicy::Preferred);
    lineEdit -> setSizePolicy(sizePolicy);
    connect(lineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(delimiter(const QString&)));
    toolBar -> addWidget(lineEdit);

    addToolBar(Qt::BottomToolBarArea, toolBar);

    //main pane ===============================================================
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

    QVBoxLayout* vBoxLayout = new QVBoxLayout;

    hBoxLayout = new QHBoxLayout;    
    pushButton = new QPushButton(tr("Oś"));
    connect(pushButton, SIGNAL(clicked()), this, SLOT(axis()));
    hBoxLayout -> addWidget(pushButton);
    pushButton = new QPushButton(tr("Oś [skala]"));
    connect(pushButton, SIGNAL(clicked()), this, SLOT(axis_scale()));
    hBoxLayout -> addWidget(pushButton);
    pushButton = new QPushButton(tr("MULTI [1]"));
    connect(pushButton, SIGNAL(clicked()), this, SLOT(multi()));
    hBoxLayout -> addWidget(pushButton);
    pushButton = new QPushButton(tr("MULTI [kody]"));
    connect(pushButton, SIGNAL(clicked()), this, SLOT(multi_codes()));
    hBoxLayout -> addWidget(pushButton);
    pushButton = new QPushButton(tr("#include"));
    connect(pushButton, SIGNAL(clicked()), this, SLOT(include()));
    hBoxLayout -> addWidget(pushButton);
    pushButton = new QPushButton(tr("#include [skala]"));
    connect(pushButton, SIGNAL(clicked()), this, SLOT(include_scale()));
    hBoxLayout -> addWidget(pushButton);
    pushButton = new QPushButton(tr("GRID"));
    connect(pushButton, SIGNAL(clicked()), this, SLOT(grid()));
    hBoxLayout -> addWidget(pushButton);
    pushButton = new QPushButton(tr("tabela średnich"));
    connect(pushButton, SIGNAL(clicked()), this, SLOT(means()));
    hBoxLayout -> addWidget(pushButton);
    pushButton = new QPushButton(tr("tabela TB/LB"));
    connect(pushButton, SIGNAL(clicked()), this, SLOT(boxes()));
    hBoxLayout -> addWidget(pushButton);
    vBoxLayout -> addLayout(hBoxLayout);

    hBoxLayout = new QHBoxLayout;
    hBoxLayout -> addWidget(_codesTable);
    hBoxLayout -> setStretchFactor(_codesTable, 1);
    hBoxLayout -> addWidget(_mainDefs);
    hBoxLayout -> setStretchFactor(_mainDefs, 2);
    hBoxLayout -> addWidget(_helpDefs);
    hBoxLayout -> setStretchFactor(_helpDefs, 1);
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

void MainWindow::delimiter(const QString& delimiter) {
    _delimiter = delimiter;
}

void MainWindow::append_mode(int checked) {
    _append_mode = checked;
}

void MainWindow::create_files(int checked) {
    _create_files = checked;
}

void MainWindow::_print_mainDefs(const QString& text) {
    if (_append_mode) {
        _mainDefs -> moveCursor(QTextCursor::End);
        _mainDefs -> insertPlainText(text);
    }
    else {
        _mainDefs -> clear();
        _mainDefs -> setText(text);
    }
}

void MainWindow::_print_helpDefs(const QString& text) {
    if (_append_mode) {
        _helpDefs -> moveCursor(QTextCursor::End);
        _helpDefs -> insertPlainText(text);
    }
    else {
        _helpDefs -> clear();
        _helpDefs -> setText(text);
    }
}

void MainWindow::_outwrite(const QString& filename, const QString& contents) {
    if (!_create_files)
        return;

    QMessageBox box;

    QFile output(QString("%1/%2").arg(_path).arg(filename));
    if (!output.open(QIODevice::WriteOnly | QIODevice::Text)) {
            box.setText(tr("Nie można stworzyć pliku"));
            box.exec();
            return;
    }

    QTextStream stream(&output);
    stream.setCodec("IBM 850");
    stream << QString(contents).append("\n");

}

void MainWindow::axis() {

    if (_current_range == range_type(1,0))
        return;

    definitions def(_variables, _current_range, _delimiter);
    def.axis();

    _print_mainDefs(def.side());
    _outwrite("Qside.rn", def.side());

    _print_helpDefs(def.tables());
    _outwrite("Qtables.rn", def.tables());

}

void MainWindow::axis_scale() {

    if (_current_range == range_type(1,0))
        return;

    definitions def(_variables, _current_range, _delimiter);
    def.axis_scale();

    _print_mainDefs(def.side());
    _outwrite("Qside.rn", def.side());

    _print_helpDefs(def.tables());
    _outwrite("Qtables.rn", def.tables());
}

void MainWindow::multi() {

    if (_current_range == range_type(1,0))
        return;

    definitions def(_variables, _current_range, _delimiter);
    def.multi();

    _print_mainDefs(def.side());
    _outwrite("Qside.rn", def.side());

    _print_helpDefs(def.tables());
    _outwrite("Qtables.rn", def.tables());
}

void MainWindow::multi_codes() {

    if (_current_range == range_type(1,0))
        return;

    definitions def(_variables, _current_range, _delimiter);
    def.multi_codes();

    _print_mainDefs(def.side());
    _outwrite("Qside.rn", def.side());

    _print_helpDefs(def.tables());
    _outwrite("Qtables.rn", def.tables());
}

void MainWindow::include() {

    if (_current_range == range_type(1,0))
        return;

    definitions def(_variables, _current_range, _delimiter);
    def.include();

    _print_mainDefs(def.side());
    _outwrite("Qside.rn", def.side());
    _outwrite(def.filename(), def.out());

    _print_helpDefs(def.tables());
    _outwrite("Qtables.rn", def.tables());
}

void MainWindow::include_scale() {

    if (_current_range == range_type(1,0))
        return;

    definitions def(_variables, _current_range, _delimiter);
    def.include_scale();

    _print_mainDefs(def.side());
    _outwrite("Qside.rn", def.side());
    _outwrite(def.filename(), def.out());

    _print_helpDefs(def.tables());
    _outwrite("Qtables.rn", def.tables());

}

void MainWindow::boxes() {

    if (_current_range == range_type(1,0))
        return;

    definitions def(_variables, _current_range, _delimiter);
    def.boxes();

    _print_mainDefs(def.side());
    _outwrite("Qside.rn", def.side());
    _outwrite("Qboxes.in", def.out());

    _print_helpDefs(def.tables());
    _outwrite("Qtables.rn", def.tables());

}

void MainWindow::means() {

    if (_current_range == range_type(1,0))
        return;

    definitions def(_variables, _current_range, _delimiter);
    def.means();

    _print_mainDefs(def.side());
    _outwrite("Qside.rn", def.side());
    _outwrite("Qmeans.in.", def.out());

    _print_helpDefs(def.tables());
    _outwrite("Qtables.rn", def.tables());

}

void MainWindow::grid() {

    if (_current_range == range_type(1,0))
        return;

    definitions def(_variables, _current_range, _delimiter);
    def.grid();

    _print_mainDefs(def.side());
    _outwrite("Qside.rn", def.side());

    _print_helpDefs(def.tables());
    _outwrite("Qtables.rn", def.tables());

}

void MainWindow::structure() {

    range_type whole_range(0, _variables.size() - 1);
    definitions def(_variables, whole_range, _delimiter);
    def.structure();

    _outwrite("QDefs_struktura.txt", def.side());

}

void MainWindow::axes() {

    range_type whole_range(0, _variables.size() - 1);
    definitions def(_variables, whole_range, _delimiter);
    def.axis();

    _outwrite("QDefs_axes.rn", def.side());
}
