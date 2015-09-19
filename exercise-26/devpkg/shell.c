#include "shell.h"
#include <stdarg.h>
#include <assert.h>

/*
    TODO:

    - better error handling
    - challenge 2 - add argument count to Shell struct and validate
*/

int Shell_exec(Shell template, ...)
{
    apr_status_t rv = APR_SUCCESS;
    apr_pool_t *p = NULL;
    
    int rc = -1;

    const char *key = NULL;
    const char *arg = NULL;
    
    int i = 0;

    rv = apr_initialize();
    assert(rv == APR_SUCCESS);

    apr_pool_create(&p, NULL);
   
    va_list argp;
    va_start(argp, template);
    
    for (key = va_arg(argp, const char *);
         key != NULL;
         key = va_arg(argp, const char *)
    ) {
        arg = va_arg(argp, const char *);
        
        // @TODO need to check arguments for not NULL?
        
        for (i = 0; template.args[i] != NULL; i++) {
            if (strcmp(template.args[i], key) == 0) {
                template.args[i] = arg;
                break;
            }
        }
    }
    
    rc = Shell_run(p, &template);
    apr_pool_destroy(p);
    apr_terminate(); // @TODO how does this work together with e. g. db which also sets up and terminates apr?

    va_end(argp);
    
    return rc;
}

// ??? why give pointer to shell command? in order to keep stack size small?
// if i pass in cmd by value, if i modify the arg, it doesnt change, right?
int Shell_run(apr_pool_t *p, Shell *cmd)
{
    printf("DEBUG executing command\n");
    
    apr_procattr_t *attr = NULL;
    apr_status_t rv;
    apr_proc_t newproc;
    
    rv = apr_procattr_create(&attr, p);
    assert(rv == APR_SUCCESS);
    
    rv = apr_procattr_io_set(attr, APR_NO_PIPE, APR_NO_PIPE, APR_NO_PIPE);
    assert(rv == APR_SUCCESS);
    
    rv = apr_procattr_dir_set(attr, cmd->dir);
    assert(rv == APR_SUCCESS);
    
    rv = apr_procattr_cmdtype_set(attr, APR_PROGRAM_PATH);
    assert(rv == APR_SUCCESS);
    
    rv = apr_proc_create(&newproc, cmd->exe, cmd->args, NULL, attr, p);
    assert(rv == APR_SUCCESS);

    rv = apr_proc_wait(&newproc, &cmd->exit_code, &cmd->exit_why, APR_WAIT);
    assert(rv == APR_CHILD_DONE);
    
    printf("DEBUG command exit code: %d\n", cmd->exit_code);
    
    assert(cmd->exit_code == 0);
    assert(cmd->exit_why == APR_PROC_EXIT);
    
    // no free needed? probably because of apr_terminate?
    
    return 0;
}

Shell CLEANUP_SH = {
    .exe = "rm",
    .dir = "/tmp",
    .args = {"rm", "-rf", "/tmp/pkg-build", "/tmp/pkg-src.tar.gz",
        "/tmp/pkg-src.tar.bz2", "/tmp/DEPENDS", NULL} // what is the point of the last NULL here??
};

Shell GIT_SH = {
    .dir = "/tmp",
    .exe = "git",
    .args = {"git", "clone", "URL", "pkg-build", NULL}
};

Shell TAR_SH = {
    .dir = "/tmp/pkg-build",
    .exe = "tar",
    .args = {"tar", "-xzf", "FILE", "--strip-components", "1", NULL}
};

Shell CURL_SH = {
    .dir = "/tmp",
    .exe = "curl",
    .args = {"curl", "-L", "-o", "TARGET", "URL", NULL}
};

Shell CONFIGURE_SH = {
    .exe = "./configure",
    .dir = "/tmp/pkg-build",
    .args = {"configure", "OPTS", NULL},
};

Shell MAKE_SH = {
    .exe = "make",
    .dir = "/tmp/pkg-build",
    .args = {"make", "OPTS", NULL}
};

Shell INSTALL_SH = {
    .exe = "sudo",
    .dir = "/tmp/pkg-build",
    .args = {"sudo", "make", "TARGET", NULL}
};
