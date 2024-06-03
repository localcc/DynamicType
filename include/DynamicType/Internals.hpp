#pragma once

#define DT_INTERNAL_CONSTRUCTOR [[deprecated("Do not call this. The instance you get will corrupt the stack. This should only be called by library internals")]]

#if defined(_MSC_VER) && !defined(__clang__) && !defined(__GNUC__)
#define DT_INTERNAL_PRE __pragma(warning(push)) __pragma(warning(disable : 4996))
#define DT_INTERNAL_POST __pragma(warning(pop))
#elif defined(__clang__)
#define DT_INTERNAL_PRE _Pragma("clang diagnostic push") _Pragma("clang diagnostic ignored -Wdeprecated-declarations")
#define DT_INTERNAL_POST _Pragma("clang diagnostic pop")
#elif defined(__GNUC__)
#define DT_INTERNAL_PRE _Pragma("GCC diagnostic push") _Pragma("GCC diagnostic ignored -Wdeprecated-declarations")
#define DT_INTERNAL_POST _Pragma("GCC diagnostic pop")
#endif
