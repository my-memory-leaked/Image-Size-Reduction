.const
half real4 0.5
one real4 1.0

.code
RLEEncode proc

movq xmm4, rbx			; store rbx content to prepare it for usage

mov rax, [rsp+40]		; &X (stack) -> rax
movq xmm0, rax			; &X (rax) -> xmm0
mov rbx, [rax]			; X ([rax]) -> rbx
movd xmm1, rbx			; X (rbx) -> xmm1

mov rax, [rsp+48]		; width (stack) -> rax
cvtsi2ss xmm2, eax		; convert width to float and move it to xmm2
mov rax, [rsp+56]		; Y (stack) -> rax
cvtsi2ss xmm3, eax		; convert Y to float and move it to xmm3
mulss xmm3, xmm2		; multiply width and Y in xmm3
addss xmm2, xmm3		; add result of multiplication to width in xmm2

movd xmm3, one			; set uncompressed flag as true

uncompressed:
	comiss xmm3, half	; check compression status
	jnae compressed		; end loop if condition not met

	mov r10, 0			; set repetition counter to 0
	mov rax, rcx		; input array adress -> rax
	add rax, [r9]		; offset adress by current element
	xor r11, r11		; clear r11 content
	mov r11b, [rax]		; current pixel -> r11

line:
	cvtss2si eax, xmm3	; convert bound to integer and move it to eax

compressed:
	movq rbx, xmm4		; restore rbx with previous value
	ret

RLEEncode endp
end