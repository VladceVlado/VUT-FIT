-- Vlastnosti Haskellu:
-- - referential transparency
-- - lazy evaluation
-- - statically typed
-- - type inference - typy neni nutne explicitne uvadet
-- - polymorphic functions - functions with type variable
-- - vse je funkce i operatory

-- Vypocet faktorialu
-- pattern matching
myFact1 :: Integer -> Integer
myFact1 0 = 1
myFact1 x = x * myFact1 (x-1)

-- Vypocet faktorialu
-- if-else statement
myFact2 :: Integer -> Integer
myFact2 x =
    if x == 0 then
        1
    else if x > 0 then
        x * myFact2 (x-1)
    else
        error "Error"

-- Vypocet faktorialu
-- funkce product
myFact3 :: Integer -> Integer
myFact3 n = product [1 .. n]

-- Vraceni prvniho prvku v trojici
-- Pomoci pattern matchingu
-- analogicky druhy, treti apod.
myFirst :: (a, b, c) -> a
myFirst (x, _, _) = x

-- Head seznamu
myHead :: [a] -> a
myHead (x:_) = x
myHead [] = error "Error"

-- Tail seznamu
myTail :: [a] -> [a]
myTail (_:x) = x
myTail [] = error "Error"

-- Odd elements of list
-- odd' :: [a] => [a]

-- Length of list
-- using list comprehension
myLen1 :: [a] -> Integer
myLen1 x = sum [1 | _ <- x]

-- Length of list
-- pattern matching and recursion
myLen2 :: [a] -> Integer
myLen2 [] = 0
myLen2 (_:x) = 1 + myLen2 x

-- Sum of the elements in the list
mySum :: (Num a) => [a] -> a
mySum [] = 0
mySum (x:y) = x + mySum y

capital :: String -> String
capital whole@(head:_) = "The first letter of " ++ whole ++ " is " ++ [head]

-- Pouziti "Guards"
bmiTell1 :: (RealFloat a) => a -> String
bmiTell1 x
    | x < 50 = "You're underweight, you emo, you!"
    | x < 80 = "You're supposedly normal. Pffft, I bet you're ugly!"
    | x < 110 = "You're fat! Lose some weight, fatty!"
    | otherwise = "You're a whale, congratulations!"

myMax :: (Ord a) => a -> a -> a
myMax x y
    | x > y = x
    | otherwise = y

-- Compare
-- functions can be defined and called as infix
myCompare :: (Ord a) => a -> a -> Ordering
a `myCompare` b
    | a > b     = GT
    | a == b    = EQ
    | otherwise = LT

-- bmiTell s where ("bindings")
bmiTell2 :: (RealFloat a) => a -> a -> String
bmiTell2 weight height
    | bmi < skinny = "You're underweight, you emo, you!"
    | bmi < normal = "You're supposedly normal. Pffft, I bet you're ugly!"
    | bmi < fat    = "You're fat! Lose some weight, fatty!"
    | otherwise    = "You're a whale, congratulations!"
    where bmi = weight / height ^ 2
          (skinny, normal, fat) = (18.5, 25.0, 30.0)

-- Inicialy pomoci where a pattern matchingu
initials :: String -> String -> String
initials firstname lastname = [f] ++ ". " ++ [l] ++ "."
    where (f:_) = firstname
          (l:_) = lastname

-- Do where je mozne davat i funkce
calcBmis :: (RealFloat a) => [(a, a)] -> [a]
calcBmis xs = [bmi w h | (w, h) <- xs]
    where bmi weight height = weight / height ^ 2

-- let <bindings> in <expression>
-- Alternativa bindingu, vyraz v in tvori samostatny vyraz,
-- ktery lze dat kamkoliv

calcBmis2 :: (RealFloat a) => [(a, a)] -> [a]
calcBmis2 xs = [bmi | (w, h) <- xs, let bmi = w / h ^ 2]

-- case expression of pattern -> result
                   -- pattern -> result
                   -- pattern -> result

-- Head s case
myHead2 :: [a] -> a
myHead2 xs = case xs of [] -> error "No head for empty lists!"
                        (x:_) -> x

describeList :: [a] -> String
describeList xs = "The list is " ++ case xs of [] -> "empty."
                                               [x] -> "a singleton list."
                                               xs -> "a longer list."

-- Recursion
-- Find maximum in a list
listMax :: (Ord a) => [a] -> a
listMax [] = error "Error"
listMax [x] = x
listMax (x:xs)
    | x > maxTail = x
    | otherwise = maxTail
    where maxTail = listMax xs

-- Find maximum in a list v2
listMax2 :: (Ord a) => [a] -> a
listMax2 [] = error "Error"
listMax2 [x] = x
listMax2 (x:xs) = myMax x (listMax2 xs)

-- Replicate X times Y number in a list
myReplicate :: (Num i, Ord i) => i -> a -> [a]
myReplicate n x
    | n <= 0    = []
    | otherwise = x:myReplicate (n-1) x

-- replicate' :: (Num i, Ord i) => i -> a -> [a]
-- replicate' n x
--     | n <= 0    = []
--     | otherwise = x:replicate' (n-1) x

-- Take element from a list
myTake :: (Num i, Ord i) => i -> [a] -> [a]
myTake n _
    | n <= 0 = []
myTake _ [] = []
myTake n (x:xs) = x : myTake (n-1) xs

-- Reverse a list
myReverse :: [a] -> [a]
myReverse [] = []
myReverse (x:xs) = myReverse xs ++ [x]


-- Repeat an element into a infinite list
myRepeat :: a -> [a]
myRepeat x = x : myRepeat x

-- Zipping two lists
myZip :: [a] -> [b] -> [(a, b)]
myZip [] _ = []
myZip _ [] = []
myZip (x:xs) (y:ys) = [(x, y)] ++ myZip xs ys
-- alternatively:
-- myZip (x:xs) (y:ys) = (x, y) : myZip xs ys

-- Is an elem in a list?
myElem :: (Eq a) => [a] -> a -> Bool
myElem [] _ = False
myElem (x:xs) y = if x == y then True else myElem xs y
-- alternatively:
-- myElem (x:xs) y
--     | x == y = True
--     | otherwise = myElem xs y

-- Implementation of quicksoft algorithm
quicksort :: (Ord a) => [a] -> [a]
quicksort [] = []
quicksort (x:xs) =
    let smaller = quicksort [y | y <- xs, y <= x]
        greater = quicksort [y | y <- xs, y > x]
    in smaller ++ [x] ++ greater

-- Higher order functions
-- Zip two lists With a function
myZipWith :: (a -> b -> c) -> [a] -> [b] -> [c]
myZipWith _ [] _ = []
myZipWith _ _ [] = []
myZipWith foo (x:xs) (y:ys) = foo x y : myZipWith foo xs ys

-- Flip arguments
myFlip :: (a -> b -> c) -> (b -> a -> c)
myFlip foo x y = foo y x

-- Map function
myMap :: (a -> b) -> [a] -> [b]
myMap _ [] = []
myMap foo (x:xs) = foo x : myMap foo xs

-- Filter function
myFilter :: (a -> Bool) -> [a] -> [a]
myFilter _ [] = []
myFilter foo (x:xs) =
    if foo x then
        x : myFilter foo xs
    else
        myFilter foo xs

-- Quicksort with filter
quicksort2 :: (Ord a) => [a] -> [a]
quicksort2 [] = []
quicksort2 (x:xs) =
    let smaller = quicksort (filter (<=x) xs)
        greater = quicksort (filter (>x) xs)
    in smaller ++ [x] ++ greater

-- Sum squares of all odd numbers less than 10000
-- takeWhile (<10000) [n^2 | n <- [1..], odd (n^2)]

-- Collatz sequence
collatzSeqOf :: (Integral a) => a -> [a]
collatzSeqOf 1 = [1]
collatzSeqOf x
    | even x = x : collatzSeqOf (x `div` 2)
    | odd x = x : collatzSeqOf (x * 3 + 1)

-- Lambda functions
-- "\ parameters -> body"
myFlip2 :: (a -> b -> c) -> (b -> a -> c)
myFlip2 f = \ x y -> f y x

-- Sum with foldl
sumFoldl :: (Num a) => [a] -> a
sumFoldl x = foldl (\ acc y -> acc + y) 0 x

-- Elem function with foldl
myElem2 :: (Eq a) => a -> [a] -> Bool
myElem2 y ys = foldl (\ acc x -> if x == y then True else acc) False ys
