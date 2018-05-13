STOPER DB 12 dup(0);    (START TIME, STOP TIME, FRAMES) each 4 bytes


;------------------------------------------------------------
;  WE : Brak
;  WY : Brak
; info: Start timer
;------------------------------------------------------------
start_stoper:
 mov ah,02
 int 1Ah      ; get time - CH - Hours; CL - Minutes; DH - seconds
 xor ebx,ebx
 mov bl,dh    ;
 xor edx,edx
 xor eax,eax
 mov al,cl    ;
 push ecx     ;
 mov ecx,60
 mul ecx      ; eax = minutes *60
 add ebx,eax  ; ebx = seconds + minutes*60
 xor edx,edx
 xor eax,eax
 pop ecx
 mov al,ch    ; eax = hours
 mov ecx,3600 
 mul ecx      ; eax = hours * 60*60 
 add ebx,eax  ; ebx = time in sec
 lea di,stoper;
 mov [di],ebx
 xor ebx,ebx
 mov [di+8],ebx
ret 


;------------------------------------------------------------
;  WE : Brak
;  WY : Brak
; info: inc frame count
;------------------------------------------------------------
inc_stoper:
 lea di,stoper
 inc dword ptr [di+8]
ret


;------------------------------------------------------------
;  WE : Brak
;  WY : Brak
; info: Stop timer
;------------------------------------------------------------
stop_stoper:
 mov ah,02
 int 1Ah      
 xor ebx,ebx
 mov bl,dh    
 xor edx,edx
 xor eax,eax
 mov al,cl
 push ecx
 mov ecx,60
 mul ecx     
 add ebx,eax 
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