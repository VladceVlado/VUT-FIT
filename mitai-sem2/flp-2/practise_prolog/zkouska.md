FLP - Příprava ke zkoušce 2020: Prolog
=

# Co znát

...

<!-------------------------------------------------------->

# Řádný termín 2019

1. Napsat predikat `elem` pro zjisteni, jestli je prvek v seznamu a predikat `del` pro odstraneni hodnoty ze seznamu. Dat si bacha na to, ze seznam muze obsahovat nenavazane promenne a prvek se s nima nesmi zunifikovat.

```prolog
elemNV(X, [V|_]) :-
    nonvar(V),
    V=X, !.
elemNV(X, [_|VS]) :-
    elemNV(X, VS).

remVals([], _, []).
remVals([X|XS], L, R) :-
    elemNV(X, L), !,
    remVals(XS, L, R).
remVals([X|XS], L, [X|R]) :-
    remVals(XS, L, R).
```

2. Transpozice matice.

```prolog
trp([[]|_], []) :- !.
trp(XSS, [L|LS]) :-
    heads(XSS, L, YSS),
    trp(YSS, LS).

heads([[H|TS]|XSS], [H|T], [TS|YSS]) :-
    heads(XSS, T, YSS).
heads([], [], []).
```

3. Predikat, ktery bere sudoku reprezentovane matici 9x9 a vytvori seznam bloku 3x3.

```prolog
blocks([[A, B, C|CS], [D, E, F|FS], [G, H, I|IS]|XSS], [[A, B, C, D, E, F, G, H, I]|LS]) :-
    blocks([CS, FS, IS|XSS], LS).
blocks([[], [], []|XSS], LS) :-
    blocks(XSS, LS).
blocks([], []).
```

4. Predikat, ktery pro danou pozici v sudoku zjisti seznam moznych hodnot, ktere se na to policko daji doplnit. Mame k dispozici predikat getIth(X,M,L), ktery z matice M vytahne radek X do seznamu L. A meli jsme napsane, jak se z pozice [X,Y] v matici urci cislo bloku.

```prolog
valsFor(X, Y, M, [V|VS]) :-
    getIth(X, M, L),
    remVals([1, 2, 3, 4, 5, 6, 7, 8, 9], L, [H|T]),
    trp(M, TM),
    getIth(Y, TM, R),
    remVals([H|T], R, [HH|TT]),
    blocks(M, BM),
    P is 3*((X-1)//3)+(1+(Y-1)//3),
    getIth(P, BM, B),
    remVals([HH|TT], B, [V|VS]).
```

5. Predikat solves, ktery s vyuzitim definovanych predikatu vyresi sudoku, ktere ma na vstupu zadane matici 9x9. Neobsazene policka jsou reprezentovana volnou promennou. Muzem pouzit getIth, getRC, ktery pro zadanou pozici a matici vrati prvek na tech souradnicich a setRC, ktery nastavi prvek v matici na nejakou hodnotu.

```prolog
solves(M) :-
    search(1, 1, M).

search(R, C, M) :-
    getRC(R, C, M, V),
    nonvar(V), !,
    goNonvar(R, C, M).
search(R, C, M) :-
    valsFor(R, C, M, VS),
    testVals(R, C, M, VS).

goNonvar(9, 9, _) :- !.
goNonvar(R, C, M) :-
    newRC(R, C, RR, CC),
    search(RR, CC, M).

testVals(9, 9, M, [X]) :-
    getRC(9, 9, M, X), !.
testVals(R, C, M, [X|_]) :-
    getRC(R, C, M, X),
    newRC(R, C, RR, CC),
    search(RR, CC, M).
testVals(R, C, M, [_|XS]) :-
    testVals(R, C, M, XS).
```

K dispozici bylo
```prolog
newRC(R, C, R, CC) :-
    C<9, !,
    CC is C+1.
newRC(R, 9, RR, 1) :-
    R<9,
    RR is R+1.

getIth(1, [X|_], X) :- !.
getIth(N, [_|XS], X) :-
    N1 is N-1,
    getIth(N1, XS, X).

getRC(R, C, M, V) :-
    getIth(R, M, L),
    getIth(C, L, V).
```

<!-------------------------------------------------------->

# Řádný termín 2018

1) /6b/ Flatten seznamu - vytvorit predikat e, ktery bere 2 argumenty. Prvni je seznam libovolne zanorenych seznamu (i prazdnych), napr. [[], [1, 2], [[[[]]],[atom, atom]]]. Druhy argument je vysledny seznam bez zanoreni.

```prolog
flatten([], []).
flatten([[]|XS], YS) :- !,
    flatten(XS, YS).
flatten([[X|XS]|YS], ZS) :- !,
    flatten([X, XS|YS], ZS).
flatten([X|XS], [X|YS]) :-
    flatten(XS, YS).
```

2) /7b/ Funkce XOR, ktera vraci symterickou diferenci dvou mnozin (sjednoceni mnozin bez jejich pruseciku). Bere prvni a druhy parametr mnozinu reprezentovanou seznamem, treti parametr je vysledna mnozina reprezentovana seznamem.

```prolog
xor([], R, R).
xor(L, [], L).
xor(L, R, Res) :-
    sub(L, R, X),
    sub(R, L, Y),
    app(X, Y, Res).

sub([], R, R).
sub(L, [], L).
sub([X|XS], YS, ZS) :-
    elem(X, YS),
    sub(XS, YS, ZS), !.
sub([X|XS], YS, [X|ZS]) :-
    sub(XS, YS, ZS).

app([], R, R).
app([X|XS], YS, [X|ZS]) :-
    app(XS, YS, ZS).

elem(X, [X|_]) :- !.
elem(X, [_|YS]) :-
    elem(X, YS).
```

3) /9b/ Napisat predikat search(PocatecniPozice, SeznamCest), ktory najde vsechny cesty z dane pozice zpet do teto pozice, delky 20 az 22 kroku (netrapit se tim, jestli vracet prvni/posledni prvek ci ne). Kazdy prvok je mozne nastivit len jeden krat vyjma prveho (== posledneho). Definicia pozicie je neznama, napiste funkci nextStep(Pos, NewPos) nad neznamym a NEKONECNYM stavovym priestorom. Mozno pouzit retract*, assert*, bagof, setof, length.

```prolog
search(P, Res) :-
    setof(Path, s(P, P, 0, Path), Res).

s(P, P, N, [P]) :-
    N=<22,
    N>=20, !.
s(_, _, N, _) :-
    N>22, !,
    fail.
s(P, P, N, _) :-
    N\=0, !,
    fail.
s(A, P, N, [A|R]) :-
    assertz(pos(A)),
    NN is N+1,
    nextStep(A, AA),
    (   not(pos(AA))
    ;   AA=P
    ),
    s(AA, P, NN, R).
s(A, _, _, _) :-
    pos(A),
    retract(pos(A)), !,
    fail.

nextStep(p(X, Y), p(XX, Y)) :-
    XX is X+1.
nextStep(p(X, Y), p(XX, Y)) :-
    XX is X-1.
nextStep(p(X, Y), p(X, YY)) :-
    YY is Y+1.
nextStep(p(X, Y), p(X, YY)) :-
    YY is Y-1.
```

4) /8b/ Napisat predikat lookup. Prvy arguement vhodne reprezentovana tabulka symbolov, 2-hy argument kluc, 3-ty argument hodnota. A posledny a vysledny argument je modifikovana, pripadne vstupna tabulka symbolov.

Predikat pracuje v dvoch rezimoch. Ak je zadana hodnota, tak sa modifikuje pripadne modifikuje zaznam (klic -> hodnota?) v tabulke symbolov. Ak nie je zadana hodnota, tak vyhladavame v tabulku hodnotu so zadanym klucom. Ak sa nemylim, tak bolo mozne pouzit vsetko zo zakladnej kniznice Prologu. Ja som pouzil var(), nonvar() na zistenie, ci (nie) je zadana hodnota a nemyslim si, ze by to bolo v zadani spomenute. -- priklad byl mozna lehce modifikovany?

```prolog
emptyT([]).

lookup(T, _, _, _) :-
    var(T), !,
    fail.
lookup(T, Var, Val, NT) :-
    nonvar(Var),
    nonvar(Val), !,
    iT(T, Var, Val, NT).
lookup(T, Var, Val, T) :-
    nonvar(Var),
    lT(T, Var, Val).

iT([], R, L, [p(R, L)]).
iT([p(R, _)|PS], R, L, [p(R, L)|PS]) :- !.
iT([p(RR, LL)|PS], R, L, [p(RR, LL)|PPS]) :-
    iT(PS, R, L, PPS).

lT([p(R, L)|_], R, L) :- !.
lT([_|PS], R, L) :-
    lT(PS, R, L).
```

<!-------------------------------------------------------->

# Vlastní příklady

```prolog
% Prvni prvek seznamu
head([X|_], X).
```

```prolog
% Posledni prvek seznamu
last([X], X) :- !.
last([_|XS], X) :- last(XS, X).
```

```prolog
% "tail" seznamu
tail([_|XS], XS).
```

```prolog
% Je prvek v seznamu
% (seznam muze obsahovat nenavazane promenne)
elem(X, [V|_]) :- nonvar(V), X=V, !.
elem(X, [_|T]) :- elem(X, T).
```

```prolog
% Odstranit prvek ze seznamu
% (jeho prvni vystkyt)
del(_, [], []) :- !.
del(X, [X|YS], YS) :- !.
del(X, [Y|YS], [Y|ZS]) :- del(X, YS, ZS).
```

```prolog
% Spojeni dvou seznamu
concat([], X, X).
concat([X|XS], YS, [X|ZS]) :- concat(XS, YS, ZS).
```

```prolog
% Obraceni seznamu
reverse([], []).
reverse([X|XS], YS) :- reverse(XS, ZS), concat(ZS, [X], YS).
```

```prolog
% Slouceni dvou serazenych seznamu
% (vysledny je serazeny)
merge(X, [], X) :- !.
merge([], X, X) :- !.
merge([X|XS], [Y|YS], [X|ZS]) :- X=<Y, merge(XS, [Y|YS], ZS), !.
merge([X|XS], [Y|YS], [Y|ZS]) :- Y=<X, merge([X|XS], YS, ZS), !.
```

```prolog
% Serazeni seznamu
mySort([], []).
mySort([X|XS], YS) :- mySort(XS, ZS), merge([X], ZS, YS).
```

```prolog
% split
split([], [[]]).
split([X|XS], [[]|YS]) :-
    X==' ',
    split(XS, YS).
split([X|XS], [[X|YS]|ZS]) :-
    X\=' ',
    split(XS, [YS|ZS]).
```

```prolog
% zip with
plus(X, Y, Z) :-
    Z is X+Y.

zipWith(_, [], _, []).
zipWith(_, _, [], []).
zipWith(F, [X|XS], [Y|YS], [Z|ZS]) :-
    C=..[F, X, Y, Z],
    call(C),
    zipWith(F, XS, YS, ZS).
```

```prolog
% signum (if - then - else)
signum(X, Y) :-
    (X == 0 ->
        Y = 0
    ;
    (X < 0 ->
        Y = -1
    ;
        Y = 1
    )).

% signum (pattern matching)
signum2(0, 0).
signum2(X, -1) :- X < 0.
signum2(X, 1) :- X > 0.
```
