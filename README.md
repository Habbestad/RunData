# RunData

Oversikt:

readers.cpp - Garminklokken produserer tcx-filer med måledata (tidspunkt, distanse, høyde, fart etc). Disse filene har en spesiell form,
og funksjonene i readers.cpp er lagd for å lese disse.

runclasses.cpp - Klasser for å oppbevare dataen og eventuelt manipulere den. BasicRun holder kun dato, total tid og total distanse.
StandardRun holder lister av data (tidspunkt, distanse, fart etc). Workout er en underklasse av StandardRun, som skal ta høyde for
"workouts" som er kjørt på Garminklokken.

timestamp.cpp - Her er kun hjelpeklassen TimeStamp, som brukes for å holde styr på datoer og tidspunkt.

test.cpp - Her tester jeg nye ting. Neste prosjekt er å plotte en oversikt over en lengre periode med løpeturer, så akkurat nå har jeg
testet hvordan man kan samle og sortere en samling løpeturer fra mappen hvor filene ligger.

Headerfilene (.h) består bare av deklarasjoner av klassene/funksjonene i tilsvarende .cpp fil.

setup.py - Setup for å bygge Cython-utvidelse.

runclasses.pxd - Er som en headerfil for Cython, så her er det bare deklarasjoner av de tingene jeg vil bruke i Python.

runclasses.pyx - Cython fil som wrapper C++ klassene til Pythonklasser.

plot_test.py - Test av å plotte en enkel løpetur. Her er et eksempel:

![bilde](test.png)


