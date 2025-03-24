DIROBJ := obj/
DIREXE := exec/
DIRHEA := include/
DIRSRC := src/

CFLAGS := -I$(DIRHEA) -c -Wall
LDLIBS := -lpthread -lrt
CC := gcc

all : dirs manager barbero cliente

dirs:
	mkdir -p $(DIROBJ) $(DIREXE)

manager: $(DIROBJ)manager.o $(DIROBJ)semaforoI.o $(DIROBJ)memoriaI.o
	$(CC) -o $(DIREXE)$@ $^ $(LDLIBS)

barbero: $(DIROBJ)barbero.o $(DIROBJ)semaforoI.o $(DIROBJ)memoriaI.o
	$(CC) -o $(DIREXE)$@ $^ $(LDLIBS)

cliente: $(DIROBJ)cliente.o $(DIROBJ)semaforoI.o $(DIROBJ)memoriaI.o
	$(CC) -o $(DIREXE)$@ $^ $(LDLIBS)
	
$(DIROBJ)%.o: $(DIRSRC)%.c
	$(CC) $(CFLAGS) $^ -o $@


clean :
	rm -rf *~ core $(DIROBJ) $(DIREXE) $(DIRHEA)*~ $(DIRSRC)*~