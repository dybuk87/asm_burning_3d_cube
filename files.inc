; 8 procedur 

NewFile proc  ; ds:dx - nazwa w ASCIIZ
 mov ah,3ch
 xor cx,cx 
 int 21h
 ret
endp

OpenFile proc ; ds:dx - nazwa w ASCIIZ wy: ax-Handle
 mov ah,3dh
 mov AL,2
 int 21h
 ret
endp
 
ReadFile proc ;WA : BX - handle  cx -ilosc w bajtach 
              ;WY : ds:dx -> wyjscie danych
 mov ah, 03fh
 int 21h
 jnc DalejRead
  mov ax,0003h
  int 10h 
  mov ah,4ch
  int 21h
  DalejRead:
  ret
endp

EndFile proc ;bx-handle  Wy: DX:AX=aktualna pozycja kursora 
 mov ah,42h 
 xor cx,cx
 mov dx,cx
 mov al,2
 ret
endp

Zapisz proc ;bx-handle cx-size ds:dx-zrodlo
 mov ah,40h
 int 21h 
 ret
endp


FileExists proc ;we: ds:dx-nazwa; wy al=1 istnieje 9Eh-nazwa
 MOV AH,4EH 
 mov cx,cx  
 int 21h
 mov al,1
 jnc ok
  xor al,al
 ok:
 ret
endp

FileSize proc  ; we: bx - Handle 
               ; wy  dx : ax  <-rozmiar 
 mov ah,42h
 mov al,2
 xor cx,cx
 xor dx,dx
 int 21h
 push dx 
 push ax


 mov ah,42h
 mov al,0
 xor cx,cx
 xor dx,dx
 int 21h
 pop ax
 pop dx 
ret
endp

Close proc  ;bx - dojscie
 mov ah,3eh
 ret
endp