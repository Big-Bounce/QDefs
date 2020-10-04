#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include "variable.h"
#include <vector>
#include <cmath>

typedef std::vector<variable> variables_type;
typedef std::pair<size_t, size_t> range_type;

struct delimiter {
    QString separator;
    int consec;
    int position;
    bool zero_length;
    delimiter(const QString& sep, int con): separator(sep), consec(con), position(-1), zero_length(false) {}
    delimiter(int pos): separator(), consec(-1), position(--pos), zero_length(true) {}
    bool is_valid() const {return (consec >= 0 && position < 0 && !zero_length) || (consec < 0 && position >= 0 && zero_length);}
};

struct infos {
    delimiter name_right;
    delimiter name_left;
    delimiter label_right;
    delimiter label_left;
    int block_size;
    bool append_mode;
    bool create_files;
    bool blocks_mode;
    infos(const delimiter& nr, const delimiter& nl, const delimiter& lr, const delimiter& ll, int bs, bool am, bool cf, bool bm):
        name_right(nr), name_left(nl), label_right(lr), label_left(ll), block_size(bs),
        append_mode(am), create_files(cf), blocks_mode(bm) {}
};


#endif // TYPEDEFS_H
