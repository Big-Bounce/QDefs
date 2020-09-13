#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include "typedefs.h"

class definitions {

    variables_type& _variables;
    range_type& _current_range;
    QString& _delimiter;

    QString _side;
    QString _tables;
    QString _out;
    QString _filename;

    inline QString _end_of_axis() const {return "/*----------------------------------------------\n";}
    inline void _out_side(const QString& text) {_out += text; _side += QString("/*%1").arg(text);}

    QString _extract_name (const QString& text);
    QString _extract_label_prefix (const QString& text, const QString& name);
    QString _extract_label_suffix (const QString& text);



public:
    definitions(variables_type&, range_type&, QString&);
    ~definitions(){}

    inline QString side() const {return _side;}
    inline QString tables() const {return _tables;}
    inline QString out() const {return _out;}
    inline QString filename() const {return _filename;}

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

};

#endif // DEFINITIONS_H
