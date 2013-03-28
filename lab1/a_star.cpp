#include "state.h"
#include "students.h"

#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <vector>
#include <queue>
#include <stack>
#include <string>
#include <algorithm>

#include <iostream>
#include <map>

using namespace std;

#define MAX (MAX_STUDENTS + 2)

int r_cost[(1 << MAX) + 1]; // real cost
int f_cost[(1 << MAX) + 1]; // estimated final cost

int CLOSED[(1 << MAX) + 1];

class cmp{
  public:
    bool operator() (const State* a, const State* b) {
      return a->getCost() > b->getCost();
    }
};

int heuristika(State* start, State* end, Students *s) {
  // BEGIN
  //  (1) najbrza dva (m1, m2) RIGHT -> LEFT
  //  (2) brzi (m1) LEFT -> RIGHT
  //  (3) najsporija dva (mk, ml) RIGHT -> LEFT
  //  (4) drugi najbrzi (m2) LEFT -> RIGHT
  // GOTO BEGIN
  int maks = 0;
  vector< int > vremena;

  int m1 = -1, m2 = -1, n = 0;

  for (int i = 0; i < s->getCount(); ++i) {
    if (start->getStatus(i) == RIGHT) {
      vremena.push_back(s->getTime(i));
      n = n + 1;
    }

    if (m1 == -1 || s->getTime(i) <= m1) {
      m2 = m1;
      m1 = s->getTime(i);
    } else if (m2 == -1 || s->getTime(i) < m2) {
      m2 = s->getTime(i);
    }
  }

  sort(vremena.begin(), vremena.end());

  if (n >= 4) {
    for (int i = n - 1; i >= 3; i -= 2) {
      // (1)
      maks += m2;
      // (2)
      maks += m1;
      // (3)
      maks += vremena[i];
      // (4)
      maks += m2;
      // korekcija
    }

  } else {
    for (int i = n - 1; i >= 0; i -= 2) {
      maks += vremena[i];
    }
  }

  return maks < 0 ? 0 : maks;
}

void solve_a_star(Students *s) {
  if (s->getCount() == 0) return;

  int VISITED = 0;
  priority_queue< State*, vector< State* >, cmp > q;

  memset(r_cost, 63, sizeof r_cost);
  memset(f_cost, 63, sizeof f_cost);
  memset(CLOSED, 0, sizeof CLOSED);

  State* start = new State(0, 0, RIGHT);
  State* goal = State::getAcceptableState(s->getCount());
  State* SOLUTION = NULL;

  r_cost[0] = 0;
  f_cost[0] = heuristika(start, goal, s);

  start->fixCost(f_cost[0]);

  for (q.push(start); !q.empty(); ) {
    State* now = q.top();   q.pop();

    if (CLOSED[now->getState()] == 0) {
      VISITED = VISITED + 1;
      CLOSED[now->getState()] = 2;
    } else if (CLOSED[now->getState()] == 1) {
      CLOSED[now->getState()] = 2;
    } else {
      continue;
    }

    if (now->getState() == goal->getState()) {
      SOLUTION = now;
      break;
    }

    // send them in pair
    for (int i = 0; i < s->getCount(); ++i) {
      if (now->getFlashlightStatus() != now->getStatus(i)) continue;
      for (int j = i + 1; j < s->getCount(); ++j) {
        if (now->getStatus(i) != now->getStatus(j)) continue;

        int addCost = max(s->getTime(i), s->getTime(j));
        State* next = State::transitionPair(now, i, j, 0);

        if (r_cost[next->getState()] > r_cost[now->getState()] + addCost) {
          if (CLOSED[next->getState()] == 2) { CLOSED[next->getState()] = 1; }

          int h = heuristika(next, goal, s);
          r_cost[next->getState()] = r_cost[now->getState()] + addCost;
          f_cost[next->getState()] = r_cost[next->getState()] + h;

          next->fixCost(f_cost[next->getState()]);

          q.push(next);
        }
      }
    }
    // send them single
    for (int i = 0; i < s->getCount(); ++i) {
      if (now->getFlashlightStatus() != now->getStatus(i)) continue;

      int addCost = s->getTime(i);
      State* next = State::transitionSingle(now, i, 0);

      if (r_cost[next->getState()] > r_cost[now->getState()] + addCost) {
        if (CLOSED[next->getState()] == 2) { CLOSED[next->getState()] = 1; }

        int h = heuristika(next, goal, s);
        r_cost[next->getState()] = r_cost[now->getState()] + addCost;
        f_cost[next->getState()] = r_cost[next->getState()] + h;

        next->fixCost(f_cost[next->getState()]);

        q.push(next);
      }
    }

  }

  // print the solution
  stack< string > sol;
  cout << SOLUTION->getCost() << " " << VISITED << endl;

  while (SOLUTION->parent != NULL) {
    State* curr = SOLUTION;
    State* prev = SOLUTION->parent;
    string step;

    int mask = curr->getState() ^ prev->getState();

    for (int i = 0; i < s->getCount(); ++i) {
      if (((mask >> i) & 1) == LEFT) {
        step += s->getStrTime(i) + " ";
      }
    }

    step += curr->getFlashlightStatus() ? "->\n" : "<-\n";
    sol.push(step);

    SOLUTION = SOLUTION->parent;
  }

  while (sol.empty() == false) {
    cout << sol.top();
    sol.pop();
  }

  return;
}

int main(int argc, char **argv) {
  int t;
  vector< int > time;

  while (scanf("%d", &t) == 1) { time.push_back(t); }

  Students* problem = new Students(time);

  solve_a_star(problem);

  delete problem;

  return 0;
}
