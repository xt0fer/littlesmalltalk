/ palindrome
!String
isPalindrome | string |
    string <- self select: [:c | c isAlphabetic].
    string <- string collect: [:c | c lowerCase].
    ^ string = string reverse
!
/ next, add a test case 
+Object subclass: #Pal 
!Pal
test: aString
    aString print.
    ' : ' print.
    aString isPalindrome print.
    Char newline print.
!
!Pal
runTests
    self test: 'A Man, A Plan, A Canal, Panama'.
    self test: 'foo bar baz'.
    self test: 'radar'
!
+Pal new runTests
/ end of file

