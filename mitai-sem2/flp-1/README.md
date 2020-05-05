# FLP 2019/2020

## Functional project - plg-2-nka

<!----------------------------------------------------------------------------->

### Reference

> [**Assignment**](assignment)<br>
> — Description of the assignment in Czech.

> [**Documentation**](docs)<br>
> — Additional information.
>
> [**Tests**](test)<br>
> — Directory with the tests. There's return code, input and outputs (1, 2) for every test.

<!----------------------------------------------------------------------------->

### Execution

Compile
```
$ make
```

Run
```
$ make run opt={-i|-1|-2} [file=filename] 
```

Test
```
$ make test 
```

Clean
```
$ make clean
```

Zip
```
$ make zip 
```

<!----------------------------------------------------------------------------->

### Requirements

Install Glasgow Haskell Compiler - ghc
```
$ sudo apt install ghc
```

Install Cabal (Haskell package manager)
```
$ sudo apt install cabal-install
```

Install packages
```
$ cabal install split
```
