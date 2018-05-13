; 3 procedury

;-----------------------------------------
;   WE: Brak
;   WY: BX - button state; CX - X; DX - Y
; info: Get mouse position and button state
;-----------------------------------------
MousePos:
 mov ax,03
 int 33h
 shr cx,1
ret


;-----------------------------------------
;   WE: Brak
;   WY: Brak
; info: draw simple cursor as cross
;-----------------------------------------
RedCross:
 pusha
 call MousePos
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
; info: Draw mouse as Image
;----------------------------------------------
ShowMouse:
 call MousePos
 mov di,cx
 mov ax,dx
 call IMG_TO_BUF
ret