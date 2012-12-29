cs50-password_cracker
=====================

The requirements for this assignment were to create a program in C that will take an DES hashed password and attempt to crack it.

I start by parsing a file of the 10,000 most commonly used passwords and comparing that to the known hash.
Then we check every word in /usr/share/dict/words
Finally I run a brute force attack iterating through every possible combination.
