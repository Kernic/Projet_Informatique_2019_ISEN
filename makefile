CC=gcc
CFLAGS=-W -Wall -g
LDFLAGS=-lm
EXEC1=process-data
SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)

# Si vous souhaitez utiliser des options d'affichage uniquement visible lors
# de votre dépannage (yes/no)
DEBUG=yes

ifeq ($(DEBUG),yes)
	CFLAGS+=-g -D__DEBUG__
endif

all: $(EXEC1)

# Rajoutez vos sources supplémentaires dans la liste
# ci-dessous après le common.o
$(EXEC1): process-data.o common.o traitementSignal.o InfoFile.o
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC1)
	
