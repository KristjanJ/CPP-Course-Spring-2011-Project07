Boosti versioon, mis oli kasutuses: 1.46.1.
Tegin ka praktikumi lehelt saadud Boost 1.41.0-ga compile teste(regexi jaoks teha ei saanud, kuna regexi dll oli puudu).

M�ned kommentaarid lahenduste kohta:

1. FindText - pattern anda regexi s�ntaksiga, ehk siis a t�hega algavad failid: "a.*".
2. SortFiles - kuup�evade kaupa sorteerimine toimub igas alamkaustas eraldi. Samale kaustale ei ole hea �le �he korra sorteerimist rakendada.
Faililaiend tuleks anda koos punktiga, kuna Boost ise k�sitleb laiendis sellisel viisil.
3. Philosophers - igal forkil on enda mutex, sest kui tegemist oleks �ldise mutexiga kogu array peale, siis
toimuks s��mine liiga aeglaselt.
