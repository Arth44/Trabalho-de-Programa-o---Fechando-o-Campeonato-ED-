#ifndef TEAMLIST_H
#define TEAMLIST_H

#include "team.h"

// Sabe aquela fila do banco? Cada pessoa é um time (Team*). A lista não cria
// pessoas: ela só organiza as que já existem, guardando em cada "nó" uma
// setinha para uma pessoa.

// Tipo opaco: fora daqui, só se vê "TeamList*". Ninguém sabe como é por
// dentro. Se um dia a gente trocar lista por vetor, o resto do programa
// nem percebe.

typedef struct teamlist TeamList;

//Leia antes de liberar qualquer lista. Esse é o ponto mais importante do arquivo, 
// então vou repetir em maiúsculo: LEIA ANTES (sim, to apelando para o método Giraldeli). 
//
// A mesma lista pode ter dois papéis:
//
//   lista DONA  -> guarda times que ninguém mais vai liberar (a lista
//                  carregada do CSV). Libere com tl_free() (libera nós +
//                  times).
//
//   lista VISÃO -> guarda os MESMOS Team*, em outra ordem (a tabela de
//                  classificação reusa os times da lista original). Libere
//                  com tl_free_nodes() (libera só os nós, times continuam
//                  vivos).
//
// Trocar as duas gera um erro gigante: usar tl_free() numa
// lista visão libera os times duas vezes -> "double free" -> programa
// aborta.

// Cria uma lista vazia.

TeamList* tl_create(void);

// Adiciona o time NO FIM da lista. Não copia o time: a lista guarda o
// ponteiro recebido (por isso a regra de posse acima). 
// Rápido (O(1)), porque lembramos quem é o último.

void tl_append(TeamList* tl, Team* t);

// Insere o time na posição `pos` (0 = início), empurrando os outros para
// frente. Se `pos` for inválido, ajusta para o limite mais próximo. É a
// operação que a classificação usa: cada time entra onde a regra de
// desempate mandar. Como enfiar uma pessoa numa posição específica da
// fila, pedindo para os de trás darem um passo.

void tl_insert(TeamList* tl, Team* t, int pos);

// Quantos times tem na lista (0 se a lista for NULL).

int tl_size(const TeamList* tl);

// Me dê o time da posição `indice` (ou NULL se não existir). Ex: "me dê
// a 3ª pessoa da fila". Em lista encadeada não tem atalho: custa O(n).

Team* tl_get(const TeamList* tl, int indice);

// Libera só a estrutura da lista, NÃO os times. Use na tabela de
// classificação (visão).

void tl_free_nodes(TeamList* tl);

// Libera tudo: nós e times. Use na lista carregada do CSV (dona).

void tl_free(TeamList* tl);

#endif
