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

struct cmp {
  bool operator() (const State* a, const State* b) {
    return a->getCost() > b->getCost();
  }
};

void solve_uniform_cost_search(Students* s) {
  if (s->getCount() == 0) return;
  priority_queue< State*, vector< State* >, cmp > q;

  State* goal = State::getAcceptableState(s->getCount());
  State* start = new State(0, 0, RIGHT);

  int VISITED = 0;
  State* SOLUTION = NULL;

  for (q.push(start); !q.empty(); q.pop()) {
    State* now = q.top();

    VISITED = VISITED + 1;
    if (goal->getState() == now->getState()) {
      SOLUTION = now;
      break;
    }

    // sending them in pairs
    for (int i = 0; i < s->getCount(); ++i) {
      if (now->getStatus(i) != now->getFlashlightStatus()) continue;

      for (int j = i + 1; j < s->getCount(); ++j) {
        if (now->getStatus(i) != now->getStatus(j)) continue;

        State* next = State::transitionPair(now, i, j, max(s->getTime(i), s->getTime(j)));

        q.push(next);
      }
    }

    // sending them alone
    for (int i = 0; i < s->getCount(); ++i) {
      if (now->getStatus(i) != now->getFlashlightStatus()) continue;

      State* next = State::transitionSingle(now, i, s->getTime(i));

      q.push(next);
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

int main(int argc, char** argv) {
  int t;
  vector< int > time;

  while (scanf("%d", &t) == 1) { time.push_back(t); }

  Students* problem = new Students(time);

  solve_uniform_cost_search(problem);

  delete problem;

  return 0;
}
