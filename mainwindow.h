#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "typedefs.h"
#include "definitions.h"
#include <QMainWindow>
#include <QTableWidget>
#include <QTextEdit>

#include <QString>

class MainWindow : public QMainWindow
{
    Q_OBJECT

    variables_type _variables;
    QTableWidget* _varsTable;
    QTableWidget* _codesTable;
    QTextEdit* _mainDefs;
    QTextEdit* _helpDefs;

    QString _path;
    range_type _current_range;

    QString _delimiter;
    bool _append_mode;
    bool _create_files;    

    void _clear_contents();
    void _clear_table(QTableWidget*);
    void _print_mainDefs(const QString&);
    void _print_helpDefs(const QString&);
    void _outwrite(const QString&, const QString&);



private slots:
    void load();
    void show_codes();

    void delimiter(const QString&);
    void append_mode(int);
    void create_files(int);

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

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
#endif // MAINWINDOW_H
