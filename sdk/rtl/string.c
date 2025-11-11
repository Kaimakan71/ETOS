/*++

Copyright (c) 2025, Quinn Stephens.
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    string.c

Abstract:

    RTL string routines.

--*/

#include <nt.h>
#include <ntrtl.h>
#include <wchar.h>

VOID
NTAPI
RtlInitAnsiString (
    OUT PANSI_STRING Destination,
    IN  PCSTR        Source
    )

/*++

Routine Description:

    Initializes an ANSI string structure with an existing string.

Arguments:

    Destination - Pointer to the ANSI string structure.

    Source - Pointer to a NULL-terminated string or NULL.

Return Value:

    None.

--*/

{
    ULONG Length;

    Destination->Buffer = (PSTR)Source;
    if (Source == NULL) {
        Destination->Length = 0;
        Destination->MaximumLength = 0;
        return;
    }

    Length = strlen(Source);
    if (Length >= ANSI_STRING_MAX_BYTES) {
        Length = ANSI_STRING_MAX_BYTES - sizeof(ANSI_NULL);
    }

    Destination->Length = (USHORT)Length;
    Destination->MaximumLength = (USHORT)(Length + sizeof(ANSI_NULL));
}

VOID
NTAPI
RtlInitUnicodeString (
    OUT PUNICODE_STRING Destination,
    IN  PCWSTR          Source
    )

/*++

Routine Description:

    Initializes a Unicode string structure with an existing string.

Arguments:

    Destination - Pointer to the Unicode string structure.

    Source - Pointer to a NULL-terminated string or NULL.

Return Value:

    None.

--*/

{
    ULONG Length;

    Destination->Buffer = (PWSTR)Source;
    if (Source == NULL) {
        Destination->Length = 0;
        Destination->MaximumLength = 0;
        return;
    }

    Length = wcslen(Source) * sizeof(WCHAR);
    if (Length >= UNICODE_STRING_MAX_BYTES) {
        Length = UNICODE_STRING_MAX_BYTES - sizeof(UNICODE_NULL);
    }

    Destination->Length = (USHORT)Length;
    Destination->MaximumLength = (USHORT)(Length + sizeof(UNICODE_NULL));
}

NTSTATUS
NTAPI
RtlAnsiStringToUnicodeString (
    OUT PUNICODE_STRING DestinationString,
    IN  PCANSI_STRING   SourceString,
    IN  BOOLEAN         AllocateDestinationString
    )

/*++

Routine Description:

    Initializes an ANSI string structure with an existing string.

Arguments:

    DestinationString - Pointer to the Unicode string structure.

    SourceString - Pointer to the ANSI string structure.

    AllocateDestinationString - Whether or not to allocate a buffer for the destination string.

Return Value:

    STATUS_SUCCESS if successful.

    STATUS_INVALID_PARAMETER_2 is SourceString is invalid.

    STATUS_NOT_IMPLEMENTED if AllocateDestinationString is TRUE.

--*/

{
    ULONG TotalLength;

    TotalLength = (SourceString->Length * sizeof(WCHAR)) + sizeof(UNICODE_NULL);
    if (TotalLength > UNICODE_STRING_MAX_BYTES) {
        return STATUS_INVALID_PARAMETER_2;
    }

    if (AllocateDestinationString) {
        DestinationString->MaximumLength = TotalLength;

        //
        // TODO: Implement buffer allocation.
        //
        DestinationString->Buffer = NULL;

        return STATUS_NOT_IMPLEMENTED;
    } else {
        if (TotalLength > DestinationString->MaximumLength) {
            return STATUS_BUFFER_OVERFLOW;
        }
    }

    DestinationString->Length = TotalLength - sizeof(UNICODE_NULL);
    if (SourceString->Length > 0) {
        RtlCopyMemory(DestinationString->Buffer, SourceString->Buffer, DestinationString->Length);
    }

    if (DestinationString->Length < DestinationString->MaximumLength) {
        DestinationString->Buffer[SourceString->Length] = UNICODE_NULL;
    }

    return STATUS_SUCCESS;
}
