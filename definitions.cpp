#include "definitions.h"
#include <QRegExp>
#include <QStringList>

definitions::definitions(variables_type& variables, range_type& current_range, QString& delimiter):
    _variables(variables), _current_range(current_range), _delimiter(delimiter) {

}

QString definitions::_extract_name (const QString& text) {
    int position = text.indexOf("_");
    if (position <= 0)
        return text;
    return text.left(position);
}

QString definitions::_extract_label_prefix (const QString& text, const QString& name) {

    int position = text.indexOf(_delimiter);
    if (position <= 0)
        return text;
    QString prefix = text.left(position);
    if (name.isEmpty())
        return prefix;

    QRegExp regExp("(\\.\\s*|\\s+)");
    position = regExp.indexIn(prefix);
    QString captured = (position < 0 ? QString() : regExp.capturedTexts().first());
    if (position <= 0 || position == prefix.size() - captured.size())
        return prefix;

    QString left = prefix.left(position);
    QString right = prefix.mid(position + captured.size());
    regExp.setPattern("\\S");
    if (left.contains("_") && right.contains(regExp))
        return name + captured + right;
    else
        return prefix;

}

QString definitions::_extract_label_suffix (const QString& text) {
    int position = text.indexOf(_delimiter);
    if (position == text.size() - _delimiter.size())
        return text;
    return text.mid(position + _delimiter.size());
}

void definitions::axis() {

    for (size_t i = _current_range.first; i <= _current_range.second; ++i) {
        QString name = _variables[i].name();
        QString label = _variables[i].label();
        QString start = _variables[i].start();
        QString stop = _variables[i].stop();
        QString width = _variables[i].width();
        variable::codes_type codes = _variables[i].codes();

        QString range;
        QString instruction;
        if (width.toUInt() == 1) {
            range = start;
            instruction = QString("col %1").arg(start);
        }
        else {
            range = QString("(%1,%2)").arg(start).arg(stop);
            instruction = QString("fld c%1:%2").arg(start).arg(width);
        }

        _side += QString("l %1; c=c%2n''\n").arg(name).arg(range);
        _side += QString("ttl %1\n").arg(label);
        _side += QString("n10 Razem\n");
        _side += QString("n23; nosort\n");
        _side += QString("%1\n").arg(instruction);
        for (variable::codes_type::iterator iter = codes.begin(); iter != codes.end(); ++iter)
            _side += QString("+ %1=%2\n").arg(iter->second).arg(iter->first);
        _side += _end_of_axis();

        _tables += QString("#include tabs.rn; ax=%1\n").arg(name);
    }

}

void definitions::axis_scale(){

    for (size_t i = _current_range.first; i <= _current_range.second; ++i) {

        QString name = _variables[i].name();
        QString label = _variables[i].label();
        QString start = _variables[i].start();
        QString stop = _variables[i].stop();
        QString width = _variables[i].width();
        variable::codes_type codes = _variables[i].codes();

        QString range;
        QString instruction;
        if (width.toUInt() == 1) {
            range = start;
            instruction = QString("col %1").arg(start);
        }
        else {
            range = QString("(%1,%2)").arg(start).arg(stop);
            instruction = QString("fld c%1:%2").arg(start).arg(width);
        }

        _side += QString("l %1; c=c%2n''\n").arg(name).arg(range);
        _side += QString("ttl %1\n").arg(label);
        _side += QString("n10 Razem\n");
        _side += QString("n23; nosort\n");
        _side += QString("%1\n").arg(instruction);
        for (variable::codes_type::iterator iter = codes.begin(); iter != codes.end(); ++iter)
            _side += QString("+ [%1] %2=%3\n").arg(iter->first).arg(iter->second).arg(iter->first);
        _side += QString("+ []=\n");
        _side += QString("+ []=\n");
        _side += QString("n25; inc=c%1; c=c%1.in.()\n").arg(range);
        _side += QString("n12 Średnia\n");
        _side += _end_of_axis();

        _tables += QString("#include tabs.rn; ax=%1\n").arg(name);

    }

}

void definitions::multi(){

    QString name = _extract_name(_variables[_current_range.first].name());
    QString label = _extract_label_prefix(_variables[_current_range.first].label(), name);
    QString start = _variables[_current_range.first].start();
    QString stop = _variables[_current_range.second].stop();

    _side += QString("l %1; c=c(%2,%3)n''; sort\n").arg(name).arg(start).arg(stop);
    _side += QString("ttl %1\n").arg(label);
    _side += QString("n10 Razem\n");
    _side += QString("n23; nosort\n");
    for (size_t i = _current_range.first; i <= _current_range.second; ++i) {
        stop = _variables[i].stop();
        label = _extract_label_suffix(_variables[i].label());
        _side += QString("col %1; %2\n").arg(stop).arg(label);
    }
    _side += _end_of_axis();

    _tables = QString("#include tabs.rn; ax=%1\n").arg(name);
}

void definitions::multi_codes(){

    QString name = _extract_name(_variables[_current_range.first].name());
    QString label = _extract_label_prefix(_variables[_current_range.first].label(), name);
    QString start = _variables[_current_range.first].start();
    QString stop = _variables[_current_range.second].stop();
    QString width = _variables[_current_range.first].width();
    QString second_last = _variables[_current_range.second].start();
    variable::codes_type codes = _variables[_current_range.first].codes();

    _side += QString("l %1; c=c(%2,%3)n''; sort\n").arg(name).arg(start).arg(stop);
    _side += QString("ttl %1\n").arg(label);
    _side += QString("n10 Razem\n");
    _side += QString("n23; nosort\n");
    _side += QString("fld c%1,c%2:%3\n").arg(start).arg(second_last).arg(width);
    for (variable::codes_type::iterator iter = codes.begin(); iter != codes.end(); ++iter)
        _side += QString("+ %1=%2\n").arg(iter->second).arg(iter->first);
    _side += _end_of_axis();

    _tables = QString("#include tabs.rn; ax=%1\n").arg(name);
}

void definitions::include(){

    QString name = _extract_name(_variables[_current_range.first].name());
    QString label = _extract_label_prefix(_variables[_current_range.first].label(), name);
    QString start;
    QString width = _variables[_current_range.first].width();
    variable::codes_type codes = _variables[_current_range.first].codes();

    _filename = name + ".in";

    QString range;
    QString instruction;
    if (width.toUInt() == 1) {
        range = "a00";
        instruction = QString("col a00");
    }
    else {
        range = QString("(a00,a0%1)").arg(width.toUInt()-1);
        instruction = QString("fld ca00:%1").arg(width);
    }

    _out_side(QString("l %1_&nr; c=c%2n''\n").arg(name).arg(range));
    _out_side(QString("ttl %1\n").arg(label));
    _out_side(QString("ttl &txt\n"));
    _out_side(QString("n10 Razem\n"));
    _out_side(QString("n23; nosort\n"));
    _out_side(QString("%1\n").arg(instruction));
    for (variable::codes_type::iterator iter = codes.begin(); iter != codes.end(); ++iter)
        _out_side(QString("+ %1=%2\n").arg(iter->second).arg(iter->first));
    _side += _end_of_axis();

    for (size_t i = _current_range.first, nr = 1; i <= _current_range.second; ++i, ++nr) {
        start = _variables[i].start();
        label = _extract_label_suffix(_variables[i].label());
        _side += QString("#include %1.in; col(a)=%2; nr=%3; txt=%4\n").arg(name).arg(start).arg(nr).arg(label);
        _tables += QString("#include tabs.rn; ax=%1_%2\n").arg(name).arg(nr);
    }
    _side += _end_of_axis();

}

void definitions::include_scale(){

    QString name = _extract_name(_variables[_current_range.first].name());
    QString label = _extract_label_prefix(_variables[_current_range.first].label(), name);
    QString start;
    QString width = _variables[_current_range.first].width();
    variable::codes_type codes = _variables[_current_range.first].codes();

    _filename = name + ".in";

    QString range;
    QString instruction;
    if (width.toUInt() == 1) {
        range = "a00";
        instruction = QString("col a00");
    }
    else {
        range = QString("(a00,a0%1)").arg(width.toUInt()-1);
        instruction = QString("fld ca00:%1").arg(width);
    }

    _out_side(QString("l %1_&nr; c=c%2n''\n").arg(name).arg(range));
    _out_side(QString("ttl %1\n").arg(label));
    _out_side(QString("ttl &txt\n"));
    _out_side(QString("n10 Razem\n"));
    _out_side(QString("n23; nosort\n"));
    _out_side(QString("%1\n").arg(instruction));
    for (variable::codes_type::iterator iter = codes.begin(); iter != codes.end(); ++iter)
        _out_side(QString("+ [%1] %2=%3\n").arg(iter->first).arg(iter->second).arg(iter->first));
    _out_side(QString("+ []=\n"));
    _out_side(QString("+ []=\n"));
    _out_side(QString("n25; inc=c%1; c=c%1.in.()\n").arg(range));
    _out_side(QString("n12 Średnia\n"));
    _side += _end_of_axis();

    for (size_t i = _current_range.first, nr = 1; i <= _current_range.second; ++i, ++nr) {
        start = _variables[i].start();
        label = _extract_label_suffix(_variables[i].label());
        _side += QString("#include %1.in; col(a)=%2; nr=%3; txt=%4\n").arg(name).arg(start).arg(nr).arg(label);
        _tables += QString("#include tabs.rn; ax=%1_%2\n").arg(name).arg(nr);
    }
    _side += _end_of_axis();


}

void definitions::boxes() {

    QString name = _extract_name(_variables[_current_range.first].name());
    QString label = _extract_label_prefix(_variables[_current_range.first].label(), name);
    QString start = _variables[_current_range.first].start();
    QString stop = _variables[_current_range.second].stop();
    QString width = _variables[_current_range.first].width();

    _filename = name + ".in";

    QString range;
    if (width.toUInt() == 1)
        range = "a00";
    else
        range = QString("(a00,a0%1)").arg(width.toUInt()-1);

    _out_side(QString("n11; c=c%1n''\n").arg(range));
    _out_side(QString("n01 &txt; c=c%1.in.()\n").arg(range));
    _side += _end_of_axis();

    _side += QString("l %1; c=c(%2,%3)n'';\n").arg(name).arg(start).arg(stop);
    _side += QString("ttl %1\n").arg(label);
    _side += QString("n10 Razem\n");
    _side += QString("n23; nosort\n");
    for (size_t i = _current_range.first; i <= _current_range.second; ++i) {
        start = _variables[i].start();
        label = _extract_label_suffix(_variables[i].label());
        _side += QString("#include temp_boxes.in; col(a)=%1; txt=%2\n").arg(start).arg(label);
    }
    _side += _end_of_axis();

    _tables = QString("#include tabs.rn; ax=%1").arg(name);
}

void definitions::means(){


    QString name = _extract_name(_variables[_current_range.first].name());
    QString label = _extract_label_prefix(_variables[_current_range.first].label(), name);
    QString start = _variables[_current_range.first].start();
    QString stop = _variables[_current_range.second].stop();
    QString width = _variables[_current_range.first].width();

    _filename = name + ".in";

    QString range;
    if (width.toUInt() == 1)
        range = "a00";
    else
        range = QString("(a00,a0%1)").arg(width.toUInt()-1);

    _out_side(QString("n25; inc=c%1; c=c%1.in.()\n").arg(range));
    _out_side(QString("n12 &txt\n"));
    _side += _end_of_axis();

    _side += QString("l %1; c=c(%2,%3)n'';\n").arg(name).arg(start).arg(stop);
    _side += QString("ttl %1\n").arg(label);
    _side += QString("n10 Razem\n");
    _side += QString("n23; nosort\n");
    for (size_t i = _current_range.first; i <= _current_range.second; ++i) {
        start = _variables[i].start();
        label = _extract_label_suffix(_variables[i].label());
        _side += QString("#include temp_boxes.in; col(a)=%1; txt=%2\n").arg(start).arg(label);
    }
    _side += _end_of_axis();

    _tables = QString("#include tabs.rn; ax=%1").arg(name);

}

void definitions::grid(){

    QString name = _extract_name(_variables[_current_range.first].name());
    QString label = _extract_label_prefix(_variables[_current_range.first].label(), name);
    QString start = _variables[_current_range.first].start();
    QString stop = _variables[_current_range.second].stop();
    QString width = _variables[_current_range.first].width();
    variable::codes_type codes = _variables[_current_range.first].codes();

    QString range;
    QString instruction;
    if (width.toUInt() == 1) {
        range = "a00";
        instruction = QString("col a00");
    }
    else {
        range = QString("(a00,a0%1)").arg(width.toUInt()-1);
        instruction = QString("fld c%1:%2").arg(start).arg(width);
    }

    _side += QString("l %1; c=c(%2,%3)n''\n").arg(name).arg(start).arg(stop);
    QString item;
    for (size_t i = _current_range.first; i <= _current_range.second; ++i) {
        item = _extract_label_suffix(_variables[i].label());
        start = _variables[i].start();
        _side += QString("n01 %1; col(a)=%2\n").arg(item).arg(start);
    }
    _side += QString("side\n");
    _side += QString("ttl %1\n").arg(label);
    _side += QString("n10 Razem; c=c%1n''\n").arg(range);
    _side += QString("n23; nosort\n");
    _side += QString("%1\n").arg(instruction);
    for (variable::codes_type::iterator iter = codes.begin(); iter != codes.end(); ++iter)
        _side += QString("+ [%1] %2=%3\n").arg(iter->first).arg(iter->second).arg(iter->first);
    _side += QString("+ []=\n");
    _side += QString("+ []=\n");
    _side += QString("n25; inc=c%1; c=c%1.in.()\n").arg(range);
    _side += QString("n12 Średnia\n");
    _side += _end_of_axis();

    _tables = QString("tab %1 grid\n").arg(name);
}

void definitions::structure() {

    int name_full = 40;
    int start_full = 10;
    int stop_full = 10;
    int format_full = 10;

    QString name("NAME");
    QString start("START");
    QString stop("STOP");
    QString format("FORMAT");

    QString name_space(name_full - name.size(), ' ');
    QString start_space(start_full - start.size(), ' ');
    QString stop_space(stop_full - stop.size(), ' ');
    QString format_space(format_full - format.size(), ' ');

    _side += QString("%1%2%3%4\n").arg(name+name_space).arg(start+start_space).arg(stop+stop_space).arg(format+format_space);
    for (size_t i = _current_range.first; i <= _current_range.second; ++i) {
        name = _variables[i].name();
        name_space = (name.size() > name_full ? QString(' ') : QString(name_full - name.size(), ' '));
        start = _variables[i].start();
        start_space = (start.size() > start_full ? QString(' ') : QString(start_full - start.size(), ' '));
        stop = _variables[i].stop();
        stop_space = (stop.size() > stop_full ? QString(' ') : QString(stop_full - stop.size(), ' '));
        format = _variables[i].format();
        format_space = (format.size() > format_full ? QString(' ') : QString(format_full - format.size(), ' '));
        _side += QString("%1%2%3%4\n").arg(name+name_space).arg(start+start_space).arg(stop+stop_space).arg(format+format_space);

     }

}
