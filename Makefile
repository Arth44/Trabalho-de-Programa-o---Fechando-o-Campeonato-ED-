# Trabalho de Programacao -- Fechando o Campeonato
#
#   make        compila e gera o executavel ./campeonato
#   make run    compila e executa
#   make clean  remove objetos e executavel
#
# O alvo `all` apenas compila: rodar o programa dentro do make misturaria
# build com execucao e esconderia erros de leitura dos CSVs.

CC     = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic -O2
TARGET = campeonato
OBJS   = main.o team.o teamlist.o match.o matchlist.o

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

# Cada .o lista os .h que inclui: se um cabecalho mudar, o objeto e refeito.
main.o: main.c team.h teamlist.h match.h matchlist.h
	$(CC) $(CFLAGS) -c main.c -o main.o

team.o: team.c team.h
	$(CC) $(CFLAGS) -c team.c -o team.o

teamlist.o: teamlist.c teamlist.h team.h
	$(CC) $(CFLAGS) -c teamlist.c -o teamlist.o

match.o: match.c match.h
	$(CC) $(CFLAGS) -c match.c -o match.o

matchlist.o: matchlist.c matchlist.h match.h
	$(CC) $(CFLAGS) -c matchlist.c -o matchlist.o

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
