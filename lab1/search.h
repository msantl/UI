#ifndef SEARCH_H
#define SEARCH_H

#define MAX (MAX_STUDENTS + 2)

int r_cost[(1 << MAX) + 1]; // real cost
int f_cost[(1 << MAX) + 1]; // estimated final cost

int CLOSED[(1 << MAX) + 1];

#endif
