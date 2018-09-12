#pragma once
#include "apploader.h"

class apploader_track
{
public:
    apploader_track();
    apploader_track(linput_t *p_input);

    bool is_good() const;

    apploader_header header;

private:
    bool read_header();
    bool validate_header();

    bool m_valid;
    linput_t* m_input_file;
    uint32_t m_file_size;
};

