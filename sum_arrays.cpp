#include <stdio.h>
#include <mpi.h>
#ifdef _OPENMP
#include <omp.h>
#endif
#ifdef _OPENACC
#include <openacc.h>
#endif


int main(int argc, char* argv[]) {

    int my_rank, n_procs;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &n_procs);

#ifdef _OPENACC
    int n_gpus = acc_get_num_devices(acc_device_nvidia);
    int i_gpu = my_rank % n_gpus;
    
    acc_set_device_num(i_gpu, acc_device_nvidia);
    acc_init(acc_device_nvidia);
#endif

    int a[4] = {1, 2, 3, 4};
    int b[4] = {1, 1, 1, 1};
    int c[4] = {0, 0, 0, 0};
    int b_sum = 0;

   #pragma acc enter data copyin(a[0:4], b[0:4]) create(c[0:4], b_sum)

   #pragma omp parallel for
   #pragma acc parallel loop present(a[0:4], b[0:4], c[0:4])
    for (int i = 0; i < 4; i++)
        c[i] = a[i] + b[i];

   #pragma omp parallel for reduction(+:b_sum)
   #pragma acc parallel loop present(b_sum) reduction(+:b_sum)
    for (int i = 0; i < 4; i++)
        b_sum += b[i];

   #pragma acc exit data copyout(c[0:4], b_sum)

    printf("From process %d: ", my_rank);
    for (int i = 0; i < 4; i++)
        printf("%d ", c[i]);
    printf("\n");

    printf("%d\n", b_sum);

    MPI_Finalize();

    return 0;
}
