#include "definitions.h"
#include <QtGlobal>
#include "extraction.h"

definitions::definitions(variables_type& variables, range_type& current_range, infos& inf):
    _variables(variables), _current_range(current_range), _inf(inf) {
}

void definitions::axis_nonblocks() {

    for (size_t i = _current_range.first; i <= _current_range.second; ++i) {

        extraction ext(_variables[i].name(), _variables[i].label(), _inf);

        QString name = ext.name();
        QString label = ext.label();
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

void definitions::axis_scale_nonblocks(){

    for (size_t i = _current_range.first; i <= _current_range.second; ++i) {

        extraction ext(_variables[i].name(), _variables[i].label(), _inf);

        QString name = ext.name();
        QString label = ext.label();
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

void definitions::multi_nonblocks(){

    extraction ext(_variables[_current_range.first].name(), _variables[_current_range.first].label(), _inf);

    QString name = ext.name_base();
    QString label = ext.named_label_base();
    QString start = _variables[_current_range.first].start();
    QString stop = _variables[_current_range.second].stop();

    _side += QString("l %1; c=c(%2,%3)n''; sort\n").arg(name).arg(start).arg(stop);
    _side += QString("ttl %1\n").arg(label);
    _side += QString("n10 Razem\n");
    _side += QString("n23; nosort\n");
    for (size_t i = _current_range.first; i <= _current_range.second; ++i) {
        ext = extraction(_variables[i].name(), _variables[i].label(), _inf);
        stop = _variables[i].stop();
        label = ext.label_fast();
        _side += QString("col %1; %2\n").arg(stop).arg(label);
    }
    _side += _end_of_axis();

    _tables = QString("#include tabs.rn; ax=%1\n").arg(name);
}

void definitions::multi_codes_nonblocks(){

    extraction ext(_variables[_current_range.first].name(), _variables[_current_range.first].label(), _inf);

    QString name = ext.name_base();
    QString label = ext.named_label_base();
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

void definitions::include_nonblocks() {

    extraction ext(_variables[_current_range.first].name(), _variables[_current_range.first].label(), _inf);

    QString name = ext.name_base();
    QString label = ext.named_label_base();
    QString start;
    QString width = _variables[_current_range.first].width();
    QString shifted_start = _variables[_current_range.first].shifted_start();
    QString shifted_stop = _variables[_current_range.first].shifted_stop();
    variable::codes_type codes = _variables[_current_range.first].codes();
    QString index;

    _filename = name + ".in";

    QString range;
    QString instruction;
    if (width.toUInt() == 1) {
        range = shifted_start;
        instruction = QString("col %1").arg(shifted_start);
    }
    else {
        range = QString("(%1,%2)").arg(shifted_start).arg(shifted_stop);
        instruction = QString("fld c%1:%2").arg(shifted_start).arg(width);
    }

    _out_side(QString("l %1%2&nr; c=c%3n''\n").arg(name).arg(_inf.name_right.separator).arg(range));
    _out_side(QString("ttl %1%2\n").arg(label).arg(_inf.label_right.separator));
    _out_side(QString("ttl &txt\n"));
    _out_side(QString("n10 Razem\n"));
    _out_side(QString("n23; nosort\n"));
    _out_side(QString("%1\n").arg(instruction));
    for (variable::codes_type::iterator iter = codes.begin(); iter != codes.end(); ++iter)
        _out_side(QString("+ %1=%2\n").arg(iter->second).arg(iter->first));
    _side += _end_of_axis();

    for (size_t i = _current_range.first, nr = 1; i <= _current_range.second; ++i) {
        ext = extraction(_variables[i].name(), _variables[i].label(), _inf);
        start = _variables[i].start();
        index = (ext.is_name_fast_empty() ? QString("v%1").arg(++nr) : ext.name_fast());
        label = ext.label_fast();
        _side += QString("#include %1.in; col(a)=%2; nr=%3; txt=%4\n").arg(name).arg(start).arg(index).arg(label);
        _tables += QString("#include tabs.rn; ax=%1%2%3\n").arg(name).arg(_inf.name_right.separator).arg(index);
    }
    _side += _end_of_axis();

}

void definitions::include_scale_nonblocks(){

    extraction ext(_variables[_current_range.first].name(), _variables[_current_range.first].label(), _inf);

    QString name = ext.name_base();
    QString label = ext.named_label_base();
    QString start;
    QString width = _variables[_current_range.first].width();
    QString shifted_start = _variables[_current_range.first].shifted_start();
    QString shifted_stop = _variables[_current_range.first].shifted_stop();
    variable::codes_type codes = _variables[_current_range.first].codes();
    QString index;

    _filename = name + ".in";

    QString range;
    QString instruction;
    if (width.toUInt() == 1) {
        range = shifted_start;
        instruction = QString("col %1").arg(shifted_start);
    }
    else {
        range = QString("(%1,%2)").arg(shifted_start).arg(shifted_stop);
        instruction = QString("fld c%1:%2").arg(shifted_start).arg(width);
    }

    _out_side(QString("l %1%2&nr; c=c%3n''\n").arg(name).arg(_inf.name_right.separator).arg(range));
    _out_side(QString("ttl %1%2\n").arg(label).arg(_inf.label_right.separator));
    _out_side(QString("ttl &txt\n"));
    _out_side(QString("n10 Razem\n"));
    _out_side(QString("n23; nosort\n"));
    _out_side(QString("%1\n").arg(instruction));
    for (variable::codes_type::iterator iter = codes.begin(); iter != codes.end(); ++iter)
        _out_side(QString("+ %1=%2\n").arg(iter->second).arg(iter->first));
    _side += QString("+ []=\n");
    _side += QString("+ []=\n");
    _side += QString("n25; inc=c%1; c=c%1.in.()\n").arg(range);
    _side += QString("n12 Średnia\n");
    _side += _end_of_axis();

    for (size_t i = _current_range.first, nr = 1; i <= _current_range.second; ++i) {
        ext = extraction(_variables[i].name(), _variables[i].label(), _inf);
        start = _variables[i].start();
        index = (ext.is_name_fast_empty() ? QString("v%1").arg(++nr) : ext.name_fast());
        label = ext.label_fast();
        _side += QString("#include %1.in; col(a)=%2; nr=%3; txt=%4\n").arg(name).arg(start).arg(index).arg(label);
        _tables += QString("#include tabs.rn; ax=%1%2%3\n").arg(name).arg(_inf.name_right.separator).arg(index);
    }
    _side += _end_of_axis();

}

void definitions::boxes_nonblocks() {

    extraction ext(_variables[_current_range.first].name(), _variables[_current_range.first].label(), _inf);

    QString name = ext.name_base();
    QString label = ext.named_label_base();
    QString start = _variables[_current_range.first].start();
    QString stop = _variables[_current_range.second].stop();
    QString width = _variables[_current_range.first].width();
    QString shifted_start = _variables[_current_range.first].shifted_start();
    QString shifted_stop = _variables[_current_range.first].shifted_stop();

    _filename = name + "BOX.in";

    QString range;
    if (width.toUInt() == 1)
        range = shifted_start;
    else
        range = QString("(%1,%2)").arg(shifted_start).arg(shifted_stop);

    _out_side(QString("n11; c=c%1n''\n").arg(range));
    _out_side(QString("n01 &txt; c=c%1.in.()\n").arg(range));
    _side += _end_of_axis();

    _side += QString("l %1; c=c(%2,%3)n'';\n").arg(name).arg(start).arg(stop);
    _side += QString("ttl %1\n").arg(label);
    _side += QString("n10 Razem\n");
    _side += QString("n23; nosort\n");
    for (size_t i = _current_range.first; i <= _current_range.second; ++i) {
        ext = extraction(_variables[i].name(), _variables[i].label(), _inf);
        start = _variables[i].start();
        label = ext.label_fast();
        _side += QString("#include temp_boxes.in; col(a)=%1; txt=%2\n").arg(start).arg(label);
    }
    _side += _end_of_axis();

    _tables = QString("#include tabs.rn; ax=%1").arg(name);
}

void definitions::means_nonblocks() {

    extraction ext(_variables[_current_range.first].name(), _variables[_current_range.first].label(), _inf);

    QString name = ext.name_base();
    QString label = ext.named_label_base();
    QString start = _variables[_current_range.first].start();
    QString stop = _variables[_current_range.second].stop();
    QString width = _variables[_current_range.first].width();
    QString shifted_start = _variables[_current_range.first].shifted_start();
    QString shifted_stop = _variables[_current_range.first].shifted_stop();

    _filename = name + "AVR.in";

    QString range;
    if (width.toUInt() == 1)
        range = shifted_start;
    else
        range = QString("(%1,%2)").arg(shifted_start).arg(shifted_stop);

    _out_side(QString("n25; inc=c%1; c=c%1.in.()\n").arg(range));
    _out_side(QString("n12 &txt\n"));
    _side += _end_of_axis();

    _side += QString("l %1; c=c(%2,%3)n'';\n").arg(name).arg(start).arg(stop);
    _side += QString("ttl %1\n").arg(label);
    _side += QString("n10 Razem\n");
    _side += QString("n23; nosort\n");
    for (size_t i = _current_range.first; i <= _current_range.second; ++i) {
        ext = extraction(_variables[i].name(), _variables[i].label(), _inf);
        start = _variables[i].start();
        label = ext.label_fast();
        _side += QString("#include temp_boxes.in; col(a)=%1; txt=%2\n").arg(start).arg(label);
    }
    _side += _end_of_axis();

    _tables = QString("#include tabs.rn; ax=%1").arg(name);

}

void definitions::grid_nonblocks() {

    extraction ext(_variables[_current_range.first].name(), _variables[_current_range.first].label(), _inf);

    QString name = ext.name_base();
    QString label = ext.named_label_base();
    QString start = _variables[_current_range.first].start();
    QString stop = _variables[_current_range.second].stop();
    QString width = _variables[_current_range.first].width();
    QString shifted_start = _variables[_current_range.first].shifted_start();
    QString shifted_stop = _variables[_current_range.first].shifted_stop();
    variable::codes_type codes = _variables[_current_range.first].codes();

    QString range;
    QString instruction;
    if (width.toUInt() == 1) {
        range = shifted_start;
        instruction = QString("col %1").arg(shifted_start);
    }
    else {
        range = QString("(%1,%2)").arg(shifted_start).arg(shifted_stop);
        instruction = QString("fld c%1:%2").arg(shifted_start).arg(width);
    }

    _side += QString("l %1; c=c(%2,%3)n''\n").arg(name).arg(start).arg(stop);
    QString item;
    for (size_t i = _current_range.first; i <= _current_range.second; ++i) {
        ext = extraction(_variables[i].name(), _variables[i].label(), _inf);
        item = ext.label_fast();
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

void definitions::axis_blocks() {

}

void definitions::axis_scale_blocks() {

}

void definitions::multi_blocks() {

    Q_ASSERT(_inf.block_size > 0);
    size_t block_size = _inf.block_size;

    size_t block_first = _current_range.first;
    size_t block_last = block_first + block_size - 1u;
    extraction ext(_variables[block_first].name(), _variables[block_first].label(), _inf);

    QString name = ext.name_base();
    QString label = ext.named_label_base();
    QString start;
    QString shifted_start = _variables[block_first].shifted_start(_variables[block_first], _variables[block_last]);
    QString shifted_stop = _variables[block_last].shifted_stop(_variables[block_first], _variables[block_last]);
    QString index;

    _filename_blocks = name + "BLC.in";

    _out_side_blocks(QString("l %1%2&nr; c=c(%3,%4)n''; sort\n").arg(name).arg(_inf.name_left.separator).arg(shifted_start).arg(shifted_stop));
    _out_side_blocks(QString("ttl %1%2\n").arg(label).arg(_inf.label_left.separator));
    _out_side_blocks(QString("ttl &txt\n"));
    _out_side_blocks(QString("n10 Razem\n"));
    _out_side_blocks(QString("n23; nosort\n"));
    for (size_t i = block_first; i <= block_last; ++i) {
        ext = extraction(_variables[i].name(), _variables[i].label(), _inf);
        shifted_stop = _variables[i].shifted_stop(_variables[block_first], _variables[block_last]);
        label = ext.label_fast();
        _out_side_blocks(QString("col %1; %2\n").arg(shifted_stop).arg(label));
    }
    _side += _end_of_axis();

    unsigned block = 0;
    while (block_first <= _current_range.second) {
       ext = extraction(_variables[block_first].name(), _variables[block_first].label(), _inf);
       start = _variables[block_first].start();
       label = ext.label_slow();
       index = (ext.is_name_slow_empty() ? QString("v%1").arg(++block) : ext.name_slow());
       _side += QString("#include %1; col(a)=%2; nr=%3; txt=%4\n").arg(_filename_blocks).arg(start).arg(index).arg(label);
       _tables += QString("#include tabs.rn; ax=%1%2%3\n").arg(name).arg(_inf.name_left.separator).arg(index);
       block_first += block_size;
    }
}

void definitions::multi_codes_blocks() {

    Q_ASSERT(_inf.block_size > 0);
    size_t block_size = _inf.block_size;

    size_t block_first = _current_range.first;
    size_t block_last = block_first + block_size - 1u;
    extraction ext(_variables[block_first].name(), _variables[block_first].label(), _inf);

    QString name = ext.name_base();
    QString label = ext.named_label_base();
    QString start;
    QString shifted_start = _variables[block_first].shifted_start(_variables[block_first], _variables[block_last]);
    QString shifted_stop = _variables[block_last].shifted_stop(_variables[block_first], _variables[block_last]);
    QString index;
    QString width = _variables[block_first].width();
    QString shifted_second_last = _variables[block_last].shifted_start(_variables[block_first], _variables[block_last]);
    variable::codes_type codes = _variables[block_first].codes();

    _filename_blocks = name + "BLC.in";

    _out_side_blocks(QString("l %1%2&nr; c=c(%3,%4)n''; sort\n").arg(name).arg(_inf.name_left.separator).arg(shifted_start).arg(shifted_stop));
    _out_side_blocks(QString("ttl %1%2\n").arg(label).arg(_inf.label_left.separator));
    _out_side_blocks(QString("n10 Razem\n"));
    _out_side_blocks(QString("n23; nosort\n"));
    _out_side_blocks(QString("fld c%1,c%2:%3\n").arg(shifted_start).arg(shifted_second_last).arg(width));
    for (variable::codes_type::iterator iter = codes.begin(); iter != codes.end(); ++iter)
        _out_side_blocks(QString("+ %1=%2\n").arg(iter->second).arg(iter->first));
    _side += _end_of_axis();

    unsigned block = 0;
    while (block_first <= _current_range.second) {
       ext = extraction(_variables[block_first].name(), _variables[block_first].label(), _inf);
       start = _variables[block_first].start();
       label = ext.label_slow();
       index = (ext.is_name_slow_empty() ? QString("v%1").arg(++block) : ext.name_slow());
       _side += QString("#include %1; col(a)=%2; nr=%3; txt=%4\n").arg(_filename_blocks).arg(start).arg(index).arg(label);
       _tables += QString("#include tabs.rn; ax=%1%2%3\n").arg(name).arg(_inf.name_left.separator).arg(index);
       block_first += block_size;
    }
}

void definitions::include_blocks() {

    Q_ASSERT(_inf.block_size > 0);
    size_t block_size = _inf.block_size;

    size_t block_first = _current_range.first;
    size_t block_last = block_first + block_size - 1u;
    extraction ext(_variables[block_first].name(), _variables[block_first].label(), _inf);

    QString name = ext.name_base();
    QString label = ext.named_label_base();
    QString start;
    QString width = _variables[block_first].width();
    QString shifted_start;
    QString shifted_stop;
    QString index;
    QString parameters;
    variable::codes_type codes = _variables[block_first].codes();

    _filename = name + ".in";
    _filename_blocks = name + "BLC.in";

    QString range;
    QString instruction;
    if (width.toUInt() == 1) {
        range = "(&kol)";
        instruction = "col &kol";
    }
    else {
        range = "(&start,&stop)";
        instruction = QString("fld c&start:%1").arg(width);
    }

    _out_side(QString("l %1%2&blcnr%3&nr; c=c%4n''\n").
              arg(name).arg(_inf.name_left.separator).arg(_inf.name_right.separator).arg(range));
    _out_side(QString("ttl %1%2\n").arg(label).arg(_inf.label_left.separator));
    _out_side(QString("ttl &blctxt\n"));
    _out_side(QString("ttl &txt\n"));
    _out_side(QString("n10 Razem\n"));
    _out_side(QString("n23; nosort\n"));
    _out_side(QString("%1\n").arg(instruction));
    for (variable::codes_type::iterator iter = codes.begin(); iter != codes.end(); ++iter)
        _out_side(QString("+ %1=%2\n").arg(iter->second).arg(iter->first));
    _side += _end_of_axis();

    QString temp_tables;
    for (size_t i = block_first, nr = 1; i <= block_last; ++i) {
        ext = extraction(_variables[i].name(), _variables[i].label(), _inf);
        shifted_start = _variables[i].shifted_start(_variables[block_first], _variables[block_last]);
        shifted_stop = _variables[i].shifted_stop(_variables[block_first], _variables[block_last]);
        index = (ext.is_name_fast_empty() ? QString("v%1").arg(++nr) : ext.name_fast());
        label = ext.label_fast();
        parameters = (width.toUInt() == 1 ?
                          QString("kol=%1").arg(shifted_start) :
                          QString("start=%1; stop=%2").arg(shifted_start).arg(shifted_stop)
        );
        _out_side_blocks(QString("#include %1; %2; nr=%3; txt=%4\n").
                         arg(_filename).arg(parameters).arg(index).arg(label));
        temp_tables += QString("#include tabs.rn; ax=%1%2&blcnr%3%4\n").
                        arg(name).arg(_inf.name_left.separator).arg(_inf.name_right.separator).arg(index);
    }
    _side += _end_of_axis();

    unsigned block = 0;
    while (block_first <= _current_range.second) {
       ext = extraction(_variables[block_first].name(), _variables[block_first].label(), _inf);
       start = _variables[block_first].start();
       label = ext.label_slow();
       index = (ext.is_name_slow_empty() ? QString("v%1").arg(++block) : ext.name_slow());
       _side += QString("#include %1; col(a)=%2; blcnr=%3; blctxt=%4\n").arg(_filename_blocks).arg(start).arg(index).arg(label);
       _tables += QString(temp_tables).replace("&blcnr", index);
       block_first += block_size;
    }

}

void definitions::include_scale_blocks() {

    Q_ASSERT(_inf.block_size > 0);
    size_t block_size = _inf.block_size;

    size_t block_first = _current_range.first;
    size_t block_last = block_first + block_size - 1u;
    extraction ext(_variables[block_first].name(), _variables[block_first].label(), _inf);

    QString name = ext.name_base();
    QString label = ext.named_label_base();
    QString start;
    QString width = _variables[block_first].width();
    QString shifted_start;
    QString shifted_stop;
    QString index;
    QString parameters;
    variable::codes_type codes = _variables[block_first].codes();

    _filename = name + ".in";
    _filename_blocks = name + "BLC.in";

    QString range;
    QString instruction;
    if (width.toUInt() == 1) {
        range = "(&kol)";
        instruction = "col &kol";
    }
    else {
        range = "(&start,&stop)";
        instruction = QString("fld c&start:%1").arg(width);
    }

    _out_side(QString("l %1%2&blcnr%3&nr; c=c%4n''\n").
              arg(name).arg(_inf.name_left.separator).arg(_inf.name_right.separator).arg(range));
    _out_side(QString("ttl %1%2\n").arg(label).arg(_inf.label_left.separator));
    _out_side(QString("ttl &blctxt\n"));
    _out_side(QString("ttl &txt\n"));
    _out_side(QString("n10 Razem\n"));
    _out_side(QString("n23; nosort\n"));
    _out_side(QString("%1\n").arg(instruction));
    for (variable::codes_type::iterator iter = codes.begin(); iter != codes.end(); ++iter)
        _out_side(QString("+ %1=%2\n").arg(iter->second).arg(iter->first));
    _out_side(QString("+ []=\n"));
    _out_side(QString("+ []=\n"));
    _out_side(QString("n25; inc=c%1; c=c%1.in.()\n").arg(range));
    _out_side(QString("n12 Średnia\n"));
    _side += _end_of_axis();

    QString temp_tables;
    for (size_t i = block_first, nr = 1; i <= block_last; ++i) {
        ext = extraction(_variables[i].name(), _variables[i].label(), _inf);
        shifted_start = _variables[i].shifted_start(_variables[block_first], _variables[block_last]);
        shifted_stop = _variables[i].shifted_stop(_variables[block_first], _variables[block_last]);
        index = (ext.is_name_fast_empty() ? QString("v%1").arg(++nr) : ext.name_fast());
        label = ext.label_fast();
        parameters = (width.toUInt() == 1 ?
                          QString("kol=%1").arg(shifted_start) :
                          QString("start=%1; stop=%2").arg(shifted_start).arg(shifted_stop)
        );
        _out_side_blocks(QString("#include %1; %2; nr=%3; txt=%4\n").
                         arg(_filename).arg(parameters).arg(index).arg(label));
        temp_tables += QString("#include tabs.rn; ax=%1%2&blcnr%3%4\n").
                        arg(name).arg(_inf.name_left.separator).arg(_inf.name_right.separator).arg(index);
    }
    _side += _end_of_axis();

    unsigned block = 0;
    while (block_first <= _current_range.second) {
       ext = extraction(_variables[block_first].name(), _variables[block_first].label(), _inf);
       start = _variables[block_first].start();
       label = ext.label_slow();
       index = (ext.is_name_slow_empty() ? QString("v%1").arg(++block) : ext.name_slow());
       _side += QString("#include %1; col(a)=%2; blcnr=%3; blctxt=%4\n").arg(_filename_blocks).arg(start).arg(index).arg(label);
       _tables += QString(temp_tables).replace("&blcnr", index);
       block_first += block_size;
    }
}

void definitions::boxes_blocks() {

}

void definitions::means_blocks() {

}

void definitions::grid_blocks() {

}

void definitions::axis() {
    if (_inf.blocks_mode)
        axis_blocks();
    else
        axis_nonblocks();
}

void definitions::axis_scale() {
    if (_inf.blocks_mode)
        axis_scale_blocks();
    else
        axis_scale_nonblocks();
}

void definitions::multi() {
    if (_inf.blocks_mode)
        multi_blocks();
    else
        multi_nonblocks();
}

void definitions::multi_codes() {
    if (_inf.blocks_mode)
        multi_codes_blocks();
    else
        multi_codes_nonblocks();
}

void definitions::include() {
    if (_inf.blocks_mode)
        include_blocks();
    else
        include_nonblocks();
}

void definitions::include_scale() {
    if (_inf.blocks_mode)
        include_scale_blocks();
    else
        include_scale_nonblocks();
}

void definitions::boxes() {
    if (_inf.blocks_mode)
        boxes_blocks();
    else
        boxes_nonblocks();
}

void definitions::means() {
    if (_inf.blocks_mode)
        means_blocks();
    else
        means_nonblocks();
}

void definitions::grid() {
    if (_inf.blocks_mode)
        grid_blocks();
    else
        grid_nonblocks();
}

