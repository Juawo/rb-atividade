#ifndef ARVORELLRB_H
#define ARVORELLRB_H

typedef struct NO* ArvLLRB;

// Definições de cores
#define RED 1
#define BLACK 0

// Estrutura do nó
struct NO {
    int info;
    struct NO *esq;
    struct NO *dir;
    int cor;
};

typedef int TIPOCHAVE;

// Funções

int cor(struct NO* H);
struct NO *criaNo(TIPOCHAVE valor);
void trocaCor(struct NO *H);
struct NO* rotacionaEsquerda(struct NO* A);
struct NO* rotacionaDireita(struct NO* A);
int insere_ArvLLRB(ArvLLRB* raiz, int valor);
struct NO* insereNO(struct NO* H, int valor, int* resp);
int remove_ArvLLRB(ArvLLRB* raiz, int valor);
struct NO* removeNO(struct NO* H, int valor);
struct NO* move2EsqRED(struct NO* H);
struct NO* move2DirRED(struct NO* H);
struct NO* balancear(struct NO* H);
struct NO* procuraMenor(struct NO* atual);
struct NO* removerMenor(struct NO* H);
int consulta_ArvLLRB(ArvLLRB *raiz, int valor); // Necessário para a função remove_ArvLLRB
int numeroNos(ArvLLRB* raiz);
void exibirArvore(ArvLLRB* raiz);

#endif