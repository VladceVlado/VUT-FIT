FLP - Příprava ke zkoušce 2020: Haskell
=

# Co znát

## Konstrukce
- if-then-else
- guards
- definice vlastnich datovych typu
- where
- let in
- monads

## Funkce z Prelude

- map
- filter
- foldr, foldl
- scanr, scanl
- zip, zipWith
- take, takeWhile
- drop, dropWhile
- read
- show
- head
- last
- tail
- length
- elem

## Složené datové typy

```haskell
data Either a b
    = Left a
    | Right b
    deriving (Eq, Ord, Read, Show)
```

```haskell
data Maybe a
    = Nothing
    | Just a
    deriving (Eq, Ord, Read, Show)
```

```haskell
data Ordering
    = LT
    | EQ
    | GT
    deriving (Eq, Ord, Bounded, Enum, Read, Show)
```

<!-------------------------------------------------------->

# Půlsemestrálka 2020

## 1)

V jazyku Haskell definujte typ pro reprezentaci vyhledávacı́ho binárnı́ho stromu nad klı́čem a hodnotou dopředu neznámého typu. Definujte funkci `ins`, která pro zadaný klı́č a hodnotu provede vloženı́ hodnoty a úpravu daného stromu dle vašeho typu. Situaci, že pro daný klı́č již hodnota existuje, vhodně ošetřete, úprava stromu se provádět nebude, vrátı́ se původnı́ strom a informace o tom, že vloženı́ selhalo. Volně využijte obsah Prelude.

```haskell
-- v1
data BST a b
    = Node a b (BST a b) (BST a b)
    | Leaf
    deriving (Show)

ins :: Ord a => a -> b -> BST a b -> (Bool, BST a b)
ins key val Leaf = (True, Node key val Leaf Leaf)
ins key val node@(Node nKey nVal left right)
    | key == nKey = (False, node)
    | key < nKey = (leftBool, Node nKey nVal newLeft right)
    | otherwise = (rightBool, Node nKey nVal left newRight)
  where
    (leftBool, newLeft) = ins key val left
    (rightBool, newRight) = ins key val right
```

```haskell
-- v2: with Either
data BST a b
    = Node a b (BST a b) (BST a b)
    | Leaf
    deriving (Show)

ins :: Ord a => a -> b -> BST a b -> Either (BST a b) (BST a b)
ins key val Leaf = Right (Node key val Leaf Leaf)
ins key val node@(Node nKey nVal left right)
    | key == nKey = Left node
    | key < nKey = update (\newLeft -> Node nKey nVal newLeft right) (ins key val left)
    | otherwise = update (\newRight -> Node nKey nVal left newRight) (ins key val right)
  where
    update :: (a -> b) -> Either a a -> Either b b
    update foo (Left left)   = Left $ foo left
    update foo (Right right) = Right $ foo right
```

## 2)

V jazyku Haskell definujte typ pro reprezentaci vyhledávacı́ho binárnı́ho stromu nad klı́čem a hodnotou dopředu neznámého typu. Definujte funkci `put`, která pro zadaný klı́č a hodnotu provede vloženı́ hodnoty a úpravu daného stromu dle vašeho typu. Pokud je vı́ce hodnot spojených se stejným klı́čem, tak je ukládá do seznamu k danému klı́či, ale neduplikuje je, tedy každá hodnota je v seznamu jen 1x!

```haskell
data BST a b
    = Node a [b] (BST a b) (BST a b)
    | Leaf
    deriving (Show)

putt :: Ord a => Eq b => a -> b -> BST a b -> BST a b
putt key val Leaf = Node key [val] Leaf Leaf
putt key val node@(Node nKey nVal left right)
    | key == nKey = Node nKey (update val nVal) left right
    | key < nKey = Node nKey nVal (putt key val left) right
    | otherwise = Node nKey nVal left (putt key val right)
  where
    update :: Eq b => b -> [b] -> [b]
    update val list =
        if val `elem` list
            then list
            else val : list
```

## 3)

V jazyku Haskell nadefinujte datový typ pro reprezentaci aritmetických výrazů s operacemi násobenı́ a sčı́tánı́ nad celými čı́sly a proměnnými -- typ proměnné je dopředu neznámý. Definujte funkci `ez`, která využije vlastnosti čı́sla nula vzhledem k operaci násobenı́ a co nejvı́ce zjednodušı́ výraz, který je dán jako parametr a ten zjednodušený vrátı́ jako výsledek. Dbejte na to "co nejvı́ce". Volně využijte obsah Prelude.

```haskell
data Expr a
    = Add (Expr a) (Expr a)
    | Mul (Expr a) (Expr a)
    | Var a
    | LInt Integer
    deriving (Show, Eq)

ez :: Eq a => Expr a -> Expr a
ez (Mul expr1 expr2) =
    if val1 == LInt 0 || val2 == LInt 0
        then LInt 0
        else Mul val1 val2
  where
    val1 = ez expr1
    val2 = ez expr2
ez (Add (LInt int1) (LInt int2)) = LInt (int1 + int2)
ez expr = expr
```

## 4)

V jazyku Haskell nadefinujte datový typ pro reprezentaci aritmetických výrazů s operacemi násobenı́ a sčı́tánı́ nad celými čı́sly a proměnnými -- typ proměnné je dopředu neznámý. Definujte funkci `eo`, která využije vlastnosti čı́sla jedna vzhledem k operaci násobenı́ a co nejvı́ce zjednodušı́ výraz, který je dán jako parametr a ten zjednodušený vrátı́ jako výsledek. Dbejte na to "co nejvı́ce". Volně využijte obsah Prelude.

```haskell
data Expr a
    = Add (Expr a) (Expr a)
    | Mul (Expr a) (Expr a)
    | Var a
    | LInt Integer
    deriving (Show, Eq)

eo :: Eq a => Expr a -> Expr a
eo (Mul expr1 expr2)
    | val1 == LInt 1 = val2
    | val2 == LInt 1 = val1
    | otherwise = Mul val1 val2
  where
    val1 = eo expr1
    val2 = eo expr2
eo (Add (LInt int1) (LInt int2)) = LInt (int1 + int2)
eo expr = expr
```

## 5)

V jazyku Haskell nadefinujte datový typ pro reprezentaci booleovských výrazů s operacemi logického součtu, součinu i negace nad booleovskými hodnotami a proměnnými -- typ proměnné je dopředu neznámý. Definujte funkci `dm`, která má dva parametry, tabulku symbolů a booleovský výraz dle vašeho typu. Tabulka symbolů je seznam dvojic proměnná, hodnota. Funkce `dm` odstranı́ dvojı́ negaci, aplikuje deMorganova pravidla a pro proměnné obsažené v tabulce dosadı́ jejich hodnoty. Vše s maximálnı́ účinnostı́. Volně využijte obsah Prelude.

```haskell
data BExpr a
    = And (BExpr a) (BExpr a)
    | Or (BExpr a) (BExpr a)
    | Neg (BExpr a)
    | BEVar a
    | BEVal Bool
    deriving (Show)

dm :: Eq a => [(a, Bool)] -> BExpr a -> BExpr a
dm table (Neg (Neg expr)) = dm table expr
dm table (Neg (And expr1 expr2)) = Or (dm table (Neg expr1)) (dm table (Neg expr2))
dm table (Neg (Or expr1 expr2)) = And (dm table (Neg expr1)) (dm table (Neg expr2))
dm table (BEVar a) = mkval table
  where
    mkval [] = BEVar a
    mkval ((var, boolVal):xs) =
        if var == a
            then BEVal boolVal
            else mkval xs
dm _ expr = expr
```

## 6)

V jazyku Haskell nadefinujte datový typ pro reprezentaci aritmetických výrazů s operacemi násobenı́ a sčı́tánı́ nad proměnnými -- typ proměnné je dopředu neznámý. Definujte funkci `tr`, která má jako parametr výraz definovaný dle vašeho typu. Na základě vlastnosti asociativity a komutativity operace sčı́tánı́ pro reálná čı́sla upravı́ datovou reprezentaci tak, aby při vyhodnocenı́ post-order průchodem bylo sčı́tánı́ jednotlivých operandů prováděno v maximálnı́ mı́ře zleva doprava. Pokud pro pořadı́ použijeme v linearizovaném zápisu závorky, tak by např. takovýto výraz `(a + b) + (c + d)` byl transformován na `((a + b) + c) + d`, nebo odbobný až na pořadı́ proměnných, nicméně význam výrazu se změnit nesmı́. Volně využijte obsah Prelude.

```haskell
data Expr a
    = Add (Expr a) (Expr a)
    | Mul (Expr a) (Expr a)
    | Var a
    deriving (Show)

tr :: Expr a -> Expr a
tr (Mul expr1 expr2)              = Mul (tr expr1) (tr expr2)
tr expr@(Add (Var _) (Var _))     = expr
tr (Add expr (Var a))             = Add (tr expr) (Var a)
tr (Add (Var a) expr2)            = Add (tr expr2) (Var a)
tr (Add expr@(Add _ _) (Add c d)) = Add (Add (tr expr) (tr c)) (tr d)
tr (Add expr1 expr2)              = Add (tr expr1) (tr expr2)
tr expr                           = expr
```

<!-------------------------------------------------------->

# Řádný termín 2019

Nadefinujte datový typ `DirTree`. Napiště funkci `dirSize`, která spočítá velikost obsahu adresáře. Napiště funkci `findAllPref`, která vrátí seznam souborů a adresářů, který obsahuje zadaný prefix. Napiště funkci `printDir`, která rekurzivně vypíše obsah adresáře. Výpis může vypadat např. takto:

```haskell
dir = Dir "root" [Dir "bin" [File "python" 100, File "gcc" 50],
    Dir "var" [File "log" 25], Dir "home" [], File "swap" 1000]

printDir dir
```

```
root
  bin
    python 100
    gcc 50
  var
    log 25
  home
  swap 1000
```

```haskell
data DirTree
    = File String Integer
    | Dir String [DirTree]
    deriving (Show)
```

```haskell
dirSize :: DirTree -> Integer
dirSize (File _ size)  = size
dirSize (Dir _ dirs) = foldr ((+) . dirSize) 0 dirs
```

```haskell
findAllPref :: String -> DirTree -> [String]
findAllPref prefix (File name _) = [name | matchPref prefix name]
findAllPref prefix (Dir name []) = [name | matchPref prefix name]
findAllPref prefix (Dir name dirTrees) =
    if matchPref prefix name
        then name : findInList prefix dirTrees
        else findInList prefix dirTrees
  where
    findInList :: String -> [DirTree] -> [String]
    findInList _ [] = []
    findInList prefix (tree:trees) = findAllPref prefix tree
        ++ findInList prefix trees

matchPref :: String -> String -> Bool
matchPref _ ""                        = False
matchPref "" _                        = True
matchPref prefix@(x:xs) string@(y:ys) = (x == y) && matchPref xs ys
```

```haskell
printDir :: DirTree -> IO ()
printDir dirTree = putStr $ getTreesStr [dirTree] 0
  where
    getTreesStr :: [DirTree] -> Integer -> String
    getTreesStr [] _ = ""
    getTreesStr (File name size:trees) indent = getIndent indent ++ name
        ++ " " ++ show size ++ "\n"
        ++ getTreesStr trees indent
    getTreesStr (Dir name dirTrees:trees) indent =
        getIndent indent ++ name ++ "\n" ++ getTreesStr dirTrees (indent + 1)
        ++ getTreesStr trees indent
    getIndent :: Integer -> String
    getIndent 0 = ""
    getIndent i = "  " ++ getIndent (i - 1)
```

<!-------------------------------------------------------->

# První opravný termín 2019

Definovat datový typ pro lambda kalkul `LE`. A poté práci nad něma.

```haskell
data LE
    = Var String
    | Abs String LE
    | App LE LE
    deriving (Show)

...
```

<!-------------------------------------------------------->

# Řádný termín 2018

Datový typ `DLog` pro zpracování logů ze souboru.
Nadefinovat funkci `pl`, která bere název souboru jako argument. Z tohoto souboru načte záznamy ve formátu `Integer#String`, případně prázdný řádek. Záznam reprezentuje datovým typem `DLog`. Dále vypíše všechny záznamy s čísly, které jsou násobkem 5 (`NUM mod 5 == 0`). Ve výpisu budou oddělené dvojtečkou (`:`). Je třeba uvést typové definice pro každou použitou funkci. Zadané byly typové definice některých funkcí pro práci s `IO` (`openFile`, `hGetContents`, `lines`, `unlines`, `ReadMode`, `WriteMode`, apod.).

```haskell
import           Data.Maybe
import           System.IO

data DLog
    = Empty
    | IT Integer String
    deriving (Show, Eq)

pl :: String -> IO ()
pl fileName = do
    fileHandle <- openFile fileName ReadMode
    content <- hGetContents fileHandle
    pr $ procLines $ lines content
    hClose fileHandle

procLines :: [String] -> [DLog]
procLines [] = []
procLines (line:lines) =
    if null line
        then Empty : procLines lines
        else readLog line : procLines lines
  where
    readLog :: String -> DLog
    readLog line = IT (read (takeWhile (/= '#') line) :: Integer) (tail $ dropWhile (/= '#') line)

pr :: [DLog] -> IO ()
pr [] = return ()
pr (Empty:dLogs) = pr dLogs
pr (IT int dLog:dLogs) =
    if int `mod` 5 == 0
        then putStrLn (show int ++ ":" ++ dLog) >> pr dLogs
        else pr dLogs
```

<!-------------------------------------------------------->

# Vlastní příklad -- Výrazy pro "bezpečné" dělení

```haskell
data Expr
    = Val Double
    | Div Expr Expr
    deriving (Show)

eval :: Expr -> Maybe Double
eval (Val n) = Just n
eval (Div x y) =
    case eval x of
        Nothing -> Nothing
        Just n ->
            case eval y of
                Nothing -> Nothing
                Just m  -> safeDiv n m
  where
    safeDiv :: Double -> Double -> Maybe Double
    safeDiv n m =
        if n == 0
            then Nothing
            else Just (n / m)
```

- Za využití syntaxe pro práci s monády

```haskell
data Expr
    = Val Double
    | Div Expr Expr
    deriving (Show)

eval :: Expr -> Maybe Double
eval (Val n) = return n
eval (Div x y) = do
    n <- eval x
    m <- eval y
    safeDiv n m
  where
    safeDiv :: Double -> Double -> Maybe Double
    safeDiv n m =
        if n == 0
            then Nothing
            else Just (n / m)
```
