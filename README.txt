Boosti versioon, mis oli kasutuses: 1.46.1.
Tegin ka praktikumi lehelt saadud Boost 1.41.0-ga compile teste(regexi jaoks teha ei saanud, kuna regexi dll oli puudu).

Mõned kommentaarid lahenduste kohta:

1. FindText - pattern anda regexi süntaksiga, ehk siis a tähega algavad failid: "a.*".
2. SortFiles - kuupäevade kaupa sorteerimine toimub igas alamkaustas eraldi. Samale kaustale ei ole hea üle ühe korra sorteerimist rakendada.
Faililaiend tuleks anda koos punktiga, kuna Boost ise käsitleb laiendis sellisel viisil.
3. Philosophers - igal forkil on enda mutex, sest kui tegemist oleks üldise mutexiga kogu array peale, siis
toimuks söömine liiga aeglaselt.
