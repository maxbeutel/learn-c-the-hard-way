#include <stdio.h>
#include <linux/limits.h>
#include <apr_general.h>
#include <apr_getopt.h>
#include <apr_strings.h>
#include <apr_lib.h>
#include <assert.h>

#include "db.h"
#include "commands.h"

int main()
{
    apr_pool_t *p = NULL;
    apr_pool_initialize();
    apr_pool_create(&p, NULL);
    
    apr_getopt_t *opt;
    apr_status_t rv;
    
    char ch = '\0';
    const char *optarg = NULL;
    const char *config_opts = NULL;
    const char *install_opts = NULL;
    const char *make_opts = NULL;
    const char *url = NULL;
    
    enum CommandType request = COMMAND_NONE;
    
    rv = apr_getopt_init(&opt, p, argc, argv);
    
    while (apr_getopt(opt, "I:Lc:m:i:d:SF:B:", &ch, &optarg) == APR_SUCCESS) {
    // "I:Lc:m:i:d:SF:B:" wtf???
        switch (ch) {
            case 'I':
                request = COMMAND_INSTALL;
                url = optarg;
                
                break;
                
            case 'L':
                request = COMMAND_LIST;
                
                break;
                
            case 'c':
                config_opts = optarg;
            
                break;

            case 'm':
                make_opts = optarg;
            
                break;
                
            case 'i':
                install_opts = optarg;
        
                break;
                
            case 'S':
                request = COMMAND_INIT;

                break;
                
            case 'F':
                request = COMMAND_FETCH;
                url = optarg;
        
                break;
                
            case 'B':
                request = COMMAND_BUILD;
                url = optarg;
        
                break;
        }
    }
    
    switch (request) {
        case COMMAND_INSTALL:
            assert(url != NULL);
            Command_install(p, url, config_opts, make_opts, install_opts);

            break;

        case COMMAND_LIST:
            DB_list();

            break;

        case COMMAND_FETCH:
            assert(url != NULL);
            Command_fetch(p, url, 1);

            printf("INFO Downloaded to %s and in /tmp/\n", BUILD_DIR);

            break;

        case COMMAND_BUILD:
            assert(url != NULL);
            Command_build(/*p, */url, config_opts, make_opts, install_opts);

            break;

        case COMMAND_INIT:
            rv = DB_init();
            assert(rv == 0);

            break;

        default:
            printf("ERROR: Invalid command given.\n"); // @TODO return 1
            
            break;
    }
    
    return 0;
}
