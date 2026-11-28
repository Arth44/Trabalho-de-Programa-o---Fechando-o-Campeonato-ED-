#include "team.h"
#include "teamlist.h"
#include "match.h"
#include "matchlist.h"
#include <stdlib.h>
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

TeamList * openTimes(){
     // 1. Abre o arquivo de times para leitura em modo texto ("rt").
    FILE *f = fopen("times.csv", "rt");
    if (f == NULL) return NULL;   // arquivo não encontrado -> erro

    // 2. Cria uma lista vazia para guardar os times.
    TeamList *tl = tl_create();
    if (tl == NULL) {
        fclose(f);
        return NULL;              // sem memória -> erro
    }

    // 3. Lê linha por linha: id + nome.
    //    Formato esperado: "0,JAVAlis"
    //    %d         -> lê um inteiro (id)
    //    ,          -> espera uma vírgula
    //    %49[^\r\n] -> lê até 49 chars que NÃO sejam \r nem \n (o nome)
    //    == 2       -> sucesso se leu os 2 campos

    int id;
    char name[50];
    char linha[100];

    // Ignora o cabeçalho
    fgets(linha, sizeof(linha), f);

    // Lê os times
    while (fscanf(f, "%d,%49[^\r\n]", &id, name) == 2) {
        Team *t = team_create(id, name);
        if (t != NULL)
            tl_append(tl, t);
    }
    fclose(f);
    return tl;
}
MatchList* openPartidas(){
     // 1. Abre o arquivo de Partidas para leitura em modo texto ("rt").
    FILE *f = fopen("partidas.csv", "rt");
    if (f == NULL) return NULL;   // arquivo não encontrado -> erro

    MatchList *ml = ml_create();
    if (ml == NULL) {
        fclose(f);
        return NULL;              // sem memória -> erro
    }

    int id;
    int time1;
    int time2;
    int golsTime1;
    int golsTime2;
    char simpleMinds[100];

    // Ignora o cabeçalho
    fgets(simpleMinds, sizeof(simpleMinds), f);

    // Lê as partidas
    while (fscanf(f, "%d,%d,%d,%d,%d",&id ,&time1,&time2,&golsTime1,&golsTime2)== 5)
    {
        Match* mt = match_create(id,time1,time2,golsTime1,golsTime2);
        if (mt != NULL)
            ml_append(ml, mt);

    }

    fclose(f);
    return ml;
}

void apurarCampeonato(TeamList *tl , MatchList *ml){
    for(int i = 0; i < ml_size(ml); i++){

        // Obtém a partida atual
        Match *m = ml_get(ml,i);
        
        // Obtém os IDs dos times
        int id1 = match_get_team1(m);
        int id2 = match_get_team2(m);

        // Obtém os gols de cada time
        int gols1 = match_get_goals_team1(m);
        int gols2 = match_get_goals_team2(m);

        // Localiza os times correspondentes  da partida pecorrendo a lista de times
        Team *t1 = NULL;
        Team *t2 = NULL;
        
        for(int j = 0; j < tl_size(tl); j++){
            Team *t = tl_get(tl, j);


            if(team_get_id(t) == id1){
                t1 =t;
            }
            else if(team_get_id(t) == id2){
                t2 =t;
            }

        }

        // Registra o resultado nos dois times
        if (t1 != NULL && t2 != NULL) {
            team_register_result(t1, gols1, gols2);
            team_register_result(t2, gols2, gols1);
        }
        
    }
}

int compararTimes(Team *ta , Team *tb){

    // 1. Maior número de pontos
    if(team_get_points(ta) != team_get_points(tb)){
        return team_get_points(ta) > team_get_points(tb);
    }
     // 2. Maior número de vitórias
    if(team_get_wins(ta) != team_get_wins(tb)){
        return team_get_wins(ta) > team_get_wins(tb);
    }
    // 3. Maior saldo de gols    
    if(team_get_goal_difference(ta) != team_get_goal_difference(tb)){
        return team_get_goal_difference(ta) > team_get_goal_difference(tb);
    }
    // 4. Maior número de gols marcados
    if(team_get_goals_scored(ta) != team_get_goals_scored(tb)){
        return team_get_goals_scored(ta) > team_get_goals_scored(tb);
    }
    // 5. Menor ID
    return team_get_id(ta) < team_get_id(tb);

}


TeamList *classificarTimes(TeamList *tl){
    TeamList * classificados = tl_create();

    if (classificados == NULL)
        return NULL;

    /*Pecorre a lista de Times e para cada time da lista
    ele compara com um time da lista de classificados até encontrar 
    em qual posição da lista de classificados ele deve ficar.*/

    for(int i = 0; i < tl_size(tl); i++){
        Team *atual = tl_get(tl, i);
        
        int j = 0;
        while(j < tl_size(classificados) && !compararTimes(atual, tl_get(classificados,j))){
            j++;
        }
        tl_insert(classificados,atual,j);


    }

    return classificados;

}
void imprimirArquivos(TeamList * timesOrdenados){
    FILE *arquivo = fopen("classificacao.csv", "wt");

    if (arquivo == NULL) {
        printf("Erro ao criar o arquivo!\n");
        return;
    }

    fprintf(arquivo, "Pos,ID,Time,V,E,D,GM,GS,S,PG\n");

    for (int i = 0; i < tl_size(timesOrdenados); i++) {
        Team *t = tl_get(timesOrdenados, i);

        fprintf(arquivo, "%d,%d,%s,%d,%d,%d,%d,%d,%d,%d\n",
                i + 1,
                team_get_id(t),
                team_get_name(t),
                team_get_wins(t),
                team_get_draws(t),
                team_get_losses(t),
                team_get_goals_scored(t),
                team_get_goals_conceded(t),
                team_get_goal_difference(t),
                team_get_points(t));
    }
    fclose(arquivo);
}

void liberarMemoria(TeamList *times,TeamList *timesOrdenados, MatchList *partidas) {

    // Libera os nós da classificação, sem liberar os times.
    tl_free_nodes(timesOrdenados);

    // Libera os nós e os times da lista original.
    tl_free(times);

    // Libera as partidas e os nós da lista de partidas.
    ml_free(partidas);
}

Team * melhorAtaque(TeamList *tl){

    // Seleciona o primeiro time da lista como referência de melhor ataque
    Team *t = tl_get(tl, 0);

    for(int i = 1; i < tl_size(tl); i++){

        // Compara se o próximo tem um ataque melhor para substituí-lo
        if(team_get_goals_scored(t) < team_get_goals_scored(tl_get(tl, i))){
            t = tl_get(tl, i);
        }
    }

    // Retorna o time com melhor ataque
    return t;
}

Team * melhorDefesa(TeamList *tl){

    // Seleciona o primeiro time da lista como referência de melhor defesa
    Team *t = tl_get(tl, 0);

    for(int i = 1; i < tl_size(tl); i++){

        // Compara se o próximo sofreu menos gols para substituí-lo
        if(team_get_goals_conceded(t) > team_get_goals_conceded(tl_get(tl, i))){
            t = tl_get(tl, i);
        }
    }

    // Retorna o time com melhor defesa
    return t;
}

Match * maiorGoleada(MatchList *ml){

    // Seleciona a primeira partida da lista como referência de maior goleada
    Match *m = ml_get(ml, 0);

    for(int i = 1; i < ml_size(ml); i++){

        // Compara se a próxima partida teve uma goleada maior
        if(match_get_goal_margin(m) < match_get_goal_margin(ml_get(ml, i))){
            m = ml_get(ml, i);
        }
    }

    // Retorna a partida com a maior goleada
    return m;
}

Team *buscarTimePorID(TeamList *tl, int id){

    for(int i = 0; i < tl_size(tl); i++){
        Team *t = tl_get(tl, i);

        if(team_get_id(t) == id){
            return t;
        }
    }

    return NULL;
}
int main(void) {

    TeamList *tl = openTimes();
    if (tl == NULL) return 1;
    MatchList *ml = openPartidas();
    if (ml == NULL) { tl_free(tl); return 1; }
    apurarCampeonato(tl,ml);
    TeamList *timesOrdenados = classificarTimes(tl);
    if (timesOrdenados == NULL) { liberarMemoria(tl, NULL, ml); return 1; }
    imprimirArquivos(timesOrdenados);

    // Imprime a tabela
    printf("%-4s %-3s %-12s %2s %2s %2s %3s %3s %3s %3s\n",
           "Pos", "ID", "Time", "V", "E", "D",
           "GM", "GS", "S", "PG");
    for (int i = 0; i < tl_size(timesOrdenados); i++) {
        Team *t = tl_get(timesOrdenados, i);
        int victory = team_get_wins(t);
        int draws = team_get_draws(t);
        int loss = team_get_losses(t);
        int gScored = team_get_goals_scored(t);
        int gConceded = team_get_goals_conceded(t);
        int saldoG = team_get_goal_difference(t);
        int points = team_get_points(t);
        

        printf("%-4d %-3d %-12s", i+1, team_get_id(t), team_get_name(t));
        for (int j = 0; j < utf8_continuation_bytes(team_get_name(t)); j++)
            putchar(' ');
        printf(" %2d %2d %2d %3d %3d %3d %3d\n", victory, draws, loss, gScored, gConceded, saldoG, points);
    }
    printf("===== Resumo do campeonato =====\n");

    
    if(ml_size(ml) == 0){ // E se nenhuma partida tiver sido disputada?

    printf("Partidas disputadas: 0 de 45\n");
    printf("Nenhuma partida foi disputada.\n");

    } else {

    printf("Partidas disputadas: %d de 45\n", ml_size(ml));

    if(ml_size(ml) != 45){
        printf("Líder: %-15s (%d pontos)\n",team_get_name(tl_get(timesOrdenados, 0)), team_get_points(tl_get(timesOrdenados,0)));
    }
    else{
        printf("Campeão: %-15s (%d pontos)\n",team_get_name(tl_get(timesOrdenados, 0)), team_get_points(tl_get(timesOrdenados,0)));
    }

    Team *tma = melhorAtaque(timesOrdenados); // identifica qual é o time com o melhor ataque 
    printf("Melhor ataque: %-15s (%d gols marcados)\n", team_get_name(tma),team_get_goals_scored(tma));    
    
    Team *tmd = melhorDefesa(timesOrdenados); // identifica qual é o time com a melhor defesa
    printf("Melhor defesa: %-15s (%d gols sofridos)\n", team_get_name(tmd),team_get_goals_conceded(tmd));
    
    Match *mmg = maiorGoleada(ml);
    if (mmg != NULL) {
        Team *goleadaT1 = buscarTimePorID(timesOrdenados, match_get_team1(mmg));
        Team *goleadaT2 = buscarTimePorID(timesOrdenados, match_get_team2(mmg));
        if (goleadaT1 != NULL && goleadaT2 != NULL) {
            printf("Maior goleada: %-15s %d x %d %-15s\n",
                   team_get_name(goleadaT1),
                   match_get_goals_team1(mmg),
                   match_get_goals_team2(mmg),
                   team_get_name(goleadaT2));
        }
    }

    // lIBERAR OS TIMES E A PARTIDA RESERVADAS DO RESULTADO DA ANÁLISE DO CAMPEONATO
    
    

    //  Libera a lista.
    liberarMemoria(tl, timesOrdenados, ml);
    return 0;
    }
}
