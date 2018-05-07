; 3 procedury

;-----------------------------------------
;   WE: Brak
;   WY: BX - przyciski; CX - X; DX - Y
; info: Pyta o pozycje myszki na ekranie
;-----------------------------------------
pozycja_myszy:
 mov ax,03
 int 33h
 shr cx,1
ret


;-----------------------------------------
;   WE: Brak
;   WY: Brak
; info: rysuje kursor jako "c(w)elownik"
;-----------------------------------------
Red_Cross:
 pusha
 call pozycja_myszy
 push cx

 mov  ax,dx
 mov  di,0
 mov  cx,319
 mov  bl,4
 call hline 

 pop cx
 mov di,cx
 mov ax,0
 mov cx,199
 call vline
 popa
ret


;----------------------------------------------
;   WE: SI - TIMG; ES - Bufor; 
;   WY: Brak
; info: Rysuje myszke jako rysunek (np. z BMP)
;----------------------------------------------
Pokaz_mysz:
 call pozycja_myszy
 mov di,cx
 mov ax,dx
 call IMG_TO_BUF
ret