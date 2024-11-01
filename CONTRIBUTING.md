## Contributing to Jas

Thank you for considering to contribute to the Jas assembler! The jas assembler is a project that is developed by a
small group of people. We are always looking for new contributors to help us improve the assembler. This guide is 
for **DEVELOPERS looking to commit code to the Jas assembler**. If you are looking to report a bug or request a
feature, please see https://github.com/cheng-alvin/jas/issues.

### Getting started
For beginners and those who are new to Git and GitHub, please see https://guides.github.com/activities/hello-world/
for a quick tutorial on how to get started with Git and GitHub. Now, please follow the steps below to get started
with contributing to Jas (especially for those who are not familiar with Git).

1. Fork and clone the repository (not the original one) to your local machine.
2. Create a new branch and commit changes
3. Submit a pull request to the Jas repository on GitHub.


> Please note that branch and commit names shall be as short as possible, branch names should contain a one or
> two word overview on the changes commited and commit messages shall show the added changes in another couple
> words with details appearing in the description.

### Building and testing
Perquisites:
- LLVM C Compiler (CLANG)
- GNU Make
- LLVM Debugger (LLDB) (optional, only needed for debugging when buidling `libjas_debug.a`)

The Jas assembler is written in C and can be built using the GNU C Compiler (GCC). The assembler can be built
running `make` inside the home directory. The assembler can be cleaned, debugged and tested as per the `Makefile` in the home directory.

### What now?
After reading the instructions here, you should have a good understanding of how to contribute
to the Jas assembler project! Write some code, drink some coffee, and have fun!

If you have any questions, please feel free to contact me at eventide1029+jas@gmail.com

Happy hacking!
