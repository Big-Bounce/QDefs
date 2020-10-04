#include "delimiterWidget.h"
#include <QLineEdit>
#include <QSizePolicy>
#include <QHBoxLayout>
#include <QRadioButton>
#include <QLabel>

delimiterWidget::delimiterWidget(const QString& name, const delimiter& d, QWidget* parent):
    QFrame(parent), _spinBox(new QSpinBox)
{
    setFrameShape(QFrame::StyledPanel);
    QHBoxLayout* hBoxLayout = new QHBoxLayout;

    QLabel* label = new QLabel(name);
    hBoxLayout -> addWidget(label);

    QLineEdit* lineEdit = new QLineEdit(d.separator);
    QSizePolicy sizePolicy = lineEdit -> sizePolicy();
    sizePolicy.setHorizontalPolicy(QSizePolicy::Preferred);
    lineEdit -> setSizePolicy(sizePolicy);
    connect(lineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(line_textChanged(const QString&)));
    hBoxLayout -> addWidget(lineEdit);

    QRadioButton* first = new QRadioButton(tr("Pierwszy"));
    connect(first, SIGNAL(toggled(bool)), this, SLOT(first_toogled(bool)));
    hBoxLayout -> addWidget(first);
    QRadioButton* last = new QRadioButton(tr("Ostatni"));
    connect(last, SIGNAL(toggled(bool)), this, SLOT(last_toogled(bool)));
    hBoxLayout -> addWidget(last);
    QRadioButton* other = new QRadioButton(tr("Inny"));
    connect(other, SIGNAL(toggled(bool)), this, SLOT(other_toogled(bool)));
    hBoxLayout -> addWidget(other);

    connect(_spinBox, SIGNAL(valueChanged(int)), this, SLOT(spin_valueChanged(int)));
    hBoxLayout -> addWidget(_spinBox);
    setLayout(hBoxLayout);

    if (d.consec < 0) {
        _spinBox -> setMinimum(d.consec);
        _spinBox -> setValue(d.consec);
        other -> click();
        return;
    }

    if (d.consec == 0) {
        _spinBox -> setMinimum(1);
        _spinBox -> setValue(1);
        last -> click();
        return;
    }

    if (d.consec == 1) {
        _spinBox -> setMinimum(1);
        _spinBox -> setValue(1);
        first -> click();
        return;
    }

    if (d.consec > 1) {
        _spinBox -> setMinimum(1);
        _spinBox -> setValue(d.consec);
        other -> click();
        return;
    }


}

void delimiterWidget::line_textChanged(const QString& text) {
    emit delimiter_changed(text);
}

void delimiterWidget::first_toogled(bool checked) {
    if (checked) {
        _spinBox -> setEnabled(false);
        emit consec_changed(1);
    }
}

void delimiterWidget::last_toogled(bool checked) {
    if (checked) {
        _spinBox -> setEnabled(false);
        emit consec_changed(0);
    }
}

void delimiterWidget::other_toogled(bool checked) {
    if (checked) {
        _spinBox -> setEnabled(true);
        emit consec_changed(_spinBox -> value());
    }
}

void delimiterWidget::spin_valueChanged(int value) {
    emit consec_changed(value);
}

void delimiterWidget::set_state(bool enable) {
    setEnabled(enable);
}
