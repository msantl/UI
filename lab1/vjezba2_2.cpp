#include "students.h"
#include "state.h"

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <queue>
#include <stack>
#include <string>

using namespace std;

void solve_a_star_second(Students* s) {

  return;
}

int main(int argc, char** argv) {
  int t;
  vector< int > time;

  while (scanf("%d", &t) == 1) { time.push_back(t); }

  Students* problem = new Students(time);

  solve_a_star_first(problem);

  delete problem;

  return 0;
}
