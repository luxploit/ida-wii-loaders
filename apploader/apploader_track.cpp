#include "apploader.h"
#include "apploader_track.h"

apploader_track::apploader_track() : m_valid(false) { }

apploader_track::apploader_track(linput_t *p_input) : m_valid(false), m_file_size(qlsize(p_input)), m_input_file(p_input)
{
    // Read the header
    if (!this->read_header()) {
        err_msg("Apploader: failed to read the header");
        return;
    }

    // Verify the header is sane
    if (!this->validate_header()) {
        err_msg("Apploader: the header verification failed");
        return;
    }
}

bool apploader_track::read_header()
{
    if (m_file_size <= sizeof(apploader_header))
        return err_msg("Apploader: file is too short to be valid");

    // Read the Apploader header
    apploader_header t_header;
    qlseek(m_input_file, 0, SEEK_SET);

    if (qlread(m_input_file, &t_header, sizeof(apploader_header)) != sizeof(apploader_header))
        return err_msg("Apploader: file is inaccessible");

    // Set header
    header = t_header;

    // Swap the endianness of entries
    header.entryPoint = swap32(header.entryPoint);
    header.size = swap32(header.size);
    header.trailerSize = swap32(header.trailerSize);
    msg("Revision date: %s\n", header.revision);
    msg("Entry Point: %08X\n", header.entryPoint);

    uint32_t totalSize = header.size + header.trailerSize + sizeof(apploader_header);
    if (m_file_size < totalSize) {
        return err_msg("Apploader: file is not long enough (size = %08X) for the specified size & trailerSize (%08X).\n" \
            "header.size = %08X header.trailerSize = %08X", m_file_size, totalSize, header.size, header.trailerSize);
    }

    if (header.entryPoint < 0x81200000 || header.entryPoint >= 0x81800000)
        return err_msg("Apploader: entry point isn't valid! Entry Point: %08X", header.entryPoint);

    m_valid = true;

    return true;
}

bool apploader_track::validate_header()
{
    if (!m_valid)
        return err_msg("Apploader: file is invalid! Entry point address: %X", header.entryPoint);

    return m_valid;
}

bool apploader_track::is_good() const
{
    return m_valid;
}