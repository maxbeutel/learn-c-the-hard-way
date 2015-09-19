#include <stdio.h>
#include <stdlib.h>
#include <glob.h>
#include <assert.h>

#define UNUSED(x) (void)(x)

int main(int argc, char *argv[])
{
    UNUSED(argc);
    UNUSED(argv);
    
    int rc = 0;
    
    glob_t paths;
    paths.gl_pathc = 0;
    paths.gl_pathv = NULL;
    paths.gl_offs = 0;

    // was genau ist dann die sortierreihenfolge??
    int glob_rc = glob("./tests/*_test.*", GLOB_NOSORT, NULL, &paths);

    if (glob_rc != 0) {
        rc = -1;
        printf("glob() fail - error %d\n", glob_rc);
        goto cleanup;
    }
    
    if (paths.gl_pathc > 0) {
        printf("found results => %lu\n", paths.gl_pathc);
    
        for (size_t i = 0; i < paths.gl_pathc; i++) {
            printf("RUN test \"%s\"\n", paths.gl_pathv[i]);
            printf("----\n");
            int test_program_rc = system(paths.gl_pathv[i]);
            printf("----\n");
            
            if (test_program_rc == -1 ) {
                printf("SYSTEM ERROR Failed to run test\n");
            } else if (test_program_rc == 0) {
                printf("PASS\n");
            } else {
                printf("FAIL return code\"%d\"\n", test_program_rc);
            }
        }        
    } else {
        printf("NO tests found");
    }

    cleanup:
        globfree(&paths);
    
    return rc;
}
