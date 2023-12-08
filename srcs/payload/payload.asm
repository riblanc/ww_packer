bits 64

.payload:
; Saves the execution context of the original program
    push    rax
    push    rdi
    push    rsi
    push    rdx
    push    rcx

; Computes useful addresses (entrypoint and message address)
    lea     r8, [rel .msg]     ; load address of .msg into r8
    lea     r9, [rel .payload] ; load address of .payload into r9
    sub     r9, [rel .ep_off]  ; compute entrypoint address

; write "..WOODY.." to stdout
    mov     rax, 1
    mov     rdi, 1
    mov     rsi, r8
    mov     rdx, .msg_len
    syscall

; restore the execution context of the original program
    pop     rcx
    pop     rdx
    pop     rsi
    pop     rdi
    pop     rax

; push the entrypoint address onto the stack to specify the next instruction after the payload
    push    r9
    ret

.msg:       db   "....WOODY....",0xa
.msg_len:   equ  $ - .msg
.ep_off:    dq   0xcafebabe ; offset (from the payload) of the entrypoint address
