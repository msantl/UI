#include "parse.h"

#include <iostream>
#include <sstream>
#include <cstdio>
#include <cstring>
#include <cstdlib>

using namespace std;

void load_input(vector< string > &premise, int &strategija) {
  stringstream ss;
  string buff, temp;

  getline(cin, buff);

  while (getline(cin, temp)) {
    premise.push_back(buff);
    buff = temp;
  }

  ss << buff;
  ss >> strategija;

  // negiraj zadnju (ciljnu)
  premise[premise.size() - 1] = "~(" + premise[premise.size() - 1] + ")";

  return;
}

string strip(string a) {
  int kraj = a.length() - 1;
  int pocetak = 0;

  while (kraj >= 0 && a.at(kraj) == ' ') --kraj;
  while (pocetak <= kraj && a.at(pocetak) == ' ') ++pocetak;

  return a.substr(pocetak, kraj - pocetak + 1);
}

