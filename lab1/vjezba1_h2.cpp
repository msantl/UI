#include "state.h"
#include "students.h"

using namespace std;

int heuristika(State* start, State* end, Students *s) {
  // n-th and (n-1)-th are going together
  // ...
  int maks = 0;
  vector< int > vremena;

  for (int i = 0; i < s->getCount(); ++i) {
    if (start->getStatus(i) == RIGHT) {
      vremena.push_back(s->getTime(i));
    }
  }

  sort(vremena.begin(), vremena.end());
  reverse(vremena.begin(), vremena.end());

  for (int i = 0; i < (int)vremena.size(); i += 2) {
    maks += vremena[i];
  }

  return maks;
}
