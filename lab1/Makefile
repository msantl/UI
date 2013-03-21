CC = g++
CFLAGS = -Wall -g -c

PROJECT = labos1
SOURCE = state.cpp students.cpp vjezba1_opt.cpp 
OBJECT = ${SOURCE:.cpp=.o}

all: $(SOURCE) $(PROJECT)

$(PROJECT): $(OBJECT)
	$(CC) $(OBJECT) -o $(PROJECT)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	-rm -f $(PROJECT) $(OBJECT)

