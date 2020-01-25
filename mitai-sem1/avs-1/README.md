# VUT FIT AVS 2019

## Projekt č. 1: Optimalizace sekvenčního kódu

<!---------------------------------------------------------------------->

### Přihlášení na superpočítač Anselm

- Na papírku jsme dostali přihlašovací údaje pro přístup na superpočítač Anselm / Salamon
    - Login ve tvaru `dd-xx-xx-xx` a heslo
- Je třeba stáhnout ssh klíč z [scs.it4i.cz](https://scs.it4i.cz/) přes přihlašovací údaje
- Privátní klíč `id_rsa-dd-19-32-17` přesunout do adresáře `~/.ssh`

Nastavení přístupových práv
```
$ chmod 600 ~/.ssh/id_rsa-dd-19-32-17
```

Připojení na Anselm
```
$ ssh -i ~/.ssh/id_rsa-dd-19-32-17 dd-19-32-17@anselm.it4i.cz
```

Připojení na Anselm přes nově vygenerovaný ssh
```
$ ssh -i ~/.ssh/id_rsa-anselm dd-19-32-17@anselm.it4i.cz
```

<!---------------------------------------------------------------------->

### Nahrání souborů na Anselm

#### sshfs

Mount Anselm home andresář lokálně přes `sshfs`
```
$ sshfs dd-19-32-17@anselm.it4i.cz:/home/training/dd-19-32-17/ /home/vdusek/anselm/ -o IdentityFile=/home/vdusek/.ssh/id_rsa-anselm
```

#### scp

Zazipování projektu z adresáře projektu
```
$ zip -r proj.zip *
```

Nahrání projektu na superpočítač přes `scp`
```
$ scp -i ~/.ssh/id_rsa-dd-19-32-17 proj.zip dd-19-32-17@anselm.it4i.cz:~/
```

<!---------------------------------------------------------------------->

### Výpočetní uzly a moduly

Pomocí utility `qsub` je možno zažádat o výpočetní výkon.
Parametr `-l` specifikuje zdroje, použít výchozí kvůli zkreslení.
Parametr `-I` značí interaktivní úlohu.
Více na [docs.it4i.cz/general/job-submission-and-execution](https://docs.it4i.cz/general/job-submission-and-execution/)
```
$ qsub -A DD-19-32 -q qexp -l select=1:ncpus=16,walltime=1:00:00 -I
```

Zjištění dostupnosti konkrétního modulu
```
$ module avail "module_name"
```

Nahrání potřebných modulů (`intel`, `PAPI`, `HDF5`, `CMake`, `Python/3.6.1`)
```
$ module load intel PAPI HDF5 CMake Python/3.6.1
```

<!---------------------------------------------------------------------->

### Přeložení a spuštění projektu

Ideálně z adresáře `build/`
```
$ mkdir build/ ; cd build/
```

Cmake (nastavit s/bez papi a jaky kroky prekladat)
```
$ cmake ../ -DCMAKE_BUILD_TYPE=Release -DWITH_PAPI=1 -DSTEPS='0;1'
```

Make
```
$ make -j
```

Spuštění projektu
```
$ ./Step0/ANN ../Data/network.h5 ../Data/testData.h5 ../Data/myOutput.h5
```

Spuštění skriptu na porovnání výstupů
```
$ python3 ../Scripts/compareOutputs.py ../Data/myOutput.h5 ../Data/testRefOutput.h5
```

Spuštění projektu s PAPI_EVENTS
```
$ PAPI_EVENTS="PAPI_EVENT1|PAPI_EVENT2" \
./Step0/ANN ../Data/network.h5 ../Data/testData.h5 ../Data/myOutput.h5
```

<!---------------------------------------------------------------------->

### Step 0: Základní implementace

#### a) Doplnit základní implementaci pro výpočet neuronu

- Výpočet neuronu

#### b) Změřit výkon základní implementace

- Zjištění doby běhu programu je triviální, stačí přeložit s papi
a spustit.

- Změření výkonu skalárních a vektorových operací, papi eventy:
`PAPI_FP_OPS`, `PAPI_VEC_SP`, `PAPI_DP_OPS`.
```
$ PAPI_EVENTS="PAPI_FP_OPS|PAPI_SP_OPS|PAPI_DP_OPS" \
./Step0/ANN ../Data/network.h5 ../Data/testData.h5 ../Data/myOutput.h5
```

- Pro zjištění procentuálního výpadků cache bude nutné zjistit
počet přístupů do cache (cache access) a poté počet výpadků
(cache miss). Podle dodatečného doplnění na fóru, se kvůli nedostupnosti
některých papi eventů máme zaměřit u L1 a L2 pouze na datovou cache
(`PAPI_L1_DCM`, `PAPI_L1_DCA`, `PAPI_L2_DCM`, `PAPI_L2_DCA`).
U L3 na celkové přístupy a výpadky (`PAPI_L3_TCM`, `PAPI_L3_TCA`).
U L1 ale není dostupný ani event `PAPI_L1_DCA`, číslo by ale mělo
odpovídat počtu `PAPI_LD_INS` + `PAPI_SR_INS`.
```
$ PAPI_EVENTS="PAPI_L1_DCM|PAPI_LD_INS|PAPI_SR_INS|PAPI_L2_DCM|PAPI_L2_DCA|PAPI_L3_TCM|PAPI_L3_TCA" \
./Step0/ANN ../Data/network.h5 ../Data/bigDataset.h5 ../Data/myOutput.h5
```

- Měření cache misses a výkonu skalárních/vektorových operací je třeba
provádět odděleně kvůli omezenému počtu registrů.

- Pro účely měření použít hodnoty ze sloupce s nejvyšší hodnotou
který není TOTAL.

```
$ make step0-cache
--------------------------------
network :: wall time 193.295 s
--------------------------------
     THREAD 0     THREAD 1     THREAD 2     THREAD 3     THREAD 4     THREAD 5     THREAD 6     THREAD 7     THREAD 8     THREAD 9     THREAD10     THREAD11     THREAD12     THREAD13     THREAD14     THREAD15 [        TOTAL ]
 106564733775        65688        17269          316        35004        22308        18661        69506          626        27127        21240        63226          415        50625        67259        55494 [ 106565248539 ]	PAPI_L1_DCM
   1655457701      2718999      6574839      9541234      9458039      7694331      7616219      2702860      9485473      4699302      8953257      3137115      9485622      1868638      3059262      1899930 [   1744352821 ]	PAPI_SR_INS
 123475719961     49160828    118451666    171825416    170346141    138630710    137248422     48889783    170816948     84749034    161260887     56662632    170850246     33873938     55272523     34444745 [ 125078203880 ]	PAPI_LD_INS
 106564733775        65688        17269          316        35004        22308        18661        69506          626        27127        21240        63226          415        50625        67259        55494 [ 106565248539 ]	PAPI_L2_DCA
 106515672245          233          381          267          296          342          372          309          327          280          239          320          328          294          219          171 [ 106515676623 ]	PAPI_L2_DCM
 106515862407          533          786          589          625          705          726          672          698          585          546          659          696          626          411          358 [ 106515871622 ]	PAPI_L3_TCA
      7796204           37          253          104           85           53          113           42          226           41           41           57           66           36           43           34 [      7797435 ]	PAPI_L3_TCM
        86.3%       0.134%      0.0146%    0.000184%      0.0205%      0.0161%      0.0136%       0.142%    0.000366%       0.032%      0.0132%       0.112%    0.000243%       0.149%       0.122%       0.161% [            - ]	derived_L1_DMR (%)
         100%       0.355%        2.21%        84.5%       0.846%        1.53%        1.99%       0.445%        52.2%        1.03%        1.13%       0.506%          79%       0.581%       0.326%       0.308% [            - ]	derived_L2_DMR (%)
     0.00732%        6.94%        32.2%        17.7%        13.6%        7.52%        15.6%        6.25%        32.4%        7.01%        7.51%        8.65%        9.48%        5.75%        10.5%         9.5% [            - ]	derived_L3_TMR (%)
      2.46MB/s      2.46MB/s      2.46MB/s      2.46MB/s      2.46MB/s      2.46MB/s      2.46MB/s      2.46MB/s      2.46MB/s      2.46MB/s      2.46MB/s      2.46MB/s      2.46MB/s      2.46MB/s      2.46MB/s      2.46MB/s [            - ]	derived_Mem_Bandwidth [MB/s]
```

```
$ make step0-perform
--------------------------------
network :: wall time 193.543 s
--------------------------------
     THREAD 0     THREAD 1     THREAD 2     THREAD 3     THREAD 4     THREAD 5     THREAD 6     THREAD 7     THREAD 8     THREAD 9     THREAD10     THREAD11     THREAD12     THREAD13     THREAD14     THREAD15 [        TOTAL ]
   7171490063       313430        77910       314342        78003       332980        78300        77895       191650        77996       176323        77856        97481       249814        78226       259065 [   7173971334 ]	PAPI_FP_OPS
 221222816394            0            0            0            0            0            0            0            0            0            0            0            0            0            0            0 [ 221222816394 ]	PAPI_SP_OPS
            4            4            4            4            3            4            4            4            4            5            4            4            4            4            4            4 [           64 ]	PAPI_DP_OPS
 214051360396            0            0            0            0            0            0            0            0            0            0            0            0            0            0            0 [ 214051360396 ]	PAPI_VEC_SP
            0            0            0            0            0            0            0            0            0            0            0            0            0            0            0            0 [            0 ]	PAPI_VEC_DP
      37.0538   0.00161944  0.000402547   0.00162415  0.000403027   0.00172045  0.000404562  0.000402469  0.000990221  0.000402991  0.000911029  0.000402268  0.000503667   0.00129074   0.00040418   0.00133854 [      37.0666 ]	derived_FLOPS (MFLOPS)
  2.06673e-08  2.06673e-08  2.06673e-08  2.06673e-08  1.55005e-08  2.06673e-08  2.06673e-08  2.06673e-08  2.06673e-08  2.58341e-08  2.06673e-08  2.06673e-08  2.06673e-08  2.06673e-08  2.06673e-08  2.06673e-08 [  3.30676e-07 ]	derived_DP_vector_FLOPS (MFLOPS)
      1143.02            0            0            0            0            0            0            0            0            0            0            0            0            0            0            0 [      1143.02 ]	derived_SP_vector_FLOPS (MFLOPS)
```

<!---------------------------------------------------------------------->

### Step 1: Vektorizace funkcí

#### a) Vynutit vektorizaci pomocí vhodných pragma

- Vynutit vektorizaci cyklu ve funkci `evaluateLayer` pomocí `#pragma omp simd`
- Přeložit skalární i vektorovou verzi funkce `evalNeuron` pomocí `#pragma omp declare simd`
- Přidat doporučované dovětky `simdlen`, `uniform`, `linear:1`

#### b) Změřit výkon vektorizované implementace

```
$ make step1-cache
--------------------------------
network :: wall time 89.9357 s
--------------------------------
     THREAD 0     THREAD 1     THREAD 2     THREAD 3     THREAD 4     THREAD 5     THREAD 6     THREAD 7     THREAD 8     THREAD 9     THREAD10     THREAD11     THREAD12     THREAD13     THREAD14     THREAD15 [        TOTAL ]
  33112101967        50944          536        50905        65171        67125          640        66175        78221          532        41615          631        14030        38759          613        62961 [  33112640825 ]	PAPI_L1_DCM
    129028408      2840285      8992348      3814997      2973548      3808598      9550562      2352170      2789209      8958587      2797808      8967864      8257071      7244195      9474318      2833889 [    214683857 ]	PAPI_SR_INS
 120624934843     51395970    161934526     68905063     53781315     68792397    171985426     42611965     50451241    161369905     50644320    161492205    148756606    130538439    170672635     51274936 [ 122169541792 ]	PAPI_LD_INS
  33112101967        50944          536        50905        65171        67125          640        66175        78221          532        41615          631        14030        38759          613        62961 [  33112640825 ]	PAPI_L2_DCA
  32998807365          287          449          567          502          317          560          428          550          412          547          535          662          491          499          443 [  32998814614 ]	PAPI_L2_DCM
  32999397882          522          815         1001          844          559          914          704         1016          729          953          931         1118          867          897          828 [  32999410580 ]	PAPI_L3_TCA
      7597465           49          231          153           96           51          339           88          117           63           89          139          139          141           89           65 [      7599314 ]	PAPI_L3_TCM
        27.5%      0.0991%    0.000331%      0.0739%       0.121%      0.0976%    0.000372%       0.155%       0.155%     0.00033%      0.0822%    0.000391%     0.00943%      0.0297%    0.000359%       0.123% [            - ]	derived_L1_DMR (%)
        99.7%       0.563%        83.8%        1.11%        0.77%       0.472%        87.5%       0.647%       0.703%        77.4%        1.31%        84.8%        4.72%        1.27%        81.4%       0.704% [            - ]	derived_L2_DMR (%)
       0.023%        9.39%        28.3%        15.3%        11.4%        9.12%        37.1%        12.5%        11.5%        8.64%        9.34%        14.9%        12.4%        16.3%        9.92%        7.85% [            - ]	derived_L3_TMR (%)
      5.16MB/s      5.16MB/s      5.16MB/s      5.16MB/s      5.16MB/s      5.16MB/s      5.16MB/s      5.16MB/s      5.16MB/s      5.16MB/s      5.16MB/s      5.16MB/s      5.16MB/s      5.16MB/s      5.16MB/s      5.16MB/s [            - ]	derived_Mem_Bandwidth [MB/s]
```


```
$ make step1-perform
--------------------------------
network :: wall time 89.4248 s
--------------------------------
     THREAD 0     THREAD 1     THREAD 2     THREAD 3     THREAD 4     THREAD 5     THREAD 6     THREAD 7     THREAD 8     THREAD 9     THREAD10     THREAD11     THREAD12     THREAD13     THREAD14     THREAD15 [        TOTAL ]
     11295409       306517       199300       301672       334921       306511       132626       202483       102344        73150       306163       322634       199700        76175       231460        73892 [     14464957 ]	PAPI_FP_OPS
 214312129288            0            0            0            0            0            0            0            0            0            0            0            0            0            0            0 [ 214312129288 ]	PAPI_SP_OPS
            4            4            4            4            4            4            5            4            4            4            4            4            5            5            4            4 [           67 ]	PAPI_DP_OPS
 214300870156            0            0            0            0            0            0            0            0            0            0            0            0            0            0            0 [ 214300870156 ]	PAPI_VEC_SP
            0            0            0            0            0            0            0            0            0            0            0            0            0            0            0            0 [            0 ]	PAPI_VEC_DP
     0.126312   0.00342765   0.00222869   0.00337347   0.00374528   0.00342759    0.0014831   0.00226428   0.00114447  0.000818006   0.00342369   0.00360788   0.00223316  0.000851834   0.00258832  0.000826304 [     0.161756 ]	derived_FLOPS (MFLOPS)
  4.47303e-08  4.47303e-08  4.47303e-08  4.47303e-08  4.47303e-08  4.47303e-08  5.59129e-08  4.47303e-08  4.47303e-08  4.47303e-08  4.47303e-08  4.47303e-08  5.59129e-08  5.59129e-08  4.47303e-08  4.47303e-08 [  7.49233e-07 ]	derived_DP_vector_FLOPS (MFLOPS)
      2396.56            0            0            0            0            0            0            0            0            0            0            0            0            0            0            0 [      2396.56 ]	derived_SP_vector_FLOPS (MFLOPS)
```

<!---------------------------------------------------------------------->

### Step 2: Přístupy do paměti

#### a) Transponování matice s váhami a využití dalších pragma

- Využití funkce `transpose2D` pro transponování matice s uloženými váhami
- Modifikace volání a rozhraní funkce `evalNeuron` pro práci s transponovanými váhami

#### b) Změřit výkon s využitím transponovaných vah

##### 1) Všechny doporučované dovětky (linear, linear:512, linear:784)

```
$ make step2-cache
--------------------------------
network :: wall time 40.2308 s
--------------------------------
     THREAD 0     THREAD 1     THREAD 2     THREAD 3     THREAD 4     THREAD 5     THREAD 6     THREAD 7     THREAD 8     THREAD 9     THREAD10     THREAD11     THREAD12     THREAD13     THREAD14     THREAD15 [        TOTAL ]
   6954689649        50675        56558        39504        61814        73718          594        65341        72492        32590        13162          582          892        10434        60795        42707 [   6955271507 ]	PAPI_L1_DCM
     64518461      2796222      2841076      2803205      2995989      3115195      9647419      2340862      3808318      4822247      7611233      8960537      9616232      7144963      2834431      4945690 [    140802080 ]	PAPI_SR_INS
 120352763032     50568245     51356852     50686681     54200080     56316438    173730128     42401799     68783764     86982775    137134005    161365264    173168904    128738715     51249634     89208806 [ 121728655122 ]	PAPI_LD_INS
   6954689649        50675        56558        39504        61814        73718          594        65341        72492        32590        13162          582          892        10434        60795        42707 [   6955271507 ]	PAPI_L2_DCA
   1159766342          598          389          620          599          370          461          543          630          481          492          468          659          598          420          604 [   1159774274 ]	PAPI_L2_DCM
   1159800907         1025          693         1055         1015          676          857          795         1100          842          857          863         1101         1026          672         1018 [   1159814502 ]	PAPI_L3_TCA
       402280           65           67          219          166           70           55          202          167          238           62           38          183           66           55          113 [       404046 ]	PAPI_L3_TCM
        5.78%         0.1%        0.11%      0.0779%       0.114%       0.131%    0.000342%       0.154%       0.105%      0.0375%      0.0096%    0.000361%    0.000515%      0.0081%       0.119%      0.0479% [            - ]	derived_L1_DMR (%)
        16.7%        1.18%       0.688%        1.57%       0.969%       0.502%        77.6%       0.831%       0.869%        1.48%        3.74%        80.4%        73.9%        5.73%       0.691%        1.41% [            - ]	derived_L2_DMR (%)
      0.0347%        6.34%        9.67%        20.8%        16.4%        10.4%        6.42%        25.4%        15.2%        28.3%        7.23%         4.4%        16.6%        6.43%        8.18%        11.1% [            - ]	derived_L3_TMR (%)
     0.613MB/s     0.613MB/s     0.613MB/s     0.613MB/s     0.613MB/s     0.613MB/s     0.613MB/s     0.613MB/s     0.613MB/s     0.613MB/s     0.613MB/s     0.613MB/s     0.613MB/s     0.613MB/s     0.613MB/s     0.613MB/s [            - ]	derived_Mem_Bandwidth [MB/s]
```

```
$ make step2-perform
--------------------------------
network :: wall time 40.231 s
--------------------------------
     THREAD 0     THREAD 1     THREAD 2     THREAD 3     THREAD 4     THREAD 5     THREAD 6     THREAD 7     THREAD 8     THREAD 9     THREAD10     THREAD11     THREAD12     THREAD13     THREAD14     THREAD15 [        TOTAL ]
     15229142       312541       313132       312433       312793       313316       312935       213400       213404       311971       312934       312529       312574       313405       312775       313639 [     19722923 ]	PAPI_FP_OPS
 214474825857            0            0            0            0            0            0            0            0            0            0            0            0            0            0            0 [ 214474825857 ]	PAPI_SP_OPS
            4            4            4            4            4            4            4            4            5            4            4            4            4            4            4            4 [           65 ]	PAPI_DP_OPS
 214459613756            0            0            0            0            0            0            0            0            0            0            0            0            0            0            0 [ 214459613756 ]	PAPI_VEC_SP
            0            0            0            0            0            0            0            0            0            0            0            0            0            0            0            0 [            0 ]	PAPI_VEC_DP
     0.378542   0.00776865   0.00778334   0.00776597   0.00777492   0.00778792   0.00777845   0.00530436   0.00530446   0.00775449   0.00777842   0.00776836   0.00776947   0.00779013   0.00777447   0.00779595 [     0.490242 ]	derived_FLOPS (MFLOPS)
  9.94257e-08  9.94257e-08  9.94257e-08  9.94257e-08  9.94257e-08  9.94257e-08  9.94257e-08  9.94257e-08  1.24282e-07  9.94257e-08  9.94257e-08  9.94257e-08  9.94257e-08  9.94257e-08  9.94257e-08  9.94257e-08 [  1.61567e-06 ]	derived_DP_vector_FLOPS (MFLOPS)
      5331.08            0            0            0            0            0            0            0            0            0            0            0            0            0            0            0 [      5331.08 ]	derived_SP_vector_FLOPS (MFLOPS)
```

##### 2) Bez dovětků, které mají negativní vliv

```
$ make step2-cache
--------------------------------
network :: wall time 21.8035 s
--------------------------------
     THREAD 0     THREAD 1     THREAD 2     THREAD 3     THREAD 4     THREAD 5     THREAD 6     THREAD 7     THREAD 8     THREAD 9     THREAD10     THREAD11     THREAD12     THREAD13     THREAD14     THREAD15 [        TOTAL ]
   6791615944        21306        79894        44958          665          310        37633        38708          536        41785        42922        56933        39864        57460        64453        27837 [   6792171208 ]	PAPI_L1_DCM
    547853401      5880890      3155688      3178704     10104242      9680931      4061006      4014131     10059205      3162873      3166535      3177671      3177701      3170583      3160303      6255776 [    623259640 ]	PAPI_SR_INS
  43092787489    106019682     57021290     57443686    181971651    174336714     73288654     72428315    181159968     57090282     57246424     57426199     57363335     57297898     57094707    112780495 [  44452756789 ]	PAPI_LD_INS
   6791615944        21306        79894        44958          665          310        37633        38708          536        41785        42922        56933        39864        57460        64453        27837 [   6792171208 ]	PAPI_L2_DCA
   3528172562          375          324          258          324          254          344          258          411          250          271          263          350          221          222          320 [   3528177007 ]	PAPI_L2_DCM
   3528607826          744          671          551          675          564          692          559          804          494          522          558          689          434          442          659 [   3528616884 ]	PAPI_L3_TCA
      1995386           46           98          153           74           57          110           14           86           23           24           26           68           38           39          185 [      1996427 ]	PAPI_L3_TCM
        15.8%      0.0201%        0.14%      0.0783%    0.000365%    0.000178%      0.0513%      0.0534%    0.000296%      0.0732%       0.075%      0.0991%      0.0695%         0.1%       0.113%      0.0247% [            - ]	derived_L1_DMR (%)
        51.9%        1.76%       0.406%       0.574%        48.7%        81.9%       0.914%       0.667%        76.7%       0.598%       0.631%       0.462%       0.878%       0.385%       0.344%        1.15% [            - ]	derived_L2_DMR (%)
      0.0565%        6.18%        14.6%        27.8%          11%        10.1%        15.9%         2.5%        10.7%        4.66%         4.6%        4.66%        9.87%        8.76%        8.82%        28.1% [            - ]	derived_L3_TMR (%)
      5.59MB/s      5.59MB/s      5.59MB/s      5.59MB/s      5.59MB/s      5.59MB/s      5.59MB/s      5.59MB/s      5.59MB/s      5.59MB/s      5.59MB/s      5.59MB/s      5.59MB/s      5.59MB/s      5.59MB/s      5.59MB/s [            - ]	derived_Mem_Bandwidth [MB/s]
```

```
$ make step2-perform
--------------------------------
network :: wall time 21.7964 s
--------------------------------
     THREAD 0     THREAD 1     THREAD 2     THREAD 3     THREAD 4     THREAD 5     THREAD 6     THREAD 7     THREAD 8     THREAD 9     THREAD10     THREAD11     THREAD12     THREAD13     THREAD14     THREAD15 [        TOTAL ]
   3858426215       136838       307351       154705        94321        75605       238871       136934        92769       109363       237929       249950       323980        75545       108737       305875 [   3861074988 ]	PAPI_FP_OPS
 324524727374            0            0            0            0            0            0            0            0            0            0            0            0            0            0            0 [ 324524727374 ]	PAPI_SP_OPS
            6            5            4            4            4            5            5            5            6            4            4            5            4            5            5            4 [           75 ]	PAPI_DP_OPS
 320666323484            0            0            0            0            0            0            0            0            0            0            0            0            0            0            0 [ 320666323484 ]	PAPI_VEC_SP
            0            0            0            0            0            0            0            0            0            0            0            0            0            0            0            0 [            0 ]	PAPI_VEC_DP
      177.021   0.00627801     0.014101   0.00709773   0.00432737   0.00346869    0.0109592   0.00628241   0.00425616   0.00501748     0.010916    0.0114675    0.0148639   0.00346594   0.00498876    0.0140333 [      177.143 ]	derived_FLOPS (MFLOPS)
  2.75275e-07  2.29396e-07  1.83517e-07  1.83517e-07  1.83517e-07  2.29396e-07  2.29396e-07  2.29396e-07  2.75275e-07  1.83517e-07  1.83517e-07  2.29396e-07  1.83517e-07  2.29396e-07  2.29396e-07  1.83517e-07 [  3.44094e-06 ]	derived_DP_vector_FLOPS (MFLOPS)
      14888.9            0            0            0            0            0            0            0            0            0            0            0            0            0            0            0 [      14888.9 ]	derived_SP_vector_FLOPS (MFLOPS)
```

<!---------------------------------------------------------------------->

### Step 3: Správná smyčka a cache

#### 3.1) Přesun #pragma omp simd

- Aplikace vektorizace na nejvnitřnější smyčku
- Přidání příslušného ošetřujícího pragma dovětku

```
$ make step3-cache
--------------------------------
network :: wall time 21.793 s
--------------------------------
     THREAD 0     THREAD 1     THREAD 2     THREAD 3     THREAD 4     THREAD 5     THREAD 6     THREAD 7     THREAD 8     THREAD 9     THREAD10     THREAD11     THREAD12     THREAD13     THREAD14     THREAD15 [        TOTAL ]
   6741218083          765          568          587        61613        44170          671        37710        64761        21242        36645        53511        49675        15198        79058        55441 [   6741739698 ]	PAPI_L1_DCM
    332315242      8998776      9000117      8997491      3116963      2904559      8991291      2796636      2312493      3753978      1569348      1573275      3104019      6087373      2355326      2351985 [    400228872 ]	PAPI_SR_INS
  42809860754    162048613    162072203    162024555     56311447     52454382    161914380     50484665     41854741     67703328     28478775     28534102     56126831    109683363     42639900     42558094 [  44034750133 ]	PAPI_LD_INS
   6741218083          765          568          587        61613        44170          671        37710        64761        21242        36645        53511        49675        15198        79058        55441 [   6741739698 ]	PAPI_L2_DCA
   3620238420          477          475          486          536          468          353          545          512          490          480          351          576          461          372          357 [   3620245359 ]	PAPI_L2_DCM
   3620277634          862          871          897          939          804          601         1003          868          798          803          555          969          804          662          652 [   3620289722 ]	PAPI_L3_TCA
      2712725           32           62          163          162           53           77          199           71           98           73           45          310          280           60           55 [      2714465 ]	PAPI_L3_TCM
        15.7%    0.000472%     0.00035%    0.000362%       0.109%      0.0842%    0.000414%      0.0747%       0.155%      0.0314%       0.129%       0.188%      0.0885%      0.0139%       0.185%        0.13% [            - ]	derived_L1_DMR (%)
        53.7%        62.4%        83.6%        82.8%        0.87%        1.06%        52.6%        1.45%       0.791%        2.31%        1.31%       0.656%        1.16%        3.03%       0.471%       0.644% [            - ]	derived_L2_DMR (%)
      0.0749%        3.71%        7.12%        18.2%        17.3%        6.59%        12.8%        19.8%        8.18%        12.3%        9.09%        8.11%          32%        34.8%        9.06%        8.44% [            - ]	derived_L3_TMR (%)
       7.6MB/s       7.6MB/s       7.6MB/s       7.6MB/s       7.6MB/s       7.6MB/s       7.6MB/s       7.6MB/s       7.6MB/s       7.6MB/s       7.6MB/s       7.6MB/s       7.6MB/s       7.6MB/s       7.6MB/s       7.6MB/s [            - ]	derived_Mem_Bandwidth [MB/s]
```

```
$ make step3-perform
--------------------------------
network :: wall time 21.6816 s
--------------------------------
     THREAD 0     THREAD 1     THREAD 2     THREAD 3     THREAD 4     THREAD 5     THREAD 6     THREAD 7     THREAD 8     THREAD 9     THREAD10     THREAD11     THREAD12     THREAD13     THREAD14     THREAD15 [        TOTAL ]
   3886396701       124679       332166       124717       335350       139817       208507       172258       240400       209672        77557        77958       336109       315593       140149       315420 [   3889547053 ]	PAPI_FP_OPS
 325001721467            0            0            0            0            0            0            0            0            0            0            0            0            0            0            0 [ 325001721467 ]	PAPI_SP_OPS
            6            4            4            4            4            5            4            4            4            5            4            5            4            4            5            6 [           72 ]	PAPI_DP_OPS
 321115350600            0            0            0            0            0            0            0            0            0            0            0            0            0            0            0 [ 321115350600 ]	PAPI_VEC_SP
            0            0            0            0            0            0            0            0            0            0            0            0            0            0            0            0 [            0 ]	PAPI_VEC_DP
      179.249   0.00575046    0.0153202   0.00575221     0.015467   0.00644865   0.00961678    0.0079449    0.0110878   0.00967051   0.00357709   0.00359559    0.0155021    0.0145558   0.00646397    0.0145478 [      179.394 ]	derived_FLOPS (MFLOPS)
  2.76733e-07  1.84488e-07  1.84488e-07  1.84488e-07  1.84488e-07   2.3061e-07  1.84488e-07  1.84488e-07  1.84488e-07   2.3061e-07  1.84488e-07   2.3061e-07  1.84488e-07  1.84488e-07   2.3061e-07  2.76733e-07 [  3.32079e-06 ]	derived_DP_vector_FLOPS (MFLOPS)
      14989.8            0            0            0            0            0            0            0            0            0            0            0            0            0            0            0 [      14989.8 ]	derived_SP_vector_FLOPS (MFLOPS)
```

#### 3.2) Vykonání po vrstvách místo po obrázcích

- Pro všechny obrázky se nejprve vyhodnotí první vrstva, poté druhá a nakonec třetí
- Mohlo by mít kladný dopad na počet výpadku cache

```
$ make step3-cache
--------------------------------
network :: wall time 22.3952 s
--------------------------------
     THREAD 0     THREAD 1     THREAD 2     THREAD 3     THREAD 4     THREAD 5     THREAD 6     THREAD 7     THREAD 8     THREAD 9     THREAD10     THREAD11     THREAD12     THREAD13     THREAD14     THREAD15 [        TOTAL ]
   6665321122          526          378          435        40333        49757        49324        32194        79168        10315        52609        57059        24673        46154        47545        40744 [   6665852336 ]	PAPI_L1_DCM
    333377782      9195569      9199481      9185835      4262310      3898040      2840128      3884497      4266798      8030688      2844623      3423471      6084767      3278245      3467550      3225067 [    410464851 ]	PAPI_SR_INS
  44994569659    165596229    165666488    165421756     76942539     70335547     51339841     70086292     77085937    144668198     51438673     61866006    109679575     59239229     62635160     58270612 [  46384841741 ]	PAPI_LD_INS
   6665321122          526          378          435        40333        49757        49324        32194        79168        10315        52609        57059        24673        46154        47545        40744 [   6665852336 ]	PAPI_L2_DCA
   3537154338          271          278          323          420          236          196          405          371          276          240          294          357          165          235          239 [   3537158644 ]	PAPI_L2_DCM
   3537157925          607          595          664          810          493          406          783          734          599          466          640          738          363          506          460 [   3537166789 ]	PAPI_L3_TCA
      7128442           39           97          180           57           53           36          157          160           63           38           38          115           27           43           35 [      7129580 ]	PAPI_L3_TCM
        14.8%    0.000318%    0.000228%    0.000263%      0.0524%      0.0707%      0.0961%      0.0459%       0.103%     0.00713%       0.102%      0.0922%      0.0225%      0.0779%      0.0759%      0.0699% [            - ]	derived_L1_DMR (%)
        53.1%        51.5%        73.5%        74.3%        1.04%       0.474%       0.397%        1.26%       0.469%        2.68%       0.456%       0.515%        1.45%       0.357%       0.494%       0.587% [            - ]	derived_L2_DMR (%)
       0.202%        6.43%        16.3%        27.1%        7.04%        10.8%        8.87%        20.1%        21.8%        10.5%        8.15%        5.94%        15.6%        7.44%         8.5%        7.61% [            - ]	derived_L3_TMR (%)
      19.4MB/s      19.4MB/s      19.4MB/s      19.4MB/s      19.4MB/s      19.4MB/s      19.4MB/s      19.4MB/s      19.4MB/s      19.4MB/s      19.4MB/s      19.4MB/s      19.4MB/s      19.4MB/s      19.4MB/s      19.4MB/s [            - ]	derived_Mem_Bandwidth [MB/s]
```

```
$ make step3-perform
--------------------------------
network :: wall time 22.2426 s
--------------------------------
     THREAD 0     THREAD 1     THREAD 2     THREAD 3     THREAD 4     THREAD 5     THREAD 6     THREAD 7     THREAD 8     THREAD 9     THREAD10     THREAD11     THREAD12     THREAD13     THREAD14     THREAD15 [        TOTAL ]
   7093318526        77629        77782       329268       345604       329345        77655       229156       329668        77602       236020        78018       329896       314869        77737       314362 [   7096543137 ]	PAPI_FP_OPS
 325665507970            0            0            0            0            0            0            0            0            0            0            0            0            0            0            0 [ 325665507970 ]	PAPI_SP_OPS
            4            4            4            4            4            4            4            4            4            4            4            3            4            4            4            4 [           63 ]	PAPI_DP_OPS
 318572208212            0            0            0            0            0            0            0            0            0            0            0            0            0            0            0 [ 318572208212 ]	PAPI_VEC_SP
            0            0            0            0            0            0            0            0            0            0            0            0            0            0            0            0 [            0 ]	PAPI_VEC_DP
      318.907   0.00349011   0.00349699    0.0148035     0.015538     0.014807   0.00349128    0.0103026    0.0148215    0.0034889    0.0106112    0.0035076    0.0148317    0.0141562   0.00349497    0.0141334 [      319.052 ]	derived_FLOPS (MFLOPS)
  1.79835e-07  1.79835e-07  1.79835e-07  1.79835e-07  1.79835e-07  1.79835e-07  1.79835e-07  1.79835e-07  1.79835e-07  1.79835e-07  1.79835e-07  1.34877e-07  1.79835e-07  1.79835e-07  1.79835e-07  1.79835e-07 [  2.83241e-06 ]	derived_DP_vector_FLOPS (MFLOPS)
      14641.5            0            0            0            0            0            0            0            0            0            0            0            0            0            0            0 [      14641.5 ]	derived_SP_vector_FLOPS (MFLOPS)
```

<!---------------------------------------------------------------------->

### Step 4: (Ne)volání funkce a zarovnání paměti

- Zarovnání alokace
- Nevolání funkce

```
$ make step4-cache
--------------------------------
network :: wall time 20.3813 s
--------------------------------
     THREAD 0     THREAD 1     THREAD 2     THREAD 3     THREAD 4     THREAD 5     THREAD 6     THREAD 7     THREAD 8     THREAD 9     THREAD10     THREAD11     THREAD12     THREAD13     THREAD14     THREAD15 [        TOTAL ]
   6666324010         7441          429        39707          543          323          407          642        39865          287         6995         7882        13840        32998        49996         6211 [   6666531576 ]	PAPI_L1_DCM
    169195661      8004769      8991608      2354186      8977899      8984694      8981009      8988469      3048295      8982775      8080996      8126588      7045717      4340211      4299858      8099681 [    276502416 ]	PAPI_SR_INS
  40307593820    144171003    161924399     42626314    161684358    161797353    161732858    161865740     55126332    161763664    145543956    146394113    126938150     78291769     77589360    145879599 [  42240922788 ]	PAPI_LD_INS
   6666324010         7441          429        39707          543          323          407          642        39865          287         6995         7882        13840        32998        49996         6211 [   6666531576 ]	PAPI_L2_DCA
   3865430484          384          281          258          343          262          285          297          400          196          473          433          379          263          343          303 [   3865435384 ]	PAPI_L2_DCM
   3865484269          724          591          547          710          579          615          646          763          378          804          775          738          553          659          575 [   3865493926 ]	PAPI_L3_TCA
      5884716          177          102          218           82           33           30           39          105           37           74           58          117           53           51           43 [      5885935 ]	PAPI_L3_TCM
        16.5%     0.00516%    0.000265%      0.0932%    0.000336%      0.0002%    0.000252%    0.000397%      0.0723%    0.000177%     0.00481%     0.00538%      0.0109%      0.0421%      0.0644%     0.00426% [            - ]	derived_L1_DMR (%)
          58%        5.16%        65.5%        0.65%        63.2%        81.1%          70%        46.3%           1%        68.3%        6.76%        5.49%        2.74%       0.797%       0.686%        4.88% [            - ]	derived_L2_DMR (%)
       0.152%        24.4%        17.3%        39.9%        11.5%         5.7%        4.88%        6.04%        13.8%        9.79%         9.2%        7.48%        15.9%        9.58%        7.74%        7.48% [            - ]	derived_L3_TMR (%)
      17.6MB/s      17.6MB/s      17.6MB/s      17.6MB/s      17.6MB/s      17.6MB/s      17.6MB/s      17.6MB/s      17.6MB/s      17.6MB/s      17.6MB/s      17.6MB/s      17.6MB/s      17.6MB/s      17.6MB/s      17.6MB/s [            - ]	derived_Mem_Bandwidth [MB/s]
```

```
$ make step4-perform
--------------------------------
network :: wall time 20.6014 s
--------------------------------
     THREAD 0     THREAD 1     THREAD 2     THREAD 3     THREAD 4     THREAD 5     THREAD 6     THREAD 7     THREAD 8     THREAD 9     THREAD10     THREAD11     THREAD12     THREAD13     THREAD14     THREAD15 [        TOTAL ]
    496364127       314554       314698       247567       314377       247180       313999       314784       344684       314452       314320       314102       314806       314800       314440       314737 [    500977627 ]	PAPI_FP_OPS
 337020512127            0            0            0            0            0            0            0            0            0            0            0            0            0            0            0 [ 337020512127 ]	PAPI_SP_OPS
            4            4            4            4            4            4            4            4            4            4            4            4            4            4            4            4 [           64 ]	PAPI_DP_OPS
 336524188900            0            0            0            0            0            0            0            0            0            0            0            0            0            0            0 [ 336524188900 ]	PAPI_VEC_SP
            0            0            0            0            0            0            0            0            0            0            0            0            0            0            0            0 [            0 ]	PAPI_VEC_DP
      24.0937    0.0152686    0.0152756     0.012017      0.01526    0.0119982    0.0152416    0.0152797    0.0167311    0.0152636    0.0152572    0.0152466    0.0152808    0.0152805     0.015263    0.0152774 [      24.3176 ]	derived_FLOPS (MFLOPS)
  1.94161e-07  1.94161e-07  1.94161e-07  1.94161e-07  1.94161e-07  1.94161e-07  1.94161e-07  1.94161e-07  1.94161e-07  1.94161e-07  1.94161e-07  1.94161e-07  1.94161e-07  1.94161e-07  1.94161e-07  1.94161e-07 [  3.10658e-06 ]	derived_DP_vector_FLOPS (MFLOPS)
      16359.1            0            0            0            0            0            0            0            0            0            0            0            0            0            0            0 [      16359.1 ]	derived_SP_vector_FLOPS (MFLOPS)
```
