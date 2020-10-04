#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include "typedefs.h"
#include "extraction.h"

class definitions {

    variables_type& _variables;
    range_type& _current_range;
    infos& _inf;

    QString _side;
    QString _tables;
    QString _out;
    QString _out_blocks;
    QString _filename;
    QString _filename_blocks;

    inline QString _end_of_axis() const {return "/*----------------------------------------------\n";}
    inline void _out_side(const QString& text) {_out += text; _side += QString("/*%1").arg(text);}
    inline void _out_side_blocks(const QString& text) {_out_blocks += text; _side += QString("/*%1").arg(text);}

public:
    definitions(variables_type&, range_type&, infos&);
    ~definitions(){}

    inline QString side() const {return _side;}
    inline QString tables() const {return _tables;}
    inline QString out() const {return _out;}
    inline QString out_blocks() const {return _out_blocks;}
    inline QString filename() const {return _filename;}
    inline QString filename_blocks() const {return _filename_blocks;}

    inline bool is_out_empty() const {return _out.isEmpty();}
    inline bool is_out_blocks_empty() const {return _out_blocks.isEmpty();}
    inline bool is_filename_empty() const {return _filename.isEmpty();}
    inline bool is_filename_blocks_empty() const {return _filename_blocks.isEmpty();}

    void axis_nonblocks();
    void axis_scale_nonblocks();
    void multi_nonblocks();
    void multi_codes_nonblocks();
    void include_nonblocks();
    void include_scale_nonblocks();
    void boxes_nonblocks();
    void means_nonblocks();
    void grid_nonblocks();

    void axis_blocks();
    void axis_scale_blocks();
    void multi_blocks();
    void multi_codes_blocks();
    void include_blocks();
    void include_scale_blocks();
    void boxes_blocks();
    void means_blocks();
    void grid_blocks();

    void structure();

    void axis();
    void axis_scale();
    void multi();
    void multi_codes();
    void include();
    void include_scale();
    void boxes();
    void means();
    void grid();



};

#endif // DEFINITIONS_H
