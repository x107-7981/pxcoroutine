/*
    This is a tool which support you use coroutine(stacked) in C.
    only support x86 system.
    If it is x86_32, please link it with pxcoroutine32.s
    If it is x86_64, please link it with pxcoroutine64.s
*/
#include <stdio.h>
#include <malloc.h>
#include "pxcoroutine.h"

void __fastcall __call_pxcrt_func(pxcrt_func (*pfunc)(pxcrt_handle, pxcrt_promise),
            pxcrt_promise prm_ptr, pxcrt_handle hpxcrt);

void __fastcall __back_pxcrt_func(pxcrt_handle hpxcrt);

void __fastcall __await_pxcrt_func(pxcrt_handle hpxcrt);

void __fastcall __recover_pxcrt_func(pxcrt_handle hpxcrt);

//create a coroutine
pxcrt_handle pxcrt_create(pxcrt_handle hpxcrt, void *stack, uint32_t stack_size,
            pxcrt_func (*pfunc)(pxcrt_handle, pxcrt_promise), pxcrt_promise prm_ptr)
{
    if(!hpxcrt)
        hpxcrt = (pxcrt*)malloc(sizeof(pxcrt));
    if(!stack)
        stack = malloc(stack_size);
    if(!stack){
        if(hpxcrt)
            free(hpxcrt);
        return NULL;
    }
    hpxcrt->stack = stack;

    // printf("pfunc : %p\nprm_ptr : %p\nhpxcrt : %p\n",pfunc,prm_ptr,hpxcrt);
    // printf("&stack : %p\nstack : %p\n",&hpxcrt->stack,hpxcrt->stack);

    __call_pxcrt_func(pfunc,prm_ptr,hpxcrt);

    return hpxcrt;
}

//delete a coroutine
pxcrt_tribool pxcrt_delete(pxcrt_handle hpxcrt,
            pxcrt_tribool doFreeHP, pxcrt_tribool doFreeStack)
{
    if(!hpxcrt)
        return Error;
    // printf("stack : %p\n",hpxcrt->stack);
    if(doFreeStack == True && hpxcrt->stack){
        free(hpxcrt->stack);
        hpxcrt->stack = NULL;
    }
    if(doFreeHP == True){
        free(hpxcrt);
        hpxcrt = NULL;
    }
    return True;
}

//return from a coroutine function (no longer back)
void pxcrt_return(pxcrt_handle hpxcrt){
    __back_pxcrt_func(hpxcrt);
}

//make a coroutine function await until recorver it
void __fastcall pxcrt_attribute(naked) pxcrt_await(pxcrt_handle hpxcrt){
    __await_pxcrt_func(hpxcrt);
    asm("ret":::);
}

//recover a coroutine function
void pxcrt_recover(pxcrt_handle hpxcrt){
    __recover_pxcrt_func(hpxcrt);
}
