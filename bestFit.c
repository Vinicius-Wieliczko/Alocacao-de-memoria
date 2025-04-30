#include <stdio.h>
#include <string.h>

int compare = 0;

void mostrablocos( int blocksSize[], int m, int processSize[], int n )
{
        printf("\nLista de buracos: ");
        for( int i = 0; i < m; i++)
                printf( "%d,", blocksSize[i] );
        printf("\nLista de requisições: ");
        for( int i = 0; i < n; i++)
                printf( "%d,", processSize[i] );
        printf("\n\n");
}

void bestFit(int blockSize[], int blocks, int processSize[], int processes)
{
    int allocation[processes];
    for(int i = 0; i < processes; i++)
        allocation[i] = -1;

    for (int i=0; i<processes; i++) {
        int indexPlaced = -1;
        for (int j=0; j<blocks; j++) {
            while (blockSize[j] == 0)
                j++;
            compare += 1;
            if (blockSize[j] >= processSize[i]) {
                if (indexPlaced == -1)
                    indexPlaced = j;
                else if (blockSize[j] < blockSize[indexPlaced]){
                    indexPlaced = j;
                }
                
//Caso encontre o tamanho perfeito já será alocado lá, sem precisar de mais comparações
                if (blockSize[indexPlaced] == processSize[i]){
                    break;                        
                }
            }
        }

        if (indexPlaced != -1) {
            allocation[i] = indexPlaced;
            blockSize[indexPlaced] -= processSize[i];
        }
    }

    printf("\nNº Processo\tTam Processo\tBloco nº\n");
        for (int i = 0; i < processes; i++) {
                printf(" %i\t\t", i+1);
                printf("%i\t\t\t", processSize[ i ]);
                if (allocation[ i ] != -1)
                        printf("%i", allocation[ i ] + 1);
                else
                        printf("Não alocado");
                printf("\n");
        }
}


void inicialize( int blockSize[], int processSize[] )
{
        int blockHole[] = { 10, 4, 20, 18, 7, 9, 12, 15 };
        int processRequest[] = { 12, 10, 9, 5, 25 };

        memcpy( blockSize, blockHole, sizeof( blockHole ) );
        memcpy( processSize, processRequest, sizeof( processRequest ) );
}


int main()
{
        int m, n;
        int blockSize[] = { 10, 4, 20, 18, 7, 9, 12, 15 };
        int processSize[] = { 12, 10, 9, 5, 25 };

        m = sizeof(blockSize) / sizeof(blockSize[0]);
        n = sizeof(processSize) / sizeof(processSize[0]);

        inicialize( blockSize, processSize );
        mostrablocos(blockSize, m, processSize, n);
        bestFit(blockSize, m, processSize, n);
        mostrablocos(blockSize, m, processSize, n);
        printf("O numero de comparacoes e: %d\n\n", compare);

        return 0 ;
}

