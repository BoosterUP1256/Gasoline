#pragma once

#ifdef FOO_LIBRARY_BUILD
// Building the library
#ifdef _WIN32
    // Use the Windows-specific export attribute
#define FOO_LIBRARY_EXPORT __declspec(dllexport)
#elif __GNUC__ >= 4
    // Use the GCC-specific export attribute
#define FOO_LIBRARY_EXPORT __attribute__((visibility("default")))
#else
    // Assume that no export attributes are needed
#define FOO_LIBRARY_EXPORT
#endif
#else
// Using (including) the library
#ifdef _WIN32
    // Use the Windows-specific import attribute
#define FOO_LIBRARY_EXPORT __declspec(dllimport)
#else
    // Assume that no import attributes are needed
#define FOO_LIBRARY_EXPORT
#endif
#endif

namespace Gas {

	FOO_LIBRARY_EXPORT void sayHi();

}