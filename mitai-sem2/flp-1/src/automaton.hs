-- VUT FIT MITAI
-- FLP 2019/2020
-- Funcional project
-- Variant: plg-2-nka
-- Author: Vladimir Dusek
-- Login: xdusek27

module Automaton (
    AState,
    ASymbol,
    ATransition(..),
    Automaton(..)
) where


-- Aliases types for States and Symbols
type AState = Int
type ASymbol = Char


-- Data structure for Transitions
data ATransition = ATransition AState ASymbol AState deriving (Eq)

-- How to show ATransition
instance Show ATransition where
    show (ATransition src symbol dst) = show src ++ "," ++ [symbol] ++ "," ++ show dst


-- Data structure for Automata
data Automaton = Automaton {
    states :: [AState],
    defaultState :: AState,
    finiteStates :: [AState],
    transitions :: [ATransition]
} deriving (Eq)

-- How to show Automaton
instance Show Automaton where
    show (Automaton states defaultState finiteStates transitions) =
        printStates states ++ "\n" ++
        printState defaultState ++ "\n" ++
        printStates finiteStates ++ "\n" ++
        printTransitions transitions


-- How to print state
printState :: AState -> String
printState = show


-- How to print states
printStates :: [AState] -> String
printStates [] = ""
printStates [head] = printState head
printStates (head:tail) = printState head ++ "," ++ printStates tail


-- How to print transitions
printTransitions :: [ATransition] -> String
printTransitions [] = ""
printTransitions [head] = show head
printTransitions (head:tail) = show head ++ "\n" ++ printTransitions tail
