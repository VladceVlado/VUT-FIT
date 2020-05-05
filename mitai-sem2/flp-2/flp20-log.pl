% VUT FIT MITAI
% FLP 2019/2020
% Logic project
% Variant: Turing machine
% Author: Vladimir Dusek
% Login: xdusek27

%--------------------------------------------------------------------------------------------------%
% Assignment
% Input:
%     <rules> == <current_state> <current_symbol> <next_state> <next_symbol>
%     ...
%     <default_tape> == <symbol><symbol>...
%
% Output:
%     <configuration> == <tape_before_head><current_state><current_symbol><rest_of_tape>
%     ...

%--------------------------------------------------------------------------------------------------%
% Predicates for reading and parsing input.
% Author: Martin Hyrs, ihyrs@fit.vutbr.cz

read_line(L, C) :-
    get_char(C),
    (
        isEOFEOL(C),
        L = [],
        !
    ;
        read_line(LL, _),
        [C|LL] = L
    ).

isEOFEOL(C) :-
    (
        C == end_of_file
    ;
        char_code(C, Code),
        Code == 10
    ).

read_lines(Ls) :-
    read_line(L, C),
    (
        C == end_of_file,
        Ls = []
    ;
        read_lines(LLs),
        Ls = [L|LLs]
    ).

split_line([], [[]]) :-
    !.
split_line([' '|T], [[]|S1]) :-
    !,
    split_line(T, S1).
split_line([32|T], [[]|S1]) :-
    !,
    split_line(T, S1).
split_line([H|T], [[H|G]|S1]) :-
    split_line(T, [G|S1]).

split_lines([], []).
split_lines([L|Ls], [H|T]) :-
    split_lines(Ls, T),
    split_line(L, H).

%--------------------------------------------------------------------------------------------------%
% Dynamic predicates

:- (dynamic rule/4).

% Add list of the rules to the database
add_rules([]).
add_rules([X|XS]) :-
    add_rule(X),
    add_rules(XS).

% Add rule to the database
add_rule([[State], [Sym], [NextState], [NextSym]]) :-
    assert(rule(State, Sym, NextState, NextSym)).

%--------------------------------------------------------------------------------------------------%
% Turing Machine

% Return current State and the Symbol under the head according to configuration
% get_state_sym(configuration, current state, current symbol)
get_state_sym([X|XS], State, Sym) :-
    % If X is a state (uppercase)
    (char_type(X, upper) ->
        State = X,
        % If XS is empty (end of the tape) then Sym is blank
        (XS == [] ->
            Sym = ' '
        ;
        % Otherwise Sym is under the head
            [Sym|_] = XS
        )
    ;
    % else
        get_state_sym(XS, State, Sym)
    ).


% TM head moves to the left
% do_left(State, NextState, Configuration, NextConfiguration)

% When TM head overflows to the left
do_left(State, _, [State|_], _) :-
    % writeln('Head overflows to the left'),  % debug
    halt.

% Otherwise
do_left(State, NextState, [X|XS], NextConf) :-
    [Sym|Tape] = XS,
    % If we found the State on the tape
    (State == Sym ->
        append([NextState], [X], Tmp),  % Concatenate NextState, X
        append(Tmp, Tape, NextConf)     % and the rest of the tape == NextConf
    ;
    % Continue with the next position otherwise
        do_left(State, NextState, XS, NewNextConf),
        NextConf = [X|NewNextConf]
    ).


% TM head moves to the right
% do_right(State, NextState, Configuration, NextConfiguration)

% When State is in the end of the tape, add space as a Blank
do_right(State, NextState, [State], NextConf) :-
    append([NextState], [' '], NextConf).

% Otherwise
do_right(State, NextState, [X|XS], NextConf) :-
    % If we have our position
    (State == X ->
        [OldSym|Tape] = XS,  % Remove symbol under the head
        append([OldSym], [NextState], Tmp),  % Concatenate OldSym, NextState
        append(Tmp, Tape, NextConf)          % and the rest of the tape == NextConf
    ;
    % Continue with the next position otherwise
        do_right(State, NextState, XS, NewNextConf),
        NextConf = [X|NewNextConf]
    ).


% TM head writes symbol to the tape
% do_write(State, NextState, NextSymbol, Configuration, NextConfiguration)

% When State is in the end of the tape, writing on the Blank
do_write(State, NextState, NextSym, [State], NextConf) :-
    append([NextState], [NextSym], NextConf).

% When re-writing symbol
do_write(State, NextState, NextSym, [X|XS], NextConf) :-
    % If we have our position
    (State == X ->
        [_|Tape] = XS,  % Remove symbol under the head
        append([NextState], [NextSym], Tmp),  % Concatenate NextState, NextSymbol
        append(Tmp, Tape, NextConf)           % and the rest of the tape == NextConf
    ;
    % Continue with the next position otherwise
        do_write(State, NextState, NextSym, XS, NewNextConf),
        NextConf = [X|NewNextConf]
    ).


% Do Turing Machine action.
do_action(State, NextState, NextSym, Conf, Confs) :-
    % print_rule(State, Sym, NextState, NextSym),  % debug

    % Action: move left
    (NextSym == 'L' ->
        do_left(State, NextState, Conf, NextConf)
    ;
    % Action: move right
    NextSym == 'R' ->
        do_right(State, NextState, Conf, NextConf)
    ;
    % Action: write
    char_type(NextSym, lower) ->
        do_write(State, NextState, NextSym, Conf, NextConf)
    ;
    % Error
        throw(error('Error'))
    ),

    % Simulate the next step
    simulate(NextConf, [Conf|Confs]).


% Simulate behaviour of the Turing Machine according to
% the configuration (Conf) and the known rules in the database.
simulate(Conf, Confs) :-
    % print_conf(Conf),  % debug

    get_state_sym(Conf, State, Sym),

    % If we're in the finite state
    (State == 'F' ->
        get_final_conf(Conf, FinalConf),  % Get final configuration,
        print_confs([FinalConf|Confs]),   % print all of them
        halt                              % and halt.
    ;
    % Execute all the possible rules otherwise
        forall(
            rule(State, Sym, NextState, NextSym),
            do_action(State, NextState, NextSym, Conf, Confs)
        )
    ).

%--------------------------------------------------------------------------------------------------%
% Other

% Print Turing Machine configuration
print_conf([]) :-
    writeln('').
print_conf([X|XS]) :-
    write(X),
    print_conf(XS).

% Print Turing Machine configurations
print_confs([]).
print_confs([X|XS]) :-
    print_confs(XS),
    print_conf(X).

% Get final conf
% get_final_conf(Conf, FinalConf)
get_final_conf([X|XS], FinalConf) :-
    % If X is a state (uppercase)
    (char_type(X, upper) ->
        FinalConf = ['F'|XS]
    ;
    % else
        get_final_conf(XS, FinalConfRest),
        FinalConf = [X|FinalConfRest]
    ).

%--------------------------------------------------------------------------------------------------%
% Debug

% Print rule
print_rule(S, Sym, NewS, NextSym) :-
    write(S),
    write(Sym),
    write(NewS),
    write(NextSym),
    writeln('').

% Print rules from the database
print_rules_db :-
    writeln('Rules:'),
    forall(
        rule(S, Sym, NewS, NextSym),
        print_rule(S, Sym, NewS, NextSym)
    ),
    writeln('').

%--------------------------------------------------------------------------------------------------%

start :-
    prompt(_, ''),

    % Get list of the lines from the input
    read_lines(ListLines),

    % Get list of the lists of the characters and strip the whitespaces
    split_lines(ListLines, ListLinesChars),

    % Remove last elem (tape) and get just the rules
    append(Rules, [_], ListLinesChars),

    % Add rules to the database
    add_rules(Rules),

    % Print rules from the database for the purpose of debugging
    % print_rules_db,  % debug

    % Get the tape
    last(ListLinesChars, [Tape]),

    % Create StartConf
    append(['S'], Tape, StartConf),

    % Simulate Turing Machine
    simulate(StartConf, []).
