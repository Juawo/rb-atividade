#include <stdio.h>
#include <stdlib.h>
#include "ArvoreLLRB.h"

// Função que retorna a cor de um nó (NULL é sempre preto)
int cor(struct NO *H)
{
    if (H == NULL)
        return BLACK; // Nós NULL são sempre pretos
    else
        return H->cor; // Retorna a cor do nó
}

// Função que cria um novo nó (sempre vermelho inicialmente)
struct NO *criaNo(TIPOCHAVE valor)
{
    struct NO *novoNo = (struct NO *)malloc(sizeof(struct NO));
    novoNo->esq = NULL;   // Filho esquerdo vazio
    novoNo->dir = NULL;   // Filho direito vazio
    novoNo->info = valor; // Armazena o valor
    novoNo->cor = RED;    // Novos nós sempre vermelhos
    return novoNo;
}

// Função que troca as cores do nó e seus filhos
void trocaCor(struct NO *H)
{
    H->cor = !H->cor; // Inverte a cor do nó atual
    if (H->esq != NULL)
        H->esq->cor = !H->esq->cor; // Inverte cor do filho esquerdo
    if (H->dir != NULL)
        H->dir->cor = !H->dir->cor; // Inverte cor do filho direito
}

// Rotação à esquerda (corrige link vermelho à direita)
struct NO *rotacionaEsquerda(struct NO *A)
{
    struct NO *B = A->dir; // B será a nova raiz
    A->dir = B->esq;       // Move subárvore esquerda de B para direita de A
    B->esq = A;            // A vira filho esquerdo de B
    B->cor = A->cor;       // B herda a cor de A
    A->cor = RED;          // A fica vermelho
    return B;              // Nova raiz
}

// Rotação à direita (corrige dois links vermelhos consecutivos à esquerda)
struct NO *rotacionaDireita(struct NO *A)
{
    struct NO *B = A->esq; // B será a nova raiz
    A->esq = B->dir;       // Move subárvore direita de B para esquerda de A
    B->dir = A;            // A vira filho direito de B
    B->cor = A->cor;       // B herda a cor de A
    A->cor = RED;          // A fica vermelho
    return B;              // Nova raiz
}

// Função auxiliar para inserir um nó na árvore
struct NO *insereNO(struct NO *H, int valor, int *resp)
{
    // Caso base: posição vazia encontrada
    if (H == NULL)
    {
        struct NO *novo = criaNo(valor);
        if (novo == NULL)
        {
            *resp = 0; // Erro na alocação
            return NULL;
        }

        *resp = 1; // Sucesso
        return novo;
    }

    // Inserção recursiva padrão de BST
    if (valor == H->info)
    {
        *resp = 0; // Valor já existe
    }
    else if (valor < H->info)
    {
        H->esq = insereNO(H->esq, valor, resp); // Insere à esquerda
    }
    else
    {
        H->dir = insereNO(H->dir, valor, resp); // Insere à direita
    }

    // Aplicação das regras LLRB para balanceamento

    // Regra 1: Link vermelho à direita vira link vermelho à esquerda
    if (cor(H->dir) == RED && cor(H->esq) == BLACK)
        H = rotacionaEsquerda(H);

    // Regra 2: Dois links vermelhos consecutivos à esquerda
    if (cor(H->esq) == RED && cor(H->esq->esq) == RED)
        H = rotacionaDireita(H);

    // Regra 3: Ambos os filhos vermelhos - propaga vermelho para cima
    if (cor(H->esq) == RED && cor(H->dir) == RED)
        trocaCor(H);

    return H;
}

// Função pública para inserir na árvore (garante que raiz seja preta)
int insere_ArvLLRB(ArvLLRB *raiz, int valor)
{
    int resp;
    *raiz = insereNO(*raiz, valor, &resp);
    if ((*raiz) != NULL)
        (*raiz)->cor = BLACK; // Raiz sempre deve ser preta
    return resp;
}

// Move link vermelho para a esquerda (usado na remoção)
struct NO *move2EsqRED(struct NO *H)
{
    trocaCor(H); // Troca as cores
    if (cor(H->dir->esq) == RED)
    {
        H->dir = rotacionaDireita(H->dir); // Rotaciona filho direito
        H = rotacionaEsquerda(H);          // Rotaciona nó atual
        trocaCor(H);                       // Troca as cores novamente
    }
    return H;
}

// Move link vermelho para a direita (usado na remoção)
struct NO *move2DirRED(struct NO *H)
{
    trocaCor(H); // Troca as cores
    if (cor(H->esq->esq) == RED)
    {
        H = rotacionaDireita(H); // Rotaciona para a direita
        trocaCor(H);             // Troca as cores novamente
    }
    return H;
}

// Função que rebalanceia a árvore após remoção
struct NO *balancear(struct NO *H)
{
    // Corrige link vermelho à direita
    if (cor(H->dir) == RED)
        H = rotacionaEsquerda(H);

    // Corrige dois links vermelhos consecutivos à esquerda
    if (H->esq != NULL && cor(H->esq) == RED && cor(H->esq->esq) == RED)
        H = rotacionaDireita(H);

    // Corrige ambos os filhos vermelhos
    if (cor(H->esq) == RED && cor(H->dir) == RED)
        trocaCor(H);

    return H;
}

// Função que encontra o menor valor na subárvore
struct NO *procuraMenor(struct NO *atual)
{
    struct NO *no1 = atual;
    struct NO *no2 = atual->esq;
    while (no2 != NULL) // Vai para o nó mais à esquerda
    {
        no1 = no2;
        no2 = no2->esq;
    }
    return no1; // Retorna o menor valor
}

// Função que remove o menor nó da subárvore
struct NO *removerMenor(struct NO *H)
{
    if (H->esq == NULL) // Se não tem filho esquerdo
    {
        free(H); // Remove o nó
        return NULL;
    }
    // Se ambos os filhos são pretos, move vermelho para esquerda
    if (cor(H->esq) == BLACK && cor(H->esq->esq) == BLACK)
        H = move2EsqRED(H);

    H->esq = removerMenor(H->esq); // Remove recursivamente
    return balancear(H);           // Rebalanceia
}

// Função auxiliar para remover um nó específico
struct NO *removeNO(struct NO *H, int valor)
{
    if (valor < H->info) // Valor está à esquerda
    {
        // Se ambos os filhos são pretos, move vermelho para esquerda
        if (cor(H->esq) == BLACK && cor(H->esq->esq) == BLACK)
            H = move2EsqRED(H);
        H->esq = removeNO(H->esq, valor); // Remove recursivamente
    }
    else // Valor está à direita ou é o valor atual
    {
        // Se filho esquerdo é vermelho, rotaciona para direita
        if (cor(H->esq) == RED)
            H = rotacionaDireita(H);

        // Se valor encontrado e não tem filho direito
        if (valor == H->info && (H->dir == NULL))
        {
            free(H);
            return NULL;
        }
        // Se ambos os filhos são pretos, move vermelho para direita
        if (cor(H->dir) == BLACK && cor(H->dir->esq) == BLACK)
            H = move2DirRED(H);

        if (valor == H->info) // Valor encontrado
        {
            struct NO *x = procuraMenor(H->dir); // Encontra sucessor
            H->info = x->info;                   // Substitui valor
            H->dir = removerMenor(H->dir);       // Remove sucessor
        }
        else
        {
            H->dir = removeNO(H->dir, valor); // Remove recursivamente
        }
    }
    return balancear(H); // Rebalanceia
}

// Função pública para remover um valor da árvore
int remove_ArvLLRB(ArvLLRB *raiz, int valor)
{
    if (consulta_ArvLLRB(raiz, valor)) // Verifica se valor existe
    {
        struct NO *h = *raiz;
        *raiz = removeNO(h, valor); // Remove o valor
        if ((*raiz) != NULL)
            (*raiz)->cor = BLACK; // Raiz sempre deve ser preta
        return 1;                 // Sucesso
    }
    else
    {
        return 0; // Valor não encontrado
    }
}

// Função para consultar/buscar um valor na árvore
int consulta_ArvLLRB(ArvLLRB *raiz, int valor)
{
    if (raiz == NULL || *raiz == NULL)
    {
        return 0; // Árvore vazia
    }
    struct NO *atual = *raiz;
    while (atual != NULL) // Percorre a árvore
    {
        if (valor == atual->info)
        {
            return 1; // Valor encontrado
        }
        if (valor < atual->info)
        {
            atual = atual->esq; // Vai para esquerda
        }
        else
        {
            atual = atual->dir; // Vai para direita
        }
    }
    return 0; // Valor não encontrado
}

// Função para contar o número de nós na árvore
int numeroNos(ArvLLRB *raiz)
{
    if (raiz == NULL || *raiz == NULL)
    {
        return 0; // Árvore vazia
    }
    // Conta 1 (nó atual) + nós da subárvore esquerda + nós da subárvore direita
    return 1 + numeroNos(&((*raiz)->esq)) + numeroNos(&((*raiz)->dir));
}

// Função auxiliar recursiva para exibir a árvore
void exibirArvore_recursivo(struct NO *raiz)
{
    if (raiz == NULL)
    {
        printf("NULL"); // Indica nó vazio
        return;
    }

    // Exibe: valor(cor)
    printf("%d(%c)", raiz->info, (raiz->cor == RED) ? 'R' : 'B');

    // Se tem filhos, exibe no formato: valor(filhoEsq,filhoDir)
    if (raiz->esq != NULL || raiz->dir != NULL)
    {
        printf("(");
        exibirArvore_recursivo(raiz->esq); // Filho esquerdo
        printf(",");
        exibirArvore_recursivo(raiz->dir); // Filho direito
        printf(")");
    }
}

// Função pública para exibir a árvore
void exibirArvore(ArvLLRB *raiz)
{
    if (raiz == NULL || *raiz == NULL)
    {
        printf("Árvore vazia!\n");
        return;
    }
    printf("Árvore (R = Vermelho, B = Preto):\n");
    exibirArvore_recursivo(*raiz); // Chama função recursiva
    printf("\n");
}