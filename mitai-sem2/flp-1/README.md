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

Glasgow Haskell Compiler -- [haskell.org/ghc/](https://www.haskell.org/ghc/)
```
$ sudo apt install ghc
```

hlint -- [github.com/ndmitchell/hlint](https://github.com/ndmitchell/hlint)
```
$ sudo apt install hlint
```

stylish-haskell -- [github.com/jaspervdj/stylish-haskell](https://github.com/jaspervdj/stylish-haskell)
```
$ sudo apt install stylish-haskell
```

hindent -- [github.com/chrisdone/hindent](https://github.com/chrisdone/hindent)
```
$ sudo apt install hindent
```

Install Cabal (Haskell package manager)
```
$ sudo apt install cabal-install
```

Install packages
```
$ cabal install split
```
