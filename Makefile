# Compilatore e opzioni di compilazione
CC = gcc
CFLAGS = -std=c89 -Wpedantic

# Elenco dei file sorgente
SOURCES_NAVE = Nave.c
SOURCES_PORTO = Porto.c
SOURCES_MASTER = Master.c

# Obiettivi
TARGET_NAVE = Nave
TARGET_PORTO = Porto
TARGET_MASTER = Master

all: $(TARGET_MASTER)

$(TARGET_NAVE): $(SOURCES_NAVE)
	$(CC) $(CFLAGS) $(SOURCES_NAVE) -o $(TARGET_NAVE)

$(TARGET_PORTO): $(SOURCES_PORTO)
	$(CC) $(CFLAGS) $(SOURCES_PORTO) -o $(TARGET_PORTO)

$(TARGET_MASTER): $(TARGET_NAVE) $(TARGET_PORTO) $(SOURCES_MASTER)
	$(CC) $(CFLAGS) $(SOURCES_MASTER) -o $(TARGET_MASTER)

clean:
	rm -f $(TARGET_NAVE) $(TARGET_PORTO) $(TARGET_MASTER)
