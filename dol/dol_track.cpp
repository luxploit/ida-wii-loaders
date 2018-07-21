#include "dol_track.h"

dol_track::dol_track() : m_valid(false) { }

dol_track::dol_track(linput_t *p_input) : m_valid(false), m_file_size(qlsize(p_input)), m_input_file(p_input)
{
    // Read the header
    if (!this->read_header()) {
        err_msg("DOL: failed to read the header");
        return;
    }

    // Verify the header is sane
    if (!this->validate_header()) {
        err_msg("DOL: the header verification failed");
        return;
    }
}

bool dol_track::read_header()
{
    // If the file size is less than 0x100 bytes, it's probably not a DOL file.
    if (m_file_size < 0x100)
        return err_msg("DOL: file is too short to be valid");

    dolhdr base_header;
    // Read the DOL header
    qlseek(m_input_file, 0, SEEK_SET);
    if (qlread(m_input_file, &base_header, sizeof(dolhdr)) != sizeof(dolhdr))
        return err_msg("DOL: header is too short or file is inaccessible");

    // Swap endianness
    for (int i = 0; i<7; i++) {
        base_header.offsetText[i] = swap32(base_header.offsetText[i]);
        base_header.addressText[i] = swap32(base_header.addressText[i]);
        base_header.sizeText[i] = swap32(base_header.sizeText[i]);
    }
    for (int i = 0; i<11; i++) {
        base_header.offsetData[i] = swap32(base_header.offsetData[i]);
        base_header.addressData[i] = swap32(base_header.addressData[i]);
        base_header.sizeData[i] = swap32(base_header.sizeData[i]);
    }
    base_header.entrypoint = swap32(base_header.entrypoint);
    base_header.sizeBSS = swap32(base_header.sizeBSS);
    base_header.addressBSS = swap32(base_header.addressBSS);

    // Set header
    header = base_header;
    return true;
}

bool dol_track::validate_header()
{
    // now perform some sanitychecks
    for (int i = 0; i < 7; i++) {

        // DOL segment MAY NOT physically stored in the header
        if (header.offsetText[i] != 0 && header.offsetText[i] < 0x100)
            return err_msg("DOL: .text segment %u is invalid, as it is located within the header", i);
        // end of physical storage must be within file
        if (header.offsetText[i] + header.sizeText[i] > m_file_size) 
            return err_msg("DOL: .text segnment %u is invalid, as it is located past the end of the file", i);
        // we only accept DOLs with segments above 2GB
        if (header.addressText[i] != 0 && !(header.addressText[i] & 0x80000000)) 
            return err_msg("DOL: .text segment %u is invalid, as it isn't in a valid address space of >= 0x80000000 (was %X)", i, header.addressText[i]);

        // remember that entrypoint was in a code segment
        if (header.entrypoint >= header.addressText[i] && header.entrypoint < header.addressText[i] + header.sizeText[i])
            m_valid = true;
    }

    if (!m_valid)
        return err_msg("DOL: entry point does not appear to be in a .text segment! Entry point address: %X", header.entrypoint);

    for (int i = 0; i < 11; i++) {

        // DOL segment MAY NOT physically stored in the header
        if (header.offsetData[i] != 0 && header.offsetData[i] < 0x100)
            return err_msg("DOL: .data segement %u is invalid, as it is located within the header", i);
        // end of physical storage must be within file
        if (header.offsetData[i] + header.sizeData[i] > m_file_size)
            return err_msg("DOL: .data segnment %u is invalid, as it is located past the end of the file", i);
        // we only accept DOLs with segments above 2GB
        if (header.addressData[i] != 0 && !(header.addressData[i] & 0x80000000))
            return err_msg("DOL: .data segment %u is invalid, as it isn't in a valid address space of >= 0x80000000 (was %X)", i, header.addressData[i]);
    }

    // if there is a BSS segment it must be above 2GB, too
    if (header.addressBSS != 0 && !(header.addressBSS & 0x80000000)) 
        return err_msg("DOL: There appears to be a .bss segment, but its address isn't in a valid address space of >= 0x80000000 (was %X)", header.addressBSS);
}

bool dol_track::is_good() const
{
    return m_valid;
}