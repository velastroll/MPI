/*
 * Computacion paralela
 * Programa de procesado paralelo de matrices (cambio 1->2 y cuenta de 2), versión secuencial.
 *
 * @author Grupo Trasgo
 */
#include <stdio.h>  /*Needed by printf function*/
#include <stdlib.h> /*Needed by malloc, free, srand & rand functions*/

int main (int argc, char* argv[])
{
   int matrix_size;
   int* global_matrix;
   int i, j;

   /*1. Memory allocation of vars & initialization*/
   if (argc < 2)
   {
      printf ("Uso: %s <celdas_lado_matriz>\n", argv[0]);
      return (EXIT_SUCCESS);
   }

   matrix_size = atoi(argv[1]);

   global_matrix = (int*) malloc(matrix_size*matrix_size*sizeof(int));
   if ((global_matrix == NULL))
   {
      perror ("Error reservando memoria");
      return -1;
   }

   /*1.1 Initialization of matrix with data in range 0 to 2*/
   srand((unsigned int)time(NULL));
   for (i = 0; i < (matrix_size*matrix_size); i++)
   {
      global_matrix[i] = (int)((((double)rand()) / (double) RAND_MAX) * 2.0);
   }

   /*2. Replacement of ones by twos*/ 
   for (i = 0; i < matrix_size; i++)
   {
      for (j = 0; j < matrix_size; j++)
      {
         if (global_matrix[i*matrix_size+j] == 1)
            global_matrix[i*matrix_size+j] = 2;
      }
   }

   /*3. Count twos*/
   int number_of_twos = 0;
   for (i = 0; i < matrix_size; i++)
   {
      for (j = 0; j < matrix_size; j++)
      {
         if (global_matrix[i*matrix_size+j] == 2)
            number_of_twos++;
      }
   }

   /*4. Print results*/
   printf("Total number of twos in the matrix = %i\n", number_of_twos);

   free(global_matrix);
}