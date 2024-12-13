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
