#include <linux/limits.h>
#include <apr_uri.h>
#include <apr_fnmatch.h>
#include <unistd.h>

#include <assert.h>

#include "commands.h"
#include "bstrlib.h" // @TODO best practice for including libs???
#include "shell.h"
#include "db.h"

// @TODO better error handling, input validation
// goto error; hab ich überall ausgelassen :D
// @TODO create dynamic hashset for Shell args as exercise?

int Command_depends(apr_pool_t *p, const char *path)
{
    FILE *in = NULL;
    bstring line = NULL;
    
    in = fopen(path, "r");
    assert(in != NULL);
    
    for (line = bgets((bNgetc)fgetc, in, '\n'); 
         line != NULL; 
         line = bgets((bNgetc)fgetc, in, '\n')
    ) {
        btrimws(line);
        printf("INFO: processing depends: %s", bdata(line));
        
        int rc = Command_install(p, bdata(line), NULL, NULL, NULL);
        assert(rc == 0);
        
        bdestroy(line);
    }
    
    fclose(in);
    
    return 0;
}

CommandStatus_t Command_fetch(apr_pool_t *p, const char *url)
{
    // @FIXME download dir ist eine konstante ...
    assert(p != NULL);
    assert(url != NULL);

    apr_uri_t info = { .port = 0 };
    apr_status_t rv = apr_uri_parse(p, url, &info);
    assert(rv == APR_SUCCESS);

    if (apr_fnmatch(GIT_PAT, info.path, 0) == APR_SUCCESS) {
        printf("DEBUG downloading git url %s\n", info.path);
        
        int shell_rc = Shell_exec(GIT_SH, "URL", url, NULL);
        assert(shell_rc == 0);

        return COMMAND_SUCCESS;
    }
    
    /*if (apr_fnmatch(DEPEND_PAT, info.path, 0) == APR_SUCCESS) {
        const char *depends_file = NULL;
        
        if (info.scheme) {
            depends_file = DEPENDS_PATH;
            int shell_rc = Shell_exec(CURL_SH, "URL", url, "TARGET", depends_file, NULL);
            
            assert(shell_rc == 0);
        } else {
            depends_file = info.path;
        }

        // recursively process the devpkg list
        int depends_rc = Command_depends(p, depends_file);
        assert(depends_rc == 0);
        
        // this indicates that nothing needs to be done
        return 0;        
    }*/

    /*if (apr_fnmatch(TAR_BZ2_PAT, info.path, 0) == APR_SUCCESS) {
        if (info.scheme) {
            int shell_rc = Shell_exec(CURL_SH, "URL", url, "TARGET", TAR_BZ2_SRC, NULL);
            assert(shell_rc == 0);
        }
        
        apr_status_t mkdir_rv = apr_dir_make_recursive( // warum braucht man das bei TAR nicht???
            BUILD_DIR,
            APR_UREAD | APR_UWRITE | APR_UEXECUTE, // wtf?
            p
        );
        assert(mkdir_rv == APR_SUCCESS);
        
        int shell_rc = Shell_exec(TAR_SH, "FILE", TAR_BZ2_SRC, NULL);
        assert(shell_rc == 0);
        
        return 0;
    }*/
    
    // @TODO error here!
    return 1;
}

int Command_build(
    /*apr_pool_t *p,*/ 
    const char *url, 
    const char *configure_opts, 
    const char *make_opts, 
    const char *install_opts
)
{
    int rc = 0;
    
    assert(access(BUILD_DIR, X_OK | R_OK | W_OK) == 0);
    
    // actually do an install
    if (access(CONFIG_SCRIPT, X_OK) == 0) {
        printf("INFO has a configure script, running it\n");
        rc = Shell_exec(MAKE_SH, "OPTS", configure_opts, NULL);
        assert(rc == 0);
    }
    
    rc = Shell_exec(MAKE_SH, "OPTS", make_opts, NULL);
    assert(rc == 0);
    
    rc = Shell_exec(INSTALL_SH, "TARGET", (install_opts != NULL ? install_opts : "install"), NULL);
    assert(rc == 0);
    
    rc = Shell_exec(CLEANUP_SH, NULL);
    assert(rc == 0);
    
    rc = DB_update(url);
    assert(rc == 0);
    
    return 0;
}

static bool Command_install_needsBuild()
{
    if (access(BUILD_DIR, X_OK | R_OK | W_OK) == 0) {
        return true;
    }
    
    return false;
}

CommandStatus_t Command_install(
    apr_pool_t *p, 
    const char *url, 
    const char *configure_opts, 
    const char *make_opts, 
    const char *install_opts
) {
    assert(p != NULL);
    assert(url != NULL);

    assert(Shell_exec(CLEANUP_SH, NULL) == 0);

    bool package_already_installed = false;
    int db_rc = DB_find(url, &package_already_installed);
    assert(db_rc == 0);

    if (package_already_installed) {
        printf("INFO Package \"%s\" already installed\n", url);
        return COMMAND_INSTALL_ALREADY_INSTALLED;
    }

    CommandStatus_t fetch_rc = Command_fetch(p, url);
    assert(fetch_rc == COMMAND_SUCCESS);

    CommandStatus_t rc = COMMAND_SUCCESS;

    if (Command_install_needsBuild(download_dir)) { /* @TODO */
        printf("INFO Building package for install\n");
        
        int build_rc = Command_build(
            url, 
            configure_opts, 
            make_opts, 
            install_opts
        );

        assert(build_rc == 0);
    } else if (fetch_rc == 0) {
        // no install needed
        printf("INFO Depends successfully installed: %s\n", url);
    } else {
        printf("ERR Install failed - fetch error\n");
        rc = COMMAND_INSTALL_ERROR_FETCH_FAILURE;
    }
    
    //Shell_exec(CLEANUP_SH, NULL);
    return rc;
}



