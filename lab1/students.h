#ifndef STUDENTS_H
#define STUDENTS_H

#include <vector>
#include <string>

#define MAX_STUDENTS 16

// class used to store the problem specific data
class Students {
  private:
    std::vector< int > time;
  public:
    // constructors
    Students();
    Students(std::vector< int >);

    // methods
    int getCount();
    int getTime(int);
    std::string getStrTime(int);
};

#endif
