#include <stdio.h>
#include <openacc.h>
#include <mpi.h>


int main(int argc, char* argv[]) {

    int my_rank, n_procs;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &n_procs);

#ifdef OPENACC
    int n_gpus = acc_get_num_devices(acc_device_nvidia);
    int i_gpu = my_rank % n_gpus;
    
    acc_set_device_num(i_gpu, acc_device_nvidia);
    acc_init(acc_device_nvidia);
#endif

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

    printf("From process %d: ", my_rank);
    for (int i = 0; i < 4; i++)
        printf("%d ", c[i]);
    printf("\n");

    MPI_Finalize();

    return 0;
}