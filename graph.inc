; 14 procedur


;------------------------------------------------------------
;  WE : Brak
;  WY : Brak
; info: Ustawia palete
;------------------------------------------------------------
spal:
 mov cx,0
 lea si,oldPal
 lea di,CurPal
 mov al,0
 mov dx,03c8h
 out dx,al
 inc dx
 _pal: 
  mov ax,cx
  shr ax,2
  xor bx,bx
    
  xchg bx,ax 
  out dx,al
  mov [si],al
  mov [di],al

  xchg bx,ax
  out dx,al
  mov [si+2],al
  mov [di+2],al

  xchg bx,ax
  out dx,al
  mov [si+1],al
  mov [di+1],al

  add si,3
  add di,3
  inc cx
  cmp cx,256
 jne _pal
ret

;------------------------------------------------------------
;  WE : ES - bufor
;  WY : Brak
; info: Gasi bufor o 5
;------------------------------------------------------------
gas:
 xor di,di
 mov cx,64000
 @petttt:
  mov al,es:[di]
  cmp al,5
  ja dal
   mov al,5
  dal:
  sub al,5
  mov es:[di],al
  inc di
 loop @petttt
ret


;---------------------------------------------------------
;  WE : X1, Y1, X2, Y2, Col : word;
;  WY : brak
; info: Rysuje linie
;---------------------------------------------------------
@c  equ [BP+4]
@y2 equ [BP+6]
@x2 equ [BP+8]
@y1 equ [BP+10]
@x1 equ [BP+12]

sgn: 
 cmp ax,0
 jg up_0
 jl _down_0
   xor ax,ax
   jmp kon_
 _down_0:
  mov ax,-1
  jmp kon_
 up_0:
  mov ax,1  
 kon_:
ret

Linia:
 push bp
 mov bp,sp

 mov di,@x1
 mov si,@x2
 mov ax,@y1

 mov cx,@x2
 sub cx,di ; cx = x2-x1
 mov bx,@y2
 sub bx,ax ; bx = y2-y1

 shl ax,6
 add di,ax
 add si,ax
 shl ax,2
 add di,ax ; di -adres 
 add si,ax ; si -adresX1->X2 

push ax
 mov ax,cx
 call sgn
 mov kierX,ax

 mov ax,bx
 call sgn
 mov kierY,ax
pop ax

 cmp cx,0
 jge _okk_3
  neg cx  
 _okk_3:    ; cx = abs(xs)

 cmp bx,0
 jge _okk_4
  neg bx
 _okk_4:    ; bx = abs(ys)

 mov dx,@c
 mov es:[di],dl
 
 cmp cx,bx
 jle _ver2

   mov ax,kierY
   cmp ax,0
   je kierY_0
   jg KierY_up   
    mov ax,-320
   jmp kierY_0    
   KierY_up:
    mov ax,320
   kierY_0:
   mov kierY,ax

   mov dx,cx
   shr dx,1
   neg dx
   mov count,dx  ; count =-(xs div 2)
 
   mov ax,@c   
pet___:
   cmp di,si
   je _papa__    
    add di,kierX
    add count,bx
    jle _rys11
     add di,kierY
     add si,kierY
     sub count,cx
   _rys11:
     mov es:[di],al
   jmp pet___
   _papa__:
   pop bp
  ret 10


   
 _ver2:
   mov ax,kierY
   cmp ax,0
   je kierYY_0
   jg KierYY_up   
    mov ax,-320
   jmp kierYY_0    
   KierYY_up:
    mov ax,320
   kierYY_0:
   mov kierY,ax

   mov si,@x1
   mov ax,@y2
   shl ax,6
   add si,ax
   shl ax,2
   add si,ax
      

   mov dx,bx
   shr dx,1
   neg dx
   mov count,dx  ; count =-(ys div 2)
 
   mov ax,@c   
pet___2:
   cmp di,si
   je _papa__2  
    add di,kierY
    add count,cx
    jle _rys112
     add di,kierX
     add si,kierX
     sub count,bx
   _rys112:
     mov es:[di],al
   jmp pet___2
   _papa__2:
nara___:
 pop bp
ret 10
 kierX DW ? 
 kierY DW ?
 count DW ?

;-----------------------------------
;  WE : DI - TIMAGE;  DX - nazwa
;  WY : brak
; info: wczytuje BMP
;-----------------------------------
Czytaj_BMP:
 call OpenFile
 mov handle,ax

 mov bx,ax
 mov cx,54
 lea dx,Naglowek
 call ReadFile     ; mamy  naglowek :)

 mov bx,handle
 mov cx,1024
 lea dx,pal
 call ReadFile     ; mamy  palete

 mov dx,03C8h
 xor al,al
 out DX,al
 inc dx
 lea si,pal
 mov cx,256
 @pet_:
  mov al,[SI+2]
  shr al,2
  out dx,al
  mov al,[SI+1]
  shr al,2
  out dx,al
  mov al,[SI+0]
  shr al,2
  out dx,al
  add si,4
 loop @pet_

 
 push di
  mov eax,[naglowek.ResX] 
  mov ebx,[naglowek.ResY]
  call GetImg 
 pop di

 mov dx,[DI+2] ; rozmiar
 mov cx,[DI+6] ; cx = Y
 mov bx,[DI+4] ; bx = X
 mov ax,[DI]   ; ax - segm
 sub dx,bx     ; rozmiar - bx
 mov di,dx
 push es
 mov es,ax

 Pet_y_:
  push cx
  push bx
   mov cx,bx
   mov bx,handle
   lea dx,pal
   call ReadFile     ; mamy  palete          
  pop bx
   lea si,pal
   mov cx,bx
   push di
   rep movsb
   pop di
   sub di,bx

   mov cx,bx
   and cx,3
   jz _ok_
    push bx
     mov bx,4
     sub bx,cx  ; ile trzeba doczytaæ?

     mov cx,bx
     mov bx,handle
     lea dx,pal
     call ReadFile     ; mamy  palete          
    pop bx  
  _ok_:
  pop cx
  loop Pet_y_
 pop es
ret
 Handle DW ?


;-----------------------------------
;  WE : brak
;  WY : brak
; info: czeka na powrót pionowy
;-----------------------------------
czekaj:
  push ax
  push dx
  mov dx, 03dah
  @wait:
  in al, dx
  and al, 08h
  jz @wait
  pop dx
  pop ax
ret

;-----------------------------------
;  WE : AL - kolor;  es - segment
;  WY : BRAK
; info: Czyœci segment 
;-----------------------------------
Fill:
 xor di,di
 mov ah,al
 push ax
 shl eax,16
 pop ax
 mov cx,16000
 rep stosd
ret

;--------------------------------
;  WE : ax - segment
;  WY : BRAK
; info: kopiuje segment na ekran
;--------------------------------
Blit:
 push ds
 push es
 mov ds,ax
 mov ax,0A000h
 mov es,ax
 xor di,di
 xor si,si
 mov cx,16000
 rep movsd
 pop es
 pop ds
ret

;------------------------------------------------
;  WE : DI - x;  AX - y;  BL-kolor;  es-segment
;  WY : BRAK
; Info: Rysuje pixel w buforze
;------------------------------------------------
plot:
 shl ax,6
 add di,ax
 shl ax,2
 add di,ax
 mov al,bl
 stosb
ret

;-------------------------------------------------------
;  WE : DI - x;  AX - y;  CX - x2 BL-kolor;  es-segment
;  WY : BRAK
; Info: Rysuje poziom¹ linie w buforze
;-------------------------------------------------------
hline:
 cmp di,cx
 jb _ok
  xchg cx,di
 _ok:
 sub cx,di
 inc cx
 shl ax,6
 add di,ax
 shl ax,2
 add di,ax
 mov al,bl
 rep stosb
ret

;-------------------------------------------------------
;  WE : DI - x;  AX - y;  CX - y2 BL-kolor;  es-segment
;  WY : BRAK
; Info: Rysuje pionow¹ pixel w buforze
;-------------------------------------------------------
vline:
 cmp ax,cx
 jb __ok
  xchg ax,cx
 __ok:
 sub cx,ax
 inc cx
 shl ax,6
 add di,ax
 shl ax,2
 add di,ax
 mov al,bl
 @_pet_y:
  stosb
  add di,319
 loop @_pet_y
ret

;-------------------------------
;   WE: SI - TIMG; AL - kolor
;   WY: brak
; info: Czyœci rysunek 
;-------------------------------
FILL_IMG:
 push es
  mov BX,[SI] 
  mov CX,[SI+2] ; rozmiar do cx
  mov es,bx     ; w ES handle rysunku  
  xor di,di     ; czyœæ offset rysunku
  rep stosb
 pop es
ret

;------------------------------------------------
;   WE: SI - TIMG; ES - Bufor; DI - X; AX - Y
;   WY: Brak
; info: Kopiuje rysunek do bufora na pozycje(X,Y)
;------------------------------------------------
IMG_TO_BUF:
 shl ax,6
 add di,ax
 shl ax,2
 add di,ax     ; ES : DI <- cell
 mov ax,[SI]   ; Seg Ÿród³a :)
 mov cx,[SI+6] ; ResY 
 mov BX,[SI+4] ; ResX
 mov DX,320    ; Szerokoœæ buforka
 sub dx,bx     ; DX = 320 - ResX
 push ds       
 mov ds,ax
 xor si,si
 _pet_y:
  push cx
   mov cx,bx
   rep movsb
   add di,dx
  pop cx
 loop _pet_y
 pop ds
ret

;---------------------------------------------------
;   WE: Wspolzendne na stosie x1,y1,x2,y2,x3,y3,col
;   WY: Brak
; info: Rysuje trójk¹t
;---------------------------------------------------
; Name: Poly
; WE  : x1, y1, x2, y2, x3, y3 : word;  c:word;
; STOS:
;       ???? ???? ????  ????  ????  ????  ????  ????  ????
;       ^^^^ ^^^^ ^^^^  ^^^^  ^^^^  ^^^^  ^^^^  ^^^^  ^^^^
;        BP   IP   C     y3    x3    y2    x2    y1    x1
; WY  : BRAK

 Radx  DD ?
 Ladx  DD ?

 @@c  equ [BP+4]

 @@y3 equ [BP+6]
 @@x3 equ [BP+8]

 @@y2 equ [BP+10]
 @@x2 equ [BP+12]

 @@y1 equ [BP+14]
 @@x1 equ [BP+16]

 Poly:
  push bp
  mov  bp,sp

;SORT PROC
  mov ax,@@y1  
  cmp ax,@@y2
  jb __ok__
   mov bx, @@y2
   mov @@y2, ax
   mov @@y1, bx

   mov bx, @@x2
   mov @@x2, ax
   mov @@x1, bx
  __ok__:

  mov ax,@@y2  
  cmp ax,@@y3
  jb _ok2
   mov bx, @@y3
   mov @@y3, ax
   mov @@y2, bx

   mov bx, @@x3
   mov @@x3, ax
   mov @@x2, bx
  _ok2:

  mov ax,@@y1  
  cmp ax,@@y2
  jb _ok3
   mov bx, @@y2
   mov @@y2, ax
   mov @@y1, bx

   mov bx, @@x2
   mov @@x2, ax
   mov @@x1, bx
  _ok3:  
; END SORT

  mov cx,@@y1
  cmp cx,@@y3
  je _bye_proc  

  cmp cx,@@y2
  je _no_1

    mov cx,@@y3
    sub cx,@@y1     ; cx = (y3-y1);
    and ecx,0ffffh  ; zeruj góre ecx...

    mov ax,@@x3
    and eax,0ffffh  
    mov bx,@@x1
    and ebx,0ffffh  
    sub eax,ebx
    mov ebx,65536
    xor edx,edx
    imul ebx
    idiv ecx
    mov Radx,eax


    mov cx,@@y2
    sub cx,@@y1     ; cx = (y2-y1);

    mov ax,@@x2
    and eax,0ffffh  
    mov bx,@@x1
    and ebx,0ffffh  
    sub eax,ebx
    mov ebx,65536
    xor edx,edx
    imul ebx
    idiv ecx
    mov Ladx,eax
       
    xor eax,eax
    mov ax,@@x1
    mov edx,65536
    mul edx
    mov ebx,eax    

    mov dx,@@y1

    @pet_1:
     push eax
     push ebx
     push ecx
     push dx

     cmp ebx,eax
     jb _ok__
       xchg ebx,eax
     _ok__:
      mov ecx,eax
      sub ecx,ebx
      shr ecx,16

      mov edi,ebx
      shr edi,16
      shl dx,6
      add di,dx
      shl dx,2
      add di,dx
      push cx
      shr cx,2
      jz _nie_po_4_
       mov al,@@c
       mov ah,al
       push ax
       shl eax,16
       pop ax  
       rep stosd
     _nie_po_4_:
      pop cx
      and cx,3
      jz _papa_l_
       mov al,@@c
       rep stosb
     _papa_l_:
     pop dx
     pop ecx
     pop ebx
     pop eax
 
     add eax,Radx
     add ebx,Ladx
     inc dx
    cmp dx,@@y2  
    jne @pet_1
    jmp faza_2 

  _no_1:
  xor ecx,ecx
  mov cx,@@y3
  sub cx,@@y1  ; cx = y3-y1

  xor edx,edx
  xor eax,eax
  xor ebx,ebx
  mov ax,@@x3
  mov bx,@@x1  
  sub eax,ebx   ; x3-x1
  mov ebx,10000h
  imul ebx      ; (x3-x1)*65536
  idiv ecx      ; (x3-x1)*65536/(y3-y1)
  mov Radx,eax  

  xor edx,edx
  xor eax,eax
  mov ax,@@x1
  mov ebx,65536
  imul ebx  

 faza_2:            ; w eax mamy x (x3-x1)
  xor ecx,ecx
  mov cx,@@y3
  cmp @@y2,cx
 je _bye_proc
  push eax
  xor ecx,ecx
  mov cx,@@y3
  sub cx,@@y2  ; cx = y3-y2      

  xor edx,edx
  xor eax,eax
  xor ebx,ebx
  mov ax,@@x3
  mov bx,@@x2  
  sub eax,ebx   ; x3-x2
  mov ebx,10000h
  imul ebx      ; (x3-x2)*65536
  idiv ecx      ; (x3-x2)*65536/(y3-y2)
  mov Ladx,eax  

  xor edx,edx
  xor eax,eax
  mov ax,@@x2
  mov ebx,65536
  imul ebx  
  mov ebx,eax

  pop eax
  mov dx,@@y2
   @pet_2:
     push eax
     push ebx
     push ecx
     push dx

     cmp ebx,eax
     jb _ok2__
       xchg ebx,eax
     _ok2__:
      mov ecx,eax
      sub ecx,ebx
      shr ecx,16

      mov edi,ebx
      shr edi,16
      shl dx,6
      add di,dx
      shl dx,2
      add di,dx
      push cx
      shr cx,2
      jz _nie_po_4
       mov al,@@c
       mov ah,al
       push ax
       shl eax,16
       pop ax  
       rep stosd
     _nie_po_4:
      pop cx
      and cx,3
      jz _papa_l
       mov al,@@c
       rep stosb
     _papa_l:

     pop dx
     pop ecx
     pop ebx
     pop eax
 
     add eax,Radx
     add ebx,Ladx
     inc dx
    cmp dx,@@y3  
    jne @pet_2


 _bye_proc:
  pop bp
 ret 14




;-------------------------------------------
;   WE: brak
;   WY: Flaga Z = 1 nie naciœniêty klawisz
; info: Sprawdza czy klepniêto w klawisz 
;-------------------------------------------
KEYPRESSED: 
 push ax
 mov ah,1
 int 16h
 pop ax
ret

 ADXL DD ?
 ADXR DD ?

tnaglowek struc
; naglowek pliku bitmapy
 id      DW ?
 size_f  DD ?
 res     DD ?
 Jump    DD ?

; nag. informacyjny
 Size_ni DD ?
 ResX    DD ?
 ResY    DD ? 
 Planes  DW ?
 BPP     DW ?
 compr   DD ?
 size_b  DD ?
 DPI_X   DD ?
 DPI_Y   DD ?
 colors  DD ?
 Used_c  DD ?
ends

naglowek TNAGLOWEK ?
pal      DB 1024 dup(?)

include files.inc