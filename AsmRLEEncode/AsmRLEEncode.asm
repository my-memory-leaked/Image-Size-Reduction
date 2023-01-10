;*****************************************************************
; 
; Author: Karol Pisarski
; Date: 4.01.2023
; Version: 1.0
;
; The goal of this project is to write an assembly language
; procedure that compresses a file using a specific implementation
; of algorithm. The procedure that uses this algorithm is then 
; called from C++.
;
;*****************************************************************
.code
RLEEncode proc

movq xmm4, rbx					; store rbx content to prepare it for usage

mov rax, [rsp+40]				; &X (stack) -> rax
movq xmm0, rax					; &X (rax) -> xmm0
mov rax, [rsp+48]				; width (stack) -> rax
cvtsi2ss xmm1, rax				; convert width to float and move it to xmm1
mov rax, [rsp+56]				; Y (stack) -> rax
cvtsi2ss xmm2, rax				; convert Y to float and move it to xmm2
mulss xmm2, xmm1				; multiply width and Y in xmm2
addss xmm1, xmm2				; add result of multiplication to width in xmm1

mov r12, 1						; set uncompressed flag as true

uncompressed:
	cmp r12, 1					; check compression status
	jne compressed				; end loop if condition not met

	mov r10, 0					; set repetition counter to 0
	mov rax, rcx				; input array adress -> rax
	add rax, [r9]				; offset adress by current element
	xor r11, r11				; clear r11 content
	mov r11b, [rax]				; current pixel -> r11

line:
	cvtss2si rax, xmm1			; convert bound to integer and move it to rax
	cmp [r9], rax				; compare current element with bound 
	jnb eol						; exit loop if current element is over bound
	mov rax, rcx				; input array adress -> rax
	add rax, [r9]				; offset adress by current element
	xor rbx, rbx				; clear rbx content
	mov bl, [rax]				; current pixel -> rbx
	cmp r11, rbx				; compare current pixel and compressed pixel
	jne eol						; exit loop if pixels not equal
	cmp r10, 255				; compare repetition counter with 255
	jnb eol						; exit loop if repetition counter is greater than 255

	inc r10						; increment repetition counter
	mov rax, [r9]				; current element -> rax
	inc rax						; increment current element
	mov [r9], rax				; rax -> current element
	movq rax, xmm0				; &X -> rax
	mov rbx, [rax]				; X -> rbx
	inc rbx						; increment X
	mov [rax], rbx				; rbx -> X
	jmp line					; jump to begin of inner loop

eol:
	cvtss2si rax, xmm1			; convert bound to integer and move it to rax
	cmp rax, [r9]				; compare current element with bound
	jne write					; jump if current element equals bound
	mov r12, 0					; set uncompressed flag to false

write:
	mov rax, rdx				; output array adress -> rax
	mov rbx, [r8]				; output size -> rbx
	add rax, rbx				; offset output array adress by current output size
	mov byte ptr [rax], r10b	; repetition counter -> output array
	inc rbx						; increment output size
	mov rax, rdx				; output array adress -> rax
	add rax, rbx				; offset output array adress by current output size
	mov byte ptr [rax], r11b	; compressed pixel -> output array
	inc rbx						; increment output size
	mov [r8], rbx				; rbx -> output size

	jmp uncompressed			; jump to begin of outer loop

compressed:
	movq rbx, xmm4				; restore rbx with previous value
	ret

RLEEncode endp
end