## Contributing to Jas

Thank you for considering to contribute to the Jas assembler! The jas assembler
is a project that is developed by a small group of people. We are always looking
for new contributors to help us improve the assembler. This guide is for
**DEVELOPERS looking to commit code to the Jas assembler**. If you are looking
to report a bug or request a feature, please use the official mailing list:
jas-assembler@google-groups.com.

### Getting started

For beginners and those who are new to Git and GitHub, please see
[here](https://guides.github.com/activities/hello-world/) for a quick tutorial
on how to get started with Git and GitHub. The Jas project is hosted on this
Github page and does not have any official mirrors on other sites at this point,
you may create a un-official mirror on other sites and hosts if you wish, but
all official development will occur here.

First of all, clone and hop into the repository’s directory:

```sh
git clone https://github.com/cheng-alvin/jas/
cd jas
```

For the safety of already stable code, we’ll use branches (Like everyone else)
to develop software and squash bugs in parallel, before starting any development
work, you must create a new branch and create a pull request with a detailed
description of your work and changes you have committed.

> Please note that branch and commit names shall be as short as possible, branch
> names should contain a one or two word overview on the changes committed and
> commit messages shall show the added changes in another couple words with
> details appearing in the description.

To ensure the quality of the contributed code, all pull requests must be
reviewed by a maintainer (which is most cases is me). Please direct all queries
and concerns to mailto:eventide1029+jas@gmail.com as well as for any feedback on
code and contributions to the Assembler.
[See below](https://github.com/cheng-alvin/jas/blob/contributing-guide-changes/CONTRIBUTING.md#how-does-maintaining-work)

### Building and testing

To build Jas, simply run `make` in the home or `libjas` directories (A debug
binary must be built in `libjas`). Tests can be added in the `tests` directory
and built using `make tests` as well in the home directory, all C test files
will be automatically built and run. You may include the Jas testing framework
[here](https://github.com/cheng-alvin/jas/blob/main/tests/test.h)

### Code style

Preferably, if you have `clang-format` installed on your system, you can simply
run `make format` in the home directory to *automatically* format the source
files to conform to our programming style as specified in
[this](https://github.com/cheng-alvin/jas/blob/main/.clang-format) file.
Otherwise you are welcome to write your patch in your own coding style and have
it formatted automatically when merged into the main branch, just as long as the
maintainers can read it, then it’ll okay.

As mentioned above, a Github action will automatically run when new code is
pushed onto the main branch to automatically format the code using
`clang-format`. You can ignore this behavior by adding a `clang-format off` and
a corresponding `clang-format on` comment in your code for small snippets that
may break or cause issues down the line if formatted automatically.

### Adding support for a instruction to the assembler

A common addition for the Jas assembler, especially since how complex the Intel
x64 instruction set is, is the addition of new instructions and instruction
encoder identities, which can be done by creating a instruction encoder table,
adding and/or registering the instruction encoder tables to the instruction
list(s), and finally testing and writing unit tests.

**First, define a instruction encode table:**

A instruction encoder table describes the identity of the instruction and how
each instance can be encoded in binary as well as some key meta data such as
what modes the instruction support and operand extensions etc. (Details will
appear in the
[`instruction.h`](https://github.com/cheng-alvin/jas/blob/main/libjas/include/instruction.h)
file) Each instruction encoder table includes *entries*, each entry defines the
meta data that correspond to a certain identity. For example, a MR identity (A
identity with a m64 and r64) will be one entry and includes the opcode, and
support status in different operating modes.

**Constructing an entry**

Entries of encoder reference tables are all located in the `libjas/encoders` directory. Each instruction, or a set of minor instructions are typically grouped together, (you can use Intel's documentation grouping conventions as a general guideline) and should be registered to the dependencies of the `instructions.inc` target. Each file contains an overarching `instructions` field that houses an array of instructions, it should be noted that all other field on the top level would be ignored, allowing contributors to add user-defined meta data such as license information, implementation comments etc. Within each instruction of the yaml file, the instruction exists *variants* of the instruction with distinct configurations of operands and instruction opcodes among other components supporting the encoding of instructions.

> Each encoder reference table is defined in a simple yaml structure, allowing for easy diffing between versions and human readability. Such yaml files are provided to a Node.Js script and compiled into a C structure array. A enum of all instructions available and a string array of instructions is generated automatically, supporting code generation and parsing features within the assembler. 

Below is a sample of the yaml structure supported by the compiler script (Not a real instruction):
``` yaml
instructions:
  - sample:
    variants:
      - opcode: [ 0x00 ]
        operands:
          - { type: r/m32, encoder: default }

        compatibility:
          long: true
          legacy: true
```

Each variant of an instruction should contain the following:
- `opcode` - An array of a maximum of 3 bytes, depicting the opcode as indicated by the Intel developer handbook. It is typical convention that bytes are expressed hexadecimal in adherence to the Intel Developer Manual; implementations of encoder tables should also avoid having bytes merged together such as `0xABCD`.
  
- `operands` -  An array with a 4 element capacity depicting the type, and options of operands expected of by the instruction. It should be noted that **all** possibilities must be listed by said array, despite its verbosity, such approach allows for better error handling in spite of some inconsistencies across instructions. Details regarding niche operand encoder options can be obtained in [`encoder.c`](https://github.com/cheng-alvin/jas/blob/encoder-rework/libjas/encoder.c) and should be set to `default` where no special consideration or encoding is required.
  
- `compatibility` - A option that contains boolean values for checking if such instruction is supported in either `long`, or `legacy` mode. The use of boolean values in yaml through the `Yes` and `No` keywords is strongly discouraged.

## How does maintaining work?
Code in Jas should be a collaborative project, there is no way that one person will have the ability to look after
all the code in such a complicated project. Once a new block of code such as function or a new file is added, they
will be automatically assigned to the author(s) of that block. For example, a new encoder is merged into the assembler,
the people who initially wrote it will take care of the encoder's source, documentation and addressing issues. Their name(s)
and email addresses as well as relevant contact details (preferably Github username) should be added on the documentation
comment for the encoder similar to the one below:

```c
// func.h

/**
 * @author example <example@example.com>
 * 
 * ...
 */
void func(); 
```

All questions, concerns and general inquires related to this block of code
should be directed to this maintainer, he or she should also review any related
PRs and keep in constant communication with other maintainers and the
contributors.

> If you do contribute some code and **do not wish** to look after it long-term
> as a maintainer (Which is 100% okay) you may drop support for it at anytime,
> just drop a email to eventide1029@gmail.com and I (Alvin) will be keen
> to do my best to look after it.

#### Becoming a maintainer

Becoming a maintainer is easy! Everyone can do it! It’s a great way to
contribute to the community and help out with the constant stream of issues. To
become a maintainer just drop in a quick email to me (Alvin)
eventide1029@gmail.com and I’ll be happy to let you look after that
portion of code.

### What to remember before submitting a PR

Once you have completed your work, remember to submit pull requests that are
organized and have a clear sense of purpose, any change from one line of code to
a whole file is okay, it just has to have a purpose and a clear reason to be
merged upstream. (but also remember to try and keep it small) Speaking of size,
please also ensure that you properly and logically organize branches and pull
requests, if changes don’t seem to fit in one pull request logically, feel free
to submit multiple, as long as it makes sense logically.

### Don’t like Github? Don’t worry!

If you don’t prefer to use Github as the platform for communication and merging
code, patches may be submitted via the mailing list (Just like old-school), the
maintainer taking care of that portion of the code will be responsible in
checking for patches and reviewing them before applying them to the source code
and merging it using a Pull request.

Basically, once the patch has been received by the maintainer from the mailing
list, the maintainer should apply and review the patch in their **own** branch
before merging it in, labeling it as *patch* with the person who originally sent
in the patch mentioned or quoted in the title.

### What now?

After reading the instructions here, you should have a good understanding of how
to contribute to the Jas assembler project! Write some code, drink some coffee,
and have fun!

If you have any questions, please feel free to email me at
eventide1029+jas@gmail.com

Happy hacking!
