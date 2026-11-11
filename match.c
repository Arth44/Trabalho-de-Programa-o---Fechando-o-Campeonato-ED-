#include "match.h"
#include <stdlib.h>

// Como a struct só tem inteiros (nenhum ponteiro, nenhuma string alocada),
// criar e liberar são operações simétricas e sem alocação secundária. Não
// há risco de vazamento dentro do Match.

struct match {
    int id;            // identificador da partida
    int time1;         // id do time 1
    int time2;         // id do time 2
    int gols_time1;    // gols do time 1
    int gols_time2;    // gols do time 2
};

// Cria uma partida. Aloca a struct e preenche os 5 campos. Devolve NULL se
// sem memória. É como montar a ficha dum jogo.

Match* match_create(int id, int team1, int team2, int goals_team1, int goals_team2) {
    Match* m = (Match*) malloc(sizeof(Match));
    if (m == NULL) return NULL;

    m->id = id;
    m->time1 = team1;
    m->time2 = team2;
    m->gols_time1 = goals_team1;
    m->gols_time2 = goals_team2;

    return m;
}

// Getters: só leem (const). Cada um devolve um campo.

int match_get_id(const Match* m) { return m->id; }
int match_get_team1(const Match* m) { return m->time1; }
int match_get_team2(const Match* m) { return m->time2; }
int match_get_goals_team1(const Match* m) { return m->gols_time1; }
int match_get_goals_team2(const Match* m) { return m->gols_time2; }

// Diferença absoluta de gols: |gols1 - gols2|. Se a diferença for negativa,
// troca o sinal (-diff). Resultado nunca negativo. Usado para "maior
// goleada".

int match_get_goal_margin(const Match* m) {
    int diff = m->gols_time1 - m->gols_time2;
    return (diff < 0) ? -diff : diff;
}

// Libera a partida. Como não tem nada alocado dentro, basta free(m).

void match_free(Match* m) {
    free(m);
}
