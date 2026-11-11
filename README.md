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
- **Sanitizadores** (ASan/UBSan) e `valgrind` para verificação de memória.
- **Manipulação de UTF-8** no alinhamento de saída.

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

## Autores

- Arthur Corrêa Sobrinho
- Caio Lucas da Silva Nascimento
