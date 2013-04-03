#include "state.h"
#include "students.h"

using namespace std;

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

