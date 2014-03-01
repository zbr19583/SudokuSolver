A Sudoku Solver 

EBNF

Input:
<input> ::= <puzzle> <eof>

<puzzle> ::= 81*<value> "\n"

<value> ::= "." | "1" | ... | "9"

Outputs:

<output> ::= <result> <eof>

<result> ::= <error> | <nosolution> | <puzzle> <puzzle>*

<error> ::= "ERROR: expected " <expected> " saw " <saw> "\n"

<expected> ::= "<value>" | "\\n" | "<eof>"

<saw> ::= "<eof>" | "\\n" | <printable> | "\\x" 2*<hex-digit>

<printable> ::= (* all characters c where isprint(c) is true *)

<hex-digit> ::= '0' | '1' | ... | 'a' | 'b' | 'c' | 'd' | 'e' | 'f'

<nosolution> ::= "No solutions.\n"

<puzzle> ::= 81*<value> "\n"

<value> ::= "." | "1" | ... | "9"
