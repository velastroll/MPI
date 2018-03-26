/*
 * Computacion paralela
 * Programa de procesado paralelo de matrices (cambio 1->2 y cuenta de 2), versión modificada.
 *
 * @author Grupo Trasgo
 * 
 * Mod by Alvaro Velasco - GitHub.com/velastroll
 */
#include <stdio.h>  /*Needed by printf function*/
#include <stdlib.h> /*Needed by malloc, free, srand & rand functions*/
#include <time.h>
#include <mpi.h>

int main (int argc, char* argv[])
{

   int i, matrix_size = atoi(argv[1]);

   /*  INICIO, DEBEMOS INICIALIZAR LOS  PROCESOS  */

    int rank;
	int num_procs;
	int data = 0;
	int tag = 0;
	MPI_Status stat;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size( MPI_COMM_WORLD, &num_procs );

      /* Miramos cuantas iteraciones corresponden a cada proceso. 
       * En caso de no ser justas, los primeros procesos tendrán una fila mas. 
       * Esta solucion de asignacion no se debe usar. */
    
    int elementsPerProc = matrix_size*matrix_size/num_procs;
    if ( (matrix_size*matrix_size)%num_procs != 0) elementsPerProc++;

    srand((unsigned int)time(NULL));

    /* Creamos un buffer en cada proceso */
    int *sub_matrix = (int*) malloc(elementsPerProc*sizeof(int));
    /*
     * NO ES NECESARIO HACER UN SCATTER SI VAMOS A HACER UN REDUCE 
     * MPI_Scatter(global_matrix, elementsPerProc, MPI_INT, sub_matrix, elementsPerProc, MPI_INT, 0, MPI_COMM_WORLD);
     **/

    for (i = 0; i < (elementsPerProc); i++)
    {
      sub_matrix[i] = (int)((((double)rand()) / (double) RAND_MAX) * 2.0);
    }
    for (i = 0; i < elementsPerProc; i++)
    {
        if (sub_matrix[i] == 1)
            sub_matrix[i] = 2;
    }
   /* Para contar el numero de 0 habrá que sumar el total de cada proceso */
   int local_number_of_twos = 0;
   for (i = 0; i < elementsPerProc; i++)
   {
         if (sub_matrix[i] == 2)
            local_number_of_twos++;
   }

   printf("Number of twos in process %i = %i\n", rank, local_number_of_twos);

    int global_sum;
    // MPI_Gather( &number_of_twos, 1, MPI_INT, cuentas, 1, MPI_INT, 0, MPI_COMM_WORLD );
    // Si hacemos Gather falta hacer la media de lo obtenido.
    MPI_Reduce ( &local_number_of_twos, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD  );
    if (rank==0)
        printf("> Total number of twos = %i\n", global_sum);

    free(sub_matrix);
}