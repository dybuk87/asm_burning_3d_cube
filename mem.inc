;---------------------------------------------------------------------------------
; 4 procedury
; TIMG  DB 8 dup(?) ;  image definition (SEG, SIZE, ResX, RESY) each one 2 bytes
;---------------------------------------------------------------------------------

;------------------------------------------------------------
;  WE : Brak
;  WY : Brak
; info: Prepare memory for *.com file
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
; info : Alloc buffer for image Przydziela pamiêæ pod rysunek
;        Rysunek to tablica 8 bajtów
;---------------------------------------
GetImg:
 mov [DI+4],AX
 mov [Di+6],BX  ; Save dimension to image
 xor dx,dx      ; DX = 0
 mul bx         ; DX : AX <- ResX,ResY
 or dx,dx       ; 
 jnz _as_no_mem ; if dx>0 not ok!! we need more memory than one segment -> OOM

 mov [DI+2],ax  ; save size
 mov bx,ax      ; backup size
 shr ax,4       ; split ax to 16 byts section (align to segment) 
 and bx,0Fh     ; if size divide by 16 then no need to pad
 jz _bez_r      ; 
  inc ax        ; we need more memory 
 _bez_r:        ; start allocation

 mov bx, ax     ; how meny blocks(16bytes)
 mov ah,48h     ; 
 int 21h        ; alloc
 jc _as_no_mem  ; no memory, reurn error message
  mov [DI],ax   ; if all ok save segment
 ret
_as_no_mem:
 lea si,_IMG_ERROR_MEM_
ret

;------------------------------------------------------------
;   WE : brak;	
;   WY : return ax as a semgnet pointer to 65536 bytes memory block or 0  if error
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
;   WE : Si - tekst pointer
;   WY : Brak
; info : show tekst on screen
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

_IMG_ERROR_MEM_ DB 'IMG ERROR : error memory alloc ',0