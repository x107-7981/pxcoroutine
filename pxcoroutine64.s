.globl __call_pxcrt_func
.globl __back_pxcrt_func
.globl __await_pxcrt_func
.globl __recover_pxcrt_func

set_pxcrt_buf:
    movq    %rbx,(%rax)
    movq    %rsp,8(%rax)
    movq    %rbp,16(%rax)
    movq    %rsi,24(%rax)
    movq    %rdi,32(%rax)
    movq    %r12,40(%rax)
    movq    %r13,48(%rax)
    movq    %r14,56(%rax)
    movq    %r15,64(%rax)
    movq    8(%rsp),%r9
    movq    %r9,72(%rax)
    movq    $0,%rax
    ret

reset_pxcrt_buf:
    movq    (%rax),%rbx
    movq    8(%rax),%rsp
    movq    16(%rax),%rbp
    movq    24(%rax),%rsi
    movq    32(%rax),%rdi
    movq    40(%rax),%r12
    movq    48(%rax),%r13
    movq    56(%rax),%r14
    movq    64(%rax),%r15
    movq    72(%rax),%r9
    movq    $0x192a3b4c5d6e7f80,%rax
    addq    $16,%rsp
    jmp     *%r9

exchenge_pxcrt_buf:
    movq    %rsp,%rdx
    movq    8(%rax),%rsp
    movq    %rdx,8(%rax)

    movq    %rbp,%r8
    movq    16(%rax),%rbp
    movq    %r8,16(%rax)
exchenge_pxcrt_buf_pass:
    movq    %rbx,%rcx
    movq    (%rax),%rbx
    movq    %rcx,(%rax)

    movq    %rsi,%r9
    movq    24(%rax),%rsi
    movq    %r9,24(%rax)

    movq    %rdi,%rcx
    movq    32(%rax),%rdi
    movq    %rcx,32(%rax)

    movq    %r12,%rdx
    movq    40(%rax),%r12
    movq    %rdx,40(%rax)

    movq    %r13,%r8
    movq    48(%rax),%r13
    movq    %r8,48(%rax)

    movq    %r14,%r9
    movq    56(%rax),%r14
    movq    %r9,56(%rax)

    movq    %r15,%rcx
    movq    64(%rax),%r15
    movq    %r15,64(%rax)

    movq    %r10,%rdx
    movq    72(%rax),%r10
    movq    %rdx,72(%rax)
    movq    $0x192a3b4c5d6e7f80,%rax
    addq    $16,%rsp
    jmp     *%r10

__call_pxcrt_func:
    movq    %r8,%rax
    call    set_pxcrt_buf
    movq    $0x192a3b4c5d6e7f80,%rax
    push    %rax
    movq    %rsp,80(%r8)
    push    %rdx
    push    %r8
    movq    %r8,%r12
    movq    %rcx,%r9
    movq    %r8,%rcx
    call    *%r9

    movq    %r12,%rax
    call    reset_pxcrt_buf

__back_pxcrt_func:
    movq    %rcx,%rax
    call    reset_pxcrt_buf

__await_pxcrt_func:
    movq    80(%rcx),%rdx
    movq    88(%rcx),%r9
    movq    %rsp,%r8
    movq    (%rsp),%r10
.Lb1:
    popq    %rax
    movq    %rax,(%r9)
    addq    $8,%r9
    cmpq    %rdx,%rsp
    jb      .Lb1
    subq    88(%rcx),%r9
    movq    %r9,80(%rcx)
    movq    %rcx,%rax
    movq    %r8,%rsp
    call    exchenge_pxcrt_buf

__recover_pxcrt_func:
    movq    80(%rcx),%rdx
    movq    88(%rcx),%r9
    movq    (%rsp),%r10
    movq    $0x192a3b4c5d6e7f80,%rax
    push    %rax
    movq    %rsp,80(%rcx)
    movq    8(%rcx),%rax
    movq    %rsp,8(%rcx)
    addq    %rdx,%r9
    subq    $8,%r9
.Lb2:
    movq    (%r9),%r8
    push    %r8
    subq    $8,%r9
    cmpq    %r9,88(%rcx)
    jbe     .Lb2
    movq    16(%rcx),%r11
    subq    %rax,%r11
    addq    %rsp,%r11
    subq    $8,%r11
    movq    %rbp,16(%rcx)
    movq    %r11,%rbp
.Lb3:
    subq    %rax,(%r11)
    addq    %rsp,(%r11)
    subq    $8,(%r11)
    movq    (%r11),%r11
    cmpq    %r11,80(%rcx)
    ja      .Lb3
    movq    %rcx,%rax
    call    exchenge_pxcrt_buf_pass
