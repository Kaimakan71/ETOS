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

    Error code if unsuccessful.

--*/

{
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
    // TODO: Don't use the firmware allocator.
    //

    Status = EfiAllocatePool(EfiLoaderData, Size, &Buffer);
    if (!NT_SUCCESS(Status)) {
        return NULL;
    }

    return Buffer;
}
