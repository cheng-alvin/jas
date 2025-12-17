## Contributing to Jas

Thank you for considering to contribute to the Jas assembler! The jas assembler
is a project that is developed by a small group of people. We are always looking
for new contributors to help us improve the assembler. This guide is for
**DEVELOPERS looking to commit code to the Jas assembler**. If you are looking
to report a bug or request a feature, please use the official mailing list:
mailto:jas-assembler@google-groups.com.

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

\*\* These entries are defined in the
[`instructions.tbl`](https://github.com/cheng-alvin/jas/blob/main/libjas/instructions.tbl)
file and is compiled using a script that compiles it into native C structs that
can be used and accessed by the other parts of the assembler. \*\*

**Constructing a entry**

Jas encoder banks has now been since compiled using a text-based format. Every
entry is line-seperated and values are seperated using `|`s, spaces are used to
pad and align the values with the others (mainly for cosmetics) but they are
eventually ignored by the compiler. Characters such as `-` are in-place for
`NULL` and will be replaced with `NULL` during compilation. To compile the
instruction encoder table, make the `tabs.c` target in the `libjas` directory,
this will invoke the Node.js script and automatically generate the `tabs.c`
file.

Here’s a sample entry:

```
# name | identity | opcode extension | opcode            | byte opcode       | pre 
# -----------------------------------------------------------------------------------------------
  cmc  | zo       | -                | 0xF5              | -                 | no_operands
```

Note that lines prepended with a `#` will be ignored and removed from the
output.

This example text-based entry depicts the sample `cmc` instruction provided by
intel in *Chapter 3 (Instruction set reference A-L) Vol 2A 3-1*, and will be
compiled to the following C structure (Note that formatting may differ):

```c
instr_encode_table_t cmc[] = {
  {
    .ident                 = OP_ZO, 
    .opcode_ext            = NULL, 
    .opcode                = {0xF5},
    .byte_instr_opcode     = NULL,
    .opcode_size           = 1,
    .pre                   = &no_operands,
    .byte_opcode_size      = 0,
  },

  INSTR_TAB_NULL,
};
```

**FAQ:** What does `zo`, `OP_ZO` mean? What are they for? These are called
*operand identies*, Intel calls them *Instruction Operand Encoding*(s), usually
found below a instruction encoder table on the manual. Every enum is mapped to
one of these *encoder funtions* and encodes the operands. (See
[here](https://github.com/cheng-alvin/jas/edit/main/CONTRIBUTING.md#adding-a-new-encoder))

**Next, register the new instruction:**

Even though you have the instruction encoder table already setup, currently the
assembler has no indication that this instruction actually *exists*. Therefore,
we’ll need to indicate to the assembler that this instruction actually exists
and there’s actually a encoder table for it somewhere in the source code. (Which
we wrote in the previous “chapter”)

So, register the instruction and the instruction encoder table to the assembler,
we’ll add the instruction’s name in the `instructions` enum in the
`instruction.h` header, the name should be prefixed with the prefix of `INSTR_`
and followed with the instruction’s name as shown on the Intel manual to
maintain consistency.

> Please ensure the instruction is not appended in the enum, but instead added
> **before** the directives section, otherwise the instruction you are trying to
> add as a *instruction* will be interpreted as an assembler directive. (All
> assembler directives are prefixed as `INSTR_DIR_` instead of just `INSTR_`)

After your shiny new instruction is registered to the `instructions` enum,
you’ll also need to register the instruction’s encoder table to the general
lookup table for instructions called `instr_table`, they must be placed in a
array in **the same** order as the enum is in. This is very important since the
assembler uses a lookup and uses the enum as a indexing tool and fetch whatever
table is needed.

### Adding a new encoder

Although most of the encoders used by 80% of all Intel x64 instructions, many
are still unsupported and require you to add it into the assembler manually,
especially many of the specialized floating point instructions and CPU specific
stuff. All encoders live in the `libjas/encoder.c` file, to write a new encoder
function, use the `DEFINE_ENCODER` macro defined in `libjas/include/encoder.h`
and providing the name of the encoder as the argument. (The name should be in
lowercase and match the naming scheme as shown on the Intel manual.) See example
below:

```c
// ...
DEFINE_ENCODER(xx){
  // ...
}
```

The `DEFINE_ENCODER` macro already gives developers some helper arguments such
as the instruction encoder table to reference, the operand array, more details
appear in the `encoder.h` header, all “invokers” of these encoder function,
including the assembler itself will conform to this structure.

After defining a encoder that you are happy with, there is no way the assembler
can have any indication of the *existence* of this encoder and cannot be
invoked. All encoders should be *appended* to the `enc_ident` enum in the
`libjas/include/encoder.h` header so that the order can match up to the encoders
array in the `enc_lookup()` function.

> The `enc_ident` enum serves as a *indexer* for the `enc_lookup()` array and
> allows the lookup table to work properly.

### How does maintaining work?

Code in Jas should be a collaborative project, there is no way that one person
will have the ability to look after all the code in such a complicated project.
Once a new block of code such as function or a new file is added, they will be
automatically assigned to the author(s) of that block. For example, a new
encoder is merged into the assembler, the people who initially wrote it will
take care of the encoder’s source, documentation and addressing issues. Their
name(s) and email addresses as well as relevant contact details (preferably
Github username) should be added on the documentation comment for the encoder
similar to the one below:

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
> just drop a email to mailto:eventide1029@gmail.com and I (Alvin) will be keen
> to do my best to look after it.

#### Becoming a maintainer

Becoming a maintainer is easy! Everyone can do it! It’s a great way to
contribute to the community and help out with the constant stream of issues. To
become a maintainer just drop in a quick email to me (Alvin)
mailto:eventide1029@gmail.com and I’ll be happy to let you look after that
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
mailto:eventide1029+jas@gmail.com

Happy hacking!
