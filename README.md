# OI-projekat

Iks-oks korištenjem MinMax algoritma

Algoritam koristi backtracking, odnosno isprobava sve poteze dok ne nadje optimalan potez.
Pretpostavka je da i protivnik takodje igra optimalno, to jest bira poteze najbolje po njega.
Tabla u svakom trenutku ima sebi dodijeljenu vrijednost koja se racuna putem funkcije evaluateBoard (funkcijom evaluacije).
Tacnije, za svaki moguci potez koji algoritam 'napravi' tabli se dodijeljuje odredjena vrijednost, zatim se potez 'ponisti'.
Potezi se 'prave i ponistavaju' sve dok se ne nadje potez koji tabli daje optimalnu vrijednost za trenutnog 
igraca, odnosno maksimalnu za maximizera, minimalnu za minimizera, i zatim se taj potez 'definitivno pravi'. 
Posto je broj mogucih poteza relativno mali, mozemo sebi priustiti da prije igranja svakog poteza 'probamo' svaki legalan potez
bez da se predugo ceka na dobijanje 'definitivnog poteza'. 
Vrijednost table govori ko trenutno vodi. Tako ukoliko korisnik pobjedjuje tabla ce imati negativnu vrijednost, ukoliko dobija racunar, tabla ce imati neku pozitivnu vrijednost.

Funkcija minimax racuna optimalne poteze i za korisnika i za racunar. 
Princip rada je sledeci: tabla prije svakog poteza ima neku evaluiranu vrijednost. 
Recimo da prvi potez pravi racunar, odnosno maximizer, on ce odabrati potez koji ce tabli da dodijeli maksimalnu vrijednost.
Sada algoritam racuna optimalan potez za korisnika, odnosno minimizera, od svih mogucih poteza bira potez koji ce da minimizuje
vrijednost table, odnsno dodijeljuje joj najmanju vrijednost iz skupa svih trenutno mogucih vrijednosti. 
Sada ponovo maksimizer bira potez koji ce maksimizovati vrijednost table i tako dok se igra ne zavrsi.
Dakle, maximizer uvijek bira poteze koji ce tabli dati najvecu vrijednost od svih mogucih vrijednosti, minimizer
bira poteze koji ce da tabli dodijeli minimalnu vrijednost od svih mogucih vrijednosti (igra optimalno za oba igraca).

Posto moze doci do situacije da vise poteza dovodi do pobjede maximizera ili minimizera da bi se osiguralo da se od svih
takvih poteza bira optimum racuna se za svaki potez dubina, odnosno broj koraka koji dovodi do pobjede ili nerijsenog rezultata ukoliko pobjeda onog ko je trenutno na potezu nije moguca. 
Zatim se taj broj koraka dodaje na vrijednost poteza u slucaju minimizera, odnosno potez se 'kaznjava', ili oduzima
od vrijednosti poteza u slucaju minimizera, istom logikom. Na taj nacin ce najmanje kaznjeni potezi imati najbolju
(najvecu za maximizera, najmanju za minimizera) vrijednost i oni ce se birati (jer su optimalni).

Igra se zavrsava kada su ili sva polja popunjena ili kada neko pobijedi, odnsono spoji 3 ista znaka horizontalno,
vertikalno ili dijagonalno. Funkcija evaluacije u tom trenutku dobija vrijednost 'beskonacnosti' ukoliko su spojena 
tri O (maximizera) - O je pobijedio, ili 'negativne beskonacnosti' ukoliko su spojena tri X (minimizera) - X je pobijedio.
'Beskonacnosti' je dodijeljena vrijednost 100 po uzoru na algoritme za igranje šaha (chess engines).
U slucaju remija vrijednost funkcije evaluacije ostaje nula.

Najbolji potezi za racunar se racunaju u funkciji findBestMove koja vraca koordinate poteza sa maksimalnom
vrijednoscu funkcije evaluacije. 
