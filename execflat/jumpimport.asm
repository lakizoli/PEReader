.data
; caption db '64-bit hello!', 0
; message db 'Hello World!', 0


.code

; ********************************************************************************
; Jump to function passed in first parameter
; input params:
;    rcx: address of function to jump on
;    rdx: first pointer to pass to the called function
;    r8: first pointer to pass to the called function
; output params:
;    rax: the returned value
; ********************************************************************************

PUBLIC asm_jump_to_initterm_e
asm_jump_to_initterm_e PROC

	mov r9, rcx
	mov rcx, rdx
	mov rdx, r8
	jmp r9

asm_jump_to_initterm_e ENDP

End