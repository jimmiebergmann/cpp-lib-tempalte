
#ifndef TEMPLATE_LIB_BUILD_HPP
#define TEMPLATE_LIB_BUILD_HPP

// Platforms

// Win32
#if defined( _WIN32 ) || defined( __WIN32__ ) || defined( _WIN64 ) || defined( __WIN64__ )
    #define LIB_TEMPLATE_PLATFORM_WINDOWS

    #if defined (_MSC_VER)
        #pragma comment(lib, "ws2_32.lib")
    #endif
// Linux
#elif defined( linux ) || defined( __linux )
    #define LIB_TEMPLATE_PLATFORM_LINUX
#else
    #error Unkown platform.
#endif

// Build type
#if defined( NDEBUG ) || !defined( _DEBUG )
    #define LIB_TEMPLATE_BUILD_RELEASE
#else
    #define LIB_TEMPLATE_BUILD_DEBUG
#endif


#if !defined(LIB_TEMPLATE_STATIC)
    // Turn off microsoft STL vsc warning
    #ifdef _MSC_VER
        #pragma warning(disable : 4251)
    #endif

    // Define as export or import, if FLARE_EXPORTS is defined.
    #if defined(LIB_TEMPLATE_PLATFORM_WINDOWS)
        #if defined(LIB_TEMPLATE_EXPORTS)
            #define LIB_TEMPLATE_API __declspec(dllexport)
        #else
            #define LIB_TEMPLATE_API __declspec(dllimport)
        #endif
    #else
        #define LIB_TEMPLATE_API
    #endif
#else
    #define LIB_TEMPLATE_API
#endif

#endif
