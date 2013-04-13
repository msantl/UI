#ifndef STRUKTURE_PODATAKA_H
#define STRUKTURE_PODATAKA_H

#include <string>

class element {
  private:
    bool invert;
  public:

    element();
    element(bool);

    bool getInvert() const;
    void setInvert(bool);

    virtual void dummy() {}
};

class literal : public element {
  private:
    char symbol;
  public:

    literal();
    literal(char);
    literal(char, bool);

    char getSymbol() const;
    void setSymbol(char);

    bool operator == (const literal &a) const {
      return
        this->getSymbol() == a.getSymbol() &&
        this->getInvert() == a.getInvert();
    }

    bool operator < (const literal &a) const {
      return
        this->getSymbol() < a.getSymbol() ||
        (
         this->getSymbol()  ==  a.getSymbol() &&
         this->getInvert() < a.getInvert()
        );
    }
};

class izraz : public element {
  private:
    std::string op;
    element* left;
    element* right;
  public:

    izraz();
    izraz(std::string, element*, element*);

    ~izraz();

    std::string getOp();
    void setOp(std::string);

    element* getLeft();
    element* getRight();

    void setLeft(element*);
    void setRight(element*);
};

literal* toLiteral(element*);
izraz* toIzraz(element*);

#endif
