//file header
    .arch armv6     //armv6 architecture
    .arm            //arm 32-bit IS
    .fpu vfp        //floating point co-processor
    .syntax unified //modern syntax


    //.data         //uncomment if needed

    .text           //start of text segment

    .global node_lookup               //make node_lookup global for linking to
    .type   node_lookup, %function    //define node_lookup to be a function
    .equ 	FP_OFF, 60 	  // fp offset distance from sp 
node_lookup:	
// function prologue
    push    {r4-r9, fp, lr}     
    add     fp, sp, FP_OFF 


//function body
    add r4, fp, #4          // front pointer address
    str r4, [fp, #-28]


    ldr r5, [fp, #-28]  
    ldr r6, [r5]
    cmp r6, #0
    beq .END
    add r6, r5, #-32        // year address
    add r7, r5, #-28        // month address
    add r8, r5, #-24       // day address
    add r9, r5, #-20      // hour address

    ldr r6, [r6]        //year
    ldr r7, [r7]        //month
    ldr r8, [r8]        //day
    ldr r9, [r9]        //hour

    cmp r6, r0
    bne .Lloop
    cmp r7, r1
    bne .Lloop
    cmp r8, r2
    bne .Lloop
    cmp r9, r3
    beq .LEqual
    b .Lloop

.Lloop:
    ldr r5, [fp, #-28]  
    add r5, r5, #20
    str r5, [fp, #-28]
    ldr r6, [r5]
    cmp r6, #0
    beq .END
    

    add r6, r5, #-32        // year address
    add r7, r5, #-28        // month address
    add r8, r5, #-24       // day address
    add r9, r5, #-20      // hour address

    ldr r6, [r6]        //year
    ldr r7, [r7]        //month
    ldr r8, [r8]        //day
    ldr r9, [r9]        //hour

    cmp r6, r0
    bne .Lloop
    cmp r7, r1
    bne .Lloop
    cmp r8, r2
    bne .Lloop
    cmp r9, r3
    beq .LEqual
    b .Lloop
.END:
    str #0, [fp, #-28]
.LEqual:
    ldr r0, [fp, #-28]



// function epilogue
    sub     sp, fp, FP_OFF   // restore sp
    pop     {r4-r9,fp, lr}      // restore saved registers
    bx      lr
// function footer - do not edit below
    .size node_lookup, (. - node_lookup) // set size for function

//file footer
    .section .note.GNU-stack,"",%progbits // stack/data non-exec (linker)
.end