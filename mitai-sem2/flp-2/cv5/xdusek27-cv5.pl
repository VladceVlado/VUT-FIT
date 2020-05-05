% VUT FIT MITAI
% FLP 2019/2020
% Exercise 5
% Author: Vladimir Dusek
% Login: xdusek27

%---------------------------------------------------------------------------%
% (1) PRIKLAD: Potencni supermnozina

subbags(Y, Xs) :- bagof(X, subbags_elem(Y, X), Xs).

subbags_elem([], []).
subbags_elem([_|T], L) :- subbags_elem(T, L).
subbags_elem([H|T1], [H|T2]) :- subbags_elem(T1, T2).

%---------------------------------------------------------------------------%
% (2) PRIKLAD: Roboti a diry v jednorozmernem prostoru

:- (dynamic robot/2, dira/1).

% FAKTA
% robot(id, pozice)
robot(0, 1).
robot(1, 4).
% dira(pozice)
dira(0).
dira(7).

% Je pozice obsazena?
obsazeno(Pos) :- robot(_, Pos).
obsazeno(Pos) :- dira(Pos).

% Vytvori robota
% - Pokud je pozice obsazena -> false
% - Pokud je pozice volna, ale robot s danym ID jiz existuje, tak
%   odstran stareho robota a vytvor noveho se stejnym ID na nove pozici
vytvor(_, Pos) :- obsazeno(Pos).
vytvor(Id, Pos) :- (
    robot(Id, Pos2) -> (
        odstran(Pos2),
        assert(robot(Id, Pos))
    ) ; (
        assert(robot(Id, Pos))
    )
).

% Vytvori diru
% - Pokud je pozice obsazena -> false
vytvor(Pos) :- not(obsazeno(Pos)), assert(dira(Pos)).

% Odstran robota nebo diru
odstran(Pos) :- retract(dira(Pos)).
odstran(Pos) :- retract(robot(_, Pos)).

% Jake pozice jsou obsazene?
obsazene_pozice(Positions) :- bagof(Pos, obsazeno(Pos), Positions).

% Jake pozice jsou obsazene roboty?
obsazene_roboty(Positions) :- bagof(Pos, Id^robot(Id, Pos), Positions).

% Jake pozice jsou obsazene roboty? -- alternativne pomoci findall
% obsazene_roboty(Positions) :- findall(Pos, robot(_, Pos), Positions).

% Pohni robotem o 1 doleva
doleva(Id) :- pohni(Id, dekrementuj).

% Pohni robotem o 1 doprava
doprava(Id) :- pohni(Id, inkrementuj).

% Pomocne predikaty inkrementace a dekrementace
inkrementuj(X, Y) :- Y is X + 1.
dekrementuj(X, Y) :- Y is X - 1.

% Robot spadne do diry -> zmizi
pohni(Id, Operace) :-
    robot(Id, Pos),
    C =.. [Operace, Pos, NewPos],
    call(C),
    dira(NewPos),
    odstran(Pos).

% Robot se srazi s jinym robotem -> vybouchnou a vznikne dira
pohni(Id, Operace) :-
    robot(Id, Pos),
    C =.. [Operace, Pos, NewPos],
    call(C),
    robot(_, NewPos),
    odstran(Pos),
    vybuch(NewPos).

% Robot se premisti na volnou pozici
pohni(Id, Operace) :-
    robot(Id, Pos),
    C =.. [Operace, Pos, NewPos],
    call(C),
    odstran(Pos),
    vytvor(Id, NewPos).

% Pohni za vyuziti ef-then-else konstrukce (alternativa)
% pohni(Id, Operace) :-
%     robot(Id, Pos),
%     C =.. [Operace, Pos, NewPos],
%     call(C),
%     obsazeno(NewPos) -> (
%         odstran(Pos),
%         robot(_, NewPos) -> (
%             vybuch(NewPos)
%         )
%     ) ; (
%         odstran(Pos),
%         vytvor(Id, NewPos)
%     ).

% Vsichni roboti vybouchnou
armagedon :- forall(robot(_, Pos), vybuch(Pos)).

% Robot vybouchne -> vznikne po nem dira
vybuch(Pos) :- odstran(Pos), vytvor(Pos).

%---------------------------------------------------------------------------%
% (3) PRIKLAD: Odemykaci gesta

% Velikost matice
g_size(3).

% Otestovani, jestli jsme v matici (indexace od 0)
g_test(X:Y) :- g_size(Size), X >= 0, X < Size, Y >= 0, Y < Size.

% Mozne pohyby v matici
g_move(X1:Y1, X2:Y2) :- X2 is X1 - 1, Y2 is Y1 - 1, g_test(X2:Y2).
g_move(X1:Y1, X2:Y2) :- X2 is X1 - 1, Y2 is Y1 + 0, g_test(X2:Y2).
g_move(X1:Y1, X2:Y2) :- X2 is X1 - 1, Y2 is Y1 + 1, g_test(X2:Y2).
g_move(X1:Y1, X2:Y2) :- X2 is X1 + 0, Y2 is Y1 - 1, g_test(X2:Y2).
g_move(X1:Y1, X2:Y2) :- X2 is X1 + 0, Y2 is Y1 + 1, g_test(X2:Y2).
g_move(X1:Y1, X2:Y2) :- X2 is X1 + 1, Y2 is Y1 - 1, g_test(X2:Y2).
g_move(X1:Y1, X2:Y2) :- X2 is X1 + 1, Y2 is Y1 + 0, g_test(X2:Y2).
g_move(X1:Y1, X2:Y2) :- X2 is X1 + 1, Y2 is Y1 + 1, g_test(X2:Y2).

% Vsechna gesta z bodu X:Y o delce Len v seznamu Res
g_one(X:Y, Len, List, Res) :-
    not(member(X:Y, List)),
    length([X:Y|List], Len2),
    Len2 == Len,
    reverse([X:Y|List], Res),
    !.

g_one(X:Y, Len, List, Res) :-
    not(member(X:Y, List)),
    g_move(X:Y, Xnew:Ynew),
    g_one(Xnew:Ynew, Len, [X:Y|List], Res).

% Vsechna gesta o dane delce
g_all(Res, Len) :-
    g_size(Size),
    dekrementuj(Size, IdxMax),
    between(0, IdxMax, X),
    between(0, IdxMax, Y),
    g_one(X:Y, Len, [], Res).

% Vsechna gesta
g_allLength(Res) :-
    g_size(Size),
    Max is Size * Size,
    between(1, Max, Len),
    g_all(Res, Len).
