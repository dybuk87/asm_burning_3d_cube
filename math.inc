; 7 procedur



;------------------------------------------------------------
;   WE : brak
;   WY : brak
; info : ustawia tabele sin, cos
;------------------------------------------------------------
InitMath3D:
mov cx,0  
lea bx,my_sin
lea di,my_cos

_tab:
   mov i,cx
   fild i
   fldPI
   fmul 
   fdiv l128
   fsincos
   fstp d [di]
   fstp d [bx]

   add di,4
   add bx,4
   inc cx
 cmp cx,256
 jne _tab
ret

;------------------------------------------------------------
;   WE : CX - ile punktow;  SI - 3D buf;  DI - 2D buf
;   WY : brak
; info : Przeksztalca punkty 2D na 3D
;------------------------------------------------------------
pers:
 pet:
  fld l256
  fld d [si+8]  
  fadd perspec  
 
  fdiv 
  fstp d [si+8]
  
  fld d [si]
  fmul d [si+8]
  fadd correct_x
  fistp w [di]  

  fld d [si+4]
  fmul d [si+8]
  fadd correct_y
  fistp w [di+2]

  add si,12
  add di,4
 loop pet
ret


;------------------------------------------------------------
;   WE : SI - 3D buf ;  DI - 3D buf;  AX- ile punktow
;   WY : brak
; info : kopiuje AX punktow z bufora SI do bufora DI
;------------------------------------------------------------
copy:
  xor dx,dx

  mov bx,ax
  add ax,bx
  add ax,bx

  mov cx,ax

  p:
   mov eax,d [si]
   mov d [di],eax
   add si,4
   add di,4
  loop p
ret



;------------------------------------------------------------
;   WE : DI - macierz
;   WY : brak
; info : Macierz jednostkowy
;------------------------------------------------------------
matrix_reset:
  fldz 
  fldz
  fld1   
  fstp d [di] 
  fstp d [di+4]
  fstp d [di+8]
 
  add di,12
  fldz 
  fld1
  fldz   
  fstp d [di] 
  fstp d [di+4]
  fstp d [di+8]  

  add di,12
  fld1
  fldz
  fldz   
  fstp d [di] 
  fstp d [di+4]
  fstp d [di+8]  
 ret


;------------------------------------------------------------
;   WE : ax - kat (0..255)
;   WY : Brak
; info : Tworzy macierz obrotu dookola osi X
;------------------------------------------------------------
matrix_rot_x:
  call matrix_reset
  shl ax,2
  mov bx,ax
  lea si,my_cos
  lea di,my_sin

  fld d [si+bx]
  fld d [di+bx]
  fchs
  fld d [di+bx]
  fld d [si+bx]

  lea di,matrix
  add di,16
  fstp d [di]
  fstp d [di+12]
  fstp d [di+4]
  fstp d [di+16]  
ret



;------------------------------------------------------------
;   WE : DI - punkt 3D; SI - macierz; CX - ile punktow
;   WY : Brak
; info : przeksztalca punkty wzglendem macierzy
;------------------------------------------------------------
transform_m:
  petlatm:
   fld d [di]
   fmul d [si]
   fld d [di+4]
   fmul d [si+4]
   fadd 
   fld d [di+8]
   fmul d [si+8]
   fadd

   fld d [di]
   fmul d [si+12]   
   fld d [di+4]
   fmul d [si+16]
   fadd 
   fld d [di+8]
   fmul d [si+20]
   fadd

   fld d [di]
   fmul d [si+24]     
   fld d [di+4]
   fmul d [si+28]
   fadd
   fld d [di+8]
   fmul d [si+32]
   fadd


   fstp d [di+8]
   fstp d [di+4]   
   fstp d [di] 
   add di,12
  loop petlatm   
ret

;------------------------------------------------------------
;   WE : ax - katX (0..255), BX - katY, CX - katZ
;   WY : Brak
; info : Tworzy macierz obrotu dookola osi X, Y i Z
;------------------------------------------------------------
matrix_rot_xyz:
 lea si,my_sin
 lea di,my_cos

 shl bx,2
 fld d [di+bx]
 fld d [si+bx]
 fstp sin_y
 fstp cos_y

 mov bx,ax
 shl bx,2
 fld d [di+bx]
 fld d [si+bx]
 fstp sin_x
 fstp cos_x

 mov bx,cx
 shl bx,2
 fld d [di+bx]
 fld d [si+bx]
 fstp sin_z
 fstp cos_z

 lea di,matrix

 fld sin_y
 fchs        ; -sy
 fld cos_y
 fmul sin_z  ; cy*sz
 fld cos_y
 fmul cos_z  ; cy*cz
 fstp d [di]
 fstp d [di+4]
 fstp d [di+8] ; pierwszy wiersz ustawiony


 fld sin_x
 fmul cos_y  ; sx*cy
 fld sin_x  
 fmul sin_y
 fmul sin_z
 fld cos_x
 fmul cos_z
 fadd        ; sx*sy*sz+cx*cz
 fld  sin_x
 fmul sin_y
 fmul cos_z
 fld  cos_x
 fmul sin_z
 fsub        ; sx*sy*cz-cx*sz
 fstp d [di+12]
 fstp d [di+16]
 fstp d [di+20] ; drugi wiersz ustawiony 

 fld cos_x
 fmul cos_y  ; cx*cy
 fld cos_x
 fmul sin_y
 fmul sin_z
 fld sin_x
 fmul cos_z
 fsub        ; cx*sy*sz-sx*cz
 fld cos_x
 fmul sin_y
 fmul cos_z
 fld sin_x
 fmul sin_z
 fadd        ; cx*sy*cz+sx*sz
 fstp d [di+24]
 fstp d [di+28]
 fstp d [di+32] ; trzeci wiersz ustawiony 
ret

; stale pomocnicze 
 d           equ dword ptr  
 w           equ word ptr  
 b           equ byte ptr 

; macierz
 matrix    DD 1.0,    0.0,   0.0
           DD 0.0,    1.0,   0.0
           DD 0.0,    0.0,   1.0 
         
; uzywane przez engin     

 my_sin    DD 256 dup(?)
 my_cos    DD 256 dup(?)
 l128      DD 128.0
 i         DW 0
 pom32     DD   0.0  
 correct_x DD 160.0
 correct_y DD 100.0
 l256      DD 256.0
 perspec   DD 50.0  
 j         DD 0

; zmienne pomocnicze dla macierzy
 sin_x     DD 0.0
 cos_x     DD 0.0
 sin_y     DD 0.0
 cos_y     DD 0.0
 sin_z     DD 0.0
 cos_z     DD 0.0
 
 angles    DW 0,0,0