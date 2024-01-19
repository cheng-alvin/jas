## Contributing to Jas

Thank you for considering to contribute to the Jas assembler! The jas assembler is a project that is developed by a
small group of people. We are always looking for new contributors to help us improve the assembler. This guide is 
for DEVELOPERS looking to commit code to the Jas assembler. If you are looking to report a bug or request a
feature, please see https://github.com/cheng-alvin/jas/issues.

### Getting started
For beginners and those who are new to Git and GitHub, please see https://guides.github.com/activities/hello-world/
for a quick tutorial on how to get started with Git and GitHub. Now, please follow the steps below to get started
with contributing to Jas (especially for those who are not familiar with Git).

1. Fork the Jas repository on GitHub.
2. Clone your forked repository (not the original one) to your local machine.
3. Create a new branch for your changes.
4. Make your changes and commit them to your branch.
5. Push your changes to your forked repository.
6. Submit a pull request to the Jas repository on GitHub.

### Building and testing
Perquisites:
- LLVM C Compiler (CLANG)
- GNU Make
- LLVM Debugger (LLDB) (optional, only needed for debugging when running `make debug`)

The Jas assembler is written in C and can be built using the GNU C Compiler (GCC). The assembler can be built
running `make` inside the `src/` directory. The assembler can be cleaned, debugged and tested as per the `Makefile
` in the `src/` directory.

### What now?
After reading the instructions in the HACKING.txt file, you should have a good understanding of how to contribute
to the Jas assembler project! Write some code, drink some coffee, and have fun!

If you have any questions, please feel free to contact me at eventide1029+jas@gmail.com

Happy hacking!
