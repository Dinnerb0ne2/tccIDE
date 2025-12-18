#include <compiler.h>
#include <libtcc.h>
#include <stdio.h>
#include <string.h>

static char g_lastError[2048];

static void error_handler(void* opaque, const char* msg) {
    strncpy(g_lastError, msg, sizeof(g_lastError) - 1);
    g_lastError[sizeof(g_lastError) - 1] = '\0';
}

int Compiler_RunCode(const char* code, char* output, size_t output_size) {
    TCCState* s = tcc_new();
    if (!s) {
        strcpy(g_lastError, "TCC init failed");
        return -1;
    }
    
    tcc_set_error_func(s, NULL, error_handler);
    tcc_set_lib_path(s, "./tcc/lib");
    tcc_set_output_type(s, TCC_OUTPUT_MEMORY);
    
    tcc_add_sysinclude_path(s, "./tcc/include");
    tcc_add_sysinclude_path(s, "./tcc/include/winapi");
    tcc_add_library(s, "user32");
    tcc_add_library(s, "kernel32");
    
    if (tcc_compile_string(s, code) != 0) {
        tcc_delete(s);
        return -1;
    }
    
    if (tcc_relocate(s, TCC_RELOCATE_AUTO) < 0) {
        strcpy(g_lastError, "Relocation failed");
        tcc_delete(s);
        return -1;
    }
    
    int ret = tcc_run(s, 0, NULL);
    snprintf(output, output_size, "Exit code: %d", ret);
    
    tcc_delete(s);
    return 0;
}

const char* Compiler_GetLastError(void) {
    return g_lastError;
}