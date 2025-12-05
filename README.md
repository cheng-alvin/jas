![Jas logo](https://github.com/cheng-alvin/jas/blob/main/logo.png)
<p align="center">
<img src='https://img.shields.io/badge/all_contributors-1-orange.svg?style=flat-square' />
<img alt='PRs Welcome' src='https://img.shields.io/badge/PRs-welcome-brightgreen.svg?style=shields'/>
  <img alt="GitHub code size in bytes" 
    src="https://img.shields.io/github/languages/code-size/cheng-alvin/jas">
  <img alt="GitHub License" src="https://img.shields.io/github/license/cheng-alvin/jas">
  <img alt="GitHub closed issues" src="https://img.shields.io/github/issues-closed/cheng-alvin/jas"/>
</p>

<p align="center" padding="10px"> 

Jas is a minimal, fast and simple zero-dependency assembler for the x64 family 
of processors, it comes packaged with tonnes of useful utility functions targeted 
for implementing into compilers, operating systems and JIT interpreters and 
other types of utility applications that need compilation to ELF or just a plain
encoded buffer. 

</p>

- Supports **60+** instructions!
- Assembles x86, x64 or 16-bit code
- Outputs **ELF object files**
- Unparalleled size and efficiency!

### ⚡Quick start
First of all, link against the binary releases [here](https://shorturl.at/rRvzD)* 
or build it from source with following the instructions below. Jas takes instructions
in an array in a struct form defined in [instruction.h](https://shorturl.at/Tx8nq) 
and passes it to a `assemble_instr()` function which generates the the actual 
buffer of an array of `uint8_t` for you to process. (However, in this situation, 
we are using the `instr_gen()` function and operand generation macros to generate 
the instruction structure automatically without the janky C structure syntax)

```c
#include <jas.h>
#include <stdlib.h>

int main(void) {
  instr_generic_t *instr = instr_gen(INSTR_MOV, 2, r64(REG_RAX), imm64(0));
  buffer_t buf = assemble_instr(MODE_LONG, instr);

  /* Do something to `buf.data` - The uint8_t array */

  instr_free(instr);
  free(buf.data);
  return 0;
}
```

Yeah! that's it! Seriously! It's that simple, this little snippet of code
generates and encodes a `mov rax, 0` instruction in binary.

> Since the returned buffer is in the form of a dynamically allocated pointer, 
> care must be taken to `free` and manage the block of memory.

### 💡 Why Jas?
Jas is a very versatile library, it opens the door for generating low level assembly 
and ELF object files to us average programers, suddenly, we can use write our own
compilers from scratch, without the complexities of LLVM or compiling down to a 
GCC backend. Jas is optimized to speed and complexity, as well as size, allowing 
it to be used in small-scale compilers and interpreters where compile time matters.

[brainfry](https://github.com/cheng-alvin/brainfry) is a Brainf*ck compiler that
compiled Brainf*ck code down to a raw object file and demonstrates key features
of the Jas compiler when used for compiling code.

### 🏗️ Build and install
The very first step in doing anything with jas is to build and link the library 
against your application code, since jas is a library. Ensure you have a C21
compiler installed (Anything will do, preferably `clang`), a Javascript runner 
(such as `node`) for building the `instructions.tbl` file, as well as GNU `make`
or something to run makefile for running the build script.

To obtain the Jas source files, run:
``` bash
git clone https://github.com/cheng-alvin/jas/
```

Now, you should have a folder named `jas` in your current directory, now you can 
just simply run `make` within that directory like so:
(Some directories and folders can be missing, you must run `make clean` before 
you actually build the library.)

``` bash
cd jas # Change into the cloned directory
make clean # Clean directory and setup
make # Build source code
```

Voila! Now you have a built copy of Jas in the `build` folder, just simply copy
and link against the archive to start building your dream compiler or assembler 
frontend!

### 📝 Licensing 
All the code under the Jas assembler project repository is licensed under the 
MIT License. Please see the supplied [LICENSE](https://shorturl.at/vqHRb)
file for legal specifications.

*No warranty of any kind is provided, please double check official sources before 
deploying code to production*

### ❤️ Contributing / reporting bugs
No software is perfect, nor is Jas. As the assembler is written in C, many security 
issues may arise, causing issues across the program. It's a shared responsibility 
to safeguard Jas of all developers, contributors and users alike. Please report 
all security or any other concerns in a timely manner via email to the official 
mailing list: jas-assembler@google-groups.com.

However, for developers interested and passionate about assemblers and low-level 
programming, please see the [contributing guide](https://shorturl.at/BMI62) in
the home directory for more information on how to contribute code to the repository! 

All changes and reports are welcome, no matter how big or small your changes are :-)

---
*Made with love by Alvin / the Jas crew and contributors ❤️ .* 

**Note:* Only releases tagged with `v1.x` provides binary releases or as 
indicated in release notes.
