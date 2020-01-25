# VUT FIT AVS 2019

## Projekt č. 2: Optimalizace paralelního kódu

<!---------------------------------------------------------------------->

### Přihlášení na superpočítač Anselm

Mount Anselm home andresář lokálně přes `sshfs`
```
$ sshfs dd-19-32-17@anselm.it4i.cz:/home/training/dd-19-32-17/ /home/vdusek/anselm/ -o IdentityFile=/home/vdusek/.ssh/id_rsa-anselm
```

Připojení na Anselm
```
$ ssh -i ~/.ssh/id_rsa-dd-19-32-17 dd-19-32-17@anselm.it4i.cz
```

<!---------------------------------------------------------------------->

### Výpočetní uzly a moduly

Zažádání o výpočetní výkon
```
$ qsub -A DD-19-32 -q qexp -l select=1:ncpus=16,walltime=1:00:00 -I
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
$ cmake ../src -DCMAKE_BUILD_TYPE=Release
```

Make
```
$ make -j
```

Run with Reference Mesh Builder
```
$ ./PMC -b ref ../data/bun_zipper_res4.pts bun_zipper_res4.obj
```

Run with OpenMP Loop Mesh Builder
```
$ ./PMC -b loop ../data/bun_zipper_res4.pts bun_zipper_res4.obj
```

Run with Octree Mesh Builder
```
$ ./PMC -b tree ../data/bun_zipper_res4.pts bun_zipper_res4.obj
```

<!---------------------------------------------------------------------->

### Přeložení a spuštění projektu přes přiložený Makefile

Spuštění z adresáře `./`
```
$ ls
assignment.pdf  data/  Makefile  PMC-xdusek27.txt  README.md  scripts/  src/
```

Přeložení + spuštění
```
$ make run-{ref|loop|tree}
```

Smazání přeložených a dočasných souborů
```
$ make clean
```

<!---------------------------------------------------------------------->

### Sources

[www.youtube.com/watch?v=B_xk71YopsA](https://www.youtube.com/watch?v=B_xk71YopsA)

[pages.tacc.utexas.edu/~eijkhout/pcse/html/omp-data.html](http://pages.tacc.utexas.edu/~eijkhout/pcse/html/omp-data.html)
