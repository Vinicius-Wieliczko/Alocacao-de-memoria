#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct node{
    int blocks;
    int position;
    struct node *next;
}node;

node *start = NULL;
node *atualNode = NULL;
int compare = 0;

void mostrablocos( int m, int processSize[], int n )
{
    node* aux = start;
    printf("\nLista de buracos: ");
    for( int i = 0; i < m; i++){
        printf( "%d,", aux->blocks );
        aux = aux->next;
    }
    printf("\nLista de requisições: ");
    for( int i = 0; i < n; i++)
            printf( "%d,", processSize[i] );
    printf("\n\n");
}

void inicialize( int blockSize[], int processSize[], int m )
{
    node *initialNode;
    initialNode = (node*)malloc(sizeof(node));

    int blockHole[] = { 10, 4, 20, 18, 7, 9, 12, 15 };
    int processRequest[] = { 12, 10, 9, 5, 25 };

    memcpy( blockSize, blockHole, sizeof( blockHole ) );
    memcpy( processSize, processRequest, sizeof( processRequest ) );
    
    initialNode->blocks = blockSize[0];
    initialNode->position = 0;
    start = initialNode;
    atualNode = start;
    node *current = start;


    for(int i = 1; i < m; i++){
        node *aux;
        aux = (node*)malloc(sizeof(node));
        aux->blocks = blockSize[i];
        aux->position = i;
        current->next = aux;
        current = current->next;
    }
    current->next = start;

}

void nextFit( int m, int processSize [], int n){
    int i, j;
    int allocation[n];
    j = 0;

    for(i = 0; i < n; i++)
            allocation[i] = -1;
    for(i = 0; i < n; i++){
        while(j < m){
            while (atualNode->blocks == 0){
                atualNode = atualNode->next;
                j++;
                }
                if(j >= m)
                  break;
            compare += 1;
          
            if (atualNode->blocks >= processSize[i]) {
                allocation[i] = atualNode->position;
                atualNode->blocks -= processSize[i];
                break;
            }
            atualNode = atualNode->next;
            j++;
        }
        j = 0;
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

void cleanMemory(int m){
    for(int i = 0; i < m; i++){
        node *aux = start;
        if(start->next != NULL){
            start = start->next;
        }
        free(aux);
    }
    start = NULL;
    atualNode = NULL;
}


int main()
{
        int m, n;
        int blockSize[] = { 10, 4, 20, 18, 7, 9, 12, 15 };
        int processSize[] = { 12, 10, 9, 5, 25 };

        m = sizeof(blockSize) / sizeof(blockSize[0]);
        n = sizeof(processSize) / sizeof(processSize[0]);

        inicialize( blockSize, processSize, m );
        mostrablocos( m, processSize, n);
        nextFit( m, processSize, n);
        mostrablocos( m, processSize, n);
        printf("O numero de comparacoes e: %d\n\n", compare);
        cleanMemory(m);

        return 0 ;
}
