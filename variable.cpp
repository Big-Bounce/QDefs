#include "variable.h"
#include <QObject>
#include <cassert>

variable::variable(const QString& name, const QString& label, const QString& format, const QString& start, const QString& stop):
    _name(name), _label(label), _format(format), _is_sorted(false) {

    bool ok;
    unsigned temp;
    temp = start.toUInt(&ok);
    if (!ok)
        throw error(QObject::tr("Nieprawidłowa wartość kolumny początkowej"));
    temp = stop.toUInt(&ok);
    if (!ok)
        throw error(QObject::tr("Nieprawidłowa wartość kolumny końcowej"));
    _start = start;
    _stop = stop;
}

void variable::add_code(const QString& code) {
    int delimiter = code.indexOf(";");
    if (delimiter < 0)
        throw error(QObject::tr("Nieprawidłowa wartość kodu"));

    QString value = code.left(delimiter);
    bool ok;
    double temp = value.toDouble(&ok);
    if (!ok)
        throw error(QObject::tr("Nieprawidłowa wartość kodu"));
    int position = value.indexOf(".");
    assert(position > 0 && position < value.size() - 1);
    if (value[position+1] == '0')
        value = value.left(position);

    QString label;
    if (++delimiter < code.size())
        label = code.mid(delimiter);
    _codes.emplace_back(value, label);

    _is_sorted = false;

}

QString variable::get_info(unsigned column) {
    assert(column < 5);
    switch(column) {
        case 0: return _name;
        case 1: return _start;
        case 2: return _stop;
        case 3: return _format;
        case 4: return _label;
    }
    return QString();
}

QString variable::second_last() const {
    unsigned second_last = _stop.toUInt() - 1;
    return QString("%1").arg(second_last);
}

QString variable::width() const {
    unsigned width = _stop.toUInt() - _start.toUInt() + 1;
    return QString("%1").arg(width);
}

variable::codes_type variable::codes() {
    if (!_is_sorted) {
        auto lambda = [] (const code_type& code1, const code_type& code2) {
            return code1.first.toUInt() < code2.first.toUInt();
        };
        _codes.sort(lambda);
        _is_sorted = true;
    }
    return _codes;

}

