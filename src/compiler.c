#include <compiler.h>
#include <libtcc.h>

static TCCState* s_tccState = NULL;
static char s_lastError[4096] = {0};

static void ErrorCallback(void* opaque, const char* msg) {
    strncpy(s_lastError, msg, sizeof(s_lastError) - 1);
}

BOOL Compiler_Init(void) {
    s_tccState = tcc_new();
    if (!s_tccState) return FALSE;
    
    tcc_set_lib_path(s_tccState, "C:/tcc/lib");
    tcc_set_output_type(s_tccState, TCC_OUTPUT_MEMORY);
    tcc_add_sysinclude_path(s_tccState, "C:/tcc/include");
    tcc_add_sysinclude_path(s_tccState, "C:/tcc/include/winapi");
    tcc_set_error_func(s_tccState, NULL, ErrorCallback);
    
    return TRUE;
}

void Compiler_Cleanup(void) {
    if (s_tccState) {
        tcc_delete(s_tccState);
        s_tccState = NULL;
    }
}

BOOL Compiler_CompileAndRun(const char* code) {
    if (!s_tccState) return FALSE;
    
    tcc_delete(s_tccState);
    s_tccState = tcc_new();
    Compiler_Init();
    
    if (tcc_compile_string(s_tccState, code) == -1) {
        return FALSE;
    }
    
    if (tcc_relocate(s_tccState, TCC_RELOCATE_AUTO) < 0) {
        strncpy(s_lastError, "Relocation failed", sizeof(s_lastError) - 1);
        return FALSE;
    }
    
    tcc_run(s_tccState, 0, NULL);
    return TRUE;
}

const char* Compiler_GetLastError(void) {
    return s_lastError;
}