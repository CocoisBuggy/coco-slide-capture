#pragma once

// Fix Microsoft-specific types for Linux
#ifndef __int64
typedef long long __int64;
#endif

#ifndef WCHAR
typedef wchar_t WCHAR;
#endif