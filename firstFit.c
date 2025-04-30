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


void firstFit( int blockSize[], int m, int processSize[], int n )
{
        int i, j;
        int allocation[n];

        for(i = 0; i < n; i++)
                allocation[i] = -1;

        for (i = 0; i < n; i++) {
                for (j = 0; j < m; j++) {

                    while (blockSize[j] == 0)
                      j++;
                    compare += 1;
                    if (blockSize[j] >= processSize[i]) {
                        allocation[i] = j;
                        blockSize[j] -= processSize[i];
                        break;
                    }
                }
        }
        printf("\nNº Processo\tTam Processo\tBloco nº\n");
        for (int i = 0; i < n; i++) {
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
        firstFit(blockSize, m, processSize, n);
        mostrablocos(blockSize, m, processSize, n);
        printf("O numero de comparacoes e: %d\n\n", compare);

        return 0 ;
}
