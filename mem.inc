;---------------------------------------------------------------------------------
; 4 procedury
; TIMG  DB 8 dup(?) ;    2 bajty seg; 2 bajty resX*resY; 2 bajty ResX; 2 bajty ResY
;---------------------------------------------------------------------------------

;------------------------------------------------------------
;  WE : Brak
;  WY : Brak
; info: Przygotowuje pami�� - niezb�dne dla *.com 
;------------------------------------------------------------
init_mem proc near
 mov ah,4ah
 mov bx,1000h
 int 21h
ret
endp

;---------------------------------------
;   WE : DI - TIMG,  AX-ResX, BX-ResY
;   WY : Brak
; info : Przydziela pami�� pod rysunek
;        Rysunek to tablica 8 bajt�w
;---------------------------------------
GetImg:
 mov [DI+4],AX
 mov [Di+6],BX  ; zapami�taj rozdzielczo�� w TIMG
 xor dx,dx      ; DX = 0
 mul bx         ; DX : AX <- ResX,ResY
 or dx,dx       ; 
 jnz _as_no_mem ; je�li dx>0 to �le!! potrzeba wi�cej pami�ci ni� segment :)

 mov [DI+2],ax  ; rozmiar 
 mov bx,ax      ; do bx stary ax
 shr ax,4       ; AX na paragrafy.. czyli bloki 16 bajtowe
 and bx,0Fh     ; je�li AX/16 dzieli si� bez reszty
 jz _bez_r      ; to ok!
  inc ax        ; w innym wypadku musimy przydzieli� o paragraf wi�cej 
 _bez_r:        ; czas przydziala�!

 mov bx, ax     ; io�c paragraf�w do BX
 mov ah,48h     ; numer funckji 
 int 21h        ; przydziel
 jc _as_no_mem  ; bark pami�ci to opier*
  mov [DI],ax   ; zapami�taj Handle
 ret
_as_no_mem:
 lea si,_IMG_ERROR_MEM_
ret

;------------------------------------------------------------
;   WE : brak;	
;   WY : ax - przydzielony segment(65536-bajt) - AX = 0 b��d
; info : Przydziela pamiec pod segment
;------------------------------------------------------------
alloc_seg proc near
 mov ah,48h
 mov bx,1000h
 int 21h
 jc as_no_mem
ret
as_no_mem:
 xor ax,ax       
ret
endp


;------------------------------------------------------------
;   WE : Si - tekst
;   WY : Brak
; info : Pisze tekst na ekranie
;------------------------------------------------------------
printf:
 lodsb
 or al,al
 jz papa
  mov ah,0Eh
  mov bl,7
  int 10h  
 jmp printf
 papa: 
ret

_IMG_ERROR_MEM_ DB 'IMG ERROR : B��d przydzia�u pami�ci pod IMG ',0