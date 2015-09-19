#include <unistd.h>

int main(int argc, char *argv[])
{
    int i = 0;
    
    while (i < 100) {
        sleep(3);
    }
    
    return 0;
}

