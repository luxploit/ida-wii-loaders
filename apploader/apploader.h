#pragma once

#include "../loader/idaloader.h"

typedef struct {
    uint8_t revision[16];
    uint32_t entryPoint;
    int32_t size;
    int32_t trailerSize;
    int8_t padding[4];
} apploader_header;