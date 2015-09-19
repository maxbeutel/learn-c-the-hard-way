#ifndef _commands_h
#define _commands_h

#include <linux/limits.h>
#include <apr_pools.h>

#define DEPENDS_PATH "/tmp/DEPENDS"

#define TAR_GZ_SRC "/tmp/pkg-src.tar.gz"
#define TAR_BZ2_SRC "/tmp/pkg-src.tar.bz2"

#define BUILD_DIR "/tmp/pkg-build"

#define GIT_PAT "*.git"
#define DEPEND_PAT "*DEPENDS"
#define TAR_GZ_PAT "*.tar.gz"
#define TAR_BZ2_PAT "*.tar.bz2"

#define CONFIG_SCRIPT "/tmp/pkg-build/configure"

enum CommandType {
    COMMAND_NONE = 0,
    COMMAND_INSTALL,
    COMMAND_LIST,
    COMMAND_FETCH,
    COMMAND_INIT,
    COMMAND_BUILD,
    
    _COMMAND_COUNT,
};

typedef enum CommandStatus {
    COMMAND_SUCCESS = 0,

    COMMAND_INSTALL_ALREADY_INSTALLED,
    COMMAND_INSTALL_ERROR_FETCH_FAILURE,
} CommandStatus_t;

CommandStatus_t Command_fetch(apr_pool_t *p, const char *url, char **download_dir);

CommandStatus_t Command_install(
    apr_pool_t *p, 
    const char *url, 
    const char *configure_opts, 
    const char *make_opts, 
    const char *install_opts
);

int Command_depends(apr_pool_t *p, const char *path);

int Command_build(
    /*apr_pool_t *p,*/
    const char *url,
    const char *configure_opts, 
    const char *make_opts, 
    const char *install_opts
);

#endif
