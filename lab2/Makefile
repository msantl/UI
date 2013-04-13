PROJECT = labos2
SOURCE = strukture_podataka.cpp parse.cpp solver.cpp

CC = g++
CFLAGS = -Wall -g -c
LDFLAGS =
OBJECTS = ${SOURCE:.cpp=.o}

all: $(SOURCE) $(PROJECT)

$(PROJECT): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(PROJECT)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	-rm -f $(PROJECT) $(OBJECTS)
