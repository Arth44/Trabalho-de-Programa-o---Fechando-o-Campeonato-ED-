#include "matchlist.h"
#include <stdlib.h>

// Esse é o único arquivo que conhece a struct MLNode e a struct matchlist.
// Fora, só se vê "MatchList*" (tipo opaco).

// Um "nó" da lista de partidas. Cada nó guarda:
//   - partida: setinha para um Match
//   - next:    setinha para o próximo nó (só para frente, sem `prev`)
// É como uma corrente de um só sentido.

typedef struct ml_node {
    Match* partida;
    struct ml_node* next;
} MLNode;

// A lista em si: primeiro nó, último nó, quantidade.

struct matchlist {
    MLNode* primeiro;
    MLNode* ultimo;
    int quantidade;
};

// Cria uma lista vazia. Como "pedir uma pilha vazia de fichas".

MatchList* ml_create(void) {
    MatchList* ml = (MatchList*) malloc(sizeof(MatchList));
    if (ml == NULL) return NULL;

    ml->primeiro = NULL;
    ml->ultimo = NULL;
    ml->quantidade = 0;

    return ml;
}

// Adiciona a partida NO FIM, preservando a ordem do arquivo.
//
// Passos:
//   1. Se ml for NULL, não faz nada (seguro).
//   2. Aloca um nó novo. Se falhar, desiste.
//   3. Guarda a partida e next = NULL (é o fim, não tem próximo).
//   4. Se já existe último, liga último->next ao novo.
//      Se não existe (lista vazia), o novo é o primeiro.
//   5. Atualiza último e quantidade.

void ml_append(MatchList* ml, Match* m) {
    if (ml == NULL) return;

    MLNode* novo = (MLNode*) malloc(sizeof(MLNode));
    if (novo == NULL) return;

    novo->partida = m;
    novo->next = NULL;

    if (ml->ultimo != NULL)
        ml->ultimo->next = novo;
    else
        ml->primeiro = novo;

    ml->ultimo = novo;
    ml->quantidade++;
}

// Quantas partidas tem (0 se ml for NULL).

int ml_size(const MatchList* ml) {
    return (ml == NULL) ? 0 : ml->quantidade;
}

// Me dê a partida da posição `indice` (ou NULL se inválido). Anda do início,
// contando. Como "me dê o 3º jogo da lista". Em lista simples não tem
// atalho pelo fim.

Match* ml_get(const MatchList* ml, int indice) {
    if (ml == NULL || indice < 0 || indice >= ml->quantidade) return NULL;

    MLNode* atual = ml->primeiro;
    for (int i = 0; i < indice; i++)
        atual = atual->next;

    return atual->partida;
}

// Libera TUDO: nós, estrutura e partidas. A MatchList é DONA das partidas,
// então libera cada Match com match_free. Guarda o próximo ANTES de liberar
// o atual (senão perde a referência).

void ml_free(MatchList* ml) {
    if (ml == NULL) return;

    MLNode* atual = ml->primeiro;
    while (atual != NULL) {
        MLNode* proximo = atual->next;
        match_free(atual->partida);
        free(atual);
        atual = proximo;
    }

    free(ml);
}
