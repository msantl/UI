import copy

class systemVar_t:
  def __init__(self, tip = "ulazna", naziv = None, values = list()):
    self.tip = tip       # tip varijable (ulazna, sustav)
    self.naziv = naziv   # naziv varijable
    self.values = values # domena vrijednosti

class systemRule_t:
  def __init__(self, prioritet = 0, naziv = None, antecedens = dict(), konzekvens = dict()):
    self.prioritet = prioritet    # prioritet pravila
    self.naziv = naziv            # naziv pravila
    self.antecedens = antecedens  # antecedens pravila
    self.konzekvens = konzekvens  # konzekvens pravila

  def clear(self):
    self.prioritet = 0
    self.naziv = None
    self.antecedens = dict()
    self.konzekvens = dict()

# globalne varijable
VARIJABLE = list()
PRAVILA = list()

# parser
def parse_input(input):
  activeRule = systemRule_t()
  prioritet_counter = 0
  naziv_counter = 1

  for line in input:
    line = line.strip()
    if line.startswith("IF"):
      line = line[line.find("IF") + 2 :].strip()
      line = line.split("&")

      for par in line:
        var = par[0: par.find("=")].strip()
        val = [a.strip() for a in par[par.find("=") + 1 :].strip().split("|")]

        activeRule.antecedens[var] = val;

      # zadan je uvjet
    elif line.startswith("THEN"):
      line = line[line.find("THEN") + 4 :].strip()
      line = line.split("&")

      for par in line:
        var = par[0: par.find("=")].strip()
        val = [a.strip() for a in par[par.find("=") + 1 :].strip().split("|")]

        # trebao bi postojati samo jedan konzekvens
        activeRule.konzekvens[var] = val[0]

      if activeRule.prioritet == 0:
        activeRule.prioritet = prioritet_counter;
      if activeRule.naziv == None:
        activeRule.naziv = naziv_counter;

      # spremi pravilo
      PRAVILA.append(copy.deepcopy(activeRule))
      activeRule.clear();
      prioritet_counter -= 1
      naziv_counter += 1

      # zadani su zakljuci
    elif line.startswith("RULE"):
      naziv = line.split(" ")[1].strip()
      prioritet = line.split(" ")[3].strip()

      activeRule.prioritet = prioritet
      activeRule.naziv = naziv
      # zadano je ime pravila i prioritet
    elif len(line):
      # zadane su varijable sustava
      tip = "sustav"
      naziv = line[0:line.find("=")].strip()

      if naziv.endswith("*"):
        tip = "ulazna"
        naziv = naziv[:-1]

      values = [val.strip() for val in line[line.find("=") + 1:].split("|")]

      VARIJABLE.append(systemVar_t(tip, naziv, values))

  # sortiraj pravila po prioritetu, padajuce
  PRAVILA.sort(key=lambda pravilo: int(pravilo.prioritet), reverse=True)

def print_system():
  print "VARIJABLE SUSTAVA"
  print "====================================================="
  for var in VARIJABLE:
    print var.naziv, var.tip, var.values
  print "====================================================="

  print "PRAVILA SUSTAVA"
  print "====================================================="
  for rule in PRAVILA:
    print rule.naziv, rule.prioritet, rule.antecedens, rule.konzekvens
  print "====================================================="

