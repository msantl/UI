#include "state.h"

#include <cstdlib>

using namespace std;

State::State() {state = 0;}

State::State(int students, int cost, int flashlight) {
  state = 0;
  state |= cost;
  state <<= 32;
  state |= students;
  state |= (flashlight ? (1LL << 31) : 0);

  parent = NULL;
}

int State::getCost() const {
  int ret = (int)(state >> 32);
  return ret;
}

int State::getState() const {
  return (int) (state & 0x7fffffff);
}

int State::getStatus(int n) const {
  return (state >> n) & 1;
}

int State::getFlashlightStatus() const {
  return (state >> 31) & 1;
}

unsigned long long int State::getHash() const {
  return state;
}

bool State::operator <(const State &a) const {
  return getCost() > a.getCost();
}

State* State::transitionSingle(State* a, int i, int addCost) {
  int s = a->getState();

  // invert the i-th
  s ^= (1 << i);

  return new State(s, a->getCost() + addCost, !a->getFlashlightStatus());
}

State* State::transitionPair(State* a, int i, int j, int addCost) {
  int s = a->getState();

  // invert the i-th and j-th bit
  s ^= (1 << i);
  s ^= (1 << j);

  return new State(s, a->getCost() + addCost, !a->getFlashlightStatus());
}

int State::getAcceptableState(int n) {
  int mask = 0;

  mask |= (1 << n) - 1;

  return mask;
}
