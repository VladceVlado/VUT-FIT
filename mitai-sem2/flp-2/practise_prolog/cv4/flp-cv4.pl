% FLP CVICENI 4 - PROLOG 1 - UVOD

% ukazka predikatu pro vypocet funkce faktorial
factorial( 0, 1 ).
factorial( N, Value ) :-
     N > 0,
     Prev is N - 1,
     factorial( Prev, Prevfact ),
     Value is Prevfact * N.

% databaze rodinnych vztahu
muz(jan).
muz(pavel).
muz(robert).
muz(tomas).
muz(petr).

zena(marie).
zena(jana).
zena(linda).
zena(eva).

otec(tomas,jan).
otec(jan,robert).
otec(jan,jana).
otec(pavel,linda).
otec(pavel,eva).

matka(marie,robert).
matka(linda,jana).
matka(eva,petr).

% Implementujte nasledujici predikaty:
/*
rodic(X,Y) :- ...
sourozenec(X,Y) :- ...
sestra(X,Y) :- ...
deda(X,Y) :- ...
je_matka(X) :- ...
teta(X,Y) :- ...
*/

% Seznamy:
neprazdny([_|_]) :- true.
hlavicka([H|_], H).
posledni([H], H) :- !.
posledni([_|T], Res) :- posledni(T, Res).

/*
% Dalsi ukoly:
spoj(  ,  ,  ) :- ...
obrat([],[]).
obrat([H|T], Res) :- ...
sluc(L, [], L).
sluc([], L, L).
sluc(  ,  ,  ) :- ...
sluc(  ,  ,  ) :- ...
serad([], []).
serad(  ,  ) :- ...
*/



plus(X,Y,Z) :- Z is X + Y.
