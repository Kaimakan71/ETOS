/*++

Copyright (c) 2025, Quinn Stephens.
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    mm.h

Abstract:

    Provides memory manager definitions.

--*/

#pragma once

#ifndef _MM_H
#define _MM_H

#include "bootlib.h"

#define MAX_STATIC_DESCRIPTOR_COUNT 1024

#define ALIGN_UP(Value, Alignment) (((Value) + ((Alignment) - 1)) & ~((Alignment) - 1))

typedef struct {
    LIST_ENTRY BufferListEntry;
    LIST_ENTRY FreeListEntry;
} MM_FREE_HEAP_ENTRY, *PMM_FREE_HEAP_ENTRY;

typedef struct {
    LIST_ENTRY BufferListEntry;
    UCHAR      Buffer[ANYSIZE_ARRAY];
} MM_USED_HEAP_ENTRY, *PMM_USED_HEAP_ENTRY;

typedef struct {
    LIST_ENTRY ListEntry;
    ULONG_PTR  HeapEnd;
    ULONG_PTR  HeapLimit;
    ULONG_PTR  HeapBase;
    ULONG_PTR  HeapStart;
} MM_HEAP_BOUNDARY, *PMM_HEAP_BOUNDARY;

extern ULONG MmDescriptorCallTreeCount;
extern ULONG MmTranslationType;

extern MEMORY_DESCRIPTOR MmStaticMemoryDescriptors[MAX_STATIC_DESCRIPTOR_COUNT];
extern PMEMORY_DESCRIPTOR MmGlobalMemoryDescriptors;
extern ULONG MmGlobalMemoryDescriptorCount, MmGlobalMemoryDescriptorsUsed;

//
// Architecture services.
//

BOOLEAN
MmArchTranslateVirtualAddress (
    IN  PVOID             VirtualAddress,
    OUT PPHYSICAL_ADDRESS PhysicalAddress
    );

//
// Heap allocation services.
//

NTSTATUS
MmHapInitialize (
    IN ULONG MinimumHeapSize,
    IN ULONG AllocationAttributes
    );

NTSTATUS
MmHapHeapAllocatorExtend (
    IN ULONG_PTR HeapSize
    );

#endif /* !_MM_H */
