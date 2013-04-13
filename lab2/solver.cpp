#include "strukture_podataka.h"
#include "parse.h"

#include <iostream>
#include <queue>
#include <set>
#include <err.h>

using namespace std;

const int MAX_KORAKA = 65536;
const int MAX_KLAUZULA = 65536;

int KLAUZULA, BROJAC;
vector< set< literal > > SVE;

string operatori[] = {"<->", "->", "&", "|"};

void print_set(set< set< literal > > a) {
  for (
    set< set< literal > >::iterator
    it = a.begin();
    it != a.end();
    ++it
  ) {
    for (
      set< literal >::iterator
      it1 = it->begin();
      it1 != it->end();
      ++it1
    ) {
      if (it1 != it->begin()) cout << " v ";

      cout << (it1->getInvert() ? "~" : "") << it1->getSymbol();
    }
    cout << endl;
  }

}

void print_tree(element* root, int depth = 0) {
  if (root == NULL) {
    return;
  } else if (literal* l = toLiteral(root)) {
    for (int i = 0; i < depth; ++i) printf(" ");

    if (l->getInvert()) printf(".");
    printf("%c\n", l->getSymbol());

  } else if (izraz *l = toIzraz(root)){
    for (int i = 0; i < depth; ++i) printf(" ");

    printf("%s\n", l->getOp().c_str());

    print_tree(l->getLeft(), depth + 1);
    print_tree(l->getRight(), depth + 1);

  } else {
    warnx("Stablo je deformirano");
  }
  return;
}

element* izgradi_stablo(string formula) {
  if (formula.length() == 0) errx(1, "Neispravna formula");

  formula = strip(formula);

  if (formula.length() == 1 && isupper(formula[0])) {
    return new literal(formula[0], 0);
  }

  int dubina = 0;
  int N = formula.length();
  element* stablo = NULL;

  for (int i = N - 1; i >= 0; --i) {
    if (formula.at(i) == ')') {
      dubina -= 1;
    } else if (formula.at(i) == '(') {
      dubina += 1;
    } else if (dubina == 0) {
      for (int j = 0; j < 4; ++j) {
        int pocetak = i - operatori[j].length() + 1;

        if (pocetak < 0) continue;
        if (operatori[j] != formula.substr(pocetak, operatori[j].length())) continue;


        if (stablo != NULL) {
          errx(1, "Neispravna formula");
        } else {
          element* left = izgradi_stablo(formula.substr(0, pocetak));
          element* right = izgradi_stablo(formula.substr(i + 1, N - i ));

          stablo = new izraz(operatori[j], left, right);

          break;
        }

      }
    }
  }

  if (stablo != NULL) return stablo;

  if (formula[0] == '~') {
    element *invert = izgradi_stablo(formula.substr(1, N - 1));
    return new izraz("~", invert, NULL);
  }

  if (formula[0] == '(' && formula[N - 1] == ')') {
    return izgradi_stablo(formula.substr(1, N - 2));
  }

  errx(1, "Neispravna formula");
}

element* ukloni_ekvivalenciju(element* stablo, int &change) {
  // F <-> G == (~F | G) & (~G | F)
  izraz *curr = toIzraz(stablo);
  if (curr == NULL) return stablo;

  curr->setLeft(ukloni_ekvivalenciju(curr->getLeft(), change));
  curr->setRight(ukloni_ekvivalenciju(curr->getRight(), change));

  if (curr->getOp() == "<->") {
    element* F = curr->getLeft();
    element* G = curr->getRight();
    element* nF = new izraz("~", F, NULL);
    element* nG = new izraz("~", G, NULL);

    element* prvi = new izraz("|", nF, G);
    element* drugi = new izraz("|", F, nG);

    curr->setOp("&");
    curr->setLeft(prvi);
    curr->setRight(drugi);

    change = 1;
  }

  return stablo;
}

element* ukloni_implikaciju(element* stablo, int &change) {
  // F -> G == ~F | G
  izraz *curr = toIzraz(stablo);
  if (curr == NULL) return stablo;

  curr->setLeft(ukloni_implikaciju(curr->getLeft(), change));
  curr->setRight(ukloni_implikaciju(curr->getRight(), change));

  if (curr->getOp() == "->") {
    element* F = curr->getLeft();
    element* nF = new izraz("~", F, NULL);

    curr->setOp("|");
    curr->setLeft(nF);

    change = 1;
  }

  return stablo;
}

element* potisni_negaciju(element* stablo, int &change) {
  // ~(F | G) == ~F & ~G
  // ~(F & G) == ~F | ~G
  izraz *curr = toIzraz(stablo);
  if (curr == NULL) return stablo;

  curr->setLeft(potisni_negaciju(curr->getLeft(), change));
  curr->setRight(potisni_negaciju(curr->getRight(), change));

  if (curr->getOp() == "~") {
    izraz* next = toIzraz(curr->getLeft());

    if (next != NULL) {
      element* F = next->getLeft();
      element* G = next->getRight();

      element* nF = new izraz("~", F, NULL);
      element* nG = new izraz("~", G, NULL);

      if (next->getOp() == "&") {
        curr->setOp("|");

        curr->setLeft(nF);
        curr->setRight(nG);

        change = 1;
      } else if (next->getOp() == "|") {
        curr->setOp("&");

        curr->setLeft(nF);
        curr->setRight(nG);

        change = 1;
      }

    }
  }

  return stablo;
}

element* primjeni_distributivnost(element* stablo, int &change) {
  // F | (G & H) == (F | G) & (F | H)
  // (F & G) | H == (F | H) & (G | H)
  izraz *curr = toIzraz(stablo);
  if (curr == NULL) return stablo;

  curr->setLeft(primjeni_distributivnost(curr->getLeft(), change));
  curr->setRight(primjeni_distributivnost(curr->getRight(), change));

  if (curr->getOp() == "|") {
    izraz* left = toIzraz(curr->getLeft());
    izraz* right = toIzraz(curr->getRight());

    if (left && left->getOp() == "&") {
      // slucaj 2
      element* F = left->getLeft();
      element* G = left->getRight();
      element* H = curr->getRight();

      element* prvi = new izraz("|", F, H);
      element* drugi = new izraz("|", G, H);

      curr->setOp("&");
      curr->setLeft(prvi);
      curr->setRight(drugi);

      change = 1;

    } else if (right && right->getOp() == "&") {
      // slucaj 1
      element* F = curr->getLeft();
      element* G = right->getLeft();
      element* H = right->getRight();

      element* prvi = new izraz("|", F, G);
      element* drugi = new izraz("|", F, H);

      curr->setOp("&");
      curr->setLeft(prvi);
      curr->setRight(drugi);

      change = 1;
    }
  }

  return stablo;
}

element* primjeni_involuciju(element* stablo, int &change) {
  // ~~F == F
  izraz *curr = toIzraz(stablo);
  if (curr == NULL) return stablo;

  curr->setLeft(primjeni_involuciju(curr->getLeft(), change));
  curr->setRight(primjeni_involuciju(curr->getRight(), change));

  if (curr->getOp() == "~") {
    izraz *next = toIzraz(curr->getLeft());
    if (next != NULL && next->getOp() == "~") {
      stablo = next->getLeft();

      change = 1;
    }
  }

  return stablo;
}

element* ponovi(element *stablo, element* (*func)(element *, int &)) {
  int status;
  do {
    status = 0;
    stablo = func(stablo, status);
  } while(status != 0);

  return stablo;
}

void ispeglaj_stablo(element* stablo, vector< set< literal > > &s, int negacija = 0) {
  literal *lit = toLiteral(stablo);
  if (lit != NULL) {
    literal L(lit->getSymbol(), negacija);
    s.back().insert(L);
    return;
  }

  izraz *curr = toIzraz(stablo);
  if (curr == NULL) return;

  ispeglaj_stablo(curr->getLeft(), s, curr->getOp() == "~");
  if (curr->getOp() == "&") {
    s.push_back(set< literal >());
  }
  ispeglaj_stablo(curr->getRight(), s, curr->getOp() == "~");

  return;
}

vector< set < literal > > cnfConvert(element *stablo) {
  // uklanjanje ekvivalencije
  stablo = ponovi(stablo, ukloni_ekvivalenciju);
  stablo = ponovi(stablo, primjeni_involuciju);

  // uklanjanje implikacije
  stablo = ponovi(stablo, ukloni_implikaciju);
  stablo = ponovi(stablo, primjeni_involuciju);

  // potiskivanje negacije do atoma
  stablo = ponovi(stablo, potisni_negaciju);
  stablo = ponovi(stablo, primjeni_involuciju);

  // primjena distributivnosti
  stablo = ponovi(stablo, primjeni_distributivnost);
  stablo = ponovi(stablo, primjeni_involuciju);

  // ispeglaj stablo
  vector< set < literal > > s;

  s.push_back(set< literal >());

  ispeglaj_stablo(stablo, s);

  delete stablo;

  return s;
}

vector< element* > parse_input(vector< string > premise) {
  vector< element* > ret;

  for (
    vector< string >::iterator
    it = premise.begin();
    it != premise.end();
    ++it
  ) {

    element* root = izgradi_stablo(*it);

    ret.push_back(root);
  }

  return ret;
}

bool is_subset(set< set< literal > > A, set< set< literal > > B) {
  // is A subset of B
  for (
    set< set< literal > >::iterator
    it = A.begin();
    it != A.end();
    ++it
  ) {
    if (B.find(*it) == B.end()) return false;
  }

  return (A.size() < B.size());
}

bool is_superset(set< literal >A, set< literal > B) {
  // is A superset of B
  for (
    set< literal >::iterator
    it = B.begin();
    it != B.end();
    ++it
  ) {
    if (A.find(*it) == A.end()) return false;
  }
  return (A.size() > B.size());
}

void ukloni_redundantne(set< set< literal > > &a) {
  set< set< literal > > next;

  for (
    set< set< literal > >::iterator
    i = a.begin();
    i != a.end();
    ++i
  ) {
    bool pokrivena = false;
    // TODO
    // apsorpcija

    // ako postoji klauzula koja
    // da li su svi iz i sadrzani u j is_superset(*i, *j)
    //
    for (
      set< set< literal > >::iterator
      j = a.begin();
      j != a.end();
      ++j
    ) {
      if (i != j && is_superset(*i, *j)) {
        pokrivena = true;
        break;
      }
    }

    if (pokrivena == false) {
      next.insert(*i);
    }

  }

  a = next;

  return;
}

void ukloni_nevazne(set< set< literal > > &a) {
  set< set< literal > > next;

  for (
    set< set< literal > >::iterator
    i = a.begin();
    i != a.end();
    ++i
  ) {
    bool tautologija = false;

    for (
      set< literal >::iterator
      j = i->begin();
      j != i->end();
      ++j
    ) {
      literal inv = *j;
      inv.setInvert(1 - inv.getInvert());

      if (i->find(inv) != i->end()) {
        tautologija = true;
        break;
      }
    }

    if (tautologija == false) {
      next.insert(*i);
    }

  }

  a = next;

  return;
}

set< set< literal > > plResolve(set< literal > A, set< literal > B) {
  set< set< literal > > ret;
  set< literal >::iterator it1, it2, it3;

  literal NIL('!', 0);

  for (it1 = A.begin(); it1 != A.end(); ++it1) {
    for (it2 = B.begin(); it2 != B.end(); ++it2) {
      if (it1->getSymbol() == it2->getSymbol() &&
          it1->getInvert() != it2->getInvert()
      ) {
        set< literal > novi;
        bool notEmpty = false;

        for (it3 = A.begin(); it3 != A.end(); ++it3) {
          if (it3 != it1) {
            novi.insert(*it3);
            notEmpty = true;
          }
        }
        for (it3 = B.begin(); it3 != B.end(); ++it3) {
          if (it3 != it2) {
            novi.insert(*it3);
            notEmpty = true;
          }
        }

        if (notEmpty == false) {
          novi.insert(NIL);
        }
        ret.insert(novi);
      }
    }
  }

  return ret;
}

void unija(set< set< literal > > &a, set< set< literal > > b) {
  for (
    set< set< literal > >::iterator
    it = b.begin();
    it != b.end();
    ++it
  ) {
    a.insert(*it);
  }
  return;
}

set< set< literal > > presjek(set< set< literal > > a, set< set< literal > > b) {
  set< set< literal > > ret;

  for (
    set< set< literal > >::iterator
    it = b.begin();
    it != b.end();
    ++it
  ) {
    if (a.find(*it) != a.end()) {
      ret.insert(*it);
    }
  }
  return ret;
}

set< set< literal > > bez_set(set< set< literal > > a, set< set< literal > > b) {
  // A \ B
  set< set< literal > > ret;

  for (
    set< set< literal > >::iterator
    it = a.begin();
    it != a.end();
    ++it
  ) {
    if (b.find(*it) == b.end()) {
      ret.insert(*it);
    }
  }
  return ret;
}

bool hasNIL(set< set< literal > > a) {
  literal NIL('!', 0);
  for (
    set< set< literal > >::iterator
    it = a.begin();
    it != a.end();
    ++it
  ) {
    if (it->find(NIL) != it->end()) return true;
  }

  return false;
}


void copySetToVector(vector< set< literal > > &a, set< set< literal > > b) {
  for (
    set< set< literal > >::iterator
    it = b.begin();
    it != b.end();
    ++it
  ) {
    a.push_back(*it);
  }
  return;
}

void pojednostavi(set< set< literal > > &s) {
  ukloni_redundantne(s);
  ukloni_nevazne(s);
}

bool plResolution(vector< element* > premise, int strategija) {
  // strategija = 0 -> skup potpore
  // strategija = 1 -> skup potpore sa pojednostavljivanjem

  set< set< literal > > clauses;
  set< set< literal > > support;
  set< set< literal > > skup;
  set< set< literal > > temp;

  vector< set< literal > >::iterator it_i;
  // pretvori u cnf
  for (
    vector< element* >::iterator
    it = premise.begin();
    it != premise.end();
    ++it
  ) {
    vector< set< literal > > temp = cnfConvert(*it);

    for (it_i = temp.begin(); it_i != temp.end(); ++it_i) {
      if (it + 1 != premise.end()) {
        clauses.insert(*it_i);
      } else {
        support.insert(*it_i);
      }
    }
  }

  if (strategija) {
    pojednostavi(clauses);
    pojednostavi(support);
  }

  print_set(clauses);
  printf("--------------\n");
  print_set(support);
  printf("--------------\n");

  unija(skup, clauses);
  unija(skup, support);

  copySetToVector(SVE, skup);

  BROJAC = 0;
  KLAUZULA = skup.size();

  while (1) {
    if (KLAUZULA > MAX_KLAUZULA) break;
    if (BROJAC > MAX_KORAKA) break;

    set< set< literal > >::iterator i, j;
    set< set< literal > > resolvents;
    set< set< literal > > novi;

    for (i = support.begin(); i != support.end(); ++i) {
      // barem jedna roditeljska klauzula mora biti iz skupa potpore

      for (j = support.begin(); j != support.end(); ++j) {
        resolvents = plResolve(*i, *j);
        if (hasNIL(resolvents)) return true;
        unija(novi, resolvents);
      }

      for (j = clauses.begin(); j != clauses.end(); ++j) {
        resolvents = plResolve(*i, *j);
        if (hasNIL(resolvents)) return true;
        unija(novi, resolvents);
      }
    }

    if (strategija) {
      pojednostavi(novi);
    }

    if (is_subset(novi, skup)) return false;
    // dodavanje novih klauzula u skup potpore

    temp = skup;

    unija(support, novi);
    unija(skup, support);

    if (strategija) {
      pojednostavi(skup);
      pojednostavi(support);
    }

    temp = bez_set(novi, temp);

    KLAUZULA = max(KLAUZULA, (int)skup.size());
    BROJAC += temp.size();

    copySetToVector(SVE, temp);
  }

  warnx("Prekoracena ogranicenja");
  return false;
}

int main(int argc, char **argv) {
  vector< element* > stabla;
  vector< string > premise;
  int strategija;

  load_input(premise, strategija);

  stabla = parse_input(premise);

  cout << (plResolution(stabla, strategija) ? "Dokazano" : "Nije dokazano") << endl;

  cout << "Broj koraka: " << BROJAC << endl;
  cout << "Najveci broj klauzula u memoriji: " << KLAUZULA << endl;

  // dump korake
  int i = 0;
  for (
    vector< set< literal > >::iterator
    it = SVE.begin();
    it != SVE.end();
    ++it
  ) {
    cout << ++i << " ";
    for (
      set< literal >::iterator
      it1 = it->begin();
      it1 != it->end();
      ++it1
    ) {
      if (it1 != it->begin()) cout << " v ";

      cout << " "<< (it1->getInvert() ? "~" : "") << it1->getSymbol();
    }
    cout << endl;
  }

  return 0;
}
