#pragma once

#include "../memutil.h"
#include "ntdef.h"
#include <cstdint>

constexpr uint64_t PDPT_SHIFT = 12;
constexpr uint64_t PD_SHIFT = 21;
constexpr uint64_t PFN_SHIFT_1GB = 30;
constexpr uint64_t PFN_SHIFT_2MB = 21;

union VirtualAddress {
  uint64_t value;
  struct {
    uint64_t offset : 12;
    uint64_t pt_index : 9;
    uint64_t pd_index : 9;
    uint64_t pdpt_index : 9;
    uint64_t pml4_index : 9;
    uint64_t reserved : 16;
  };
};

struct _MMPTE {
  ULONGLONG Valid : 1;
  ULONGLONG Dirty1 : 1;
  ULONGLONG Owner : 1;
  ULONGLONG WriteThrough : 1;
  ULONGLONG CacheDisable : 1;
  ULONGLONG Accessed : 1;
  ULONGLONG Dirty : 1;
  ULONGLONG LargePage : 1;
  ULONGLONG Global : 1;
  ULONGLONG CopyOnWrite : 1;
  ULONGLONG Unused : 1;
  ULONGLONG Write : 1;
  ULONGLONG PageFrameNumber : 36;
  ULONGLONG ReservedForHardware : 4;
  ULONGLONG ReservedForSoftware : 4;
  ULONGLONG WsleAge : 4;
  ULONGLONG WsleProtection : 3;
  ULONGLONG NoExecute : 1;
};

typedef union _CR3 {
  uint64_t value;
  struct {
    uint64_t ignored_1 : 3;     // Ignored
    uint64_t write_through : 1; // Write-through caching policy
    uint64_t cache_disable : 1; // Cache disable
    uint64_t ignored_2 : 7;     // Ignored
    uint64_t pml4_p : 40;       // Base address of the PML4T
    uint64_t reserved : 12;     // Reserved for future use
  } fields;
} CR3;

// export
uint64_t translate_va(VirtualAddress va);