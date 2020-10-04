#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "typedefs.h"
#include "definitions.h"
#include <QMainWindow>
#include <QTableWidget>
#include <QTextEdit>
#include "dosCodec.h"
#include <QString>
#include <QLineEdit>
#include <QSpinBox>

class MainWindow : public QMainWindow
{
    Q_OBJECT

    variables_type _variables;

    QTableWidget* _varsTable;
    QTableWidget* _codesTable;    
    QTextEdit* _mainDefs;
    QTextEdit* _helpDefs;

    range_type _current_range;
    infos _infos;

    QString _path;
    dosCodec _codec;
    QLineEdit* _searchingLine;
    QSpinBox* _blocksSpinBox;

    void _clear_contents();
    void _clear_table(QTableWidget*);
    void _print_mainDefs(const QString&);
    void _print_helpDefs(const QString&);
    void _outwrite(const QString&, const QString&);
    void _prepare_output (definitions& def);

private slots:
    void load();
    void show_codes();

    void name_right_delimiter(const QString&);
    void name_right_index(int);
    void name_left_delimiter(const QString&);
    void name_left_index(int);
    void label_right_delimiter(const QString&);
    void label_right_index(int);
    void label_left_delimiter(const QString&);
    void label_left_index(int);

    void append_mode(bool);
    void create_files(bool);
    void blocks_mode(bool);

    void axis();
    void axis_scale();
    void multi();
    void multi_codes();
    void include();
    void include_scale();
    void boxes();
    void means();
    void grid();

    void structure();
    void axes();

    void search();
    void block_size(int);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

};
#endif // MAINWINDOW_H
