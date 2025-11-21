/*++

Copyright (c) 2025, Quinn Stephens.
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    table.c

Abstract:

    Table services.

--*/

#include "bootlib.h"

PVOID
BlTblFindEntry (
    IN  PVOID                  *Table,
    IN  ULONG                  EntryCount,
    OUT PULONG                 EntryIndex,
    IN  PTABLE_SEARCH_CALLBACK Callback,
    IN  PVOID                  Argument1,
    IN  PVOID                  Argument2,
    IN  PVOID                  Argument3,
    IN  PVOID                  Argument4
    )

/*++

Routine Description:

    Searches a table using a callback function.

Arguments:

    Table - Pointer to the first entry in the table.

    EntryCount - The number of table entries to search.

    EntryIndex - Pointer to a ULONG that receives the found entry's index.

    Callback - Pointer to a callback that determines if an entry should be selected.

    Argument[1-4] - Passed directly to the callback.

Return Value:

    The value of the entry if found.

    NULL if not found.

--*/

{
    //
    // Validate parameters.
    //
    if (Table == NULL || EntryIndex == NULL) {
        return NULL;
    }

    //
    // Search the table.
    //
    for (ULONG Index = 0; Index < EntryCount; Index++) {
        //
        // Skip empty entries.
        //
        if (Table[Index] == NULL) {
            continue;
        }

        //
        // Ask the callback what to do.
        //
        if (Callback(Table[Index], Argument1, Argument2, Argument3, Argument4)) {
            *EntryIndex = Index;
            return Table[Index];
        }
    }

    return NULL;
}

NTSTATUS
BlTblMap (
    IN PVOID               *Table,
    IN ULONG               EntryCount,
    IN PTABLE_MAP_CALLBACK Callback
    )

/*++

Routine Description:

    Maps a table using a callback function.

Arguments:

    Table - Pointer to the first entry in the table.

    EntryCount - The number of table entries to map.

    Callback - Pointer to a callback which is called for each entry in the table.

Return Value:

    STATUS_SUCCESS if successful.

    STATUS_INVALID_PARAMETER if Table is invalid.

    The most recent error code if unsuccessful.

--*/

{
    NTSTATUS Status, ReturnStatus;

    //
    // Validate table.
    //
    if (Table == NULL) {
        return STATUS_INVALID_PARAMETER;
    }

    //
    // Map the table.
    //
    ReturnStatus = STATUS_SUCCESS;
    for (ULONG Index = 0; Index < EntryCount; Index++) {
        //
        // Skip empty entries.
        //
        if (Table[Index] == NULL) {
            continue;
        }

        //
        // Call the callback.
        //
        Status = Callback(Table[Index], Index);
        if (!NT_SUCCESS(Status)) {
            ReturnStatus = Status;
        }
    }

    return ReturnStatus;
}
