#include<stdio.h>
#include <stdlib.h>
#include<math.h>
#include<mpi.h>

/**
 * Vamos a plantear un array del doble de tamanno que procesos haya.
 * A cada proceso le tocaran dos posiciones, donde escribira su rango.
 * Al hacer Allreduce, cada posición del array debera tener el mismo valor,
 * que sera la suma de los rangos.
 * */

#define ROOT_RANK 0

int main(int argc, char *argv[]) {
    int i;
    int rank, num_procs;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size( MPI_COMM_WORLD, &num_procs);

    if (rank == ROOT_RANK)
        printf("We are using %i procs.\n", num_procs);

    /* Cada proceso crea un array del número de elementos totales. */
    int elemPerProc = 2;

    /* Inicializamos la capa local */
    int *local_layer = (int *)malloc( sizeof(int) * elemPerProc * num_procs);
    for ( i = 0 ; i < num_procs*elemPerProc ; i++ ){
        local_layer[i]=0;
    }

    /* La rellenamos */
    for( i=0 ; i<elemPerProc ; i++) {
        local_layer[i] = rank;
    }

    /** Se nos quedaria de este modo:
     * local_layer -> [ 0, 0 ]
     * local_layer -> [ 1, 1 ]
     * local_layer -> [ 2, 2 ]
     * local_layer -> [ 3, 3 ]
     **/
    printf("Layer of proc %i -> [ %i, %i ]\n", rank, local_layer[0], local_layer[1]);

    
    /* Una vez vemos como son, las juntamos todas en la misma capa: */
    int *sum_layer = (int *) malloc( sizeof(int) * elemPerProc );
    for( i=0 ; i<elemPerProc ; i++) {
        sum_layer[i] = 0;
    }

    MPI_Allreduce( local_layer, sum_layer, 2, MPI_INT, MPI_SUM, MPI_COMM_WORLD ); 

    /* Layer -> [ 6, 6 ] */
    if (rank == ROOT_RANK)
        printf("Layer -> [ %i, %i ]\n", sum_layer[0], sum_layer[1] );
   

}