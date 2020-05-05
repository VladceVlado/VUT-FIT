
subbags([], [[]]).
%subbags([X|XS], P) :- ..

addOneToAll(_, [], []).
%addOneToAll(E, [L|LS], [[E|L]|T]) :- ...





:- dynamic robot/2, dira/1.

%obsazeno(P) :- ...
%vytvor(I, P) :- ...
%vytvor(P) :- ...

%odstran(P) :- ...

%obsazene_pozice(X) :- bagof(...).
%obsazene_roboty(X) :- bagof(...).

inkrementuj(X,Y) :- Y is X+1.
dekrementuj(X,Y) :- Y is X-1.
doleva(I) :- pohni(I, dekrementuj).
doprava(I) :- pohni(I, inkrementuj).
%pohni(I, Operace) :- ...

%armageddon :- ...
%vybuch(P) :- ...









g_size(3).

/*
g_test(X:Y) :-
*/

/*
g_move(X1:Y1, X2:Y2) :- X2 is X1 - 1, Y2 is Y1 - 1, g_test(X2:Y2).
g_move(X1:Y1, X2:Y2) :- X2 is X1 - 1, Y2 is Y1 + 0, g_test(X2:Y2).
g_move(X1:Y1, X2:Y2) :- X2 is X1 - 1, Y2 is Y1 + 1, g_test(X2:Y2).
g_move(X1:Y1, X2:Y2) :- X2 is X1 + 0, Y2 is Y1 - 1, g_test(X2:Y2).
g_move(X1:Y1, X2:Y2) :- X2 is X1 + 0, Y2 is Y1 + 1, g_test(X2:Y2).
g_move(X1:Y1, X2:Y2) :- X2 is X1 + 1, Y2 is Y1 - 1, g_test(X2:Y2).
g_move(X1:Y1, X2:Y2) :- X2 is X1 + 1, Y2 is Y1 + 0, g_test(X2:Y2).
g_move(X1:Y1, X2:Y2) :- X2 is X1 + 1, Y2 is Y1 + 1, g_test(X2:Y2).
*/

/*
g_one(X:Y, Len, L, R) :-
g_one(X:Y, Len, L, R) :-
*/

/*
g_all(R, Len) :-
g_all(R, Len) :-
*/

/*
g_allLength(R) :-
g_allLength(R, Len) :-
g_allLength(R, Len) :-
*/

