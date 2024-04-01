#include "mmu_paging/mmu_paging.h"

extern "C" NTSTATUS __cdecl DriverEntry(PDRIVER_OBJECT, PUNICODE_STRING) {
  int femboy = 0;

  VirtualAddress va;
  va.value = reinterpret_cast<uint64_t>(&femboy);

  uint64_t physical_address = translate_va(va);

  if (physical_address == STATUS_INVALID_ADDRESS) {
    Dbg("Failed to get physical address");
  } else {
    Dbg("Physical Address -> 0x%llx", physical_address);
  }

  return STATUS_SUCCESS;
}