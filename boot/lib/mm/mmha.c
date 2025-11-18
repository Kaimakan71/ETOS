/*++

Copyright (c) 2025, Quinn Stephens.
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    mmha.c

Abstract:

    Heap allocation services.

--*/

#include "efilib.h"
#include "mm.h"

ULONG HapInitializationStatus = 0;
PLIST_ENTRY MmFreeList;
ULONG HapMinimumHeapSize;
ULONG HapAllocationAttributes;
LIST_ENTRY MmHeapBoundaries;

NTSTATUS
BlMmFreeHeap (
    IN PVOID Pointer
    )

/*++

Routine Description:

    Frees allocated heap memory.

Arguments:

    Pointer - Pointer to the memory to free.

Return Value:

    STATUS_SUCCESS if successful.

    STATUS_UNSUCCESSFUL if the heap allocator is not initialized.

    Error code if unsuccessful.

--*/

{
    //
    // The heap allocator must be initialized.
    //
    if (HapInitializationStatus != 1) {
        return STATUS_UNSUCCESSFUL;
    }

    //
    // TODO: Don't use the firmware allocator.
    //

    return EfiFreePool(Pointer);
}

PVOID
BlMmAllocateHeap (
    IN ULONG_PTR Size
    )

/*++

Routine Description:

    Allocates memory on the heap.

Arguments:

    Size - The size, in bytes, to allocate.

Return Value:

    Pointer to the allocated memory, if successful.

    NULL if unsuccessful.

--*/

{
    NTSTATUS Status;
    PVOID Buffer;

    //
    // The heap allocator must be initialized.
    //
    if (HapInitializationStatus != 1) {
        return NULL;
    }

    //
    // TODO: Don't use the firmware allocator.
    //

    Status = EfiAllocatePool(EfiLoaderData, Size, &Buffer);
    if (!NT_SUCCESS(Status)) {
        return NULL;
    }

    return Buffer;
}

NTSTATUS
MmHapHeapAllocatorExtend (
    IN ULONG_PTR HeapSize
    )

/*++

Routine Description:

    Extends the heap to the desired size.

Arguments:

    HeapSize - The requested heap size.

Return Value:

    STATUS_SUCCESS if successful.

    STATUS_INTEGER_OVERFLOW if an integer overflow occurs.

--*/

{
    NTSTATUS Status;
    PMM_HEAP_BOUNDARY HeapBoundary;
    ULONG_PTR HeapLimit;

    //
    // Add space for extra data.
    //
    Status = RtlSizeTAdd(HeapSize, 2 * PAGE_SIZE, &HeapSize);
    if (!NT_SUCCESS(Status)) {
        return Status;
    }

    //
    // Apply the minimum heap size restriction.
    //
    if (HeapSize < HapMinimumHeapSize) {
        HeapSize = HapMinimumHeapSize;
    }

    //
    // Round to the page size.
    //
    HeapSize = ALIGN_UP(HeapSize, PAGE_SIZE);
    if (HeapSize == 0) {
        return STATUS_INTEGER_OVERFLOW;
    }

    //
    // Check if a heap already exists.
    //
    if (!IsListEmpty(&MmHeapBoundaries)) {
        HeapBoundary = CONTAINING_RECORD(MmHeapBoundaries.Flink, MM_HEAP_BOUNDARY, ListEntry);
        HeapLimit = HeapBoundary->HeapLimit + PAGE_SIZE;
        if (HeapLimit <= HeapBoundary->HeapEnd) {
            DebugError(L"Existing heap expansion not implemented\r\n");
            return STATUS_NOT_IMPLEMENTED;
        }
    }

    //
    // Allocate a new heap.
    //
    // Status = BlMmAllocatePagesInRange(
    //     &HeapBase,
    //     HeapSize >> PAGE_SHIFT,
    //     BlLoaderHeap,
    //     HapAllocationAttributes,
    //     0,
    //     0
    // );

    return STATUS_SUCCESS;
}

NTSTATUS
MmHapInitialize (
    IN ULONG MinimumHeapSize,
    IN ULONG AllocationAttributes
    )

/*++

Routine Description:

    Allocates memory on the heap.

Arguments:

    MinimumHeapSize - The minimum size of the heap.

    AllocationAttributes - Flags to refer to when allocating heap memory.

Return Value:

    STATUS_SUCCESS if successful.

    Any other status value returned by MmHapHeapAllocatorExtend.

--*/

{
    NTSTATUS Status;

    DebugInfo(L"Initializing heap allocator...\r\n");

    //
    // Initialize heap state.
    //
    MmFreeList = NULL;
    HapMinimumHeapSize = ALIGN_UP(MinimumHeapSize, PAGE_SIZE);
    HapAllocationAttributes = AllocationAttributes & 0x20000;
    InitializeListHead(&MmHeapBoundaries);

    //
    // Expand the heap.
    //
    Status = MmHapHeapAllocatorExtend(sizeof(PVOID));
    if (!NT_SUCCESS(Status)) {
        return Status;
    }

    HapInitializationStatus = 1;
    return STATUS_SUCCESS;
}
