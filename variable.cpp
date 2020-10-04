#include "variable.h"
#include <QObject>
#include <QtGlobal>

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
    Q_ASSERT(position > 0 && position < value.size() - 1);
    if (value[position+1] == '0')
        value = value.left(position);

    QString label;
    if (++delimiter < code.size())
        label = code.mid(delimiter);
    _codes.emplace_back(value, label);

    _is_sorted = false;

}

QString variable::get_info(unsigned column) {
    Q_ASSERT(column < 5);
    switch(column) {
        case 0: return _name;
        case 1: return _start;
        case 2: return _stop;
        case 3: return _format;
        case 4: return _label;
    }
    return QString();
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

bool variable::has_text(const QString& text) const {
    if (_name.contains(text, Qt::CaseInsensitive)) return true;
    if (_label.contains(text, Qt::CaseInsensitive)) return true;
    return false;
}

QString variable::shifted_start(const variable& bottom, const variable& top) const {

    bool ok;

    unsigned floor = bottom._start.toUInt(&ok);
    Q_ASSERT(ok);
    unsigned me = _start.toUInt(&ok);
    Q_ASSERT(ok);
    Q_ASSERT(floor <= me);
    QString shifted_me = QString("%1").arg(me -= floor);

    unsigned roof = (top._stop.isEmpty() ? bottom._stop.toUInt(&ok) : top._stop.toUInt(&ok));
    Q_ASSERT(ok);
    Q_ASSERT(roof >= floor);
    QString shifted_roof = QString("%1").arg(roof - floor);

    int how_many_zeros = shifted_roof.size() - shifted_me.size();
    Q_ASSERT(how_many_zeros >= 0);
    QString zeros = (how_many_zeros > 0 ? QString(how_many_zeros, '0') : QString());
    return QString("a%1%2").arg(zeros).arg(me);

}

QString variable::shifted_stop(const variable& bottom, const variable& top) const {

    bool ok;

    unsigned floor = bottom._start.toUInt(&ok);
    Q_ASSERT(ok);
    unsigned me = _stop.toUInt(&ok);
    Q_ASSERT(ok);
    Q_ASSERT(floor <= me);
    QString shifted_me = QString("%1").arg(me -= floor);

    unsigned roof = (top._stop.isEmpty() ? bottom._stop.toUInt(&ok) : top._stop.toUInt(&ok));
    Q_ASSERT(ok);
    Q_ASSERT(roof >= floor);
    QString shifted_roof = QString("%1").arg(roof - floor);

    int how_many_zeros = shifted_roof.size() - shifted_me.size();
    Q_ASSERT(how_many_zeros >= 0);
    QString zeros = (how_many_zeros > 0 ? QString(how_many_zeros, '0') : QString());
    return QString("a%1%2").arg(zeros).arg(me);

}

QString variable::shifted_start() const {
    return shifted_start(*this);
}
QString variable::shifted_stop() const {
    return shifted_stop(*this);
}
