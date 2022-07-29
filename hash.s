// CES30 assignment template
// 
//
// Describe target Hardware to the assembler
.arch   armv6
.arm
.fpu	vfp
.syntax unified
.align 4
.bss
.data
/////////////////////////////////////////////////////////

.text                       // start of the text segment
/////////////////////////////////////////////////////////
// function hashFun 
/////////////////////////////////////////////////////////
.type   hashFun, %function // define as a function
.global hashFun            // export function name
.equ    FP_OFFSET, 28      // (regs - 1) * 4
/////////////////////////////////////////////////////////

// put any .equ for hashFun here - delete this line

/////////////////////////////////////////////////////////

hashFun:
    push    {r4-r9, fp, lr}     // use r4-r9 protected regs
    add     fp, sp, FP_OFFSET   // locate our frame pointer
    // do not edit the prologue above
    // You can use temporary r0-r3 and preserved r4-r9
    // Store return value (results) in r0
    /////////////////////////////////////////////////////////
    
    // TODO: Write your code here to implement the hash function.
    // For reference, the C implementation is shown here:
    // hash = c + (hash << 6) + (hash << 16) - hash;
    str r0, [fp, #-4]
    str r1, [fp, #-8]
    ldr r2, [fp, #-8]
    ldr r3, [fp, #-8]
    lsl r2, r2, #6
    lsl r3, r3, #16
    ldr r4, [fp, #-4]
    ldr r5, [fp, #-4]
    add r4, r4, r2
    add r4, r4, r3
    sub r4, r4, r5
    mov r0, r4
    

    /////////////////////////////////////////////////////////
    // do not edit the epilogue below
    sub     sp, fp, FP_OFFSET   // restore sp
    pop     {r4-r9,fp, lr}      // restore saved registers
    bx      lr                  // function return 
    .size   hashFun,(. - hashFun)
    