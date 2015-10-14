#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

void copy(char *str1, char *str2)
{
    int i = 0;

    while ((str2[i] = str1[i]) != '\0') {
        i++;
    }
}

void safer_copy(char *str1, int str1_len, char *str2, int str2_len)
{
    assert(str1 != NULL);
    assert(str2 != NULL);

    if (str1_len == 0 || str2_len == 0) {
        return;
    }

    int copy_len = (str2_len < str1_len) ? str2_len : str1_len;
    printf("copy len is %d\n", copy_len);

    for (int i = 0; i < copy_len; i++) {
        str2[i] = str1[i];
    }

    str2[copy_len] = '\0';
}


int safercopy(int from_len, char *from, int to_len, char *to)
{
    int i = 0;
    int max = from_len > to_len - 1 ? to_len - 1 : from_len;

    // to_len must have at least 1 byte
    if(from_len < 0 || to_len <= 0) return -1;

    for(i = 0; i < max; i++) {
        to[i] = from[i];
    }

    to[to_len - 1] = '\0';

    return i;
}

int main(void)
{
    char *str1 = malloc(sizeof(char) * 4);
    assert(str1 != NULL);

    char *str2 = malloc(sizeof(char) * 4);
    assert(str2 != NULL);

    char *str3 = malloc(sizeof(char) * 5);
    assert(str3 != NULL);


    str1[0] = 'a';
    str1[1] = 'b';
    str1[2] = 'c';
    str1[3] = '\0';

    str2[0] = 'x';
    str2[1] = 'y';
    str2[2] = 'z';
    str2[3] = '\0';

    str3[0] = 't';
    str3[1] = 'e';
    str3[2] = 's';
    str3[3] = 't';
    str3[4] = '\0';

    safercopy(strlen(str3), str3, strlen(str2), str2);
    //safer_copy(str3, strlen(str3), str2, strlen(str2));
    //safer_copy(str1, strlen(str1), str3, strlen(str3));
    //safer_copy(str1, strlen(str1), str2, strlen(str2));

    printf("str1: %s\n", str1);
    printf("str2: %s\n", str2);

    free(str1);
    free(str2);
    free(str3);

    return 0;
}
