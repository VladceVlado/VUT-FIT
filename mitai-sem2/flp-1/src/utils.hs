-- VUT FIT MITAI
-- FLP 2019/2020
-- Funcional project
-- Variant: plg-2-nka
-- Author: Vladimir Dusek
-- Login: xdusek27

module Utils where

import Data.Char


-- Help text
helpMsg = "\nVUT FIT MITAI\nFLP 2019/2020\nFuncional project\nVariant: plg-2-nka\n\
          \Author: Vladimir Dusek\nLogin: xdusek27\nUsage: ./plg-2-nka {-i|-1|-2} [file]\n"

-- Error invalid program execution
errorArgMsg = "Error: invalid program execution\n" ++ helpMsg

-- Error invalid input syntax
errorSyntaxMsg = "Error: invalid syntax of the input\n" ++ helpMsg

-- Error invalid input: nonterms
errorNontermsMsg = "Error: invalid nonterms definition\n" ++ helpMsg

-- Error invalid input: terms
errorTermsMsg = "Error: invalid terms definition\n" ++ helpMsg

-- Error invalid input: default nonterm
errorDefaultNontermMsg = "Error: invalid default nonterm definition\n" ++ helpMsg

-- Error invalid input: rules
errorRulesMsg = "Error: invalid rules definition\n" ++ helpMsg

-- Error invalid Symbol
errorSymbolMsg = "Error: invalid Symbol (Nonterm / Term)\n" ++ helpMsg

-- Error invalid Nonterm
errorNontermMsg = "Error: invalid Nonterm\n" ++ helpMsg

-- Error invalid Term
errorTermMsg = "Error: invalid Term\n" ++ helpMsg

-- Internal error
errorInternal = "Error: internal problem"


-- If the length of every string in the list is 1 return True, False otherwise
isLenStrsOne :: [String] -> Bool
isLenStrsOne = foldr (\ head -> (&&) (length head == 1)) True


-- If all characters in the string are lowercase return True, False otherwise
isStrLower :: String -> Bool
isStrLower = foldr ((&&) . isLower) True


-- If all characters in the string are uppercase return True, False otherwise
isStrUpper :: String -> Bool
isStrUpper = foldr ((&&) . isUpper) True


-- If all characters in the string are digits return True, False otherwise
isStrInt :: String -> Bool
isStrInt = foldr ((&&) . isDigit) True


-- If the element is a member of the list return True, False otherwise
isElemOf :: (Eq a) => a -> [a] -> Bool
isElemOf elem = foldl (\ x0 x -> ((x == elem) || x0)) False
