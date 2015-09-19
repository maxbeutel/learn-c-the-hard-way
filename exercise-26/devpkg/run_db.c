#include <stdio.h>
#include <assert.h>

#include "db.h"

int main(void)
{
    printf("DB_FILE %s\n", DB_FILE);
    
    int rc = DB_init();
    assert(rc == 0);
    
    DB_update("www.example.com");
    
    printf("Entry exists: %d\n", DB_find("www.example.com"));
    printf("Entry not exists: %d\n", DB_find("www.example2.com"));
    
    DB_list();
    
    return 0;
}
