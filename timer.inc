; 3 procedury - chyba nie do konca dziala :P

STOPER DB 12 dup(0);    4 bajty start w sekundach; 4 bajty stop w sekundach; 4 bajt frames


;------------------------------------------------------------
;  WE : Brak
;  WY : Brak
; info: Wlancza stoper
;------------------------------------------------------------
start_stoper:
 mov ah,02
 int 1Ah      ; pobiera czas - CH - godziny; CL - minuty; DH - sekundy
 xor ebx,ebx
 mov bl,dh    ; sekundy w ebx
 xor edx,edx
 xor eax,eax
 mov al,cl    ; eax = minuty 
 push ecx     ; ecx na stos
 mov ecx,60
 mul ecx      ; eax = minuty *60
 add ebx,eax  ; ebx = sekundy + minuty*60
 xor edx,edx
 xor eax,eax
 pop ecx
 mov al,ch    ; eax = godziny
 mov ecx,3600 
 mul ecx      ; eax = godziny * 60*60 
 add ebx,eax  ; ebx = czas w sekundach
 lea di,stoper;
 mov [di],ebx
 xor ebx,ebx
 mov [di+8],ebx
ret 


;------------------------------------------------------------
;  WE : Brak
;  WY : Brak
; info: zwieksza ilosc klatek
;------------------------------------------------------------
inc_stoper:
 lea di,stoper
 inc dword ptr [di+8]
ret


;------------------------------------------------------------
;  WE : Brak
;  WY : Brak
; info: Wylancza stoper
;------------------------------------------------------------
stop_stoper:
 mov ah,02
 int 1Ah      ; pobiera czas - CH - godziny; CL - minuty; DH - sekundy
 xor ebx,ebx
 mov bl,dh    ; sekundy w ebx
 xor edx,edx
 xor eax,eax
 mov al,cl
 push ecx
 mov ecx,60
 mul ecx     
 add ebx,eax ; ebx = sekundy + minuty*60
 xor edx,edx
 xor eax,eax
 pop ecx
 mov al,ch
 mov ecx,3600
 mul ecx
 add ebx,eax
 lea di,stoper
 mov [di+4],ebx
ret 