/*
*  IDA Nintendo GameCube rel Loader Module
*  (C) Copyright 2013 Stephen Simpson
*
*/

#include "rel.h"
#include "rel_track.h"


/*-----------------------------------------------------------------
*
*   Check if input file can be a rel file. The supposed header
*   is checked for sanity. If so return and fill in the formatname
*   otherwise return 0
*
*/

int idaapi accept_file(qstring *fileFormatName, qstring *processor, linput_t *li, const char *filename)
{
  rel_track test_valid(li);

  // Check if valid
  if (!test_valid.is_good())
    return 0;

  // file has passed all sanity checks and might be a rel
  fileFormatName->sprnt("Nintendo REL");
  processor->sprnt("PPC");

  return(ACCEPT_FIRST | 0xD07);
}



/*-----------------------------------------------------------------
*
*   File was recognised as rel and user has selected it.
*   Now load it into the database
*
*/

void idaapi load_file(linput_t *fp, ushort neflag, const char * /*fileformatname*/)
{
    msg("---------------------------------------\n");
    msg("Nintendo REL Loader Plugin 0.1\n");
    msg("---------------------------------------\n");

    // We need PowerPC support to do anything with rels
    set_processor_type("ppc:PAIRED", SETPROC_LOADER);

    // setup 32-bit more for hexppc64 (crd: emoose)
    EAH.setup(false);
    inf_set_64bit(false);

    // Set lis+addi resolution to aggressive
    int lisres = 1;
    PH.set_idp_options("PPC_LISOFF", IDPOPT_BIT, &lisres);

    set_compiler_id(COMP_GNU);

    rel_track track(fp);
    inf_set_start_ea(track.get_base_address());

    // map selector 1 to 0
    set_selector(1, 0);

    track.apply_patches();
}

/*-----------------------------------------------------------------
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
