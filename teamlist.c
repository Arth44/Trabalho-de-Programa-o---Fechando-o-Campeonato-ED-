#include "teamlist.h"
#include <stdlib.h>


// Esse é o único arquivo que conhece a struct Node e a struct teamlist.
// Todo o resto do programa só vê "TeamList*" e chama as funções. 

// Um "nó" da lista. Cada nó guarda:
//   - time: setinha para um Team (a pessoa na fila)
//   - next: setinha para o PRÓXIMO nó (quem está atrás)
//   - prev: setinha para o nó ANTERIOR (quem está na frente)
// A lista é "dupla" porque tem next E prev. É como uma corrente em que
// cada elo conhece o elo da frente e o de trás.

typedef struct node {
    Team* time;
    struct node* next;
    struct node* prev;
} Node;

// A lista em si: primeiro nó, último nó, quantidade.

struct teamlist {
    Node* primeiro;
    Node* ultimo;
    int quantidade;
};

// Cria uma lista vazia. Como "pedir uma fila vazia ao almoxarifado": vem
// sem ninguém, com o contador zerado.

TeamList* tl_create(void) {
    TeamList* tl = (TeamList*) malloc(sizeof(TeamList));
    if (tl == NULL) return NULL;

    tl->primeiro = NULL;
    tl->ultimo = NULL;
    tl->quantidade = 0;
    return tl;
}

// ligar_entre: função INTERNA (static = só pode ser usada neste arquivo).
// Costura um nó novo entre dois nós existentes (`antes` e `depois`).
//
// Analogia: enfiar uma pessoa entre duas pessoas numa fila.
//   - o novo aponta para trás (antes) e para frente (depois);
//   - se existe pessoa antes, ela passa a apontar para o novo;
//     se não existe (novo é o primeiro), a lista começa no novo;
//   - se existe pessoa depois, ela passa a apontar para trás para o novo;
//     se não existe (novo é o último), a lista termina no novo;
//   - aumenta o contador.
//
// Por que centralizar? Para append e insert usarem a MESMA costura. Se
// corrigirmos um bug aqui, os dois métodos herdam a correção.

static void ligar_entre(TeamList* tl, Node* novo, Node* antes, Node* depois) {
    novo->prev = antes;
    novo->next = depois;

    if (antes != NULL) antes->next = novo;
    else               tl->primeiro = novo;

    if (depois != NULL) depois->prev = novo;
    else                tl->ultimo = novo;

    tl->quantidade++;
}

// Adiciona no FIM.
// Rápido (O(1)), porque lembramos quem é o último.

void tl_append(TeamList* tl, Team* t) {
    if (tl == NULL) return;

    Node* novo = (Node*) malloc(sizeof(Node));
    if (novo == NULL) return;

    novo->time = t;
    ligar_entre(tl, novo, tl->ultimo, NULL);  // costura no fim (depois = NULL)
}

// Insere na posição `pos`, empurrando os outros para frente.
//
// Passos:
//   1. Normaliza pos: negativo vira 0; maior que o tamanho vira o fim.
//   2. Se pos == tamanho, usa append (mais rápido).
//   3. Cria o nó com o time.
//   4. Acha o nó que hoje ocupa `pos` (o novo entra antes dele). Atalho:
//      se pos está na segunda metade, anda de trás para frente (visita menos
//      nós). É como pegar o atalho mais curto numa fila.
//   5. Costura com ligar_entre.

void tl_insert(TeamList* tl, Team* t, int pos) {
    if (tl == NULL) return;

    if (pos < 0) pos = 0;
    if (pos > tl->quantidade) pos = tl->quantidade;

    if (pos == tl->quantidade) {
        tl_append(tl, t);
        return;
    }

    Node* novo = (Node*) malloc(sizeof(Node));
    if (novo == NULL) return;
    novo->time = t;

    Node* depois;
    if (pos < tl->quantidade / 2) {
        // pos na primeira metade: anda para frente a partir do início
        depois = tl->primeiro;
        for (int i = 0; i < pos; i++)
            depois = depois->next;
    } else {
        // pos na segunda metade: anda para trás a partir do fim (mais curto)
        depois = tl->ultimo;
        for (int i = tl->quantidade - 1; i > pos; i--)
            depois = depois->prev;
    }

    ligar_entre(tl, novo, depois->prev, depois);
}

// Quantos times tem na lista? Se a lista for NULL, devolve 0 (seguro para
// quem chama). Não percorre nada, só lê o contador.

int tl_size(const TeamList* tl) {
    return (tl == NULL) ? 0 : tl->quantidade;
}

// Me dê o time da posição `indice` (ou NULL se inválido). Valida o índice e
// anda pelo lado mais curto. "Me dê a 3ª pessoa da fila": você conta
// a partir do início ou do fim, o que for mais perto.

Team* tl_get(const TeamList* tl, int indice) {
    if (tl == NULL || indice < 0 || indice >= tl->quantidade) return NULL;

    Node* atual;
    if (indice < tl->quantidade / 2) {
        atual = tl->primeiro;
        for (int i = 0; i < indice; i++)
            atual = atual->next;
    } else {
        atual = tl->ultimo;
        for (int i = tl->quantidade - 1; i > indice; i--)
            atual = atual->prev;
    }

    return atual->time;
}

// Libera só a ESTRUTURA da lista, não os times. Use na tabela de
// classificação (visão): ela guarda os mesmos Team* da lista original, em
// outra ordem. Os times continuam vivos porque pertencem a outra lista.

void tl_free_nodes(TeamList* tl) {
    if (tl == NULL) return;

    Node* atual = tl->primeiro;
    while (atual != NULL) {
        Node* proximo = atual->next;
        free(atual);        // libera só o nó, não o time
        atual = proximo;
    }

    free(tl);
}

// Libera TUDO: nós e times. Use na lista que é DONA dos times (a carregada
// do CSV). Guarda o próximo ANTES de liberar o atual (senão perde a
// referência).

void tl_free(TeamList* tl) {
    if (tl == NULL) return;

    Node* atual = tl->primeiro;
    while (atual != NULL) {
        Node* proximo = atual->next;
        team_free(atual->time);   // libera o time (dono)
        free(atual);              // libera o nó
        atual = proximo;
    }

    free(tl);
}
