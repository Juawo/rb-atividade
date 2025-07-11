#include<stdio.h>
#include<stdlib.h>
#include "ArvoreLLRB.h"

int main(int argc, char** argv) {

    ArvLLRB r;
    int v, b, n;
    int continuar = 1;

    do {

        printf("\n\tMenu de Opção\n\n");
        printf("1. Inicializa a arvore \n");
        printf("2. Insere na árvore\n");
        printf("3. Buscar um valor na árvore\n");
        printf("4. Conta o número de nós da árvore\n");
        printf("5. Imprime a árvore\n");
        printf("6.Remove um nó da árvore\n");
        printf("0.  Sair\n");

        scanf("%d", &continuar);
        system("cls || clear");

        switch (continuar) {
            case 1:
                r = NULL;
                printf("Árvore criada com sucesso!");
                break;

            case 2:
                printf("Digite um número para inserir na arvore:");
                scanf("%d", &v);
                if (insere_ArvLLRB(&r, v)) {
                    printf("Nó inserido com sucesso!");
                } else {
                    printf("Erro ao inserir o nó ou valor já existe!");
                }
                break;
                
            case 3:
                printf("Digite um valor para pesquisar na árvores!!");
                scanf("%d", &b);
                if (consulta_ArvLLRB(&r, b)) {
                    printf("O valor %d foi localizado na árvore", b);
                } else {
                    printf("O valor %d, não foi localizado", b);
                }
                break;
                
            case 4:
                n = numeroNos(&r);
                printf("A árvore possui: %d nós", n);
                break;
                
            case 5:
                exibirArvore(&r);
                break;
            
            case 6:
                printf("Digite um valor para remover da árvore:");
                scanf("%d",&v);
                if (remove_ArvLLRB(&r, v)) {
                    printf("Valor removido com sucesso!");
                } else {
                    printf("Valor não encontrado na árvore!");
                }
                break;

            case 0:
                break;
            default:
                printf("Digite uma opção valida\n");
        }
    } while (continuar);
}