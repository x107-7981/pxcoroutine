/*
    This is a tool which support you use coroutine(stacked) in C.
    Made by x107.
    Recent update : 2024.12.21
    (Only support x86_64 system now)
*/
#ifndef PXCRT_H
#define PXCRT_H
#include <stdint.h>

#ifdef __GNUC__
#define pxcrt_attribute(...) __attribute__((__VA_ARGS__))
#else
#define pxcrt_attribute(...) __declspec(VA_ARGS)
#endif

#if ( (defined(__GNUC__) && defined(_X86_) && !defined(__x86_64))\
    ||(defined(_M_IX86) )) /*X86_32*/
#define pxcrt_X86 1
#elif (defined(__x86_64) || defined(_M_X64)) /*X86_64*/
#define pxcrt_X86_64 1
#else
#error no support!
#endif

#ifdef __cplusplus
extern "C"{
#endif

#ifdef pxcrt_X86
#define pxcrt_retnum 0x1a2b3c4du
typedef uint32_t register_t;

//The buffer of coroutine
typedef struct pxcrt_buf{
    register_t Ebx;
    register_t Esp;
    register_t Ebp;
    register_t Esi;
    register_t Edi;
    register_t Eip;
}pxcrt_buf;

#elif (defined(pxcrt_X86_64))
#define pxcrt_retnum 0x192a3b4c5d6e7f80ull
typedef uint64_t register_t;

//The buffer of coroutine
typedef struct pxcrt_buf{
    register_t Rbx;
    register_t Rsp;
    register_t Rbp;
    register_t Rsi;
    register_t Rdi;
    register_t R12;
    register_t R13;
    register_t R14;
    register_t R15;
    register_t Rip;
}pxcrt_buf;

#endif /*X86/X86_64*/

typedef struct pxcrt{
    //the buffer of pxcrt
    //in active time is the caller's buffer
    //in unactive time is the coroutine function's buffer
    //!!DO NOT MOVE!!(asm connect)
    pxcrt_buf buffer;

    //in active time means stack_start,
    //in unactive time means stack_size
    //!!DO NOT MOVE!!(asm connect)
    register_t stack_s; 

    //the space where the coroutine function's stack
    //  saved during unactive time
    //!!DO NOT MOVE!!(asm connect)
    void *stack;
}pxcrt;

//The return type of coroutine function
typedef void pxcrt_func;

//The handle of coroutine
typedef pxcrt* pxcrt_handle;

//the promise of coroutine
typedef void* pxcrt_promise;

//tribool, 0 means false, 1 means true, -1 means error
typedef enum pxcrt_tribool{
    Error = -1,
    False = 0,
    True = 1
}pxcrt_tribool;

//create a coroutine
pxcrt_handle pxcrt_create(
    //a handle where can build coroutine
    //if it is NULL, it will build a new one in the heap, and return it
    pxcrt_handle hpxcrt, 

    //the address of stack
    //if it is NULL, it will build a new one in the heap
    void *stack,

    //the size of the stack
    //notice that if the stack are overflow, your program will die immediately
    uint32_t stack_size,

    //the coroutine function, which can return numbers to the promise 
    pxcrt_func (*pfunc)(pxcrt_handle, pxcrt_promise), 

    //the return address
    //if it is NULL, it will not be able to return numbers
    pxcrt_promise prm_ptr);

//delete a coroutine
pxcrt_tribool pxcrt_delete(
    //the handle of the coroutine
    pxcrt_handle hpxcrt,

    //if you want to free the handle in the heap
    //if your handle isn't in the heap, make it False 
    pxcrt_tribool doFreeHP,

    //if you want to free the stack in the heap
    //if your stack isn't in the heap, make it False 
    pxcrt_tribool doFreeStack);

//return from a coroutine function (no longer back)
void pxcrt_return(pxcrt_handle hpxcrt);

//make a coroutine function await until recover it
void __fastcall pxcrt_await(pxcrt_handle hpxcrt);

//recover a coroutine function
void pxcrt_recover(pxcrt_handle hpxcrt);

#ifdef __cplusplus
}
#endif

#endif