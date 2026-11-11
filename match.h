#ifndef MATCH_H
#define MATCH_H

// Uma partida é uma ficha com 5 campos, exatamente como no arquivo
// partidas.csv: ID, Time1, Time2, GolsTime1, GolsTime2. É como anotar o
// resultado de um jogo num papel: "jogo 3: time 1 vs time 2, placar X a Y".

// Os times são guardados por ID (inteiro), NÃO por ponteiro. Por quê?
// Uma partida é um REGISTRO HISTÓRICO: ela não deve manter um time vivo nem
// depender da ordem em que os times foram carregados. Quem apura traduz
// ID -> Team* quando precisar.

typedef struct match Match;

// Cria uma partida com os 5 campos. Devolve NULL se sem memória.

Match* match_create(int id, int team1, int team2, int goals_team1, int goals_team2);

// Getters: só leem (const).

int match_get_id(const Match* m);
int match_get_team1(const Match* m);
int match_get_team2(const Match* m);
int match_get_goals_team1(const Match* m);
int match_get_goals_team2(const Match* m);

// Diferença absoluta de gols: |GolsTime1 - GolsTime2|. É o critério "maior
// goleada" do relatório. Ficar AQUI evita que o main invente a fórmula (e
// erre o sinal) em dois lugares diferentes.

int match_get_goal_margin(const Match* m);

// Libera a partida. Aceita NULL.

void match_free(Match* m);

#endif
