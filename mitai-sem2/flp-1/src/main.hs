-- VUT FIT MITAI
-- FLP 2019/2020
-- Funcional project
-- Variant: plg-2-nka
-- Author: Vladimir Dusek
-- Login: xdusek27

module Main (main) where

import System.Environment
import System.IO
import Data.List
import Data.List.Split
import Data.Char

import Grammar
import Automaton
import Utils


-- Parsing command line arguments
parseArgs :: [String] -> (String, String)
parseArgs [opt]
    | opt == "-i" = ("-i", "")
    | opt == "-1" = ("-1", "")
    | opt == "-2" = ("-2", "")
    | otherwise = error errorArgMsg
parseArgs [opt, file]
    | opt == "-i" = ("-i", file)
    | opt == "-1" = ("-1", file)
    | opt == "-2" = ("-2", file)
    | otherwise = error errorArgMsg
parseArgs _ = error errorArgMsg


-- Get the input string from stdin
getInputStdin :: () -> IO String
getInputStdin () = getContents


-- Get the input string from the file
getInputFile :: String -> IO String
getInputFile name = do
    file <- openFile name ReadMode
    hGetContents file


-- Get PLG from input string
inputToPlg :: String -> IO Grammar
inputToPlg = return . parseInput . lines


-- Get PLG from the list of the lines of the input
parseInput :: [String] -> Grammar
parseInput (first:second:third:remaining) =
    Grammar {
        nonterms = nonterms,
        terms = terms,
        defaultNonterm = defaultNonterm,
        rules = rules
    }
  where nonterms = parseNonterms first
        terms = parseTerms second
        defaultNonterm = parseDefaultNonterm third nonterms
        rules = parseRules remaining nonterms terms
parseInput _ = error errorSyntaxMsg


-- Parse nonterm symbols
parseNonterms :: String -> [GSymbol]
parseNonterms "" = error errorNontermsMsg
parseNonterms string =
    if numOfNonterms == uniqNumOfNonterms && nontermsIndexZero nonterms then
        nonterms
    else
        error errorNontermsMsg
  where nonterms = stringsToNonterms $ splitOn "," string
        numOfNonterms = length nonterms
        uniqNumOfNonterms = (length . nub) nonterms

        -- Nonterms with indexes are not allowed in the PLG definition
        nontermsIndexZero :: [GSymbol] -> Bool
        nontermsIndexZero [] = True
        nontermsIndexZero (Nonterm _ int : tail) = int == 0 && nontermsIndexZero tail


-- Parse term symbols
parseTerms :: String -> [GSymbol]
parseTerms "" = []
parseTerms string =
    if numOfTerms == uniqNumOfTerms then
       terms
    else
       error errorTermsMsg
  where terms = stringsToTerms $ splitOn "," string
        numOfTerms = length terms
        uniqNumOfTerms = (length . nub) terms


-- Parse default nonterm
parseDefaultNonterm :: String -> [GSymbol] -> GSymbol
parseDefaultNonterm "" _ = error errorDefaultNontermMsg
parseDefaultNonterm string nonterms =
    if length defaultNonterms == 1 && isElemOf defaultNonterm nonterms then
        defaultNonterm
    else
        error errorDefaultNontermMsg
  where defaultNonterms = parseNonterms string
        defaultNonterm = head defaultNonterms


-- Parse rules
parseRules :: [String] -> [GSymbol] -> [GSymbol] -> [GRule]
parseRules [""] _ _ = []
parseRules string nonterms terms =
    if numOfRules == uniqNumOfRules then
        rules
    else
        error errorRulesMsg
  where rules = parseRulesInner string nonterms terms
        numOfRules = length rules
        uniqNumOfRules = (length . nub) rules

        -- Inner parser for recursion
        parseRulesInner :: [String] -> [GSymbol] -> [GSymbol] -> [GRule]
        parseRulesInner [] _ _ = error errorRulesMsg
        parseRulesInner (head:tail) nonterms terms =
            if null tail then
                [parseRule head nonterms terms]
            else
                parseRule head nonterms terms : parseRulesInner tail nonterms terms


-- Parse rule
parseRule :: String -> [GSymbol] -> [GSymbol] -> GRule
parseRule string nonterms terms =
    if length splitted == 2 then
        GRule src dst
    else
        error errorRulesMsg
  where splitted = splitOn "->" string
        src = parseRuleSrc (splitted !! 0) nonterms
        dst = parseRuleDst (splitted !! 1) nonterms terms


-- Parse rule src
parseRuleSrc :: String -> [GSymbol] -> GSymbol
parseRuleSrc "" _ = error errorRulesMsg
parseRuleSrc _ [] = error errorRulesMsg
parseRuleSrc string nonterms =
    if isElemOf src nonterms then
        src
    else
        error errorRulesMsg
  where src = (head . parseNonterms) string


-- Parse rule dst
parseRuleDst :: String -> [GSymbol] -> [GSymbol] -> [GSymbol]
parseRuleDst "" _ _  = error errorRulesMsg
parseRuleDst _ [] _  = error errorRulesMsg
parseRuleDst string nonterms terms =
    if isDstValid dst nonterms terms && (not . isNonterm . head) dst then
        dst
    else
        error errorRulesMsg
  where dst = stringToSymbols string

        isDstValid :: [GSymbol] -> [GSymbol] -> [GSymbol] -> Bool
        isDstValid [] _ _ = True
        isDstValid _ [] _ = False
        isDstValid (head:tail) nonterms terms
            | isNonterm head && not (isElemOf head nonterms) = False
            | isTerm head && not (isElemOf head terms)       = False
            | otherwise                                = isDstValid tail nonterms terms


-- PLG to PRG (option -1)
plgToPrg :: Grammar -> IO Grammar
plgToPrg plg = return
    Grammar {
        nonterms = nub $ nonterms plg ++ getSrcsFromRules prgRules,
        terms = terms plg,
        defaultNonterm = defaultNonterm plg,
        rules = prgRules
    }
  where
    prgRules = plgRulesToPrgRules (rules plg)


-- PLG rules to the PRG rules
plgRulesToPrgRules :: [GRule] -> [GRule]
plgRulesToPrgRules plgRules = nub (rules1 ++ rules2 ++ rules3 ++ rules4)
  where rules1 = getRules1 plgRules  -- A -> #
        rules2 = getRules2 plgRules  -- A -> aB
        rules3 = getRules3 plgRules (rules1 ++ rules2)  -- A -> aaaB
        rules4 = getRules4 plgRules (rules1 ++ rules2 ++ rules3)  -- A -> aaa


-- PRG to NKA (option -2)
prgToNka :: Grammar -> IO Automaton
prgToNka grammar = return
    Automaton {
        states = states,
        defaultState = defaultState,
        finiteStates = finiteStates,
        transitions = transitions
    }
  where dictionary = zip (nonterms grammar) [1..(length (nonterms grammar))]  -- [('A', 1), ...]
        states = getNkaStates dictionary
        defaultState = getNkaDefaultState dictionary (defaultNonterm grammar)
        finiteStates = getNkaFiniteStates dictionary (rules grammar)
        transitions = getNkaTransitions dictionary (rules grammar)


-- Get list of the States of the Automaton
getNkaStates :: [(GSymbol, AState)] -> [AState]
getNkaStates dictionary = [1..(length dictionary)]


-- Get Default state of the Automaton
getNkaDefaultState :: [(GSymbol, AState)] -> GSymbol -> AState
getNkaDefaultState = lookUp


-- Get list of the Finite states of the Automaton
getNkaFiniteStates :: [(GSymbol, AState)] -> [GRule] -> [AState]
getNkaFiniteStates _ [] = []
getNkaFiniteStates dictionary (GRule src [Epsilon ()] : tail) =
    lookUp dictionary src : getNkaFiniteStates dictionary tail
getNkaFiniteStates dictionary (_:tail) = getNkaFiniteStates dictionary tail


-- Get list of the Transitions of the Automaton
getNkaTransitions :: [(GSymbol, AState)] -> [GRule] -> [ATransition]
getNkaTransitions _ [] = []
getNkaTransitions dictionary (GRule _ [Epsilon ()] : tail) = getNkaTransitions dictionary tail
getNkaTransitions dictionary (GRule srcNonterm [Term dstTermChar, dstNonterm] : tail) =
    ATransition (lookUp dictionary srcNonterm) dstTermChar (lookUp dictionary dstNonterm)
        : getNkaTransitions dictionary tail


-- Lookup in Dictionary [('A', 1), ...]
lookUp :: [(GSymbol, AState)] -> GSymbol -> AState
lookUp [] _ = -1
lookUp ((symbol, state):tail) defaultSymbol =
    if defaultSymbol == symbol then
        state
    else
        lookUp tail defaultSymbol


-- Main
main :: IO ()
main = do
    args <- getArgs

    let (opt, file) = parseArgs args

    input <- if file == "" then getInputStdin () else getInputFile file

    plg <- inputToPlg input

    if opt == "-i" then
        print plg  -- print == (putStrLn . show)
    else if opt == "-1" then do
        prg <- plgToPrg plg
        print prg
    else do
        prg <- plgToPrg plg
        nka <- prgToNka prg
        print nka
