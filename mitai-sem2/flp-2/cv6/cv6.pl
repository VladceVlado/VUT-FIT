%---------------------------------------------------------------------------%
% (1) Rozdil mnozin

% Je prvek prvkem seznamu?
prvek(H, [H|_]) :-
    !.
prvek(H, [_|T]) :-
    prvek(H, T).

% Rozdil seznamu
rozdil([], _, []).
rozdil([H|T], S, R) :-
    prvek(H, S),
    rozdil(T, S, R).
rozdil([H|T], S, [H|R]) :-
    not(prvek(H, S)),
    rozdil(T, S, R).

%---------------------------------------------------------------------------%
% (2) Problem N (8) dam

% Generate list with integers from N to 1
sequence(0, []) :-
    !.
sequence(N, [N|T]) :-
    plus(Ndec, 1, N),
    sequence(Ndec, T).

% Get all the permutations of the sequence list
sequence_permutation(N, Res) :-
    sequence(N, L),
    permutation(L, Res).

queens(Solution) :-
    queens(8, Solution).
queens(N, Solution) :-
    sequence_permutation(N, Solution),
    test(Solution).

% Test if queens are ok on diagonals
test([]) :-
    !.
test([H|T]) :-
    test(H, 1, T),
    test(T).

test(_, _, []) :-
    !.
test(Pos, Dist, [H|T]) :-
    not(plus(Pos, Dist, H)),
    not(plus(H, Dist, Pos)),
    succ(Dist, DistInc),
    test(Pos, DistInc, T).

%---------------------------------------------------------------------------%
% (3) Cesta konem po sachovnici
%   - ze startovni do cilove pozice, kolika zpusoby to lze provest?

% :- dynamic velikost/2.

% XR, YR -> rozmery sachovnice
% XS, YS -> startovni pozice
% XE, YE -> koncova pozice
% N -> pocet moznych cest
% cesty(XR, YR, XS, YS, XE, YE, N) :-
%     assert(velikost(XR, YR)),
%     ...,
%     findall(),
%     ...

/*
testPoz(X,Y) :-
*/

/*
skok(X,Y,XN,YN) :- XN is X + 2, YN is Y + 1, testPoz(XN, YN).
skok(X,Y,XN,YN) :-
skok(X,Y,XN,YN) :-
skok(X,Y,XN,YN) :-
skok(X,Y,XN,YN) :-
skok(X,Y,XN,YN) :-
skok(X,Y,XN,YN) :-
skok(X,Y,XN,YN) :-
*/

/*
cesta(X,Y,X,Y,[X:Y]).
cesta(X,Y,XE,YE,[X:Y|T]) :-
cesta(X, Y, _, _, _) :-
*/
