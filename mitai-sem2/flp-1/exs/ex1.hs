-- Zdrojovy kod s programama z 1. cviceni

-- Typova signatura funkce
-- Definice funkce
mySqrt :: Double -> Double
mySqrt x = (sqrt x) + 1

add :: Int -> Int -> Int
add x y = x + y

-- Retezec je seznam znaku
-- "abc" = 'a':('b':('c':[]))
dropString :: Int -> String -> String
dropString 0 s = s
dropString n (x:xs) = dropString (n-1) xs

-- Faktorial v1 (pattern matching)
-- linear recursion
factorial1 :: Double -> Double
factorial1 0 = 1
factorial1 n = n * factorial1 (n-1)

-- Faktorial v2 (if-else statement)
factorial2 :: Double -> Double
factorial2 n =
    if n > 0 then
        n * factorial2 (n-1)
    else if n == 0 then
        1
    else
        error "chyba"

factorial3 :: Double -> Maybe Double
factorial3 n =
    if n < 0 then
        Nothing
    else
        Just (factorial1 n)

factorial4 :: Double -> Either String Double
factorial4 n =
    if n < 0 then
        Left "Err"
    else
        Right (factorial1 n)

-- Chybova funkce "error", ma signaturu
-- error :: String -> a

-- Factorial
-- tail recursion (efektivita)
factorial5 :: Double -> Double
factorial5 n = if n <= 1 then 1 else function 1 n
    where
        function :: Double -> Double -> Double
        function z 0 = z
        function z n = function (z*n) (n-1)

-- Fibonacci
-- linear recursion
fib :: Integer -> Integer
fib 0 = 0
fib 1 = 1
fib n = fib (n-1) + fib (n-2)

-- Fibonacci
-- tail recursion
fib2 :: Integer -> Integer
fib2 x = function 0 1 x
    where
        function :: Integer -> Integer -> Integer -> Integer
        function a b 0 = a
        function a b x = function b (a+b) (x-1)

-- List
-- : operator pridani
-- funkce na delku seznamu
len :: [a] -> Int
len [] = 0
len (x1:x2) = 1 + len x2

-- Add with tuple
add2 :: Num a => (a, a) -> a
add2 (x, y) = x + y

-- V Haskellu jsou funkce pouze s jednim parametrem
-- Izomorfismus
-- funkce curry a uncurry (neco nad tim co umozni "vice parametru")


-- Prednaska 2
-- Knihovna "Prelude"
--    - Seznamit se zaklnima funkcema
--    - Zkusit si nektere z nich napsat
