import math
import random

def funkcija(x, w0, w1):
  net = w0 + w1 * x
  if net > 700: return 1
  if net < -700: return 0

  return 1 / (1 + math.exp(-net))

class NeuronskaMreza:
  def __init__(self, tezina, tezine):
    self.tezina = tezina
    self.tezine = tezine
    self.greska = -1

  def evaluate(self, x):
    c11 = funkcija(x, self.tezine[0], self.tezine[1])
    c12 = funkcija(x, self.tezine[2], self.tezine[3])
    c13 = funkcija(x, self.tezine[4], self.tezine[5])
    c14 = funkcija(x, self.tezine[6], self.tezine[7])
    c21 = self.tezine[8] + self.tezine[9] * c11 + self.tezine[10] * c12 + self.tezine[11] * c13 + self.tezine[12] * c14;

    return c21;

  def pogreska(self, skup, cache = 1):
    if cache == 1 and self.greska != -1:
      return self.greska

    result = 0

    for key in skup:
      result += (skup[key] - self.evaluate(key)) ** 2

    self.greska = result

    return result

def krizaj(mreza1, mreza2):
  mreza = NeuronskaMreza(mreza1.tezina, [0] * mreza2.tezina)

  for i in range(mreza1.tezina):
    mreza.tezine[i] = (mreza1.tezine[i] + mreza2.tezine[i]) / 2

  return mreza

def mutiraj(mreza, P, K):
  for i in range(mreza.tezina):
    if random.random() < P:
      mreza.tezine[i] += random.gauss(0, K)

  return

def init_selekcija(mreze, skup):
  maks = 0
  for mreza in mreze:
    maks = max(maks, mreza.pogreska(skup))

  fitness = []
  length = []
  suma = 0

  maks += 1

  for mreza in mreze:
    fit = maks - mreza.pogreska(skup)
    fitness.append(fit)
    suma += fit

  for fit in fitness:
    length.append(fit / suma)

  return length

def selekcija(length):
  p = random.random()
  id = 0

  for len in length:
    if p - len <= 0:
      return id
    p -= len
    id += 1

  return id

