/ palindrome
!String
isPalindrome | string |
    string <- self select: [:c | c isAlphabetic].
    string <- string collect: [:c | c lowerCase].
    ^ string = string reverse
!
/ next, add a test case 
+Object subclass: #Palindrome
!Palindrome
test: aString
    aString print.
    ' : ' print.
    aString isPalindrome print.
    Char newline print.
!
!Palindrome
runTests
    self test: 'A Man, A Plan, A Canal, Panama'.
    self test: 'foo bar baz'.
    self test: 'radar'
!
+Palindrome new runTests
/ end of file
