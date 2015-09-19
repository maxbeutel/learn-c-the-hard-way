#include <stdio.h>
#include <dlfcn.h>
#include <assert.h>

typedef int (*lib_function)(const char *data);

int main(int argc, char *argv[])
{
    assert(argc == 4 && "USAGE: ex29 libex29.so function data.");

    int rc = 0;
    
    char *lib_file = argv[1];
    char *func_to_run = argv[2];
    char *data = argv[3];
    
    void *lib = dlopen(lib_file, RTLD_NOW);
    assert(lib != NULL);
    
    lib_function func;
    
    *(void **) (&func) = dlsym(lib, func_to_run);
    assert(func != NULL);
    
    rc = func(data);
    assert(rc == 0);
    
    rc = dlclose(lib);
    assert(rc == 0);
    
    return 0; 
}
