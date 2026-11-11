#include "team.h"
#include "teamlist.h"
#include <stdio.h>

// Esse main é só um rascunho: lê times.csv e imprime uma tabela com zeros.
// Serve para mostrar que os TADs compilam e funcionam. (É aqui que entra seu trampo, Caio)

// utf8_continuation_bytes: conta quantos bytes de "continuação" UTF-8 o nome tem.

// Em UTF-8, caracteres acentuados (ex.: "õ" de ESCorpiões) ocupam 2 bytes:
//   - o primeiro começa com 11xxxxxx
//   - o segundo (continuação) começa com 10xxxxxx
// A máscara 0xC0 (11000000) + comparação com 0x80 (10000000) detecta
// esses bytes.

// Por que isso importa? Porque printf com "%-15s" conta BYTES, não letras.
// Um nome com acento ocupa mais bytes e a coluna sai torta.

static int utf8_continuation_bytes(const char *s) {
    int n = 0;
    for (const unsigned char *p = (const unsigned char *)s; *p; p++)
        if ((*p & 0xC0) == 0x80)
            n++;
    return n;
}

int main(void) {
    // 1. Abre o arquivo de times para leitura em modo texto ("rt").
    FILE *f = fopen("times.csv", "rt");
    if (f == NULL) return 1;   // arquivo não encontrado -> erro

    // 2. Cria uma lista vazia para guardar os times.
    TeamList *tl = tl_create();
    if (tl == NULL) {
        fclose(f);
        return 1;              // sem memória -> erro
    }

    // 3. Lê linha por linha: id + nome.
    //    Formato esperado: "0,JAVAlis"
    //    %d         -> lê um inteiro (id)
    //    ,          -> espera uma vírgula
    //    %49[^\r\n] -> lê até 49 chars que NÃO sejam \r nem \n (o nome)
    //    == 2       -> sucesso se leu os 2 campos

    int id;
    char name[50];
    while (fscanf(f, "%d,%49[^\r\n]", &id, name) == 2) {
        Team *t = team_create(id, name);
        if (t != NULL)
            tl_append(tl, t);
    }
    fclose(f);

    // 4. Imprime a tabela (rascunho: estatísticas zeradas).
    printf("ID  Time            V   E   D   GM  GS  S   PG\n");
    for (int i = 0; i < tl_size(tl); i++) {
        Team *t = tl_get(tl, i);
        int goal_diff = team_get_goals_scored(t) - team_get_goals_conceded(t);
        int points = 3 * team_get_wins(t) + team_get_draws(t);
        printf("%-3d %-15s", team_get_id(t), team_get_name(t));
        for (int j = 0; j < utf8_continuation_bytes(team_get_name(t)); j++)
            putchar(' ');
        printf(" %-3d %-3d %-3d %-3d %-3d %-3d %-3d\n",
               team_get_wins(t), team_get_draws(t), team_get_losses(t),
               team_get_goals_scored(t), team_get_goals_conceded(t),
               goal_diff, points);
    }

    // 5. Libera a lista.
    tl_free(tl);
    return 0;  
}
