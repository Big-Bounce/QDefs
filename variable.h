#ifndef VARIABLE_H
#define VARIABLE_H

#include <QString>
#include <list>
#include <tuple>

class variable {

public:
    typedef std::pair<QString, QString> code_type;
    typedef std::list<code_type> codes_type;

    QString _name;
    QString _label;
    QString _format;
    QString _start;
    QString _stop;
    codes_type _codes;
    bool _is_sorted;

public:
    variable() {}
    variable(const QString& name, const QString& label, const QString& format, const QString& start, const QString& stop);
    void add_code(const QString& code);
    QString get_info(unsigned);
    inline size_t codes_count() const {return _codes.size();}
    codes_type codes();

    inline QString name() const {return _name;}
    inline QString label() const {return  _label;}
    inline QString format() const {return  _format;}
    inline QString start() const {return  _start;}
    inline QString stop() const {return _stop;}
    QString second_last() const;
    QString width() const;

    class error {
         QString _what;
    public:
         error(const QString& what): _what(what) {}
         QString what() const {return _what;}         
    };

};

#endif // VARIABLE_H
