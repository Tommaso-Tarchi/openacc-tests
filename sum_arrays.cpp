#include <stdio.h>
#include <openacc.h>


int main() {

    int a[4] = {1, 2, 3, 4};
    int b[4] = {1, 1, 1, 1};
    int c[4] = {0, 0, 0, 0};

#ifdef _OPENACC
   #pragma acc data copy(a[0:4], b[0:4], c[0:4])
    {
       #pragma acc parallel loop gang present(a[0:4], b[0:4], c[0:4])
#endif
        for (int i = 0; i < 4; i++)
            c[i] = a[i] + b[i];
#ifdef _OPENACC
    }
#endif

    for (int i = 0; i < 4; i++)
        printf("%d ", c[i]);
    printf("\n");

    return 0;
}
