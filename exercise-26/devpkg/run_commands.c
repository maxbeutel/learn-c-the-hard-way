#include <stdio.h>
#include <assert.h>

#include "db.h"
#include "commands.h"

int main(void)
{
    apr_status_t rv;
    apr_pool_t *p = NULL;
    
    CommandStatus_t rc = 0;

    rv = apr_initialize();
    assert(rv == APR_SUCCESS);

    apr_pool_create(&p, NULL);
    assert(p != NULL);
    
    /*******************************/
    
    // init DB
    rc = DB_init();    
    assert(rc == 0);

    // fetch by git url
    const char *git_url = "https://github.com/maxbeutel/nutwerk-orm-extension.git";

    rc = Command_install(
        p,
        git_url,
        NULL,
        NULL,
        NULL
    );
    assert(rc == COMMAND_SUCCESS || rc == COMMAND_INSTALL_ALREADY_INSTALLED);
    
    /*******************************/

    apr_pool_destroy(p);
    apr_terminate();

    return 0;
}