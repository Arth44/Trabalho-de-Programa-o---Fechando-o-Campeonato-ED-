# Trabalho de Programação — Fechando o Campeonato

Programa em C que **apura um campeonato de futebol** a partir de dois arquivos
CSV e imprime a tabela classificada, um resumo e um `classificacao.csv`.

## Premissa

Dado um arquivo `times.csv` (os 10 times do campeonato) e um arquivo
`partidas.csv` (até 45 partidas disputadas), o programa:

1. **Lê** os dois arquivos.
2. **Apura** vitórias, empates, derrotas, gols marcados e gols sofridos de
   cada time.
3. **Classifica** os times pelos critérios, em ordem:
   pontos ganhos → vitórias → saldo de gols → gols marcados → menor ID.
4. **Exibe** a tabela e um resumo (campeão/líder, melhor ataque, melhor
   defesa, maior goleada).
5. **Grava** `classificacao.csv` com a mesma ordem da tela.

## Assuntos abordados

- **TAD (Tipo Abstrato de Dados)** e tipo opaco: interface no `.h`,
  implementação no `.c`, encapsulamento.
- **Lista simplesmente encadeada** (MatchList) e **lista duplamente
  encadeada com cauda** (TeamList).
- **Posse de memória (ownership)**: lista dona vs. lista visão, e a ordem
  correta de liberação.
- **Invariantes por construção**: saldo e pontos derivados, nunca armazenados.
- **Inserção ordenada** (O(n²)) com comparador em cascata
  (PG → V → saldo → GM → menor ID).
- **Complexidade** (Big-O) das operações.
- **Makefile** com dependências de header e separação de build/execução.
- **Manipulação de UTF-8** no alinhamento de saída.

## Principais TADs

- **Team** (`team.h`/`team.c`): representa um time. Guarda id, nome e as
  estatísticas (V, E, D, GM, GS). Saldo e pontos são calculados, nunca
  armazenados. Possui `team_register_result`, que decide sozinha se a
  partida foi vitória, empate ou derrota.
- **TeamList** (`teamlist.h`/`teamlist.c`): coleção de times implementada
  com lista duplamente encadeada (com ponteiro para o último nó). Possui
  `tl_insert`, usado para manter a tabela ordenada.
- **Match** (`match.h`/`match.c`): representa uma partida, com os 5 campos
  do `partidas.csv`. Guarda IDs dos times (não ponteiros).
- **MatchList** (`matchlist.h`/`matchlist.c`): coleção de partidas
  implementada com lista simplesmente encadeada, preservando a ordem do
  arquivo.

## Decisões de implementação

- **Posse de memória.** A lista carregada do CSV é a *dona* dos
  times: ela os libera no fim com `tl_free`. A tabela de classificação
  reusa os mesmos `Team*` em outra ordem e libera só a
  estrutura com `tl_free_nodes`. Usar `tl_free` causa *double
  free*. A regra está documentada em `teamlist.h`, onde o erro ocorre.
- **Classificação por inserção ordenada** (sem `qsort`), conforme sugerido
  no enunciado, com comparador em cascata: PG → V → saldo → GM → menor ID.
- **Partida guarda IDs, não ponteiros**, por ser um registro histórico.
- **Maior goleada em empate:** vale a primeira partida do arquivo (a
  `MatchList` preserva a ordem de leitura).
- **Melhor ataque/defesa em empate:** vale o time mais bem classificado.

## Como compilar e rodar

```bash
make          # compila e gera ./campeonato
make run      # compila e executa
make clean    # remove objetos e executável
```

Requisitos: `gcc` com suporte a C99 e `make`.

## Entradas

- `times.csv` — lista dos times (`id,nome`), 10 times.
- `partidas.csv` — partidas disputadas (`id,time1,time2,gols1,gols2`).
  
## Saída

- Tabela classificada impressa no terminal.
- Resumo do campeonato.
- Arquivo `classificacao.csv` gerado na mesma pasta.

## Estrutura do projeto

```
Trabalho de Programação - Fechando o Campeonato/
├── main.c            # carga, apuração, classificação, relatório
├── team.c / team.h   # TAD Team
├── teamlist.c / .h   # TAD TeamList (lista duplamente encadeada)
├── match.c / match.h # TAD Match
├── matchlist.c / .h  # TAD MatchList (lista simplesmente encadeada)
├── Makefile
├── times.csv
└── README.md
```

## Autores

- Arthur Corrêa Sobrinho 20252BSI0046
- Caio Lucas da Silva Nascimento 20252BSI0070
