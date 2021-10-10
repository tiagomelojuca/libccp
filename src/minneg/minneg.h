#ifndef CCP_MINNEG_H_
#define CCP_MINNEG_H_

#ifdef _WIN32
    #ifdef BUILD_DLL
        #define _APICCP __declspec(dllexport)
    #else
        #define _APICCP __declspec(dllimport)
    #endif
#else
    #define _APICCP
#endif

#ifdef __cplusplus
extern "C" {
#endif

// ----------------------------------------------------------------------------

int _APICCP dummy_func_sum(int, int);

// ----------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif // CCP_MINNEG_H_
