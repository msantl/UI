#ifndef STATE_H
#define STATE_h

#define LEFT 1
#define RIGHT 0

class State {
  private:
    // first 32 bits    = cost
    // second 32 bits   = state descriptor
    // state descriptor = 0-15 bit => student state
    //                    16   bit => flashlight state
    unsigned long long int state;
  public:
    State* parent;
    // constructors
    State();
    State(int, int, int);

    // methods
    void fixCost(int);
    int getCost() const;
    int getState() const;
    int getStatus(int) const;
    int getFlashlightStatus() const;
    unsigned long long int getHash() const;

    static State* getAcceptableState(int);
    static State* transitionSingle(State*, int, int);
    static State* transitionPair(State*, int, int, int);

    // operator
    bool operator < (const State &) const;

};

#endif
