/*
 *  IDA Nintendo GameCube DOL Loader Module
 *  (C) Copyright 2004 by Stefan Esser
 *
 */

#include "../loader/idaloader.h"
#include "dol.h"
#include "dol_track.h"

/*--------------------------------------------------------------------------
 *
 *   Check if input file can be a DOL file. Therefore the supposed header
 *   is checked for sanity. If it passes return and fill in the formatname
 *   otherwise return 0
 *
 */

int idaapi accept_file(qstring *fileFormatName, qstring *processor, linput_t *li, const char *fileName)
{
    dol_track track(li);

    if (!track.is_good())
    {
        msg("File is not a Nintendo GameCube DOL!");
        return 0;
    }

    // file has passed all sanity checks and might be a DOL
    fileFormatName->sprnt("Nintendo GameCube DOL");
    processor->sprnt("PPC");

    return(ACCEPT_FIRST | 0xD07);
}



/*--------------------------------------------------------------------------
 *
 *   File was recognised as DOL and user has selected it. Now load it into
 *   the database
 *
 */

void idaapi load_file(linput_t *fp, ushort /*neflag*/, const char * /*fileformatname*/)
{
    msg("---------------------------------------\n");
    msg("Nintendo GameCube DOL Loader Plugin 0.1\n");
    msg("---------------------------------------\n");
  
    // We need PowerPC support to do anything with rels
    set_processor_type("ppc:PAIRED", setproc_level_t::SETPROC_LOADER);

    // Set lis+addi resolution to aggressive
    int lisres = 1;
    ph.set_idp_options("PPC_LISOFF", IDPOPT_BIT, &lisres);

    set_compiler_id(COMP_GNU);

    dol_track track(fp);

    // read DOL header into memory
    if (!track.is_good())
        qexit(1);
  
    // every journey has a beginning
    inf.start_ea = inf.start_ip = track.header.entrypoint;

    // map selector 1 to 0
    set_selector(1, 0);

    // create all code segments
    for (uint i=0, snum=1; i < 7; i++, snum++) {
    qstring buf;
    
    // 0 == no segment
    if (track.header.addressText[i] == 0)
        continue;
    
    // create a name according to segmenttype and number
    buf.sprnt(NAME_CODE "%u", snum);
    
    // add the code segment
    if (!add_segm(1, track.header.addressText[i], track.header.addressText[i] + track.header.sizeText[i], buf.c_str(), CLASS_CODE))
        qexit(1);
    
    // set addressing to 32 bit
    set_segm_addressing(getseg(track.header.addressText[i]), 1);

    // and get the content from the file
    file2base(fp, track.header.offsetText[i], track.header.addressText[i], track.header.addressText[i ]+ track.header.sizeText[i], FILEREG_PATCHABLE);
  }

  // create all data segments
  for (uint i = 0, snum = 1; i < 11; i++, snum++) {
    qstring buf;

    // 0 == no segment
    if (track.header.addressData[i] == 0)
        continue;

    // create a name according to segmenttype and number
    buf.sprnt(NAME_DATA "%u", snum);

    // add the data segment
    if (!add_segm(1, track.header.addressData[i], track.header.addressData[i] + track.header.sizeData[i], buf.c_str(), CLASS_DATA))
        qexit(1);
    
    // set addressing to 32 bit
    set_segm_addressing(getseg(track.header.addressData[i]), 1);

    // and get the content from the file
    file2base(fp, track.header.offsetData[i], track.header.addressData[i], track.header.addressData[i] + track.header.sizeData[i], FILEREG_PATCHABLE);
  }

  // is there a BSS defined?
  if (track.header.addressBSS != NULL) {
    // then add it
    if(!add_segm(1, track.header.addressBSS, track.header.addressBSS + track.header.sizeBSS, NAME_BSS, CLASS_BSS))
        qexit(1);

    // and set addressing mode to 32 bit
    set_segm_addressing(getseg(track.header.addressBSS), 1);
  }
}

/*--------------------------------------------------------------------------
 *
 *   Loader Module Descriptor Blocks
 *
 */

extern "C" loader_t LDSC = {
  IDP_INTERFACE_VERSION,
  0, /* no loader flags */
  accept_file,
  load_file,
  NULL,
};
