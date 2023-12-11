bits 64

.payload:
; Saves the execution context of the original program
    push    rax
    push    rdi
    push    rsi
    push    rdx
    push    rcx

; write "..WOODY.." to stdout
    lea     r8,	    [rel .msg]     ; load .msg address into r8
    mov     rax,    1
    mov     rdi,    1
    mov     rsi,    r8
    mov     rdx,    .msg_len
    syscall

; Compute segment address
    lea     r9,	    [rel .payload] ; load .payload address into r9
    sub     r9,	    [rel .seg_off] ; compute segment address

; mprotect segment to add write permission
    mov     rax,    10
    mov     rdi,    r9
    mov     rsi,    [rel .seg_len]
    mov     rdx,    7
    syscall

; xor the segment with the key
.xor:
    mov     r8, 	-1
    ; get the value of r8 modulo 32
    inc     r8
    xor     rdx,	rdx
    mov     rax,	r8
    mov     rcx,	32
    div     rcx

    ; xor the segment with the key
    lea     r10,            [rel .key]
    mov     r10b,           byte [r10 + rdx]
    xor     byte [r9 + r8], r10b

    cmp     r8,             rsi
    jne     .xor + 7

; restore the original permissions
    mov     rax,    10
    mov     rdi,    r9
    mov     rsi,    [rel .seg_len]
    mov     rdx,    5
    syscall

; restore the execution context of the original program
    pop     rcx
    pop     rdx
    pop     rsi
    pop     rdi
    pop     rax

; Compute entrypoint address
    lea     r9,	    [rel .payload] ; load .payload address into r9
    sub     r9,	    [rel .ep_off]  ; compute entrypoint address

; push the entrypoint address onto the stack to specify the next instruction after the payload
    push    r9
    ret

.msg:       db      "....WOODY....",0xa
.msg_len:   equ     $ - .msg
.ep_off:    dq      0xcafebabe ; offset (from the payload) of the entrypoint address
.seg_off:   dq      0xdeadbeef ; offset (from the payload) of the segment address
.seg_len:   dq      0x00000042 ; exec segment size
.key:       db      "WOODY_WOOD_SUPER_SECRET_KEY_!!!!"