## Welcome!
Welcome! Thank you for your continued interest for the Jas assembler. For those who don't know, Jas is a MIT licensed, free and open-source x86 assembler capable of generating ELF files designed to run on on linux systems. Jas is written in C and is designed to be simple, fast and easy to use. This project was also designed to be readable and easy to understand. This allows others to learn from the code and expand their knowledge on x86 assembly and how these instructions are encoded.

This guide aims for helping people who are willing to learn how to roll their own assembler, or want an understanding under the hood of how an assembler works. 

### Perquisites
In this guide, I would assume that you have a solid background in computer science and low-level engineering as well as a being proficient in C and finally understanding of x86 assembly and some encoding structures can be very helpful.

If you're not familiar with x86 assembly, I would recommend reading the [Intel manual](https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html) along with the following resources:
- https://www.cs.virginia.edu/~evans/cs216/guides/x86.html
- https://www.tutorialspoint.com/assembly_programming/assembly_basic_syntax.htm
- https://www.felixcloutier.com/x86/
- https://www.swansontec.com/sintel.html

So, if you do not know what things like hexadecimal, binary, registers and operands are. I would not recomend you to continue reading this guide. It might be a good idea to read up on these topics before continuing.

### What is an assembler?
We all know what an assembler is. An assembler is just that magical black box that takes your assembly code and turns it into machine code. Right? Not quite. This whole thing to define an assembler is the whole point of Jas! We have invested hours to understand the inner workings of an assembler and how it works. We have also invested hours to understand how the x86 instruction set is encoded. This guide will help you understand how an assembler works and how to write one yourself (Not that fast).

### Clarification of key information and terms
Before we start, I would like to clarify some key information and terms that will be used throughout this guide. This is to ensure that we are all on the same page. 

1. **x86 only, no ARM (For now)** - For now, we will be investigating the x86 instruction set. However, the concepts and ideas that we will be discussing here can be applied to other instruction sets as well.

2. **Standard 8086 instruction set only** - The instructions that will be assembled to machine code will be the instructions referenced in the 8086 instruction set. This is because the 8086 instruction set is the most basic instruction set that is supported by all x86 processors. This means that all `CPUID` flags will be ignored and we will not be using any instructions that are processor specific. (There's like what? 1000+ instructions in the x86 instruction set? We don't want to be here forever)

3. **Jas is an in-code assembler** - Jas has always been an in-code assembler. This means that the assembler is written in C instructions can be called using a series of functions instead of worrying out parsing a file. There's two reasons to this. 1. I was too lazy to implement a parser 2. It allows users to not worry about obeying syntax and allows them to focus on the assembly code itself, the types are basically provided by the IDE :D

### Let's go!
Now that we have all the boring stuff out of the way, let's get started! Get comfy, grab a cup of coffee, (and be ready to take notes) and let's get started!

## Chapter 1: Boot camp
Welcome to assembler boot camp! In this chapter, we will be going through the basics of an assembler's encoding. It's just the basics that would build up to the next chapter where we will be going through the inner workings of an assembler. In this chapter, the terms people throw around will be clarified and allow you to understand the encoder. 

If you're confident with your knowledge of the basics, feel free to skip this chapter and move on to the next one.

Subjects may also be severely simplified for the sake of simplicity. Please consult other sources for more information.

**Please ensure that you have consulted other sources for more up-to-date information such as the Intel and/or AMD developer manuals**

### Let's get familiar with the x86 instruction set
x86 has a very complex instruction set, we have 87 instructions (A bit better than using 1000+ instructions). These instructions can be categorized into 5 categories:

1. Data transfer instructions
2. Arithmetic instructions
3. Logic instructions
4. Control flow instructions
5. ~~String instructions~~ (I don't think there's much information on this one, so we will be ignoring this one)

Normally, the instructions are represented in a form that have a  **prefix** followed by an **opcode** then **operands**.

*Note that the relevant information about x86 instruction encoding, including opcode maps and instruction formats, can often be found in "Volume 2: Instruction Set Reference." Specific chapters of interest include "Chapter 2: Instruction Format," "Chapter 3: General-Purpose Instructions," and "Chapter 4: System Instructions."*

### Prefixes
Prefixes are optional bytes that can be prepended to the instruction to modify the behavior of the instruction. Some prefixes modify the size of operands and addresses, while others modify the behavior of the instruction itself and extends the behavior. Common prefixes are REX prefixes that are used to extend the register space to 64 bits, operand size prefixes that are used to change the size of the operands and address size prefixes that are used to change the size of the addresses.

*Intel explains it better than I do, so please see "Chapter 3: General-Purpose Instructions"*

### Opcodes
Opcodes are the only mandatory byte of the assembly instruction. It tells the processor about the operation that needs to be performed. Opcodes can range from 1 byte to 3 bytes long. The opcode can also be further extended in some cases. (But this is all you need to know for you to read the next sections)

For example, the opcode of `cmc` is `0xF5`. The `0xF5` byte tells the processor to perform the `cmc` operation on the chip.

### Operands
Operands associate data to the opcode and allows the result to be affected by the data. Operands can be registers, memory addresses or immediate values. Operands can be further categorized into 3 categories:
- Immediate operands
- Register/indirect operands
- Memory operands

(Note that operands can also be mixed, for example, an instruction can have an operand as a register *or* a memory address)

### Immediate operands
Immediate operands are operands that are encoded in the instruction itself. Immediate operands are usually used for constants and are encoded in the instruction itself. Immediate operands can be encoded in 1, 2, 4 or 8 bytes. The size of the immediate operand is determined by the operand size prefix. For example, the `mov` instruction can have an immediate operand of 1, 2, 4 or 8 bytes. The size of the immediate operand is determined by the operand size prefix. They're just simply dumped onto the instruction encoding.

### Register/indirect + Memory operands
Register operands are operands are a bit more complicated. Register operands are operands that are encoded in the instruction itself as well. But, a register code along with an addressing mode can be associated using a ModR/M byte.

*For more information on ModR/M bytes, please see [here](https://en.wikipedia.org/wiki/ModR/M)*

### More on the ModR/M byte
As mentioned above, the registers are encoded in a ModR/M byte. These bytes encode 2 bits as the addressing mode, 3 bits as the register code and 3 bits as the register code of the operand. The addressing mode determines how the operand is accessed. The register code determines the register that is used. Some bits can also be left blank or used as an opcode extension. Frankly, the ModR/M byte is a bit complicated and I would recommend you to read the Intel manual for more information.

Each type of register such as `rax` or `eax` is assigned a code from 1-7. (The register field in the ModR/M byte is only 3 bits, 0b111 = 7 hence it's 7) In this case, the `rax` or `eax` is assigned in value `0` by the processor. 

As you've already forseen, there's way more registers than 7! We have both 32-bit, 8-bit, 16-bit registers and even 64-bit registers! (Excluding any 128-bit registers like `xmm1`) That's why we have the REX prefix and opcode extensions that changes the register sizes. For example, `0` is `eax` with a 32-bit operand size prefix, but `0` is `rax` with a 64-bit operand REX size prefix.

**Always ensure to consult the Intel manual for the correct ModR/M byte encoding, exceptions that can also occur in different contexts**

## Chapter 2: Structure of the assembler
