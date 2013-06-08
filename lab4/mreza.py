import sys
import random
from mojlib import *

def main():
  POGRESKA = 0.1
  ITERACIJA = 10000
  N = 13
  K = 1
  P = 4.0 / N
  MREZA = 50

  TEST = dict()
  TRAIN = dict()

  if len(sys.argv) < 7:
    print "Usage: mreza <pogreska> <iteracija> K MREZA <train_file> <test_file>"
    sys.exit(1)
  elif len(sys.argv) == 8:
    P = float(sys.argv[7])

  POGRESKA = float(sys.argv[1])
  ITERACIJA = int(sys.argv[2])
  K = float(sys.argv[3])
  MREZA = int(sys.argv[4])

  # train file
  file = open(sys.argv[5], "r");
  for line in file.readlines():
    (x, fx) = line.strip().split(" ")
    TRAIN[float(x)] = float(fx)
  file.close()

  # test file
  file = open(sys.argv[6], "r");
  for line in file.readlines():
    (x, fx) = line.strip().split(" ")
    TEST[float(x)] = float(fx)
  file.close()

  # populacija od MREZA jediniki
  mreze = []
  for i in range(MREZA):
    mreze.append(NeuronskaMreza(N, [random.gauss(0, 2) for x in range(N)]))

  # najbolja mreza
  rezultat = None

  # ucenje
  for counter in range(ITERACIJA):
    nova = []
    # uzmi najbolju
    best_id = 0
    for i in range(MREZA):
      if mreze[i].pogreska(TRAIN) < mreze[best_id].pogreska(TRAIN):
        best_id = i

    print "Generacija %d, pogreska %f" % (counter, mreze[best_id].pogreska(TRAIN))
    rezultat = mreze[best_id]

    if mreze[best_id].pogreska(TRAIN) < POGRESKA:
      break

    nova.append(mreze[best_id]);

    # niz duljina intervala za selekciju
    length = init_selekcija(mreze, TRAIN)

    # proporcionalna selekcija
    for n in range(MREZA - 1):
      i = selekcija(length)
      j = selekcija(length)

      dijete = krizaj(mreze[i], mreze[j])
      mutiraj(dijete, P, K)

      nova.append(dijete)

    # nova generacija
    mreze = nova

  print "Parametri, N = %d, K = %f, P = %F, MREZA = %d" % (N, K, P, MREZA)
  print "pogreska (TRAIN) %f" % (rezultat.pogreska(TRAIN))
  print "pogreska (TEST ) %f" % (rezultat.pogreska(TEST, 0))

  return

if __name__=="__main__":
  main()
