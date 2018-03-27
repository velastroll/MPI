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
        local_layer[i+rank*elemPerProc] = rank;
    }

    /** Se nos quedaria de este modo:
    *  Layer of proc 0 -> [ 0, 0, 0, 0, 0, 0, 0, 0]
    *  Layer of proc 1 -> [ 0, 0, 1, 1, 0, 0, 0, 0]
    *  Layer of proc 3 -> [ 0, 0, 0, 0, 0, 0, 3, 3]
    *  Layer of proc 2 -> [ 0, 0, 0, 0, 2, 2, 0, 0]
    **/
    printf("Layer of proc %i -> [ %i, %i, %i, %i, %i, %i, %i, %i]\n", rank, local_layer[0], local_layer[1], local_layer[2], local_layer[3], local_layer[4], local_layer[5], local_layer[6], local_layer[7]);
    
    /* Una vez vemos como son, las juntamos todas en la misma capa: */
    int *sum_layer = (int *) malloc( sizeof(int) * elemPerProc * num_procs );
    for( i=0 ; i<elemPerProc*num_procs ; i++) {
        sum_layer[i] = 0;
    }
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Allreduce( local_layer, sum_layer, elemPerProc*num_procs, MPI_INT, MPI_SUM, MPI_COMM_WORLD ); 

    /* Layer -> [ 0, 0, 1, 1, 2, 2, 3, 3] */
    if (rank == ROOT_RANK){
        printf("Layer -> [ %i, %i, %i, %i, %i, %i, %i, %i]\n", sum_layer[0], sum_layer[1], sum_layer[2], sum_layer[3], sum_layer[4], sum_layer[5], sum_layer[6], sum_layer[7]);
    }

}