#include "state.h"

#include <cstdlib>
#include <cstdio>

using namespace std;

State::State() {state = 0; parent = NULL;}

State::State(int students, int cost, int flashlight) {
  state = 0;
  state |= cost;
  state <<= 32;
  state |= students;
  state |= flashlight ? (1 << 16) : 0;

  parent = NULL;
}

void State::fixCost(int h) {
  int mask = this->getState();
  int cost = h;
  state = 0;
  state |= cost;
  state <<= 32;
  state |= mask;
}

int State::getCost() const {
  return (int)(state >> 32);
}

int State::getState() const {
  return (int)(state & 0xffffffff);
}

int State::getStatus(int n) const {
  return (int)((state >> n) & 1);
}

int State::getFlashlightStatus() const {
  return (int)((state >> 16) & 1);
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

  s ^= (1 << 16);

  State* ret = new State(s, a->getCost() + addCost, 0);

  ret->parent = a;
  return ret;
}

State* State::transitionPair(State* a, int i, int j, int addCost) {
  int s = a->getState();

  // invert the i-th and j-th bit
  s ^= (1 << i);
  s ^= (1 << j);

  s ^= (1 << 16);

  State* ret = new State(s, a->getCost() + addCost, 0);

  ret->parent = a;
  return ret;
}

State* State::getAcceptableState(int n) {
  int mask = 0;
  mask |= (1 << n) - 1;

  return new State(mask, 0, LEFT);
}
