#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define TAM 1000000

int main(int argc, char **argv){

    MPI_Init(&argc,&argv); // mpi init 

    int vetor[TAM]; // vetor que armazeno as quantidades de letras
    int rank, size;
    int h = 0;
    char *result;
    char line[100]; // line para fazer a leitura das linhas do .txt
    
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // rank para definir slaves e master
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int slice =  TAM/(size-1);

    int *localArray = malloc(slice * sizeof(int)); // faço alocacao dinamica para evitar qualquer erro de segmentation

    for (int i = 0; i <= TAM; i++) {
        vetor[i] = 0;
    }

    int localCinco, localSeis, localDez; // variaveis que me auxiliam no mpi_reduce
    int globalCinco, globalSeis, globalDez;

    int buffer_inicial = 32;
    int buffer;

    FILE * names = fopen(argv[1], "r"); // abro o arquivo que passo como parametro

    while(1)
    {
        if(rank==0){
            for (int dest = 1; dest < size; ++dest) {
                MPI_Send(&vetor[(dest-1)*slice], slice, MPI_FLOAT, dest, 0, MPI_COMM_WORLD);  
                // no master, uso o mpi_send com o vetor de parametro
            } 
        }
        else {
            // entramos nos slaves, puxo o vetor localArray
            MPI_Recv(localArray, slice, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
            buffer_inicial *= 4; // como ele inicia com 32, multiplico por 4 para que o 1° seja = 128.
            for (int i = 0; i < buffer_inicial; i++) {
                result = fgets(line, buffer_inicial, names); 
                
                if (strlen(result) < 6) // significa que a linha tem menos de 6 letras
                    localArray[0] += 1;
                else if (strlen(result) > 10) // significa que a linha tem mais de 6 letras
                    localArray[2] += 1;
                else                    // significa que a linha tem entre 7 e 10 letras
                    localArray[1] += 1; 
            }
            
            localCinco = localArray[0];
            localSeis = localArray[1];
            localDez = localArray[2];

            printf("\n");

        }
        // mpi_reduce para puxar essas informacoes apos sair dos slaves

        MPI_Reduce(&buffer_inicial, &buffer, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD); 
        MPI_Reduce(&localSeis, &globalSeis, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
        MPI_Reduce(&localDez, &globalDez, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
        MPI_Reduce(&localCinco, &globalCinco, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

        if (rank == 0){
            printf("Qtd de menores que seis eh %d para um buffer de %d.\n", globalCinco, buffer);
            printf("Qtd de entre seis e dez eh %d para um buffer de %d.\n", globalSeis, buffer);
            printf("Qtd de maiores que dez eh %d para um buffer de %d.\n", globalDez, buffer);
        }

        if (buffer * 20 > TAM) { 
            // nao podemos rodar mais por conta do limite do TAM. Aqui acaba e executo o com buffer de 128 mais uma vex
            printf("\n\nBuffer chegou no seu tamanho maximo: %d\n\nFim de linha por aqui!\n\nAntes de finalizar, ele faz uma ultima vez com o buffer inicial\n\n", buffer);

            for (int i = 0; i < 128; i++) {
                result = fgets(line, 128, names); 
                
                if (strlen(result) < 6)
                    localArray[0] += 1;
                else if (strlen(result) > 10)
                    localArray[2] += 1;
                else 
                    localArray[1] += 1;
            }

            // prints que finalizam o codigo
            
            printf("Qtd de menores que seis eh %d para um buffer de 128.\n", localArray[0]);
            printf("Qtd de entre seis e dez eh %d para um buffer de 128.\n", localArray[1]);
            printf("Qtd de maiores que dez eh %d para um buffer de 128.\n", localArray[2]);

            printf("\n\nObrigado por ter vindo ate aqui!\n");
            MPI_Finalize();
            return 0;
        }
    }

    return 0;
}