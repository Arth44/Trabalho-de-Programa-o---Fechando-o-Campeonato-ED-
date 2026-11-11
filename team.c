#include "team.h"
#include <stdlib.h>   // malloc, free
#include <string.h>   // strlen, strcpy

// Esse é o único arquivo que conhece a struct team por dentro. Todo o resto
// do programa só vê "Team*" e chama as funções.

struct team {
    int id;                // identificador do time (0..9)
    char* nome;            // nome (copiado para um buffer próprio)
    int vitorias;          // quantas partidas ganhou
    int empates;           // quantas empatou
    int derrotas;          // quantas perdeu
    int gols_marcados;     // total de gols feitos
    int gols_sofridos;     // total de gols sofridos
    // Note: não tem saldo nem pontos. São calculados na hora.
};

// Cria um time novo. Pense como "fabricar uma ficha em branco".
// Passos:
//   1. Se o nome for NULL, desiste (devolve NULL) — evita quebrar depois.
//   2. Pede uma caixa do tamanho de um Team (malloc). Sem memória? Desiste.
//   3. Pede uma caixa para o nome (strlen + 1, o +1 é o '\0'). Se falhar,
//      libera o que já tinha (para não vazar) e desiste.
//   4. Copia o nome para a caixa própria (strcpy). Agora o time não depende
//      do texto original continuar vivo.
//   5. Zera os contadores e devolve o time novinho.

Team* team_create(int id, const char* nome) {
    if (nome == NULL) return NULL;

    Team* t = (Team*) malloc(sizeof(Team));
    if (t == NULL) return NULL;

    t->nome = (char*) malloc(strlen(nome) + 1);
    if (t->nome == NULL) {
        free(t);            // libera o que já tinha pedido
        return NULL;
    }
    strcpy(t->nome, nome);  // copia o nome para a caixa própria

    t->id = id;
    t->vitorias = 0;
    t->empates = 0;
    t->derrotas = 0;
    t->gols_marcados = 0;
    t->gols_sofridos = 0;

    return t;
}

// Os getters: perguntar dados do time. O "const" é a promessa de só ler.
// Cada um devolve um campo da struct. Simples assim.

int team_get_id(const Team* t) { return t->id; }
const char* team_get_name(const Team* t) { return t->nome; }
int team_get_wins(const Team* t) { return t->vitorias; }
int team_get_draws(const Team* t) { return t->empates; }
int team_get_losses(const Team* t) { return t->derrotas; }
int team_get_goals_scored(const Team* t) { return t->gols_marcados; }
int team_get_goals_conceded(const Team* t) { return t->gols_sofridos; }

// Registra o resultado de uma partida DESTE time.
// Você passa o placar do ponto de vista do time: gols que ele fez e gols
// que ele sofreu.
//
// Passos:
//   1. Se o time for NULL, não faz nada (seguro).
//   2. Soma os gols aos totais (sempre, independente do resultado).
//   3. Compara os gols para decidir o desfecho:
//        fez > sofreu  -> vitória++
//        fez == sofreu -> empate++
//        fez < sofreu  -> derrota++
//
// O chamador só passa o placar; a função faz o raciocínio. Assim não há
// chance de marcar vitória e esquecer de somar pontos (que são derivados).

void team_register_result(Team* t, int goals_scored, int goals_conceded) {
    if (t == NULL) return;

    // Os gols entram sempre, independente do resultado...
    t->gols_marcados += goals_scored;
    t->gols_sofridos += goals_conceded;

    // ...e o desfecho define qual dos três contadores cresce.
    if (goals_scored > goals_conceded)
        t->vitorias++;
    else if (goals_scored == goals_conceded)
        t->empates++;
    else
        t->derrotas++;
}

// Derivados: calculados na hora, nunca guardados.
//   saldo  = gols marcados - gols sofridos
//   pontos = 3*vitórias + empates
// Como não são campos, não podem ficar inconsistentes.

int team_get_goal_difference(const Team* t) {
    return t->gols_marcados - t->gols_sofridos;
}

int team_get_points(const Team* t) {
    return 3 * t->vitorias + t->empates;
}

// Libera o time da memória. Libera o nome (que foi alocado separado) e
// depois o time. Ordem importa: se liberar o time antes, perde a setinha
// para o nome (vazamento). Aceita NULL.
// É como "devolver o brinquedo e suas peças" — primeiro as peças, depois
// o brinquedo.

void team_free(Team* t) {
    if (t != NULL) {
        free(t->nome);   // 1º: libera o nome
        free(t);         // 2º: libera o time
    }
}
