#include <stdio.h>
#include <assert.h>

#include "shell.h"

int main(void)
{
    const char *url = "https://github.com/composer/satis/archive/master.zip";
    const char *depends_file = "foo.zip";

    int rc = Shell_exec(CURL_SH, "URL", url, "TARGET", depends_file, NULL);
    assert(rc == 0);
    
    return 0;
}
