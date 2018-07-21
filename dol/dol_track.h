#pragma once
#include "dol.h"

class dol_track
{
public:
    dol_track();
    dol_track(linput_t *p_input);

    bool is_good() const;

    dolhdr header;

private:
    bool read_header();
    bool validate_header();

    bool m_valid;
    linput_t* m_input_file;
    uint32_t m_file_size;
};

