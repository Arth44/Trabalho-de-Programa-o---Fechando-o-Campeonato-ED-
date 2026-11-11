#ifndef MATCHLIST_H
#define MATCHLIST_H

#include "match.h"

// Implementação: lista SIMPLESMENTE encadeada (só ponteiro `next`, sem
// `prev`). Por quê simples? Porque as partidas só são percorridas do início
// ao fim, na ordem de leitura. Não há busca por posição nem inserção no
// meio, então o ponteiro `prev` seria "peso morto" (memória gasta sem uso).
// E o enunciado pede lista encadeada, não dupla.
//
// A ORDEM É REQUISITO: se duas partidas empatarem em diferença de gols, a
// maior goleada é a que aparece PRIMEIRO no arquivo. Logo a lista não pode
// reordenar nada. É como ordem de chegada: quem chegou primeiro tem
// prioridade.

// Posse: a MatchList é DONA das suas partidas. ml_free() libera a lista, os
// nós e cada Match.

typedef struct matchlist MatchList;

// Cria uma lista vazia.

MatchList* ml_create(void);

// Adiciona a partida NO FIM (preserva a ordem do arquivo). Rápido (O(1)),
// porque lembramos quem é o último.

void ml_append(MatchList* ml, Match* m);

// Quantas partidas tem (0 se a lista for NULL).

int ml_size(const MatchList* ml);

// Me dê a partida da posição `indice` (ou NULL se inválido). Anda do
// início, contando. Como "me dê o 3º jogo da lista".

Match* ml_get(const MatchList* ml, int indice);

// Libera tudo: nós, estrutura e partidas. Aceita NULL.

void ml_free(MatchList* ml);

#endif
