# BMS

- Bezdrátové a mobilní sítě
- Wireless and Mobile Networks

<!------------------------------------------------------------->

## 0) Organizace

- 5 kr
- Zakončení: zápočet + zkouška
- Rozsah:
    - 39 hod. přednášky
    - 13 hod. projekty
- Bodové hodnocení:
    - 70 zkouška
    - 15 půlsemestrální test
    - 15 projekty
- Distanční výuka:
    - Microsoft Teams
- Záznamy: ne
- Předmět postavený na:
    - Jochen Schiller -- Mobile Communications 1st Edition

<!------------------------------------------------------------->

## 0) Opakování, prerekvizity

#### Frekvence (kmitočet)
- Počet opakování děje za daný časový okamžik.
- Značka: f
- Jednotka: Hz (Hetz)

#### Perioda
- Doba trvání opakování jednoho periodického děje.
- Značka: T
- Jednotka: s (sekunda)

#### Vlnová délka
- Vzdálenost dvou nejbližších bodů postupného periodického vlnění.
- "Jak jsou od sebe jednotlivé vlny daleko"
- Značka: lambda
- Jednotka: m (metr)

#### Fázová rychlost (rychlost šíření)
- Rychlost jakou se pohybuje fáze vlnění (vlna) prostorem.
- Značka: v_f
- Jednotka: m/s (metr za sekundu)

#### Elektromagnetické vlnění
- Děj, při němž se prostorem šíří vlnění elektrického a magnetického pole (elektromagnetické vlny).
- viz. Faraday, Maxwell, Hertz
- Rozdělujeme podle vlnových délek (frekvence)
    - Nás zde budou primárně zajímat rádiové vlny (300 GHz — 30 Hz).
- Existují konvence pro používají konkrétních frekvencí pro konkrétní účely
    - Telekomunikační úřad poté přidělují konkrétní frekvence konkrétním soukromím subjektům (rádia, televize, ...).

<!------------------------------------------------------------->

## 1) Základy bezdrátové komunikace

### Antény
- Zařízení pro příjem nebo vysílání elektromagnetického záření, konkrétně rádiových vln.
- Izotropní zářič - teoretická referenční anténa, která vyzařuje do všech směrů stejně
- Reálné antény mají vždy směrový efekt
    - Např. dipóly - nahoru a dolů (antény na střeše auta)
- Signál (síla vln) postupně slábne se vzdáleností

<!-- Dosah sireni signalu -->
![](https://i.imgur.com/so8flFr.png =600x)

#### Směrové a sektorové antény
![](https://i.imgur.com/edaoM6N.png =650x)

#### Diverzita
- Seskupení 2 nebo více antén
- Způsoby diverzity
    - Přepínaná diverzita, selektivní diverzita
        - Přijímač si vybere anténu s nejlepším signálem
    - Kombinovaná diverzita
        - Kombinuje výstupy pro zvýšení zisku
        - Je nutné správné sfázování

#### Hata Model
- Model šíření signálu pro predikování ztráty / nezachycení vln v exteriorním prostředí.
- Typicky různé městské objekty, budovy, ...

### Typ provozu

#### Jednosměrný
- Od vysílače k příjimači
- Broadcast, rozhlas, radio, ...

#### Obousměrný

##### Simplex
- V jednom okamžiku přenos pouze jedním směrem
- Je třeba přepínat mezi příjmem a vysíláním
- Komunikace na jedné frekvenci
- Např. vysílačky

##### Dusimplex
- V jednom okamžiku přenos pouze jedním směrem
- Provoz probíhá na dvou kmitočtech, tj. radiostanice na jiném kmitočtu vysílá a na jiném přijímá

##### Duplex
- Obousměrný způsob provozu - přenos informace v obou směrech současně
- Vyžaduje použití dvou kmitočtů

##### Semidumplex
- Obousměrný provoz při kterém jedna stanice (základna) pracuje v duplexním režimu a druhá (pohybová) v dusimplexním.
- Provoz probíhá na dvou kmitočtech.

### Sdílení spektra
- Spektrum = šířka pásma

#### Multiplexovování
- Multiplexování je proces, ve kterém je více analogových signálů nebo digitálních datových toků kombinováno do jednoho signálu.
- Cílem je co možná nejefektivnější využití daného přenosového média.
- Zařízení provádějící multiplexování se nazývá multiplexor a zařízení provádějící převod signálu zpět na jednotlivé signály se nazývá demultiplexor.

##### Frekvenční multiplex
- Princip přenosu více signálů jedním společným širokopásmovým přenosovým médiem, při kterém každý z přenášených signálů používá jinou část kmitočtového pásma.
- Výhody:
    - Není třeba žádná dynamická koordinace
    - Funguje pro analogové i digitální sítě
- Nevýhody:
    - Plýtvání pásmem pokud je provoz rozložen nerovnoměrně
    - Potřebné ochranné intervaly

##### Časový multiplex
- Princip přenosu více signálů jedním společným přenosovým médiem, při kterém jsou jednotlivé signály odděleny tím, že se každý z nich je vysílán pouze v krátkých pevně definovaných časových intervalech nazývaných časové sloty (timeslot – TS).
- Laicky řečeno „každý chvilku tahá pilku“.
- Výhody:
    - Pouze jedna nosná v jednom okamžiku
    - Vysoká propustnost i v případě mnoha uživatelů
- Nevýhody:
    - Nutná synchronizace

##### Časový a frekvenční multiplex
- Kombinace předchozích dvou
- Kanál dostává určitou frekvenci pro určitý časový interval
- Příklad využití:
    - GSM (Groupe Spécial Mobile)
- Výhody:
    - Lepší ochrana proti odposlechu
    - Ochrana proti frekvenčně omezené interferenci (ovlivňování/prolínání signálů)
- Nevýhody
    - Nutná časová i frekvenční koordinace

##### Kódový multiplex (CDMA)
- Metoda digitálního multiplexování, tzn. přenosu vícero digitálních signálů prostřednictvím jediného sdíleného média, která jednotlivé signály rozlišuje tím, že každé z nich používá odlišné (vhodně navržené) kódování.
- Všechny kanály používají totéž pásmo současně.
- Implementováno pomocí technologií s rozprostřeným spektrem (spread spectrum)
- Výhody:
    - Efektivní využití pásma
    - Není nutná koordinace a synchronizace
    - Lepší ochrana proti interferenci a odposlouchávání
- Nevýhody:
    - Nižší rychlost přenosu
    - Složitější regenerace signálu

### Celulární (buňkové) sítě

- Celulární neboli buňková rádiová síť je rádiová telekomunikační síť, v níž komunikaci v rozlehlejší geografické oblasti zajišťuje množství základnových stanic, které svým dosahem vytvářejí soustavu vzájemně se překrývajících poměrně malých buněk.
- Implementují prostorový multiplex (základní stanice pokrývá pouze určité území - buňka)
- Výhody:
    - Vysoká kapacita, hodně uživatelů
    - Základnové stanice se starají o interferenci apod.
- Nevýhody:
    - Základnové stanice musí spojovat pevná síť
    - Je třeba implementovat tzv. handover (přechod spojení od jedné buňky k druhé)
    - Interference mezi buňkami
- Velikost buňky se pohybuje od 100m ve městech až přes 35km v krajině (GSM), až po cca 100km

#### Frekvenční plánování
- Jaká buňka dostane jakou frekvenci
- Frekvence se dá znovu použít v určité vzdálenosti mezi základnovými stanicemi
- Standardní model používá 7 frekvencí


### Modulace a demodulace

- Dobré vysvětlení: [youtu.be/FqSWpi8n1rY](https://youtu.be/FqSWpi8n1rY)
- Na straně vysílače
    - Vstupní digitální signál (baseband) je třeba převést do analogového - digitální modulace
    - Analogový signál je pak třeba dostat na správnou frekvenci (do správného formátu) kombinací s nosným signálem - analogová modulace
- Na straně přijímače
    - Oddělení nosného signálu - analogová demodulace
    - Převod analogového signálu do digitálního - synchronizace digitalizace

#### Nosný signál/vlna  (carrier signal/wave)
- Je signál (obvykle sinusový), který je pomocí některého druhu modulace zkombinován se vstupním signálem nesoucím užitečnou informaci, tak zvaným modulačním signálem, za účelem přenosu tohoto modulačního signálu.
- Nosná vlna má obvykle mnohem vyšší frekvenci než modulační signál, a modulací se dosáhne toho, že výsledný modulovaný signál má podobné vlastnosti jako nosná vlna.
- Díky použití modulace lze přenášet informace otevřeným prostorem pomocí elektromagnetických vln (rádiová komunikace), nebo pomocí několika nosných vln o různých frekvencích sdílet společné fyzické přenosové médium pomocí frekvenčního multiplexu (např. v systémech kabelové televize).
- Termín se také používá pro elektromagnetické vlnění bez přítomnosti modulačního signálu.

#### Analogová modulace
- Posouvá frekvenci baseband signálu (vstupního) na frekvenci nosné
- Proč to dělat?
    - Kvůli přenosovému médiu - baseband se špatně vysílá
    - Kmitočtové příděli, potřebuji být na nějaké konkrétní frekvenci

##### Amplitudová modulace (AM)
- Využívá změny signálu pro přenos informace
- Velikost změny se nazývá **hloubka modulace**
- Špatná odolnost proti rušení

##### Frekvenční modulace (FM)
- Využívá změny frekvence pro přenos informace
- Velikost změny se nazývá **kmitočtový zdvih**

##### Fázová modulace (PM)

- Využívá změny posuvu fáze nosné pro přenos informace
- Je podobná frekvenční modulaci s velmi malým zdvihem
- Informace je zachycena o kolik je posunuta konkrétní fáze (vlna)

#### Digitální modulace (klíčování)
- Digitální data jsou převedena na analogový signál - baseband

##### Klíčování amplitudovým posuvem (Amplitude Shift Keying — ASK)
- Pokud vstupní signál je 1, tak přenáším nosnou, pokud 0, tak nic
- Jednoduché, malé požadavky na pásmo, náchylné na rušení (snadné zaměnitelné šum za 1)

##### Klíčování frekvenčním posuvem (Frequency Shift Keying — FSK)
- Dvě frekvence nosné (nižší a vyšší), pro 1 přenáším např vyšší, pro 0 nižší
- Větší šířka pásma

##### Gaussovské dolní propustnosti (Gaussian Minimum Shift Keying — GMSK)
- Spojení analogové a digitální modulace
- Vyhlazení hran digitálního signálu

##### Klíčování fázovým posuvem (Phase Shift Keying — PSK)
- Složitější, odolnější proti rušení
- Např. Binary Phase Shift Keying
    - Bit 0: sinusovka
    - Bit 1: invertovaná sinusovka
- Jednoduché, špatné využití pásma, robustní (odolné proti rušení)

##### Bitová a baudová rychlost
- **Bitová rychlost** (Bit rate) - kolik bitů za sekundu
- **Baudová rychlost** (Baud rate) - kolik signálových jednotek za sekundu
- Pro ASK, FSK, BPSK: Bit rate = Baud rate
- Baudová rychlost je menší nebo rovna bitové

##### Kvadraturní klíčování fázovým posuvem (Quadrature phase-shift keying — QPSK, 4PSK)
- Symbol je definován posuven sinusovky: sin(x + c)
    - c může posunout sinusovku 4mi způsoby (4 stavy - 2 bity)
- 2 bity jsou zakódovány jako jeden symbol
- Bit rate = 2 * baud rate

##### Kvadraturní amplitudové klíčování (Quadrature Amplitude Modulation — QAM)
- Kombinace aplitudové a fázové modulace
- Pomocí jednoho symbolu je možné zakódovat až 4 bitů
- 4-QAM: 1 amplituda, 4 fáze (4 stavy - 2 bity)
- 8-QAM: 2 amplitudy, 4 fáze (8 stavů - 3 bity)
- 16-QAM: 3 amplitudy, 12 fází | 4 amplitudy, 8 fází | 2 amplitudy, 8 fází (16 stavů - 4 bity)
- 32-QAM: ... (32 stavů - 5 bitů)
- 64-QAM: ... (64 stavů - 6 bitů)
- 128-QAM: ... (128 stavů - 7 bitů)
- 256-QAM: ... (256 stavů - 8 bitů)


### Hierarchická modulace
- Pro případ, že se nemůžeme adaptivně přepínat mezi jednotlivými X-QAM, protože máme pouze jednosměrný kanál
- Motivace: zákazníkům s kvalitním signálem chceme vysílat velkou rychlostí, pro zákazníky s nekvalitním signálem malou rychlostí
- Stream s vysokou prioritou (High Priority - HP) je zabudován ve streamu s nízkou prioritou (Low Priority - LP)
- Např.: máme 64 QAM
    - Účastník s kvalitním signálem dekóduje vše
    - Účastník s nekvalitním signálem dekóduje 16 bitů jako jeden (dostává QPSK - pomalejší, ale robustnější signál)

### Modulace s více nosnými
- ...

### Ultra wideband
- ...

<!------------------------------------------------------------->

## 2) Přístup k médiu

### Motivace
- Buňkové sítě, komunikace zákazníků se základovou stanici
- **Downlink**: Základová stanice → Mobilní stanice
- **Uplink**: Mobilní stanice → Základová stanice
- **Řídící kanály**: Kanály pro navázání spojení
- **Datové kanály**: Kanály pro přenos dat

#### Problém skrytého terminálu (Problem of hidden terminal)

![hidden terminal](https://i.imgur.com/ltbGR0r.png =250x)

- A vysílá pro B
- C neslyší A
- C chce vysílat pro B
- C slyší volné médium
- Vzniká kolize u B, A neslyší kolizi
- A je skrytý pro C


#### Problém vystaveného terminálu (Problem of exposed terminal)

![](https://i.imgur.com/CkRG706.png =400x)

- Mějme dva příjemce C a D
- A vysílá pro C, C je mimo dosah B
- B chce vysílat pro D, D je mimo dosah A
- B ví, že A vysílá a myslí si, že nemůže vysílat, protože by zarušil jeho klienta C - ačkoliv by se tak nestalo

![](https://i.imgur.com/ZmEuXUX.png =500x)

### Code Division Multiple Access (CDMA)

- Všechny systémy s rozprostřeným spektrem
- Kódové sdílení médie
- ToDo

### LFSR-based Stream Ciphers

- Linear Feedback Shift Register

- ToDo


<!------------------------------------------------------------->

## 3) Bezdrátové telekomunikační sítě

### Typický postup pro přenos

![](https://i.imgur.com/N253gdE.png =600x)

### 1) Kódování hlasu a digitalizace
- Z analogového hlasu uděláme digitální signál.
- Velká redundance, provádíme kompresy.
- Dojde ke ztrátě nějakých informací a přidání šumu (kvantizační šum) při dekódování.

#### Pulzně kódová modulace (Pulse-code modulation - PCM)
- Signál se navzorkuje a výška se zakóduje do binárního kódu
- Kolik vzorků, tolik stavů potřebuju

![](https://i.imgur.com/mwkznz0.png =600x)

- Např.:
    - Předpokládáme, že hlas potřebuje 4 kHz, podle X teorému potřebujeme 2x tolik vzorků, tedy 8000 vzorků za sekundu.
    - PCM = 8000 vzorků * 8 bitů / vzorek = 64 Kb/s

### 2) Kódování kanálu
- Data přenáším přes rádiové kanály, které jsou nespolehlivé.
    - Data mohu ztratit, bit může přeskočit, ...
- Počítáme s tím, zavádíme redundanci tak, aby bylo možné většinu chyb opravit.
    - Chceme minimalizovat opětovné přenášení, to je problém.
    - Detekce chyb pomocí BER - Bit Error Rate, FER - Frame Error Rate
- Zvyšuje se tím výrazně objem dat
- Metody
    - **Blokové korekční kódy** — vstup je rámec, výstup je původní rámec + zabezpečovací bity
    - **Konvoluční korekční kódy** — nezná pojem rámec, vstupují bity a vystupují bity rychleji
    - Turbo korekční kódy

#### Princip blokových kódů
- Alternativně "Forward Error Correction — FEC"

![](https://i.imgur.com/FIgX8Ur.png =600x)

- Zavádíme redundanci, proto pochopitelně n > k
- Možnosti:
    - Detekujeme chybu a lze ji opravit
    - Detekujeme chybu a nelze opravit, poté opětovný přenos
    - Nedetekujeme chybu (neznamená, že tam skutečně není)
- **Hammingova vzdálenost**
    - Počet bitů ve kterých se dvě binární posloupnosti liší
- Hammingův kód

#### Hammingův kód
- Chytře umísťuje paritní bity, na základě toho je schopen detekovat chybu a případně ji i opravit
- Parametry (n,k)
- 2^k platných klíčových slov
- k/n je kódový poměr
- Na každé pozici mocniny 2 je kontrolní bit

![](https://i.imgur.com/b3f7bgn.png)


### 3) Prokládání (interleaving)
- Pokud se na kanálu objevuje nějaké rušení, tak je většinou impulsní a zaruší několik bitů vedle sebe.
    - To se nelíbí mechanismům kódování kanálu, ty mají rády pokud je zarušen "sem tam" nějaký bit, ale ne shluk vedle sebe.
- Zabraňujeme tomu tak, že prohazujeme pořadí jednotlivých bitů.

### 4) Šifrování
- Šifrování dat
- Může být v různé fázi, není shoda
- Umístění šifrování tak jak ve schématu je problém, jelikož se mohou objevit chyby díky přenosu.
- Což se většine šifrovacích algoritmů nelíbí, proto by dávalo smysl to umístit až za "(De)kódování kanálu".

### 5) Modulace
- Dostat náš signál do analogové podoby pro přenos a na potřebnou frekvenci
    - Úprava frekvencí, amplitud, fází
- Také typicky prováděn multiplexing - více signálů přes jednu anténu
- (Konkrétně bylo řešeno dříve)

### 6) Přenos signálu přes telekomunikační síť

### 7) Demodulace
- Dostat signál z podoby vhodné pro přenos na podobu vhodnou pro dekódování
    - Úprava frekvencí, amplitud, fází
- Demultiplexing
- (Konkrétně bylo řešeno dříve)

### 8) Dešifrování
- Dešifrování, platí to stejné jako u šifrování

### 9) Prokládání (Deinterleaving)
- Překládání bitů analogicky k prokládání na druhé straně

### 10) Dekódování kanálu
- Opravení chyb

### 11) Dekódování hlasu
- Pokud chceme řeč, tak převod do analogové podoby

<!------------------------------------------------------------->

## Půlsemestrálka

### 2019/2020 — Skupina A

#### 1) Popsat vertikální a horizontální handover.
- ...

#### 2) Co je to multiplexing? Popsat kódový multiplex.
- Multiplexování je proces, ve kterém je více analogových signálů nebo digitálních datových toků kombinováno do jednoho signálu.
- Cílem je co možná nejefektivnější využití daného přenosového média.
- Zařízení provádějící multiplexování se nazývá multiplexor a zařízení provádějící převod signálu zpět na jednotlivé signály se nazývá demultiplexor.
- **Kódový multiplex**
    - Metoda digitálního multiplexování, tzn. přenosu vícero digitálních signálů prostřednictvím jediného sdíleného média, která jednotlivé signály rozlišuje tím, že každé z nich používá odlišné (vhodně navržené) kódování.
    - Všechny kanály používají totéž pásmo současně.
    - Implementováno pomocí technologií s rozprostřeným spektrem (spread spectrum)
    - Výhody:
        - Efektivní využití pásma
        - Není nutná koordinace a synchronizace
        - Lepší ochrana proti interferenci a odposlouchávání
    - Nevýhody:
        - Nižší rychlost přenosu
        - Složitější regenerace signálu

#### 3) Co je PSK za modulaci? Nakreslit obrázek pro BPSK.
- Jedná se o klíčování fázovým posuvem (Phase Shift Keying)
    - Posunování sinusovky (sin(x+c))
- Složitější, odolnější proti rušení
- Např. Binary Phase Shift Keying (BPSK)
    - Bit 0: sinusovka
    - Bit 1: invertovaná sinusovka
- Jednoduché, špatné využití pásma, robustní (odolné proti rušení)

#### 4) Co je to zlatá sekcenve? Jak se vyrobí?
- ...

#### 5) Jak funguje polling?
- ...

#### 6) LSFR - zadaná rovnice a IV, napsat první byte.
- viz sesit

#### 7) Hammingův kód (7,4) - je v kódu nějaká chyba? Případně opravit.
- viz sesit

<!------------------------------------------------------------->

### 2018/2019 — Skupina A

#### 1) Co je soft/hard handover? Kdo to iniciuje?
- ...

#### 2) Co je to simplexní provoz?
- V jednom okamžiku přenos pouze jedním směrem
- Je třeba přepínat mezi příjmem a vysíláním
- Komunikace na jedné frekvenci

#### 3) Co je to interleaving?
- Jedna z fází přenosu hlasu přes rádiové/trunkové sítě
- Pokud se na kanálu objevuje nějaké rušení, tak je většinou impulsní a zaruší několik bitů vedle sebe.
    - To se nelíbí mechanismům kódování kanálu, ty mají rády pokud je zarušen "sem tam" nějaký bit, ale ne shluk vedle sebe.
- Zabraňujeme tomu tak, že prohazujeme pořadí jednotlivých bitů.

#### 4) Co je to MACA a co to řeší?
- ...

#### 5) Co je to hierarchické modulace?
- Pro případ, že se nemůžeme adaptivně přepínat mezi jednotlivými X-QAM, protože máme pouze jednosměrný kanál
- Motivace: zákazníkům s kvalitním signálem chceme vysílat velkou rychlostí, pro zákazníky s nekvalitním signálem malou rychlostí
- Stream s vysokou prioritou (High Priority - HP) je zabudován ve streamu s nízkou prioritou (Low Priority - LP)
- Např.: máme 64 QAM
    - Účastník s kvalitním signálem dekóduje vše
    - Účastník s nekvalitním signálem dekóduje 16 bitů jako jeden (dostává QPSK - pomalejší, ale robustnější signál)


#### 6) Co je to QPSK? Máme bitovou rychlost 9600 bps, jaká bude baudová rychlost?
- Kvadraturní klíčování fázovým posuvem (Quadrature phase-shift keying — QPSK, 4PSK)
- Baudová rychlost bude = 9600 / 2 = 4800

#### 7) CDMA - doplnit tabulku, konkrétně tři poslední řádky, tedy průběh vynásobeného signálu, výstup integrátoru a výsledný signál.
- ...

<!------------------------------------------------------------->

### 2018/2019 — Skupina B

#### 1) Popsat diverzitu antén.
- Seskupení 2 nebo více antén
- Způsoby diverzity
    - Přepínaná diverzita, selektivní diverzita
        - Přijímač si vybere anténu s nejlepším signálem
    - Kombinovaná diverzita
        - Kombinuje výstupy pro zvýšení zisku
        - Je nutné správné sfázování

#### 2) Co je to semiduplex?
- Obousměrný provoz při kterém jedna stanice (základna) pracuje v duplexním režimu a druhá (pohybová) v dusimplexním.
- Provoz probíhá na dvou kmitočtech.

#### 3. Popsat problém exposed terminálu?
- Mějme dva příjemce C a D
- A vysílá pro C, C je mimo dosah B
- B chce vysílat pro D, D je mimo dosah A
- B ví, že A vysílá a myslí si, že nemůže vysílat, protože by zarušil jeho klienta C - ačkoliv by se tak nestalo

#### 4. Popsat PCM modulaci.
- Pulzně kódová modulace (Pulse-code modulation)
- Algoritmus pro převod analogového hlasu do digitální podoby.
- Signál se navzorkuje a výška se zakóduje do binárního kódu
- Kolik vzorků, tolik stavů potřebuju

#### 5. Co je to PRMA?
- ...

#### 6. Mějme 16-QAM, zadaná bitová rychlost je 9600 bps, jaká je baudová rychlost?
- Kvadratická amplitudová modulace - 16 stavů pomocí aplitudové a fázové modulace.
    - 4 bity v jednom stavu - jedné signálové jednotce (vlna?).
- 9600 / 4 = 2400

#### 7. CDMA - doplnit tabulku, konkrétně tři poslední řádky, tedy průběh vynásobeného signálu, výstup integrátoru a výsledný signál.
- ...

<!------------------------------------------------------------->

### 2017/2018 — Skupina A

#### 1) Co je to izotropní zářič, nakreslit vyzařovací charakteristiky dipól a sektorovou anténu.
- Izotropní zářič — teoretická referenční anténa, která vyzařuje do všech směrů stejně

#### 2) Co je to multiplexing? Vysvětlit frekvenční multiplex.
- Multiplexování je proces, ve kterém je více analogových signálů nebo digitálních datových toků kombinováno do jednoho signálu.
- Cílem je co možná nejefektivnější využití daného přenosového média.
- Zařízení provádějící multiplexování se nazývá multiplexor a zařízení provádějící převod signálu zpět na jednotlivé signály se nazývá demultiplexor.
- **Frekvenční multiplex**
    - Princip přenosu více signálů jedním společným širokopásmovým přenosovým médiem, při kterém každý z přenášených signálů používá jinou část kmitočtového pásma.
    - Výhody:
        - Není třeba žádná dynamická koordinace
        - Funguje pro analogové i digitální sítě
    - Nevýhody:
        - Plýtvání pásmem pokud je provoz rozložen nerovnoměrně
        - Potřebné ochranné intervaly

#### 3) Bylo nakresleno 8-PSK, spočítat rychlost přenosu při 2000 baudech.
- 8 stavů - 3 bity v jedné signálové jednotce
- 2000 * 3 = 6000 bit/s

#### 4) Co je to slotted aloha a jaké jsou využití?
- ...

#### 5) Nakreslit LFSR x^5 + x^4 + x^3 + x + 1, spočítat první byte.
- viz sesit

#### 6) Co je to Hammingova vzdálenost?
- Počet rozdílných bitů ve dvou bitových posloupnostech

#### 7) Co je to PCM?
- Pulzně kódová modulace (Pulse-code modulation)
- Algoritmus pro převod analogového hlasu do digitální podoby.
- Signál se navzorkuje a výška se zakóduje do binárního kódu
- Kolik vzorků, tolik stavů potřebuju

#### 8) Popsat navázání hovoru v GSM.
- ...

<!------------------------------------------------------------->

### 2017/2018 — Skupina B
1. co je to vyzařovací charakteristika a nakreslit pro izotropní zářič a sektorovou anténu
2. co je to multiplex a vysvětlit kódový multiplex
3. bylo nakresleno 16-QAM, spočítat rychlost přenosu při 500 baudech
4. lze použít CSMA/CD v bezdrátových sítích a proč?
5. nakreslit LFSR x^5 + x^4 + x^2 + x + 1, spočítat první byte
6. co je to prokládání a k čemu slouží?
7. doplnit/spočítat kontrolní bity hammingová kódu (12,8)
8. popsat architekturu systému GSM


<!------------------------------------------------------------->

### 2016/2017 — Skupina A
1. Vysvětlit pojmy: Diverzita antén, Dusimplexní přenos, TDMA, PSK
2. Popsat princip explicitních rezervačních algoritmů DAMA
3. Dekódování CDMA - doplnit tabulku
4. Popsat navázání hovoru v trunkových sítích
5. Co je typické pro geostacionární satelity, napsat komunikační systém, který je využívá
6. Dekódovat zprávu v Hammingově kódu (7,4) (matice P byla zadána)
7. GPS - jake jsou vstupy, co je potřeba, kolik satelitu je třeba pro fix, co je P-kód


ToDo:
- LSFR
- Dekodovani CDMA
