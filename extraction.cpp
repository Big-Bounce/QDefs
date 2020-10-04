#include "extraction.h"
#include <QRegExp>
#include <QStringList>

extraction::extraction(const QString& name, const QString& label, const infos& inf): _name(name), _label(label) {

    //name
    int left_position = _position(name, inf.name_left.separator, inf.name_left.consec);
    int left_offset = inf.name_left.separator.size();
    int right_position = _position(name, inf.name_right.separator, inf.name_right.consec);
    int right_offset = inf.name_right.separator.size();
    int position = (inf.blocks_mode ? left_position : right_position);
    int offset = (inf.blocks_mode ? left_offset : right_offset);

    if (position > 0)
        _name_base = name.left(position);
    else if (right_position > 0)
        _name_base = name.left(right_position);
    else
        _name_base = name;

    if (inf.blocks_mode)
        if (left_position >= 0 && (left_position += left_offset) < right_position)
            _name_slow = name.mid(left_position, right_position - left_position);

    if (right_position >=0 && (right_position += right_offset) < name.size())
        _name_fast = name.mid(right_position);

    //label
    left_position = _position(label, inf.label_left.separator, inf.label_left.consec);
    left_offset = inf.label_left.separator.size();
    right_position = _position(label, inf.label_right.separator, inf.label_right.consec);
    right_offset = inf.label_right.separator.size();
    position = (inf.blocks_mode ? left_position : right_position);
    offset = (inf.blocks_mode ? left_offset : right_offset);

    if (position > 0)
        _label_base = label.left(position);
    else if (right_position > 0)
        _label_base = _label.left(right_position);
    else
        _label_base = label;

    if (inf.blocks_mode) {
        if (left_position >= 0 && (left_position += left_offset) < right_position)
            _label_slow = label.mid(left_position, right_position - left_position);
        else if (left_position >= 0 && left_position < label.size())
            _label_slow = label.mid(left_position);
        else
            _label_slow = _label_base;
    }

    if (right_position >=0 && (right_position += right_offset) < label.size())
        _label_fast = label.mid(right_position);
    else
        _label_fast = _label_slow;

}

int extraction::_position(const QString& text, const QString& subtext, int consec) {
    if (text.isEmpty() || consec < 0)
        return -1;
    if (consec == 0)
        return text.lastIndexOf(subtext);

    int begin = 0, result = 0;
    while (--consec >= 0) {
        if (begin >= text.size())
            return -1;
        result = text.indexOf(subtext, begin);
        if (result < 0)
            return -1;
        begin = result + subtext.size();
    }
    return result;

}

QString extraction::named_label_base() const {
    QRegExp regExp("(\\.\\s*|\\s+)");
    int position = regExp.indexIn(_label_base);
    QString captured = (position < 0 ? QString() : regExp.capturedTexts().first());
    if (position <= 0 || position == _label_base.size() - captured.size())
        return label_base();

    QString left = _label_base.left(position);
    QString right = _label_base.mid(position + captured.size());
    if (left.contains(_name_base) && right.contains(regExp))
        return _name_base + captured + right;
    else
        return label_base();
}
