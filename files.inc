; 8 procedur 

NewFile proc  ; ds:dx - file name in ASCIIZ
 mov ah,3ch
 xor cx,cx 
 int 21h
 ret
endp

OpenFile proc ; ds:dx - file name in ASCIIZ wy: ax-Handle
 mov ah,3dh
 mov AL,2
 int 21h
 ret
endp
 
ReadFile proc ;WA : BX - handle,  cx - byte count, ds:dx -> data output buffer
              ;WY : data saved in buffer ds:dx 
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

EndFile proc ;bx-handle  Wy: DX:AX=current file cursor position 
 mov ah,42h 
 xor cx,cx
 mov dx,cx
 mov al,2
 ret
endp

Write proc ;bx-handle cx - size ds:dx - source
 mov ah,40h
 int 21h 
 ret
endp


FileExists proc ;we: ds:dx - file name in ASCIIZ; 
				; wy: al=1 file exists 9Eh - file name
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
               ; wy  dx : ax  <- file size
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

Close proc  ;bx - file handler
 mov ah,3eh
 ret
endp