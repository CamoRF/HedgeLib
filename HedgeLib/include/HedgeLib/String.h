#pragma once
#include "HedgeLib.h"
#include "Errors.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#else
#include <stdbool.h>
#endif

#ifdef _WIN32
typedef wchar_t hl_NativeChar;
#define HL_NATIVE_TEXT(str) L##str
#else
typedef char hl_NativeChar;
#define HL_NATIVE_TEXT(str) str
#endif

#define hl_NativeStr hl_NativeChar*
#define HL_CREATE_NATIVE_STR(len) ((hl_NativeStr)\
    malloc((len) * sizeof(hl_NativeChar)))

#define HL_TOLOWERASCII(c) ((c >= '[' || c <= '@') ? c : (c + 32))

HL_API extern const char* const hl_EmptyString;

#ifdef _WIN32
HL_API extern const hl_NativeStr const hl_EmptyStringNative;
#else
#define hl_EmptyStringNative hl_EmptyString
#endif

HL_API size_t hl_StringEncodeCodepointUTF8(
    char* u8str, uint32_t codepoint);

HL_API size_t hl_StringEncodeCodepointUTF16(
    uint16_t* u16str, uint32_t codepoint);

HL_API size_t hl_StringDecodeCodepointUTF8(
    const char* u8str, uint32_t* codepoint);

HL_API size_t hl_StringDecodeCodepointUTF16(
    const uint16_t* u16str, uint32_t* codepoint);

inline size_t hl_StrLen(const char* str)
{
    return strlen(str);
}

inline size_t hl_StrLenNative(const hl_NativeStr str)
{
#ifdef _WIN32
    return wcslen(str);
#else
    return strlen(str);
#endif
}

HL_API bool hl_StringsEqualInvASCII(const char* str1, const char* str2);
HL_API bool hl_StringsEqualInvASCIINative(
    const hl_NativeStr str1, const hl_NativeStr str2);

inline size_t hl_StringGetReqUTF8UnitCount(uint32_t cp)
{
    // Return the amount of code units required to store the given
    // codepoint, or 3 if the codepoint is invalid. (Invalid characters
    // are replaced with 0xFFFD which requires 3 units to encode)
    return (cp < 0x80) ? 1 : (cp < 0x800) ? 2 :
        (cp < 0x10000) ? 3 : (cp < 0x110000) ? 4 : 3;
}

inline size_t hl_StringGetReqUTF16UnitCount(uint32_t cp)
{
    // Return 2 if the given codepoint is not in the BMP, otherwise 1.
    // (Invalid characters are replaced with 0xFFFD which is in the BMP)
    return (cp >= 0x10000 && cp < 0x110000) ? 2 : 1;
}

HL_API size_t hl_StringGetReqUTF16BufferCountUTF8(
    const char* str, size_t HL_DEFARG(len, 0));

inline size_t hl_StringGetReqNativeBufferCountUTF8(
    const char* str, size_t HL_DEFARG(len, 0))
{
#ifdef _WIN32
    return hl_StringGetReqUTF16BufferCountUTF8(str, len);
#else
    return (len) ? len : (strlen(str) + 1);
#endif
}

HL_API size_t hl_StringGetReqUTF8BufferCountUTF16(
    const uint16_t* str, size_t HL_DEFARG(len, 0));

HL_API size_t hl_StringGetReqUTF8BufferCountCP932(
    const char* str, size_t HL_DEFARG(len, 0));

HL_API size_t hl_StringGetReqUTF16BufferCountCP932(
    const char* str, size_t HL_DEFARG(len, 0));

HL_API size_t hl_StringGetReqCP932BufferCountUTF8(
    const char* str, size_t HL_DEFARG(len, 0));

HL_API size_t hl_StringGetReqCP932BufferCountUTF16(
    const uint16_t* str, size_t HL_DEFARG(len, 0));

inline size_t hl_StringGetReqNativeBufferCountUTF16(
    const uint16_t* str, size_t HL_DEFARG(len, 0))
{
#ifdef _WIN32
    return (len) ? (wcslen(reinterpret_cast<const wchar_t*>(str)) + 1) : len;
#else
    return hl_StringGetReqUTF8BufferCountUTF16(str, len);
#endif
}

inline bool hl_StringsEqualNative(const hl_NativeStr str1, const hl_NativeStr str2)
{
#ifdef _WIN32
    return !wcscmp(str1, str2);
#else
    return !strcmp(str1, str2);
#endif
}

HL_API enum HL_RESULT hl_StringConvertUTF8ToUTF16(
    const char* u8str, uint16_t** u16str, size_t HL_DEFARG(u8bufLen, 0));

HL_API enum HL_RESULT hl_StringConvertUTF8ToCP932(
    const char* u8str, char** cp932str, size_t HL_DEFARG(u8bufLen, 0));

HL_API enum HL_RESULT hl_StringConvertUTF16ToCP932(
    const uint16_t* u16str, char** cp932str, size_t HL_DEFARG(u16bufLen, 0));

HL_API enum HL_RESULT hl_StringConvertUTF8ToNative(
    const char* u8str, hl_NativeStr* nativeStr, size_t HL_DEFARG(u8bufLen, 0));

HL_API enum HL_RESULT hl_StringConvertUTF16ToUTF8(
    const uint16_t* u16str, char** u8str, size_t HL_DEFARG(u16bufLen, 0));

HL_API enum HL_RESULT hl_StringConvertUTF16ToNative(
    const uint16_t* u16str, hl_NativeStr* nativeStr, size_t HL_DEFARG(u16bufLen, 0));

HL_API enum HL_RESULT hl_StringConvertCP932ToUTF8(
    const char* cp932str, char** u8str, size_t HL_DEFARG(cp932bufLen, 0));

HL_API enum HL_RESULT hl_StringConvertCP932ToUTF16(
    const char* cp932str, uint16_t** u16str, size_t HL_DEFARG(cp932bufLen, 0));

HL_API enum HL_RESULT hl_StringConvertCP932ToNative(
    const char* cp932str, hl_NativeStr* nativeStr, size_t HL_DEFARG(cp932bufLen, 0));

HL_API enum HL_RESULT hl_NativeStrBiggerCopy(const hl_NativeStr str,
    size_t extraLen, hl_NativeStr* newStr);

#ifdef __cplusplus
}

// Windows-specific overloads
#ifdef _WIN32
inline size_t hl_StrLen(const hl_NativeStr str)
{
    return wcslen(str);
}

inline bool hl_StringsEqualInvASCII(
    const hl_NativeStr str1, const hl_NativeStr str2)
{
    return hl_StringsEqualInvASCIINative(str1, str2);
}
#endif
#endif
