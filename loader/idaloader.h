#define USE_STANDARD_FILE_FUNCTIONS 1
#define __X64__ 1

#ifndef __IDA_LOADER_H__
#define __IDA_LOADER_H__

#include <ida.hpp>
#include <fpro.h>
#include <idp.hpp>
#include <loader.hpp>
#include <name.hpp>
#include <bytes.hpp>
#include <offset.hpp>
#include <segment.hpp>
#include <segregs.hpp>
#include <fixup.hpp>
#include <entry.hpp>
#include <auto.hpp>
#include <diskio.hpp>
#include <kernwin.hpp>
#include <nalt.hpp>
#include <typeinf.hpp>

#define CLASS_CODE    "CODE"
#define NAME_CODE     ".text"

#define CLASS_DATA    "DATA"
#define NAME_DATA     ".data"

#define CLASS_BSS     "BSS"
#define NAME_BSS      ".bss"

#define CLASS_EXTERN  "XTRN"
#define NAME_EXTERN   ".ref"

inline bool err_msg(const char *format, ...)
{
    va_list va;
    va_start(va, format);

    std::string fmt_nl = format;
    fmt_nl += "\n";

    int nbytes = vmsg(fmt_nl.c_str(), va);
    va_end(va);
    return false;
}

#endif //#ifndef __IDA_LOADER_H__
