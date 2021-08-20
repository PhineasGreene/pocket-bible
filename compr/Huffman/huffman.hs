import System.Environment
import System.IO
import Data.ByteString
import Data.Bits
import Data.Function (on)
import Data.List

data Tree = Leaf Char | Branch Tree Tree

instance Show (Tree) where
    show (Leaf c) = show c
    show (Branch l r) = "{" ++ (show l) ++ ", " ++ (show r) ++ "}"

countChars :: String -> [(Tree, Int)]
countChars s = map (\(c, i) -> (Leaf c, i)) $ map (\x -> (head x, length x)) $ group $ sort s

hufftree :: [(Tree, Int)] -> Tree
hufftree s = fst $ (hufftree' (sorttree s)) !! 0
  where sorttree  :: [(Tree, Int)] -> [(Tree, Int)]
        sorttree t = sortBy (flip compare `on` snd) t
        hufftree' :: [(Tree, Int)] -> [(Tree, Int)]
        hufftree' [t] = [t]
        hufftree' ((t1, f1):(t2, f2):[]) = [(Branch t1 t2, f1+f2)]
        hufftree' t = hufftree' $ sorttree (h ++ [b])
           where len = length t
                 l = t !! (len - 1)
                 r = t !! (len - 2)
                 b = (Branch (fst l) (fst r), (snd l) + (snd r))
                 h = (take (len -2) t)

--[ For storing the character frequency list
-- in the compressed file. ]--
printCount :: [(Tree, Int)] -> String
printCount [] = ""
printCount (((Leaf c), i):x) = (c:(show i)) ++ (printCount x)

encodeChar :: Char -> Tree -> [Bool]
encodeChar _ (Leaf _) = []
encodeChar c (Branch l r) | search l c = False:(encodeChar c l)
                          | search r c = True:(encodeChar c r)
    where search :: Tree -> Char -> Bool
          search (Leaf t) c = t == c
          search (Branch l r) c = (search l c) || (search r c)

encode :: [(Tree, Int)] -> String -> String
encode = undefined

printBits :: Char -> String
printBits c = (str 0) ++ (str 1) ++ (str 2) ++ (str 3) ++ (str 4) ++ (str 5) ++ (str 6) ++ (str 7)
    where str i = if testBit c i then "1" else "0"

main = do
    args <- getArgs
    bibText <- readFile $ args !! 0
    let count = countChars bibText
    let header = printCount count
    let out = header ++ (encode count bibText)
    writeFile (args !! 1) out
