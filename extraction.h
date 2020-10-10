#ifndef EXTRACTION_H
#define EXTRACTION_H

#include <QString>
#include "typedefs.h"

class extraction
{
    QString _name;
    QString _label;
    QString _name_base;
    QString _name_slow;
    QString _name_fast;
    QString _label_base;
    QString _label_slow;
    QString _label_fast;

    int _position(const QString& text, const QString& subtext, int consec);

public:
    extraction(const QString& name, const QString& label, const infos& inf);

    inline bool is_name_base_empty() const {return _name_base.isEmpty();}
    inline bool is_name_slow_empty() const {return _name_slow.isEmpty();}
    inline bool is_name_fast_empty() const {return _name_fast.isEmpty();}
    inline bool is_label_base_empty() const {return _label_base.isEmpty();}
    inline bool is_label_slow_empty() const {return _label_slow.isEmpty();}
    inline bool is_label_fast_empty() const {return _label_fast.isEmpty();}

    inline QString name() const {return _name.trimmed();}
    inline QString label() const {return _label.trimmed();}
    inline QString name_base() const {return _name_base.trimmed();}
    inline QString name_slow() const {return _name_slow.trimmed();}
    inline QString name_fast() const {return _name_fast.trimmed();}
    inline QString label_base() const {return _label_base.trimmed();}
    inline QString label_slow() const {return _label_slow.trimmed();}
    inline QString label_fast() const {return _label_fast.trimmed();}

    QString named_label_base() const;
    QString named_label() const;
};

#endif // EXTRACTION_H
