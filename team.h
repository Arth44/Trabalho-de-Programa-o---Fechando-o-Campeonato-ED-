#ifndef TEAM_H
#define TEAM_H

//Paixão disse que gostou da minha analogia do Hotel para explicar alocação dinâmica e a liberação de memória.
// Bom... aqui vai mais uma:

// Sabe quando você vai num restaurante e pede o cardápio? Você não vê a
// cozinha, não vê as panelas, não vê o cozinheiro suando. Você só vê o que
// pode pedir e o preço. Esse arquivo é esse cardápio: ele diz o que existe
// (as funções) e como chamar, mas não mostra como é feito por dentro. O
// ".c" é a cozinha. Quem usa (o main.c) só precisa do cardápio.

// Tipo opaco: fora daqui, ninguém sabe o que tem dentro de um Team. Só sabe
// que existe. É uma caixa preta: quem usa, não sabe como o interior funciona.

typedef struct team Team;

// Cria um time novo a partir de um id e um nome. Devolve o time pronto,
// ou NULL se a memória não der conta. Pense como "fabricar uma ficha em
// branco e escrever o nome nela".

Team* team_create(int id, const char* nome);

// Os "getters" — perguntar dados do time sem mexer nele. O "const" é a
// promessa: "eu só leio, não modifico".

int team_get_id(const Team* t);
const char* team_get_name(const Team* t);
int team_get_wins(const Team* t);
int team_get_draws(const Team* t);
int team_get_losses(const Team* t);
int team_get_goals_scored(const Team* t);
int team_get_goals_conceded(const Team* t);

// Registra o resultado de uma partida DESTE time. Você passa quantos gols
// ele fez e quantos sofreu, e a função decide sozinha se foi vitória,
// empate ou derrota. O chamador não precisa pensar nisso: menos chance
// de errar. Chame uma vez por time por partida (duas vezes por partida no
// total: uma para cada time).

void team_register_result(Team* t, int goals_scored, int goals_conceded);

// Saldo e pontos NÃO são guardados. São calculados na hora de pedir.
// Por quê? Se guardássemos, alguém poderia somar uma vitória e esquecer
// os 3 pontos — e a tabela ficaria errada sem ninguém perceber. Calculando,
// esse erro nem pode acontecer.

int team_get_goal_difference(const Team* t);   // GM - GS
int team_get_points(const Team* t);            // 3*V + E

// Libera o time da memória. Aceita NULL (não faz nada), para você chamar
// sem medo. Como "devolver o brinquedo e suas peças".

void team_free(Team* t);

#endif
