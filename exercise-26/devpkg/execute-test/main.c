#include <unistd.h>
#include <stdio.h>
#include <assert.h>

#include <linux/limits.h>
#include <apr_thread_proc.h>

int main(void)
{
    printf("hallo!\n");
    
    apr_status_t rv;
    apr_pool_t *p = NULL;
    apr_proc_t newproc;
    
    rv = apr_initialize();
    assert(rv == APR_SUCCESS);

    apr_pool_create(&p, NULL);
    
    
    // --
    char cwd[1024];
    assert(getcwd(cwd, 1024) != NULL);
    
    apr_procattr_t *attr = NULL;
    
    rv = apr_procattr_create(&attr, p);
    assert(rv == APR_SUCCESS);
    
    rv = apr_procattr_io_set(attr, APR_NO_PIPE, APR_NO_PIPE, APR_NO_PIPE);
    assert(rv == APR_SUCCESS);
    
    rv = apr_procattr_dir_set(attr, cwd);
    assert(rv == APR_SUCCESS);

    printf("cwd: %s\n", cwd);

    rv = apr_procattr_cmdtype_set(attr, APR_PROGRAM_ENV );
    assert(rv == APR_SUCCESS);


    /* pseudo code of args to apr_proc_create() */
    int argc = 0;
    const char* argv[32];   /* 32 is a magic number. enough size for the number of arguments list */
    argv[argc++] = "test.sh"; /* program path of the command to run */
    /*argv[argc++] = "-i";
    argv[argc++] = "foo";
    argv[argc++] = "--longopt";
    argv[argc++] = "bar";*/
    argv[argc++] = NULL;  
    
    rv = apr_proc_create(&newproc, "test.sh", argv, /* env */ NULL, attr, p);
    assert(rv == APR_SUCCESS);
    
    //
    int exitCode = 0;
    apr_exit_why_e exitWhy = 0;
    
    rv = apr_proc_wait(&newproc, &exitCode, &exitWhy, APR_WAIT);
    if (APR_STATUS_IS_CHILD_DONE(rv)) {
        printf("child done: why = %d, exit status = %d\n", exitWhy, exitCode);
    } else {
        printf("child notdone\n");
    }
    
    
    /*assert(rv == APR_CHILD_DONE);
    
    assert(exitCode == 0);
    assert(exitWhy == APR_PROC_EXIT);*/
    // --
    
    
    apr_pool_destroy(p);
    apr_terminate();
    
    return 0;
}