#ifndef CCP_PUNC_H_
#define CCP_PUNC_H_

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

int _APICCP dummy_func_sub(int, int);

// ----------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif // CCP_PUNC_H_
