# GAL 2020/2021

> [**Assignment**](assignment/gal_zadani_projektu_2020.pdf)<br>
> — Obecné zadání z WISu v pdf

<!---------------------------------------------------------------------->

## Commands

Compile the source code
```
$ cd src ; make
```

Generate data for measuring the execution of algorithms (`data_graphs/`)
```
$ ./scripts/generate_graphs.py
```

Run measurement of all variants (`data_times/`)
```
$ ./scripts/run_measurement.py
```

Get the plot for every variant (`plots/`)
```
$ ./scripts/get_plots.py
```

<!---------------------------------------------------------------------->

## Téma 38 - Paralelizace: Radio coloring

### Zadání

Nastudujte problém radio coloring ([en.wikipedia.org/wiki/Radio_coloring](https://en.wikipedia.org/wiki/Radio_coloring)) a navrhněte nebo nastudujte a implementujte vhodný paralelní algoritmus a experimentálně ověřte jeho časovou náročnost.

### Zdroje

#### Sekvenční algoritmy
- A Graph Radio k-Coloring Algorithm
    - [papers/A_Graph_Radio_k-Coloring_Algorithm.pdf](papers/A_Graph_Radio_k-Coloring_Algorithm.pdf)
    - [www.researchgate.net/publication/269025730_A_Graph_Radio_k-Coloring_Algorithm](https://www.researchgate.net/publication/269025730_A_Graph_Radio_k-Coloring_Algorithm)

#### Paralelní algoritmy

- An Exact Parallel Algorithm for the Radio k-coloring Problem
    - [papers/An_Exact_Parallel_Algorithm_for_the_Radio_k-coloring_Problem.pdf](papers/An_Exact_Parallel_Algorithm_for_the_Radio_k-coloring_Problem.pdf)
    - [www.researchgate.net/publication/337392990_An_Exact_Parallel_Algorithm_for_the_Radio_k-coloring_Problem](https://www.researchgate.net/publication/337392990_An_Exact_Parallel_Algorithm_for_the_Radio_k-coloring_Problem)

- Parallel Algorithm for Radiocoloring a Graph
    - [papers/PARALLEL_ALGORITHM_FOR_RADIOCOLORING_A_GRAPH.pdf](papers/PARALLEL_ALGORITHM_FOR_RADIOCOLORING_A_GRAPH.pdf)
    - [www.researchgate.net/publication/249869674_PARALLEL_ALGORITHM_FOR_RADIOCOLORING_A_GRAPH](https://www.researchgate.net/publication/249869674_PARALLEL_ALGORITHM_FOR_RADIOCOLORING_A_GRAPH)
