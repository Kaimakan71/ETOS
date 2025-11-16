/*++

Copyright (c) 2025, Quinn Stephens.
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    mem.c

Abstract:

    Memory manipulation routines.

--*/

#include <string.h>

void *
memset (
    void *s,
    int c,
    size_t n
    )

{
    void *ptr = s;

    while (n--) {
        *(char *)s = (char)c;
        s = (char *)s + 1;
    }

    return ptr;
}

void *
memcpy (
    void *dest,
    const void *src,
    size_t n
    )

{
    void *ptr = dest;

    while (n--) {
        *(char *)dest = *(char *)src;
        dest = (char *)dest + 1;
        src = (char *)src + 1;
    }

    return ptr;
}

void *
memmove (
    void *dest,
    const void *src,
    size_t n
    )

{
    void *ptr = dest;

    /* Check for overlap */
    if (src > dest || ((char *)src + n) < (char *)dest) {
        /* Low-to-high copy */
        return memcpy(dest, src, n);
    }

    /* High-to-low copy */
    dest = (char *)dest + n;
    src = (char *)src + n;
    while (n--) {
        dest = (char *)dest - 1;
        src = (char *)src - 1;
        *(char *)dest = *(char *)src;
    }

    return ptr;
}
