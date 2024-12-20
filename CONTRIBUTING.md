## Contributing to Jas

Thank you for considering to contribute to the Jas assembler! The jas assembler is a project that is developed by a
small group of people. We are always looking for new contributors to help us improve the assembler. This guide is 
for **DEVELOPERS looking to commit code to the Jas assembler**. If you are looking to report a bug or request a
feature, please see [this](https://github.com/cheng-alvin/jas/issues).

### Getting started
For beginners and those who are new to Git and GitHub, please see [here](https://guides.github.com/activities/hello-world/)
for a quick tutorial on how to get started with Git and GitHub. 

The Jas project is hosted on this Github page and does not have any official mirrors on other sites at this point, 
you may create a un-official mirror on other sites and hosts if you wish, but all official development will occur
here.

First of all, clone and hop into the repository's directory:

```sh
git clone https://github.com/cheng-alvin/jas/
cd jas
```

For the safety of already stable code, we'll use branches (Like everyone else) to develop software and squash bugs
in parallel, before starting any development work, you must create a new branch and create a pull request with a 
detailed description of your work and changes you have commited. 

> Please note that branch and commit names shall be as short as possible, branch names should contain a one or
> two word overview on the changes commited and commit messages shall show the added changes in another couple
> words with details appearing in the description.

To ensure the quality of the contributed code, all pull requests must be reviewed by a maintainer (which is most
cases is me). Please direct all queries and concerns to eventide1029+jas@gmail.com as well as for any feedback 
on code and contributions to the Assembler.

### Building and testing
To build Jas, simply run `make` in the home or `libjas` directories (A debug binary must be built in `libjas`). 
Tests can be added in the `tests` directory and built using `make tests` as well in the home directory, all C 
test files will be automatically built and run. You may include the Jas testing framework [here](https://github.com/cheng-alvin/jas/blob/main/tests/test.h) 

> The build and testing scripts must be successful before submitting a pull request up to the repo,
> if tests and compilation scripts fail, you must put your PR as a "draft" instead.

### Code style
Preferably, if you have `clang-format` installed on your system, you can simply run `make format` in the home 
directory to *automatically* format the source files to conform to our programming style as specified in 
[this](https://github.com/cheng-alvin/jas/blob/main/.clang-format) file. Otherwise you are welcome to write
your patch in your own coding style and have it formatted automatically when merged into the main branch,
just as long as the maintainers can read it, then it'll okay.

As mentioned above, a Github action will automatically run when new code is pushed onto the main branch to
automatically format the code using `clang-format`. You can ignore this behaviour by adding a `clang-format off`
and a corrisponding `clang-format on` comment in your code for small snippets that may break or cause issues 
down the line if formatted automatically.

### Adding suport for a instruction to the assembler
A common addition for the Jas assembler, especially since how complex the Intel x64 instruction set is, is the 
addition of new instructions and instruction encoder identities, which can be done by creating a instruction 
encoder table, adding and/or registering the instructions to the instruction identities, and finally testing 
and writing unit tests.

**First, define a instruction encode table:**

A instruction encoder table describes the identity of the instruction and how each instance can be enocded
in binary as well as some key meta data such as what modes the instruction support and operand extensions
etc. (Details will appear in the [`instruction.h`](https://github.com/cheng-alvin/jas/blob/main/libjas/include/instruction.h) file)
Each insruction encoder table includes *entries*, each entry defines the meta data that corrisond to a certain
identity. For example, a MR identity (A identity with a m64 and r64) will be one entry and includes the 
opcode, and support status in different operating modes. Below is an example of a instruction encoded in the `MR` identity and can be encoded as `FF /r` or `FA /r` when using the byte-instruction mode (More examples will be [here](https://github.com/cheng-alvin/jas/blob/main/libjas/instruction.c))


```c
{
  .ident = OP_MR,
  .opcode_ext = NULL,
  .opcode = {0xff},
  .support = MODE_SUPPORT_ALL,
  .byte_instr_opcode = {0xfa},
  .opcode_size = 1,
  .pre = NULL,
}

```

### What are instruction encoder identities?
Many instructions share lots of operand encoding logic that can be encapsulated. Each operand encoding identities have a certain order of operand types, allowing code to be shared among instructions who have similar operand inputs. 

In Jas, we have organized these functions as codes like `MR`, `RM` or `Z` in which it corresponds to a certain combination of operands types (or classes if your fancy) within an instruction. However, tOnce an instruction struct is mapped to one of the operand identities, the assembler has narrowed down the instruction to a small set of possibilities allowing it to encode more efficiently. Each identity will, based on the instruction struct, encode the instruction into machine code, since every instruction in the operand identity has a similar encoding format.

(Or, if you're struggling to understand me, it's a bit like the quadratics identites we used in high school, remember DOTS?)

*As always, the Intel manual is the best place to find more information, in fact, the operand identities are official "guidelines" of operand combinations Intel has placed out; you can find it below the opcode encoder table*

### The benefits of identities
Once an instruction struct is mapped to one of the operand identities, the assembler has narrowed down the instruction to a small set of possibilities allowing it to encode more efficiently. Each identity will, based on the instruction struct, encode the instruction into machine code, since every instruction in the operand identity has a similar encoding format. The operand identites eliminates the large code size, improves and removed many performance overheads and improves reliability by packing everything in a function for encoding.

### What to remember before submitting a PR
Once you have completed your work, remember to submit pull requests that are organised and have a clear sense of 
purpose, any change from one line of code to a whole file is okay, it just has to have a purpose and a clear 
reason to be merged upstream. (but also remember to try and keep it small) Speaking of size, please also ensure 
that you properly and logically organise banches and pull requests, if changes don't seem to fit in one pull
request logically, feel free to submit multiple, as long as it makes sense logically.

### What now?
After reading the instructions here, you should have a good understanding of how to contribute
to the Jas assembler project! Write some code, drink some coffee, and have fun!

If you have any questions, please feel free to email me at eventide1029+jas@gmail.com

Happy hacking!
