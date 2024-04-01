#pragma once

#include "ntdef.h"
#include "ntddk.h"
#include <cstdint>
#include <intrin.h>

#define Dbg(fmt, ...) \
    DbgPrintEx(0, 0, "[fv] " fmt "\n", __VA_ARGS__)