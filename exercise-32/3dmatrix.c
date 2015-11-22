#include <stdio.h>

//struct name { komponente_1, ..., komponente_n };

typedef struct _MATRIX {
    union {
        struct {
            int i0, i1, i2;
            int i3, i4, i5;
        };
        int arr[2][3];
    };
} MATRIX;

int main(void)
{
    MATRIX m = {
        { { 1, 2, 3,
            4, 5, 6, } },
    };

    printf("%d\n", m.i3);
    printf("%d\n", m.arr[1][0]);

    return 0;
}
