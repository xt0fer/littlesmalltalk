# Some smaples

## guessnumber.st

you can do this in LST

```
-> File fileIn: 'guessnumber.st'
subclass created: GuessNumber
method inserted: runGame
new game!
guess? 10
10
too high
guess? 5
5
too low
guess? 7
7
You got it 
7
Done
GuessNumber
file in completed
-> 
```

or even 

```
-> File fileIn: 'palindrome.st'
method inserted: isPalendrome
subclass created: Pal
method inserted: test:
method inserted: runTests
A Man, A Plan, A Canal, Panama : true
foo bar baz : false
radar : true
Pal
file in completed
-> 
```