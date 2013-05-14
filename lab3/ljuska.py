import sys

from parser import *
from backward_chaining import *

def main():
  if len(sys.argv) != 2:
    print "Usage: ljuska.py <input file>"
    sys.exit(1)

  # ucitaj i parsiraj datoteku
  file = open(sys.argv[1], "r");
  parse_input(file.readlines());
  file.close()

  # ispis stanja sustava
  print_system()

  # unos trazenog cilja
  while 1:
    print "Unesite trazeni cilj"
    cilj = sys.stdin.readline().strip()

    if checkInput(cilj, VARIJABLE) == True:
      break
    else:
      print "Neispravna vrijednost"

  # backward_chaining
  print "--------------Solving----------------"
  solve(cilj, PRAVILA, VARIJABLE)

  print "----------------Done-----------------"
  sys.exit(0)

if __name__ == "__main__":
  main()
