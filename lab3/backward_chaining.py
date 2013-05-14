import sys

RADNA_MEMORIJA = dict()
KONFLIKTNI_SKUP = list()
VISITED = list()
STOG = list()

def checkInput(input, vars):
  ispravan_naziv = False
  for var in vars:
    if var.naziv == input:
      ispravan_naziv = True

  if ispravan_naziv == True:
    return True
  else:
    return False

def checkInputValue(input, vars, parametar):
  ispravan_naziv = False
  for var in vars:
    if var.naziv == parametar and input in var.values:
      ispravan_naziv = True

  if ispravan_naziv == True:
    return True
  else:
    return False

def print_KS():
  print "-------Konfliktni skup-------"
  for pravilo in KONFLIKTNI_SKUP:
    print pravilo.naziv,
  print "\n-----------------------------"
  return

def print_RM():
  print "-------Radna memorija--------"
  for value in RADNA_MEMORIJA:
    print value, "->", RADNA_MEMORIJA[value]
  print "\n-----------------------------"
  return

def solve(cilj, pravila, varijable):
  STOG.append(cilj)

  broj_koraka = 0
  max_broj_koraka = 1024

  while len(STOG) > 0 and broj_koraka < max_broj_koraka:
    broj_koraka += 1
    # korak 2
    vrati_se_na_korak_2 = False
    # u svakom koraku ispis RM, KS i pravila koje pali

    # print radna memorija
    print_RM()

    vrh_stoga = STOG.pop()
    print "-> Vrh stoga", vrh_stoga

    # sva pravila koja zadovoljavaju desni cilj
    for pravilo in pravila:
      if vrh_stoga in pravilo.konzekvens:
        if pravilo not in VISITED:
          if pravilo not in KONFLIKTNI_SKUP:
            KONFLIKTNI_SKUP.append(pravilo)

    # drzimo konfliktni skup prioritetnim :)
    KONFLIKTNI_SKUP.sort(key=lambda pravilo: int(pravilo.prioritet), reverse=True);

    # print konfliktni skup
    print_KS()

    while len(KONFLIKTNI_SKUP) > 0:
      pravilo = KONFLIKTNI_SKUP.pop(0)
      # while petlja u slucaju vracanja na korak 4
      while 1:
        premise_zadovoljene = True
        premise_nema_radnoj_memoriji = None
        # provjera zadovoljenosti premisa

        for premisa in pravilo.antecedens:
          if premisa not in RADNA_MEMORIJA:
            premise_nema_radnoj_memoriji = premisa
            break

        for premisa in pravilo.antecedens:
          premisa_zadovoljena = False

          for values in pravilo.antecedens[premisa]:
            if premisa in RADNA_MEMORIJA and RADNA_MEMORIJA[premisa] == values:
              premisa_zadovoljena = True
              break

          if premisa in RADNA_MEMORIJA and premisa_zadovoljena == False:
            premise_zadovoljene = False
            break

        # for premisa in pravilo.antecedens

        # korak 4
        if premise_zadovoljene == False:
          # obrisi pravilo iz skupa
          VISITED.append(pravilo);
          break
        elif premise_nema_radnoj_memoriji == None:
          print "PALI PRAVILO", pravilo.naziv

          # dodaj u radnu memoriju
          for premisa in pravilo.konzekvens:
            RADNA_MEMORIJA[premisa] = pravilo.konzekvens[premisa]

          vrati_se_na_korak_2 = True
          # obrisi pravilo iz konfliktnog skupa
          VISITED.append(pravilo);
          break

          pass
        elif premise_nema_radnoj_memoriji != None:

          for pr in pravila:
            if premise_nema_radnoj_memoriji in pr.konzekvens:
              STOG.append(vrh_stoga)
              STOG.append(premise_nema_radnoj_memoriji)
              vrati_se_na_korak_2 = True
              break;

          ulazna_varijabla = False
          for var in varijable:
            if var.tip == "ulazna" and var.naziv == premise_nema_radnoj_memoriji:
              ulazna_varijabla = True

          # ako je varijabla ulazna pitaj korisnika
          if vrati_se_na_korak_2 == False and ulazna_varijabla == True:
            while 1:
              print "Unesite vrijednost", premise_nema_radnoj_memoriji
              premisa = sys.stdin.readline().strip()
              if checkInputValue(premisa, varijable, premise_nema_radnoj_memoriji) == True:
                break;
              else:
                print "Neispravna vrijednost", premisa
            # while 1

            RADNA_MEMORIJA[premise_nema_radnoj_memoriji] = premisa
            # vrati se na korak 4a
          else:
            break

      # while 1

      if vrati_se_na_korak_2 == True:
        break

    # for pravilo in KONFLIKTNI_SKUP

  print "---------------------------"
  if cilj in RADNA_MEMORIJA:
    print RADNA_MEMORIJA[cilj]
  else:
    print "Pojma nemam!"

  return
