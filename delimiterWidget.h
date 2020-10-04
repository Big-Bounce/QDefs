#ifndef DELIMITERWIDGET_H
#define DELIMITERWIDGET_H

#include <QFrame>
#include <QSpinBox>
#include "typedefs.h"

class delimiterWidget : public QFrame
{
    Q_OBJECT

    QSpinBox* _spinBox;

public:
    delimiterWidget(const QString& name, const delimiter& d, QWidget* parent = 0);

signals:
    void delimiter_changed(const QString&);
    void consec_changed(int);

private slots:
    void line_textChanged(const QString&);
    void first_toogled(bool);
    void last_toogled(bool);
    void other_toogled(bool);
    void spin_valueChanged(int);

public slots:
    void set_state(bool);


};

#endif // DELIMITERWIDGET_H
