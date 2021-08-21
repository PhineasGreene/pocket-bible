# Pocket Bible
The goal of this project is to go from the 4361566 byte ASCII King James Bible (kjv.bible) to something much much smaller.
## Compressed Bibles
I haven't even scraped tiny yet. See individual READMEs for more. Ratio is `Original Size / (Compressed Size + Decompressor Size)`.
| Bible | Compressed Size | Decompressor Size | Ratio |
|-------|-----------------|-------------------|-------|
|Huffman + PrePro| 2293551 bytes| 37624 bytes | 1.87  |
|Huffman| 2456732 bytes   | 21224 bytes       | 1.76  |
|6-Bit  | 3147819 bytes   | 16512 bytes       | 1.38  |
|PrePro | 4195769 bytes   | 16400 bytes       | 1.04  |
|ASCII  | 4361566 bytes   | 0 bytes           | 1     |
