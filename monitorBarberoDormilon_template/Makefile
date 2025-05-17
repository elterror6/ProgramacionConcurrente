CC := g++
LDLIBS := -pthread 
CPPFLAGS := -g -I. -Wall
OBJS := Manager.o Barbero.o Cliente.o

all: Manager

Manager: $(OBJS)
	$(CC) $(LDLIBS) -o Manager $(OBJS)

Manager.o: Barbero.h Cliente.h Barbero.o Cliente.o
	$(CC) $(CPPFLAGS) -c Manager.cpp

Barbero.o: Barbero.h	
	$(CC) $(CPPFLAGS) -c Barbero.cpp

Cliente.o: Cliente.h	
	$(CC) $(CPPFLAGS) -c Cliente.cpp

clean:
	rm -rf *~ core Manager $(OBJS)