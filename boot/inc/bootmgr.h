/*++

Copyright (c) 2025, Quinn Stephens.
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    bootmgr.h

Abstract:

    Provides boot manager definitions.

--*/

#pragma once

#ifndef _BOOTMGR_H
#define _BOOTMGR_H

#include "bootlib.h"

NTSTATUS
BmpLaunchBootEntry (
    IN  PBOOT_APPLICATION_ENTRY BootEntry,
    OUT PULONG                  EntryIndex,
    IN  ULONG                   LaunchCode,
    IN  BOOLEAN                 AllowRecovery
    );

NTSTATUS
BmTransferExecution (
    IN  PBOOT_APPLICATION_ENTRY BootEntry,
    OUT PULONG                  LaunchCode,
    OUT PBOOLEAN                CanRecover
    );

NTSTATUS
BmMain (
    IN PBOOT_APPLICATION_PARAMETERS ApplicationParameters
    );

#endif /* !_BOOTMGR_H */
