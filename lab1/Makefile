CC = g++
CFLAGS = -Wall -g

PROJECT = labos1
SOURCE = state.cpp students.cpp vjezba1.cpp
OBJECT = ${SOURCE:.cpp=.o}

all: $(SOURCE) $(PROJECT)

$(PROJECT): $(OBJECT)
	$(CC) $(OBJECT) -o $(PROJECT)

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-rm -f $(PROJECT) *.o

