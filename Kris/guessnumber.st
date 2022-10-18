"guess number"
+Object subclass: #GuessNumber 
!GuessNumber
runGame
    | t g notdone |
    t <- 10 atRandom.
    notdone  <- true.
    'new game!' printNl.

    [ notdone ] whileTrue: [
        'guess? ' print.
        g <- (String input) asNumber.
        g printNl.
        ( t == g) ifTrue: [
            notdone <- false.
            'You got it ' printNl.
            t printNl.
            ]
        ifFalse: [
        ( t > g ) ifTrue: [
            'too low' printNl.
            ]
            ifFalse: [
            'too high' printNl.
            ].
        ]
        ].
    'Done' printNl.
!
+GuessNumber new runGame.
/ end of file