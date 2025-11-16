/*++

Copyright (c) 2025, Quinn Stephens.
All rights reserved.
Provided under the BSD 3-Clause license.

Module Name:

    wprintf.c

Abstract:

    Wide formatted string printing routines.

--*/

#include <wchar.h>

static
size_t
print_hex (
    wchar_t *dest,
    size_t maxlen,
    unsigned int num
    )

{
    size_t n;
    int shift;
    unsigned int digit;

    n = 0;
    shift = 28;
    while (n < maxlen && shift >= 0) {
        digit = (num >> shift) & 0xf;
        if (digit >= 0xa) {
            *dest = 'a' + (digit - 0xa);
        } else {
            *dest = '0' + digit;
        }

        dest++;
        n++;
        shift -= 4;
    }

    return n;
}

static
size_t
print_dec (
    wchar_t *dest,
    size_t maxlen,
    unsigned int num
    )

{
    size_t n;
    unsigned int div, pad, digit;

    n = 0;
    div = 1000000000;
    pad = 1;
    while (n < maxlen && div > 0) {
        digit = num / div;
        num %= div;

        if (digit != 0) {
            pad = 0;
        }

        if (!pad || div == 1) {
            *dest++ = '0' + digit;
            n++;
        }

        div /= 10;
    }

    return n;
}

static
size_t
print_str (
    wchar_t *dest,
    size_t maxlen,
    wchar_t *str
    )

{
    size_t len;

    len = wcsnlen(str, maxlen);
    wmemcpy(dest, str, len);

    return len;
}

int
vswprintf_s (
    wchar_t *wcs,
    size_t maxlen,
    const wchar_t *format,
    va_list args
    )

{
    wchar_t *dest;
    size_t total, len;

    /* Validate arguments */
    if (wcs == NULL || maxlen == 0 || format == NULL) {
        return -1;
    }

    /* Reserve space for terminator */
    maxlen--;

    dest = wcs;
    total = 0;
    while (total < maxlen && *format != '\0') {
        if (*format != L'%') {
            *dest++ = *format++;
            total++;
            continue;
        }

        format++;
        switch (*format) {
        case 'x':
            len = print_hex(dest, maxlen - total, va_arg(args, unsigned int));
            dest += len;
            total += len;
            format++;
            break;
        case 'd':
            len = print_dec(dest, maxlen - total, va_arg(args, unsigned int));
            dest += len;
            total += len;
            format++;
            break;
        case 's':
            len = print_str(dest, maxlen - total, va_arg(args, wchar_t *));
            dest += len;
            total += len;
            format++;
            break;
        case L'\0':
            break;
        case '%':
        default:
            *dest++ = *format++;
            total++;
            break;
        }
    }

    wcs[total] = L'\0';
    return (int)total;
}
