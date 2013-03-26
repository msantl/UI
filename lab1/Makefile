CC = g++
CFLAGS = -Wall -g

COMMON_S = state.cpp students.cpp
COMMON_O = ${COMMON_S:.cpp=.o}

all: ucs 0_a_star 1_a_star 2_a_star 3_a_star

ucs: vjezba1.o $(COMMON_O)
	$(CC) $(CFLAGS) $(COMMON_O) vjezba1.o -o $@

0_a_star: vjezba1_opt.o $(COMMON_O)
	$(CC) $(CFLAGS) $(COMMON_O) vjezba1_opt.o -o $@

1_a_star: vjezba2_1.o $(COMMON_O)
	$(CC) $(CFLAGS) $(COMMON_O) vjezba2_1.o -o $@

2_a_star: vjezba2_2.o $(COMMON_O)
	$(CC) $(CFLAGS) $(COMMON_O) vjezba2_2.o -o $@

3_a_star: a_star.o $(COMMON_O)
	$(CC) $(CFLAGS) $(COMMON_O) a_star.o -o $@

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-rm -f ucs 0_a_star 1_a_star 2_a_star 3_a_star *.o

