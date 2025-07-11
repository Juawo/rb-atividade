#include <stdio.h>
#include <stdlib.h>
#include "ArvoreLLRB.h"

int cor(struct NO *H)
{
    if (H == NULL)
        return BLACK;
    else
        return H->cor;
}

struct NO *criaNo(TIPOCHAVE valor)
{
    struct NO *novoNo = (struct NO *)malloc(sizeof(struct NO));
    novoNo->esq = NULL;
    novoNo->dir = NULL;
    novoNo->info = valor;
    novoNo->cor = RED;
    return novoNo;
}

void trocaCor(struct NO *H)
{
    H->cor = !H->cor;
    if (H->esq != NULL)
        H->esq->cor = !H->esq->cor;
    if (H->dir != NULL)
        H->dir->cor = !H->dir->cor;
}

struct NO *rotacionaEsquerda(struct NO *A)
{
    struct NO *B = A->dir;
    A->dir = B->esq;
    B->esq = A;
    B->cor = A->cor;
    A->cor = RED;
    return B;
}

struct NO *rotacionaDireita(struct NO *A)
{
    struct NO *B = A->esq;
    A->esq = B->dir;
    B->dir = A;
    B->cor = A->cor;
    A->cor = RED;
    return B;
}

struct NO *insereNO(struct NO *H, int valor, int *resp)
{
    if (H == NULL)
    {
        struct NO *novo = criaNo(valor);
        if (novo == NULL)
        {
            *resp = 0;
            return NULL;
        }

        *resp = 1;
        return novo;
    }

    if (valor == H->info)
    {
        *resp = 0;
    }
    else if (valor < H->info)
    {
        H->esq = insereNO(H->esq, valor, resp);
    }
    else
    {
        H->dir = insereNO(H->dir, valor, resp);
    }

    if (cor(H->dir) == RED && cor(H->esq) == BLACK)
        H = rotacionaEsquerda(H);

    if (cor(H->esq) == RED && cor(H->esq->esq) == RED)
        H = rotacionaDireita(H);

    if (cor(H->esq) == RED && cor(H->dir) == RED)
        trocaCor(H);

    return H;
}

int insere_ArvLLRB(ArvLLRB *raiz, int valor)
{
    int resp;
    *raiz = insereNO(*raiz, valor, &resp);
    if ((*raiz) != NULL)
        (*raiz)->cor = BLACK;
    return resp;
}

struct NO *move2EsqRED(struct NO *H)
{
    trocaCor(H);
    if (cor(H->dir->esq) == RED)
    {
        H->dir = rotacionaDireita(H->dir);
        H = rotacionaEsquerda(H);
        trocaCor(H);
    }
    return H;
}

struct NO *move2DirRED(struct NO *H)
{
    trocaCor(H);
    if (cor(H->esq->esq) == RED)
    {
        H = rotacionaDireita(H);
        trocaCor(H);
    }
    return H;
}

struct NO *balancear(struct NO *H)
{
    if (cor(H->dir) == RED)
        H = rotacionaEsquerda(H);

    if (H->esq != NULL && cor(H->esq) == RED && cor(H->esq->esq) == RED)
        H = rotacionaDireita(H);

    if (cor(H->esq) == RED && cor(H->dir) == RED)
        trocaCor(H);

    return H;
}

struct NO *procuraMenor(struct NO *atual)
{
    struct NO *no1 = atual;
    struct NO *no2 = atual->esq;
    while (no2 != NULL)
    {
        no1 = no2;
        no2 = no2->esq;
    }
    return no1;
}

struct NO *removerMenor(struct NO *H)
{
    if (H->esq == NULL)
    {
        free(H);
        return NULL;
    }
    if (cor(H->esq) == BLACK && cor(H->esq->esq) == BLACK)
        H = move2EsqRED(H);

    H->esq = removerMenor(H->esq);
    return balancear(H);
}

struct NO *removeNO(struct NO *H, int valor)
{
    if (valor < H->info)
    {
        if (cor(H->esq) == BLACK && cor(H->esq->esq) == BLACK)
            H = move2EsqRED(H);
        H->esq = removeNO(H->esq, valor);
    }
    else
    {
        if (cor(H->esq) == RED)
            H = rotacionaDireita(H);

        if (valor == H->info && (H->dir == NULL))
        {
            free(H);
            return NULL;
        }
        if (cor(H->dir) == BLACK && cor(H->dir->esq) == BLACK)
            H = move2DirRED(H);

        if (valor == H->info)
        {
            struct NO *x = procuraMenor(H->dir);
            H->info = x->info;
            H->dir = removerMenor(H->dir);
        }
        else
        {
            H->dir = removeNO(H->dir, valor);
        }
    }
    return balancear(H);
}

int remove_ArvLLRB(ArvLLRB *raiz, int valor)
{
    if (consulta_ArvLLRB(raiz, valor))
    {
        struct NO *h = *raiz;
        *raiz = removeNO(h, valor);
        if ((*raiz) != NULL)
            (*raiz)->cor = BLACK;
        return 1;
    }
    else
    {
        return 0;
    }
}

int consulta_ArvLLRB(ArvLLRB *raiz, int valor)
{
    if (raiz == NULL || *raiz == NULL)
    {
        return 0;
    }
    struct NO *atual = *raiz;
    while (atual != NULL)
    {
        if (valor == atual->info)
        {
            return 1;
        }
        if (valor < atual->info)
        {
            atual = atual->esq;
        }
        else
        {
            atual = atual->dir;
        }
    }
    return 0;
}

int numeroNos(ArvLLRB *raiz)
{
    if (raiz == NULL || *raiz == NULL)
    {
        return 0;
    }
    return 1 + numeroNos(&((*raiz)->esq)) + numeroNos(&((*raiz)->dir));
}

void exibirArvore_recursivo(struct NO *raiz)
{
    if (raiz == NULL)
    {
        printf("NULL");
        return;
    }

    printf("%d(%c)", raiz->info, (raiz->cor == RED) ? 'R' : 'B');

    if (raiz->esq != NULL || raiz->dir != NULL)
    {
        printf("(");
        exibirArvore_recursivo(raiz->esq);
        printf(",");
        exibirArvore_recursivo(raiz->dir);
        printf(")");
    }
}

void exibirArvore(ArvLLRB *raiz)
{
    if (raiz == NULL || *raiz == NULL)
    {
        printf("Árvore vazia!\n");
        return;
    }
    printf("Árvore (R = Vermelho, B = Preto):\n");
    exibirArvore_recursivo(*raiz);
    printf("\n");
}