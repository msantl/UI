#include "students.h"

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <queue>
#include <stack>
#include <string>

using namespace std;

#define MAX (MAX_STUDENTS + 2)

int cost[(1 << MAX) + 1];

class State_t {
  private:
    int state;
  public:
    State_t* prev;

    State_t() {state = 0; prev = NULL;}

    void set(int i) {
      state |= 1 << i;
    }

    int get(int i) {
      return (state >> i) & 1;
    }

    void inv(int i) {
      state ^= 1 << i;
    }

    int getState() const {
      return state;
    }
};

class cmp {
  public:
    bool operator() (const State_t* a, const State_t* b) {
      return cost[a->getState()] > cost[b->getState()];
    }
};

void solve_uniform_cost_search(Students* s) {
  memset(cost, 63, sizeof cost);
  priority_queue< State_t*, vector< State_t* >, cmp > q;

  State_t* start = new State_t();
  State_t* end = new State_t();

  int VISITED = 0;
  State_t* SOLUTION;

  // creating the final state
  for (int i = 0; i < s->getCount(); ++i) { end->set(i); } end->set(17);

  cost[0] = 0;

  for (q.push(start); !q.empty(); q.pop()) {
    State_t *next, *now = q.top();
    int addCost;

    VISITED = VISITED + 1;
    if (now->getState() == end->getState()) {
      SOLUTION = now;
      break;
    }

    for (int i = 0; i < s->getCount(); ++i) {
      if (now->get(i) != now->get(17)) continue;
      next = new State_t(*now);
      next->prev = now;

      next->inv(i);
      next->inv(17);

      addCost = s->getTime(i);

      if (cost[next->getState()] > cost[now->getState()] + addCost) {
        cost[next->getState()] = cost[now->getState()] + addCost;
        q.push(next);
      }

      for (int j = i + 1; j < s->getCount(); ++j) {
        if (now->get(i) != now->get(j)) continue;
        next = new State_t(*now);
        next->prev = now;

        next->inv(i);
        next->inv(j);

        next->inv(17);

        addCost = max(s->getTime(i), s->getTime(j));

        if (cost[next->getState()] <= cost[now->getState()] + addCost) continue;

        cost[next->getState()] = cost[now->getState()] + addCost;
        q.push(next);
      }
    }

  }

  // print the solution
  stack< string > sol;
  cout << cost[SOLUTION->getState()] << " " << VISITED << endl;

  while (SOLUTION->prev != NULL) {
    State_t* curr = SOLUTION;
    State_t* prev = SOLUTION->prev;
    string step;

    int mask = (curr->getState() ^ prev->getState()) & 0x0000ffff;

    for (int i = 0; i < s->getCount(); ++i) {
      if (((mask >> i) & 1) == 1) {
        step += s->getStrTime(i) + " ";
      }
    }

    step += curr->get(17) ? "->\n" : "<-\n";
    sol.push(step);

    SOLUTION = SOLUTION->prev;
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
