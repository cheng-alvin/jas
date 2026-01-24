![Jas logo](https://github.com/cheng-alvin/jas/blob/main/logo.png)

<p align="center">
<img src='https://img.shields.io/badge/all_contributors-1-orange.svg?style=flat-square' />
<img alt='PRs Welcome'
src='https://img.shields.io/badge/PRs-welcome-brightgreen.svg?style=shields'/>
<img alt="GitHub code size in bytes"
src="https://img.shields.io/github/languages/code-size/cheng-alvin/jas"> <img
alt="GitHub License"
src="https://img.shields.io/github/license/cheng-alvin/jas"> <img alt="GitHub
closed issues"
src="https://img.shields.io/github/issues-closed/cheng-alvin/jas"/> </p>

<p align="center" padding="10px"> Jas is a minimal, fast and simple assembler
for the x64 family of processors, and is targeted for the implementation into
compilers, operating systems and JIT interpreters and other types of applications
that demand fast compilation to ELF object code or just a plain encoded buffer.
</p>

- Supports **60+** instructions!
- Assembles x86, x64 or 16-bit code
- Outputs **ELF object files**
- Unparalleled size and efficiency!

[brainfry](https://github.com/cheng-alvin/brainfry) is a demonstration project
of the Jas assembler that compiles Brainf\*ck code into raw ELF object code.
Brainfry demonstrates how Jas can simplify the development process without the
implications that come with usage of heavy-duty LLVM or GCC based backends,
that's the beauty of Jas!

### ⚡Quick start

```c
#include <jas.h>
#include <stdlib.h>

int main(void) {
  instr_generic_t *instr = instr_gen(INSTR_MOV, 2, r64(REG_RAX), imm32(0));
  buffer_t buf = assemble_instr(MODE_LONG, instr);

  /* Do something to `buf.data` - The uint8_t array */

  instr_free(instr);
  free(buf.data);
  return 0;
}
```

Yeah! that's it! Seriously! It's that simple, this little snippet of code
generates and encodes a `mov rax, 0` instruction. See our documentation website
[here](https://github.com/cheng-alvin/jas/tree/add-docs?tab=readme-ov-file#-documentation)
for more information about the Jas assembler library.

### 💡 Why Jas?

Jas strives to lower the barrier of entry for low-level compiler development,
while providing a performant solution for serious production-grade projects. The
Jas project believes that generating machine code should'nt be a chore. Code
generation should'nt be an afterthought, but rather its the beating heart
driving the soul of the compiler toolchain!

Jas avoids the complexities associated with the LLVM or GCC toolchains, making
it perfect for small-scale compilers and interpreters where compile time
matters. Optimized for speed and simplicity -- Its a minimalist's dream
assembler, and we hope you enjoy it!

### 🏗️ Build and install

The very first step in doing anything with Jas is to build the library into an
archive file. The entirety of the Jas source tree has been written in C and
requires a suitable toolchain in order to compile the archive files. Details on
the prerequisites for building and developing Jas appear below.

#### Build dependencies

<!-- @mdformat pause -->
<!-- Formatter paused so natural line breaking can be preserved -->

- A standard **C11**-compatible compiler + linker (preferably `clang` or `gcc`)
- Node.js (later than v23.0.0), or ES6 compatible alternative
- GNU Make, or suitable alternative
- `clang-format` to format C code (formatting requirements as shown below)
- `js-yaml` installed globally, supports the generation of the encoder table.

<!-- @mdformat resume -->

- `lldb` _if wishing to debug the Jas assembler_

Invoke **both** the `clean` and `/build/lib/libjas.a` Make targets to build Jas'
optimized release archive and copy relevant headers into the `/build/`
directory, which can be copied over to your project's dependency folder for
linking. More information regarding the building of Jas for the purposes of
debugging can be found
[here](https://github.com/cheng-alvin/jas/blob/main/CONTRIBUTING.md).

### 📖 Documentation

The Jas documentation website is accessible online [here]() and can be built
and served locally by running and statically serving the `mkdocs.yml` file via
the `mkdocs` command line tool. The Jas assembler documentation website provides
a reference for the assembler's function as well as step by step guides for
leveraging the ins-and-outs of the assembler!

### 📝 Licensing

All the code under the Jas assembler project repository is licensed under the
[MIT license](https://en.wikipedia.org/wiki/MIT_License) which is a popular
open-source license for projects like this! Please see
[LICENSE](https://github.com/cheng-alvin/jas/blob/main/LICENSE) for the details.

_No warranty of any kind is provided, please double check official sources
before deploying code to production_

### ❤️ Contributing / reporting bugs

No software is perfect, nor is Jas. As the assembler is written in C, many
security issues may arise, causing issues across the program. It’s a shared
responsibility to safeguard Jas of all developers, contributors and users alike.
Please report all security or any other concerns in a timely manner via email to
the official mailing list: jas-assembler@google-groups.com.

However, for developers interested and passionate about assemblers and low-level
programming, please see the
[contributing guide](https://github.com/cheng-alvin/jas/blob/main/CONTRIBUTING.md)
in the home directory for more information on how to contribute code to the
repository!

All changes and reports are welcome, no matter how big or small your changes are
:-)

---

_Made with love by Alvin / the Jas crew and contributors ❤️ ._

\*_Note:_ Only releases tagged with `v1.x` provides binary releases or as
indicated in release notes.
