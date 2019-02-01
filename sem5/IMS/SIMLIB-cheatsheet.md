# Cheatsheet

## Prostriedky pre diskrétne modelovanie

- **Process**
- **Event**
- **Facility**
- **Store**
- **Queue**
- **Statistics**

## Struktura modelu

```C++
    #include "simlib.h"

    <deklarace zařízení>

    <deklarace tříd - procesy, události>

    <popis simulačného experimentu>
```

```C++

int main() {
    Init(t0,t1); // inicializacia simulatoru a modeloveho casu (pociatocny, koncovy)
    Run();       // beh simulacie
}
```

```C++
// Modelovy cas je reprezentovany premennou:
double Time;
Time = 10;
```

```C++
// Generatory pseudonahodnych cisiel

double Random(); // rovnomerne rozlozenie R(0,1)

double Uniform(double L, double H); // rovnomerne rozlozenie

double Exponential(double E); // exp. rozlozenie, stred E

double Normal(double M, double S);  // normal. rozlozenie stred M, rozptyl S

```

```c++

// popis udalosti
class Udalost : public Event {
    void Behavior() {
        // prikazy udalosti
        Activate(Time + e);
    }
};

// planovanie udalosti
(new Udalost)->Activate();  // cas Time
(new Udalost)->Activate(t); // cas t
```

```c++

// generovanie transakcii (procesov):
class Gener : public Event {
    void Behavior() {
        (new Proc)->Activate();
        Activate(Time+Exponential(2));
    }
};

int main() {
    Init(t0, t1);
    (new Gener)->Activate();
}
```

```c++
// transakcia opusta system
class Proc : public Process {
    void Behaviour() {

    } // implicitne opusta system
};

```

```c++

// proces transakcie
class Transakce : public Process {
    public:
        Transakce ( parametry ) { // konstruktor
            // nepovinny popis inicializacie procesu
        }
        void Behavior() {
            // popis chovania procesu
        }
};

```

Pre aktivaciu procesu sa vola metoda Behavior (chovanie).
Vykonanie metody je prerusene pri cakani:
    -> vo fronte pri zariadeni ( Seize, Enter )
    -> pri explicitnnom Wait(dt);

---

**Kalendar udalosti**:

```c++

// Inicializacia casu, kalendara, modelu... 
while (Kalendar je neprazdny) {
    // vyber prvy zaznam z kalendara
    if ( aktivacny cas udalost > T_END )
        koniec simulacie
    Nastav cas na aktivacni cas udalosti
    Vykonaj popis chovania udalosti
}

```


```c++
// Priklad: Timeout - prerusenie cakania vo fronte

class Timeout : public Event {
    Process *Id;
    public:
        Timeout(Process *p, double dt): Id(p)
        {
            Activate(Time+dt); // kdy bude
        }
        void Behavior()
        {
            Id->Cancel();  // zrusenie procesu
            Cancel();      // zrusenie udalosti
        }
};

class MProc : public Process {
    
    void Behavior() {
        Timeout *t = new Timeout(this, 10); 
        Seize(F);   // mozne cakanie vo fronte
        delete t;   // len ak nebol timeout
    }
};
```

```c++
// priorita procesu
class MProc : public Process {
    public:
        MProc() : Process( PRIORITA1 ) { };
        void Behavior() {
            Priority = 3;  // zmena priority Seize(F);
            Priority = 0;  // implicitna priorita
        }
};
```

```c++

Queue q;
// ... 

    void Behavior()     // popis chovania procesu
    {   
        q.Insert(this); // vlozi sa do fronty
        Passivate();    // suspenduje sa
    }

// ...

// Iny proces (alebo udalost) moze z fronty vyberat:
// ...
if (!q.Empty())
{
    Process *tmp = q.GetFirst();
    tmp->Activate(); // aktivacia
}
```

```c++
// Zariadenie je obsaditelne procesom (vylucny pristup)
// Zaradenie obsahuje 2 fronty poziadavkov
// 1. (vnejsi) fronta cakajucich poziadavkov
// 2. (vnitrni) fronta prerusenych poziadavkov

Seize(Proces, PrioritaObsluhy = 0);

// Je potreba rozlisovat dva typy priorit v SIMLIB
// 1. priorita procesu (razeni do front, Priority)
// 2. priorita obsluhy (2. parameter metody Seize)
```

```c++
// Priklad -- obsadenie zariadenia
Facility F("Fac");
...
class P : Process {
    void Behavior()
    {
        ...
        Seize(F);   // obsadenie linky
        Wait(Exponential(10)); // vykonanie obsluhy
        Release(F); // uvolnenie linky
        ...
    }
};

```

```c++

// Sklad -- Store
// sklad nema prioritu obsluhy

Store Voziky("Voziky", 50); // kapacita c

// Proces typicky vykonava operacie:
Enter(Voziky, 1);
Leave(Voziky, 1);

// obdrzana kapacita nesuvisi s procesom -> vratit ju moze lubovolny iny proces
```

```c++
// Zariadenie -- neblokujuce obsadenie linky
// transakcia pristupuje k zariadeniu ale nechce cakat vo fronte

Facility F("Fac");

class Proc : Process {
    void Behavior()
    {
        ...
        if (!F.Busy())
            Seize(F);
        else
        ...
    }
};
```

```c++
// Nahodny vyber z N zariadeni
const int N = 3;
Facility F[N];

class Proc : Process {
    void Behavior()
    {
      ...
      int idx = int( N * Random() );
      Seize(F[idx]);
      ...
      Release(F[idx]);
      ...
    }
};
```

```c++
// Vyber s prioritou

const int N = 3;
Facility F[N];
...
int idx;

for(idx=0; idx < N-1; idx++)
    if(!F[idx].Busy())
        break; // prve neobsadene

Seize(F[idx]);
...
```

```c++
// Vyber podla dlzky fronty

const int N = 30;
Facility F[N];
int idx=0;

for (int a=0; a < N; a++)
    if (F[a].QueueLen() < F[idx].QueueLen())
        idx=a;
Seize(F[idx]);
...
```

```c++
// Statistiky v SIMLIB/c++

// Triedy: Stat, TStat, Histogram

// spolocne operacie:

s.Clear(); // inicializacia
s.Output(); // tisk
s(x);       // zaznam hodnoty x

// Trieda Stat
int main()
{
    Stat p;

    for (int a=0; a<1000; a++)
        p(Uniform(0, 100));

    p.Output();
}

// output
+------------------------------------------------+
| STAT                                           |
+------------------------------------------------+
|  Min = 0.403416                Max = 99.9598   |
|  Number of records = 1000                      |
|  Average value = 49.8424                       |
|  Standard deviation = 28.8042                  |
+------------------------------------------------+

// Trieda TStat
// sleduje casovy priebeh vstupnej veliciny.
// Vypocet priemernej hodnoty vstupu, dlzka fronty za urcity casovy interval


// Histogram

//Histogram("Jmeno pro tisk", OdHodnoty, Krok, PocetTrid);
Histogram expo("Expo", 0, 1, 15);
  for (int a=0; a<1000; a++)
     expo(Exponential(3));

// output
+-----------------------------------------------+
| HISTOGRAM Expo                                |
+-----------------------------------------------+
| STATISTIC Expo                                |
+-----------------------------------------------+
|  Min = 0.00037629              Max = 24.8161  |
|  Number of records = 10000                    |
|  Average value = 2.94477                      |
|  Standard deviation = 2.91307                 |
+-----------------------------------------------+
```

## Cely priklad -- Samoobsluha v SIMLIB

```c++

#include "simlib.h"
const int POC_POKLADEN = 5;

// zariadenia
Facility Pokladny[POC_POKLADEN];
Store Lahudky("Oddeleni lahudek", 2);
Store Voziky("Seradiste voziku", 50);

Histogram celk("Celkova doba v systemu", 0, 5, 20);

class Zakaznik : public Process {
    void Behavior() {
        double prichod = Time;  // zaznam casu prichodu
        Enter(Voziky, 1);
        if ( Random() <= 0.30 )
        {
            Enter(Lahudky, 1);
            Wait(Exponential(2));
            Leave(Lahudky, 1);
        }
        Wait(Uniform(10, 15)); // nakup

        // vyber podla dlzky fronty;
        int i = 0;
        for (int a=1; a < POC_POKLADEN; a++)
            if (Pokladny[a].QueueLen() < Pokladny[i].QueueLen())
                i=a;
        Seize(Pokladny[i]);     // u pokladny
        Wait( Exponential(3) );
        Release(Pokladny[i]);
        Leave(Voziky, 1);
        celk(Time-prichod);     // zaznam casu
    }
};

class Prichody : public Event {
  void Behavior()
  {
    (new Zakaznik)->Activate();
    Activate( Time + Exponential(8) );
  }
};

int main() // popis experimentu
{
    SetOutput("Samoo.dat");
    Init(0, 1000);
    (new Prichody)->Activate(); // start generatoru
    Run(); // beh simulace

   // tisk statistik:
   celk.Output();
   Lahudky.Output();
   Voziky.Output();
   for (int a=0; a < POC_POKLADEN; a++)
       Pokladny[a].Output();
}

```