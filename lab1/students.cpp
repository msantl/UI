#include "students.h"

#include <vector>
#include <string>
#include <sstream>

using namespace std;

Students::Students() {}

Students::Students(vector< int > _time) : time(_time) {}

int Students::getCount() {
  return (int)time.size();
}

int Students::getTime(int n) {
  return time.at(n);
}

string Students::getStrTime(int n) {
  stringstream ss;
  string ret;
  ss << time.at(n);
  ss >> ret;
  return ret;
}
