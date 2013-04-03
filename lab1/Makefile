CC = g++
CFLAGS = -Wall -g

COMMON_S = state.cpp students.cpp search.cpp
COMMON_O = ${COMMON_S:.cpp=.o}

all: 0_a_star 1_a_star 2_a_star 3_a_star

0_a_star: vjezba1_h0.o $(COMMON_O)
	$(CC) $(CFLAGS) $(COMMON_O) vjezba1_h0.o -o $@

1_a_star: vjezba1_h1.o $(COMMON_O)
	$(CC) $(CFLAGS) $(COMMON_O) vjezba1_h1.o -o $@

2_a_star: vjezba1_h2.o $(COMMON_O)
	$(CC) $(CFLAGS) $(COMMON_O) vjezba1_h2.o -o $@

3_a_star: vjezba1_h3.o $(COMMON_O)
	$(CC) $(CFLAGS) $(COMMON_O) vjezba1_h3.o -o $@

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-rm -f ucs 0_a_star 1_a_star 2_a_star 3_a_star *.o

