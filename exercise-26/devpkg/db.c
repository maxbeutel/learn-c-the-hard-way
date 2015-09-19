#include <unistd.h>
#include <linux/limits.h>
#include <apr_errno.h>
#include <apr_file_io.h>
#include <assert.h>

#include "db.h"
#include "bstrlib.h"

/*
- error checking
- use bstring everywhere
- avoid double install?
...
*/

static FILE *DB_open(const char *path, const char *mode)
{
    return fopen(path, mode);
}

static void DB_close(FILE *db)
{
    fclose(db);
}

static bstring DB_load()
{
    FILE *db = NULL;
    bstring data = NULL;
    
    db = DB_open(DB_FILE, "r");
    assert(db != NULL);
    
    data = bread((bNread)fread, db);
    assert(data != NULL);
    
    DB_close(db);

    return data;
}

int DB_update(const char *url)
{
    bool found = false;
    assert(DB_find(url, &found) == 0);
    
    if (found) {
        printf("INFO: Already recorded as installed: %s\n", url);
    }
    
    FILE *db = DB_open(DB_FILE, "a+");
    assert(db != NULL);
    
    bstring line = bfromcstr(url);
    bconchar(line, '\n');
    
    int rc = fwrite(line->data, blength(line), 1, db);
    assert(rc == 1);
    
    bdestroy(line);
    
    DB_close(db);
    
    return 0;
}

int DB_find(const char *url, bool *found)
{
    assert(url != NULL);
    assert(found != NULL);
    
    bstring data = NULL;
    bstring line = bfromcstr(url);
    
    int rc = 0;
    
    data = DB_load();
    assert(data != NULL);
    
    if (binstr(data, 0, line) == BSTR_ERR) {
        *found = false;
    } else {
        *found = true;
    }

    bdestroy(data);
    bdestroy(line);    

    return rc;
}

int DB_init()
{
    apr_status_t rv;
    apr_pool_t *mp;

    rv = apr_initialize();
    assert(rv == APR_SUCCESS);

    apr_pool_create(&mp, NULL);

    if (access(DB_DIR, W_OK | X_OK) == -1) {
        apr_status_t rc = apr_dir_make_recursive(
            DB_DIR,
            APR_UREAD | APR_UWRITE | APR_UEXECUTE | APR_GREAD | APR_GWRITE | APR_GEXECUTE, 
            mp
        );

        assert(rc == APR_SUCCESS);
    }
    
    if (access(DB_FILE, W_OK) == -1) {
        FILE *db = DB_open(DB_FILE, "w");
        assert(db != NULL);
        DB_close(db);
    }
    
    apr_pool_destroy(mp);
    apr_terminate();

    return 0;
}

int DB_list()
{
    bstring data = DB_load();
    assert(data != NULL);
    
    printf("%s", bdata(data));
    bdestroy(data);
    
    return 0;
}

