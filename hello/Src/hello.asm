        

bdos    equ     5
;
putstr  equ     9
;
        org     100h
           
        ld      c, putstr
        ld      de, hello
        call    bdos
        ret
       
hello:   db      "Hello World!$",13,10

