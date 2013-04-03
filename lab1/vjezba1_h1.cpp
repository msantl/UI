#include "state.h"
#include "students.h"

using namespace std;

int heuristika(State* start, State* end, Students *s) {
  // return the max time
  int maks = 0;

  for (int i = 0; i < s->getCount(); ++i) {
    if (start->getStatus(i) == RIGHT) {
      maks = max(maks, s->getTime(i));
    }
  }

  return maks;
}

