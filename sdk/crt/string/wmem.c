/*++

Copyright (c) 2025, Quinn Stephens.
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    wmem.c

Abstract:

    Wide-character memory manipulation routines.

--*/

#include <wchar.h>

wchar_t *
wmemset (
    wchar_t *wcs,
    wchar_t wc,
    size_t n
    )

{
    wchar_t *ptr = wcs;

    while (n--) {
        *wcs++ = wc;
    }

    return ptr;
}

wchar_t *
wmemcpy (
    wchar_t *dest,
    const wchar_t *src,
    size_t n
    )

{
    wchar_t *ptr = dest;

    while (n--) {
        *dest++ = *src++;
    }

    return ptr;
}

wchar_t *
wmemmove (
    wchar_t *dest,
    const wchar_t *src,
    size_t n
    )

{
    wchar_t *ptr = dest;

    /* Check for overlap */
    if (src > dest || ((wchar_t *)src + n) < (wchar_t *)dest) {
        /* Low-to-high copy */
        return wmemcpy(dest, src, n);
    }

    /* High-to-low copy */
    dest = (wchar_t *)dest + n;
    src = (wchar_t *)src + n;
    while (n--) {
        dest = (wchar_t *)dest - 1;
        src = (wchar_t *)src - 1;
        *(wchar_t *)dest = *(wchar_t *)src;
    }

    return ptr;
}
