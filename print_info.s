//file header
    .arch armv6     //armv6 architecture
    .arm            //arm 32-bit IS
    .fpu vfp        //floating point co-processor
    .syntax unified //modern syntax

//function imports
    .extern printf

    .section .rodata
.Lmsg1: .string "Total size: %lu\n"
.Lmsg2: .string "Total entries: %lu\n"
.Lmsg3: .string "Longest chain: %lu\n"
.Lmsg4: .string "Shortest chain: %lu\n"
.Lmsg5: .string "Empty buckets: %lu\n"

    //.data         //uncomment if needed

    .text           //start of text segment

    .global print_info               //make print_info global for linking to
    .type   print_info, %function    //define print_info to be a function
    .equ 	FP_OFF, // FILL THIS 	 //fp offset distance from sp

print_info:	
// function prologue

// function body

// function epilogue


// function footer - do not edit below
    .size print_info, (. - print_info) // set size for function
// ==========================================================================

//file footer
    .section .note.GNU-stack,"",%progbits // stack/data non-exec (linker)
.end