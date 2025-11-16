/*++

Copyright (c) 2025, Quinn Stephens.
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    wstr.c

Abstract:

    Wide-character string manipulation routines.

--*/

#include <wchar.h>

size_t
wcslen (
    const wchar_t *s
    )

{
    const wchar_t *ptr;

    ptr = s;
    while (*ptr != L'\0') {
        ptr++;
    }

    return ptr - s;
}

size_t
wcsnlen (
    const wchar_t *s,
    size_t maxlen
    )

{
    const wchar_t *ptr;

    ptr = s;
    while (maxlen-- && *ptr != L'\0') {
        ptr++;
    }

    return ptr - s;
}

int
wcscmp (
    const wchar_t *s1,
    const wchar_t *s2
    )

{
    while (*s1 == *s2) {
        if (*s1 == L'\0') {
            return 0;
        }

        s1++;
        s2++;
    }

    return *s1 - *s2;
}

int
wcsncmp (
    const wchar_t *s1,
    const wchar_t *s2,
    size_t n
    )

{
    while (n > 0) {
        if (*s1 != *s2) {
            return *s1 - *s2;
        }

        if (*s1 == L'\0') {
            return 0;
        }

        n--;
        s1++;
        s2++;
    }

    return 0;
}

wchar_t *
wcschr (
    const wchar_t *wcs,
    wchar_t wc
    )

{
    while (*wcs != wc) {
        if (*wcs == L'\0') {
            return NULL;
        }

        wcs++;
    }

    return (wchar_t *)wcs;
}

wchar_t *
wcsstr (
    const wchar_t *haystack,
    const wchar_t *needle
    )

{
    const wchar_t *ptr = haystack;

    if (*needle == L'\0') {
        return (wchar_t *)haystack;
    }

    while ((ptr = wcschr(ptr, *needle)) != NULL) {
        if (wcscmp(ptr, needle) == 0) {
            return (wchar_t *)ptr;
        }
    }

    return NULL;
}

wchar_t *
wcscpy_s (
    wchar_t *dest,
    size_t maxlen,
    const wchar_t *src
    )

{
    for (size_t i = 0; i < maxlen; i++, src++) {
        dest[i] = *src;

        if (*src == L'\0') {
            break;
        }
    }

    return dest;
}

wchar_t *
wcscat_s (
    wchar_t *dest,
    size_t maxlen,
    const wchar_t *src
    )

{
    for (size_t i = wcsnlen(dest, maxlen); i < maxlen; i++, src++) {
        dest[i] = *src;

        if (*src == L'\0') {
            break;
        }
    }

    return dest;
}
