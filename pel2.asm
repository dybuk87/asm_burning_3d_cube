.model tiny 
.386p
.code 
 org 100h 
  start:

  call init_mem
  mov ax,cs
  mov ds,ax
  call Get_Buf

  mov ax,buf1
  mov es,ax    ; es permanently locked as buf 1
  mov ax,0013h 
  int 10h      
  
  call spal
  call initMath3D


xor al,al
call fill

call start_stoper
Program:   
call inc_stoper

mov ax,ile_p
lea si,point
lea di,tempp
call copy

mov ax,angx
inc ax
xor ah,ah
mov angx,ax

mov bx,angy
inc bx
xor bh,bh
mov angy,bx
xor cx,cx
call matrix_rot_xyz


lea DI,tempp
lea Si,matrix
mov cx,ile_p
call transform_m


mov cx,ile_p
lea si,tempp
lea di,dest
call pers


mov cx,ile_f
lea bx,dest
lea si,figure

@rys_:
 push cx
 push bx
 push si

 
 mov di,[si]
 shl di,2
 push w[bx+di]      
 push w[bx+di+2]    
 pop y1
 pop x1

 mov di,[si+2]
 shl di,2
 push w[bx+di]      
 push w[bx+di+2]    
 pop y2
 pop x2

 mov di,[si+4]
 shl di,2
 push w[bx+di]      
 push w[bx+di+2]    
 pop y3
 pop x3

 push x1
 push y1
 push x2
 push y2
 push 200
 call linia

 push x2
 push y2
 push x3
 push y3
 push 200
 call linia

 push x3
 push y3
 push x1
 push y1
 push 200
 call linia

 pop si
 pop bx
 pop cx
 add si,6
dec cx
jnz @rys_

call vsync
mov ax,es    ;  in es we have ptr to buffer
call blit    ;  blit buffer to screen

call gas
call blur

call keypressed
jz Program
call stop_stoper

  mov ax,0003h
  int 10h
 
  lea di,stoper
  xor edx,edx
  mov eax,[di+8]
  mov ebx,[di+4]
  sub ebx,[di]
  div ebx  
  ; eax FPS
  mov bx,0b800h
  mov es,bx
  call PiszLint
  

  lea si, AUTOR
  call printf

  xor ax,ax
  int 16h         

  xor ax,ax
  int 16h         

  mov ah,4ch
  int 21h
  
Get_Buf:
  call alloc_seg
  cmp ax,0
  je error_
    mov buf1, ax 
    lea di,IMG
    mov ax,10
    mov bx,10
    call GetImg
   ret
 error_:
  mov ah,4ch
  int 21h  
ret 

piszLINT:
  push eax
  push edx
  push ebx
  push di
  
   xor ebx,ebx
   mov bx,Base
   lea di,temp
   xor dl,dl
   mov [di],dl
   Div__:
    xor edx,edx
    inc di
    DIV EBX
    add dl,48
    mov [di],dl
    cmp eax,0 
   jne Div__

  call wywal

  pop di
  pop ebx
  pop edx
  pop eax
ret

wywal:
  mov al,[di]
  pp:
   mov ah, 0Eh
   mov bh, 00h
   mov bl, 07h
   int 10h

   dec di
   mov al,[di]
  cmp al,0
  jne pp
ret


blur: 
 mov si,320d
 mov ax,64000
 sub ax,320
 sub ax,329
 xchg ax,cx
 All:
  xor ax,ax
  mov al,es:[di]
  add al,es:[di-1]
  adc ah,0
  add al,es:[di+1]
  adc ah,0
  add al,es:[di+320]
  adc ah,0
  shr ax,2
   jz omin
    dec ax
   omin:
    mov es:[di-320],al
    inc di
 loop all
 ret


; SI - points
; CX - points count
draw:
  pet2:
   mov ax,w [si+2]
   mov di,w [si]
   push cx
   mov bl,100
   call plot 
   pop cx
   add si,4
   add bx,2
  loop pet2
ret


include mem.inc
include math.inc
include graph.inc
include mysz.inc
include timer.inc

BASE DW 10
temp  DB 20 dup(?)
nazwa DB 'logo.bmp',0
okkk DB 'OK $'
buf1 DW ?
IMG  DW 4 dup (?)  ; 8 bytes for image definition
x1   DW 0
x2   DW 0
x3   DW 0
y1   DW 0
y2   DW 0
y3   DW 0

kat  DW 0


d equ dword ptr 

ile_p equ 8
ile_f equ 12

point  DD  -5.0, -5.0, -5.0
       DD   5.0, -5.0, -5.0
       DD   5.0,  5.0, -5.0 
       DD  -5.0,  5.0, -5.0

       DD  -5.0, -5.0,  5.0
       DD   5.0, -5.0,  5.0
       DD   5.0,  5.0,  5.0 
       DD  -5.0,  5.0,  5.0    ; 8 vertexes

       
figure DW  0, 1, 2
       DW  0, 2, 3
       DW  5, 4, 7
       DW  5, 7, 6  
       DW  1, 5, 6
       DW  1, 6, 2 
       DW  4, 0 ,3
       DW  4, 3, 6
       DW  4, 5, 1
       DW  4, 1, 0
       DW  3, 2, 6
       DW  3, 6, 7  ; 12 faces (2 face for one cube side)
       

    

tempp   DD ile_p*3  dup(?)
dest    DW ile_p*2  dup(?)
angx    DW 0
angy    DW 20


OldPal  DB 256*3    dup(?)
CurPal  DB 256*3    dup(?)
destPal DB 256*3    dup(?)

  AUTOR DB  13,10,"+----------------------------------------+"
        DB  13,10,"| AUTOR  : Dybuk87                       |"        
        DB  13,10,"+----------------------------------------+"
        DB  13,10
        DB  13,10,"     Press any key ",0

         
end start
