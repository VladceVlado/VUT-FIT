% radkovy komentar
/* blokovy komentar*/

%---------------------------------------------------%
% FAKTA

slunecno.

zena(marie).
zena(jana).
zena(linda).
zena(eva).

muz(tomas).
muz(jan).
muz(robert).
muz(pavel).

otec(tomas, jan).
otec(jan, robert).
otec(jan, jana).
otec(pavel, linda).
otec(pavel, eva).

matka(marie, robert).
matka(linda, jana).
matka(eva, petr).

%---------------------------------------------------%
% PRAVIDLA

% Kdo ma cepici a velkou hlavu, ten ma velkou cepici
velka_cepice(X) :- cepice(X), velka_hlava(X).

% Je-li neco strom, ker nebo bylina, pak je to rostlina
rostlina(X) :- strom(X).
rostlina(X) :- ker(X).
rostlina(X) :- bylina(X).

% Ekvivalentni zapis:
% rostlina(X) :- strom(X); ker(X); bylina(X).

%---------------------------------------------------%
% (1) PRIKLAD

rodic(X, Y) :- otec(X, Y); matka(X, Y).

sourozenec(X, Y) :- rodic(Z, X), rodic(Z, Y).

sestra(X, Y) :- sourozenec(X, Y), zena(X).

deda(X, Y) :- otec(X, Z), rodic(Z, Y).

je_matka(X) :- matka(X, _).

teta(X, Y) :- zena(X), rodic(Z, Y), sourozenec(X, Z).

%---------------------------------------------------%
% SEZNAMY - heterogenni

% Head1 = .(jan, .(tomas, [])).

% Rozdeleni seznamu na hlavicku a zbytek
% [H | T]

% Pattern matching jako v Haskellu

% Predikat neprazdny
neprazdny([_|_]) :- true.

% Predikat hlavicka
hlavicka([H|_], H).

% Predikat posledni
posledni([X], X).
posledni([_|XS], Y) :- posledni(XS, Y).

% Vestaveny predikat "trace." pro debugovani
% Dobra vec!

%---------------------------------------------------%
% (2) PRIKLAD

spoj([], XS, XS).
spoj([X|XS], YS, [X|ZS]) :- spoj(XS, YS, ZS).

%---------------------------------------------------%
% (3) PRIKLAD

obrat([], []).
obrat([X|XS], YS) :- obrat(XS, ZS), spoj(ZS, [X], YS).

%---------------------------------------------------%
% (4) PRIKLAD

% Vykricnik -> operator rez / cut
% Pokud prolog prejde pres !, uz se nevrati zpet,
% nedokaze backtrackovat a pokracovat dale
% ve vyhledavani.

sluc(XS, [], XS).
sluc([], XS, XS).
sluc([X|XS], [Y|YS], [X|ZS]) :- X =< Y, !, sluc(XS, [Y|YS], ZS).
sluc([X|XS], [Y|YS], [Y|ZS]) :- sluc([X|XS], YS, ZS).

%---------------------------------------------------%
% (5) PRIKLAD

serad([], []).
serad([X|XS], YS) :- serad(XS, ZS), sluc([X], ZS, YS).

%---------------------------------------------------%
% (6) PRIKLAD

split([], [[]]).
split([X|XS], [[]|YS]) :- X == ' ', split(XS, YS).
split([X|XS], [[X|YS]|ZS]) :- X \= ' ', split(XS, [YS|ZS]).

%---------------------------------------------------%
% (7) PRIKLAD

plus(X, Y, Z) :- Z is X + Y.

zipWith(_, [], _, []).
zipWith(_, _, [], []).
zipWith(F, [X|XS], [Y|YS], [Z|ZS]) :-
    C =.. [F, X, Y, Z],
    call(C),
    zipWith(F, XS, YS, ZS).
