#pragma once

#include "driver.h"

#define MM_COPY_MEMORY_PHYSICAL             0x1

typedef struct _MM_COPY_ADDRESS {
	union {
		PVOID            VirtualAddress;
		PHYSICAL_ADDRESS PhysicalAddress;
	};
} MM_COPY_ADDRESS, * PMMCOPY_ADDRESS;

extern "C" NTSTATUS NTAPI MmCopyMemory(
	PVOID           TargetAddress,
	MM_COPY_ADDRESS SourceAddress,
	SIZE_T          NumberOfBytes,
	ULONG           Flags,
	PSIZE_T         NumberOfBytesTransferred
);

template <typename T>
NTSTATUS read_physical(uint64_t pa, T& value) {
    MM_COPY_ADDRESS src;
    src.PhysicalAddress.QuadPart = pa;

    SIZE_T bytes;
    NTSTATUS status = MmCopyMemory(&value, src, sizeof(T), MM_COPY_MEMORY_PHYSICAL, &bytes);

    if (!NT_SUCCESS(status) || bytes != sizeof(T)) {
        return STATUS_UNSUCCESSFUL;
    }

    return STATUS_SUCCESS;
}