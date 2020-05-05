-- VUT FIT MITAI
-- FLP 2019/2020
-- Funcional project
-- Variant: plg-2-nka
-- Author: Vladimir Dusek
-- Login: xdusek27

module Grammar (
    GSymbol(..),
    GRule(..),
    Grammar(..),
    stringToSymbols,
    stringsToNonterms,
    stringsToTerms,
    isNonterm,
    isTerm,
    isEpsilon,
    getSrcsFromRules,
    getRules1,
    getRules2,
    getRules3,
    getRules4
) where

import Data.Char
import Utils


-- GSymbol is an aggregation data type for Terms, Nonterms and Epsilon
data GSymbol = Term Char | Nonterm Char Int | Epsilon () deriving (Eq)

-- How to show GSymbol
instance Show GSymbol where
    show (Term char) = [char]
    show (Nonterm char 0) = [char]
    show (Nonterm char int) = char : show int
    show (Epsilon ()) = "#"


-- Data structure for GRule
data GRule = GRule GSymbol [GSymbol] deriving (Eq)

-- How to show GRule
instance Show GRule where
    show (GRule src dst) = show src ++ "->" ++ printRuleDst dst


-- Data structure for Grammar
data Grammar = Grammar {
    nonterms :: [GSymbol],
    terms :: [GSymbol],
    defaultNonterm :: GSymbol,
    rules :: [GRule]
} deriving (Eq)

-- How to show Grammar
instance Show Grammar where
    show (Grammar nonterms terms defaultNonterm rules) =
        printSymbols nonterms ++ "\n" ++
        printSymbols terms ++ "\n" ++
        show defaultNonterm ++ "\n" ++
        printRules rules


-- How to print the list of the symbols (in Grammar)
printSymbols :: [GSymbol] -> String
printSymbols [] = ""
printSymbols [head] = show head
printSymbols (head:tail) = show head ++ "," ++ printSymbols tail


-- How to print the list of the rules (in Grammar)
printRules :: [GRule] -> String
printRules [] = ""
printRules [head] = show head
printRules (head:tail) = show head ++ "\n" ++ printRules tail


-- How to print the right side (dst) of the GRule
printRuleDst :: [GSymbol] -> String
printRuleDst = foldr ((++) . show) ""


-- Convert String to list of Symbols
-- etc. "abA1" -> [Term 'a', Term 'b', Nonterm 'A' 1]
stringToSymbols :: String -> [GSymbol]
stringToSymbols "" = []
stringToSymbols (head:tail)
    | null tail && (head == '#' || isAlpha head)       = [stringToSymbol [head]]
    | not (null tail) && isLower head                  = stringToSymbol [head] : stringToSymbols tail
    | not (null tail) && isUpper head && isStrInt tail = [stringToSymbol (head : tail)]
    | otherwise                                        = error errorSymbolMsg
  where -- Convert String to GSymbol
        -- etc. "A1" -> Nonterm 'A' 1
        --      "a"  -> Term 'a'
        stringToSymbol :: String -> GSymbol
        stringToSymbol (head:tail)
            | head == '#'  && null tail                        = Epsilon ()
            | isLower head && null tail                        = Term head
            | isUpper head && null tail                        = Nonterm head 0
            | isUpper head && not (null tail) && isStrInt tail = Nonterm head (read tail :: Int)
            | otherwise                                        = error errorSymbolMsg


-- Convert list of Strings to list of Nonterms
-- etc. ["A", "A1"] -> [Nonterm 'A' 0, Nonterm 'A' 1]
stringsToNonterms :: [String] -> [GSymbol]
stringsToNonterms [] = []
stringsToNonterms (head:tail) = stringToNonterm head : stringsToNonterms tail
  where -- Convert String to Nonterm
        -- etc. "A1" -> Nonterm 'A' 1
        stringToNonterm :: String -> GSymbol
        stringToNonterm "" = error errorNontermMsg
        stringToNonterm (head:tail)
            | isUpper head && null tail                        = Nonterm head 0
            | isUpper head && not (null tail) && isStrInt tail = Nonterm head (read tail :: Int)
            | otherwise                                        = error errorNontermMsg


-- Convert list of Strings to list of Terms
-- etc. ["a", "b"] -> [Term 'a', Term 'b']
stringsToTerms :: [String] -> [GSymbol]
stringsToTerms [] = []
stringsToTerms (head:tail) = stringToTerm head : stringsToTerms tail
  where -- Convert String to Term
        -- etc. "a"  -> Term 'a'
        stringToTerm :: String -> GSymbol
        stringToTerm [head] = if isLower head then Term head else error errorTermMsg
        stringToTerm _ = error errorTermMsg


-- Is GSymbol Nonterm?
isNonterm :: GSymbol -> Bool
isNonterm (Nonterm _ _) = True
isNonterm _ = False


-- Is GSymbol Term?
isTerm :: GSymbol -> Bool
isTerm (Term _) = True
isTerm _ = False


-- Is GSymbol Epsilon?
isEpsilon :: GSymbol -> Bool
isEpsilon (Epsilon ()) = True
isEpsilon _ = False


-- Is rule rule type 1? (A -> #)
isRule1 :: GRule -> Bool
isRule1 (GRule (Nonterm _ _) [Epsilon ()]) = True
isRule1 _ = False


-- Is rule rule type 2? (A -> aB)
isRule2 :: GRule -> Bool
isRule2 (GRule (Nonterm _ _) [Term _, Nonterm _ _]) = True
isRule2 _ = False


-- Is rule rule type 3? (A -> aaaB)
isRule3 :: GRule -> Bool
isRule3 (GRule src [Nonterm _ _]) = True
isRule3 (GRule src (Term _:tail)) = isRule3 (GRule src tail)
isRule3 _ = False


-- Is rule rule type 4? (A -> aaa)
isRule4 :: GRule -> Bool
isRule4 (GRule _ [Term _]) = True
isRule4 (GRule src (Term _:tail)) = isRule4 (GRule src tail)
isRule4 _ = False


-- Get list of src Nonterms from list of Rules
getSrcsFromRules :: [GRule] -> [GSymbol]
getSrcsFromRules [] = []
getSrcsFromRules (GRule src@(Nonterm _ _) _ : tail) = src : getSrcsFromRules tail
getSrcsFromRules (_:tail) = getSrcsFromRules tail


-- A -> #
getRules1 :: [GRule] -> [GRule]
getRules1 [] = []
getRules1 (rule:tail) =
    if isRule1 rule then
        rule : getRules1 tail
    else
        getRules1 tail

-- Proc toto nefunguje????????
--getRules1 :: [GRule] -> [GRule]
--getRules1 rules = filter (\ rule -> (isNonterm (fst(rule))) && ((snd(rule) !! 0) == Epsilon ())) (rules)


-- A -> aB
getRules2 :: [GRule] -> [GRule]
getRules2 [] = []
getRules2 (rule:tail) =
    if isRule2 rule then
        rule : getRules2 tail
    else
        getRules2 tail


-- Get PRG rules from Rules type 3 (A -> aaaB)
getRules3 :: [GRule] -> [GRule] -> [GRule]
getRules3 [] prgRules = prgRules
getRules3 (rule:tail) prgRules =
    if isRule3 rule then
        getRules3Inner rule prgRules ++ getRules3 tail (prgRules ++ getRules3Inner rule prgRules)
    else
        getRules3 tail prgRules


-- Get PRG rules from GRule type 3 (A -> aaaB)
getRules3Inner :: GRule -> [GRule] -> [GRule]
getRules3Inner rule@(GRule src (dstHead:dstTail)) prgRules =
    if isRule2 rule then
        [rule]
    else
        GRule src [dstHead, newNonterm] :
            getRules3Inner (GRule newNonterm dstTail)
              (prgRules ++ [GRule src [dstHead, newNonterm]])
  where nonterms = getNontermsFromRules prgRules
        newNonterm = Nonterm (getCharFromSymbol src)
            (1 + getLastIndex nonterms (getCharFromSymbol src) 1)


-- Get PRG rules from Rules type 4 (A -> aaa)
getRules4 :: [GRule] -> [GRule] -> [GRule]
getRules4 [] prgRules = prgRules
getRules4 (rule:tail) prgRules =
    if isRule4 rule then
        getRules4Inner rule prgRules ++ getRules4 tail (prgRules ++ getRules4Inner rule prgRules)
    else
        getRules4 tail prgRules


-- Get PRG rules from GRule type 4 (A -> aaa)
getRules4Inner :: GRule -> [GRule] -> [GRule]
getRules4Inner (GRule src []) prgRules = [GRule src [Epsilon ()]]
getRules4Inner (GRule src (dstHead:dstTail)) prgRules = GRule src [dstHead, newNonterm] :
    getRules4Inner (GRule newNonterm dstTail) (prgRules ++ [GRule src [dstHead, newNonterm]])
  where nonterms = getNontermsFromRules prgRules
        newNonterm = Nonterm (getCharFromSymbol src)
            (1 + getLastIndex nonterms (getCharFromSymbol src) 1)


-- Get last used index of the Nonterm in the list of Rules
getLastIndex :: [GSymbol] -> Char -> Int -> Int
getLastIndex [] _ idx = idx
getLastIndex (head:tail) char idx =
    if char == headChar && headIdx > idx then
        getLastIndex tail char headIdx
    else
        getLastIndex tail char idx
  where headIdx = getIndexFromSymbol head
        headChar = getCharFromSymbol head


-- Get index from the symbol (Nonterm)
getIndexFromSymbol :: GSymbol -> Int
getIndexFromSymbol (Nonterm _ int) = int
getIndexFromSymbol _ = error errorInternal


-- Get char from the symbol
getCharFromSymbol :: GSymbol -> Char
getCharFromSymbol (Term char) = char
getCharFromSymbol (Nonterm char _) = char
getCharFromSymbol _ = error errorInternal


-- Get list of the occured Nonterms in the list of the Rules
getNontermsFromRules :: [GRule] -> [GSymbol]
getNontermsFromRules = foldr ((++) . getNontermsFromRule) []


-- Get list of the occured Nonterms in the GRule
getNontermsFromRule :: GRule -> [GSymbol]
getNontermsFromRule (GRule src dst) = src : getNontermsFromSymbols dst


-- Get list of the Nonterms from the list of the Symbols
getNontermsFromSymbols :: [GSymbol] -> [GSymbol]
getNontermsFromSymbols [] = []
getNontermsFromSymbols (head@(Nonterm _ _):tail) = head : getNontermsFromSymbols tail
getNontermsFromSymbols (_:tail) = getNontermsFromSymbols tail
