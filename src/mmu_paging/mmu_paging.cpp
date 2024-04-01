#include "mmu_paging.h"

NTSTATUS fetch_page(uint64_t address, _MMPTE &page) {
  if (!NT_SUCCESS(read_physical({address}, page)) || !page.Valid) {
    Dbg("Failed to read page\n");
    return STATUS_INVALID_ADDRESS;
  }
  return STATUS_SUCCESS;
}

uint64_t translate_va(VirtualAddress va)  {
  CR3 cr3;
  cr3.value = __readcr3();

  uint64_t physical_address;

  _MMPTE pml4e;
  if (!NT_SUCCESS(fetch_page((cr3.fields.pml4_p << PAGE_SHIFT) +
                                 va.pml4_index * sizeof(uint64_t),
                             pml4e))) {
    return STATUS_INVALID_ADDRESS;
  }

  _MMPTE pdpte;
  if (!NT_SUCCESS(fetch_page((pml4e.PageFrameNumber << PAGE_SHIFT) +
                                 va.pdpt_index * sizeof(uint64_t),
                             pdpte))) {
    return STATUS_INVALID_ADDRESS;
  }

  // 1GB page
  if (pdpte.LargePage) {
    physical_address = (pdpte.PageFrameNumber << PFN_SHIFT_1GB) +
                       (va.pd_index << PD_SHIFT) + (va.pt_index << PAGE_SHIFT) +
                       va.offset;
    return physical_address;
  }

  _MMPTE pde;
  if (!NT_SUCCESS(fetch_page((pdpte.PageFrameNumber << PAGE_SHIFT) +
                                 va.pd_index * sizeof(uint64_t),
                             pde))) {
    return STATUS_INVALID_ADDRESS;
  }

  // 2MB page
  if (pde.LargePage) {
    physical_address = (pde.PageFrameNumber << PFN_SHIFT_2MB) +
                       (va.pt_index << PAGE_SHIFT) + va.offset;
    return physical_address;
  }

  _MMPTE pte;
  if (!NT_SUCCESS(fetch_page((pde.PageFrameNumber << PAGE_SHIFT) +
                                 va.pt_index * sizeof(uint64_t),
                             pte))) {
    return STATUS_INVALID_ADDRESS;
  }

  physical_address = (pte.PageFrameNumber << PAGE_SHIFT) + va.offset;

  return physical_address;
}