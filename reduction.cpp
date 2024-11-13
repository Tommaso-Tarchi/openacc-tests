#include <stdio.h>
#ifdef _OPENMP
#include <omp.h>
#endif
#ifdef _OPENACC
#include <openacc.h>
#endif
#include <time.h>


int main() {

    int count = 0;

    clock_t t; 
    t = clock(); 

   #pragma acc enter data create(count)

   #pragma omp parallel for reduction(+:count)
   #pragma acc parallel loop gang num_gangs(10) present(count) reduction(+:count)
    for (int i = 0; i < 1000000000; i++)
       //#pragma omp atomic
        count++;

   #pragma acc exit data copyout(count)

   //#pragma acc wait

    t = clock() - t; 
    double exec_time = ((double)t) / CLOCKS_PER_SEC;

    printf("count = %d\n", count);
    printf("execution time = %f s\n", exec_time);

    return 0;
}
