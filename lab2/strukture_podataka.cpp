#include "strukture_podataka.h"

using namespace std;

element::element() : invert(false) {}
element::element(bool status) : invert(status) {}

bool element::getInvert() const {return invert;}
void element::setInvert(bool status) {invert = status;}

literal::literal() {}
literal::literal(char s) : symbol(s) {}
literal::literal(char s, bool i) : symbol(s) {
  this->setInvert(i);
}

char literal::getSymbol() const {return symbol;}
void literal::setSymbol(char s) {symbol = s;}

izraz::izraz() : left(NULL), right(NULL) {}
izraz::izraz(string o, element* l, element *r) : op(o), left(l), right(r) {}

izraz::~izraz() {
  if (left) delete left;
  if (right) delete right;
}

string izraz::getOp() {return op;}
void izraz::setOp(string o) {op = o;}

element* izraz::getLeft() {return left;}
element* izraz::getRight() {return right;}

void izraz::setLeft(element *l) {left = l;}
void izraz::setRight(element *r) {right = r;}

literal* toLiteral(element *el) {
  return dynamic_cast<literal *>(el);
}

izraz* toIzraz(element *el) {
  return dynamic_cast<izraz *>(el);
}
