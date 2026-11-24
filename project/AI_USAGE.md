# AI Tool Usage

## Tools Used

**ChatGPT (OpenAI)**

ChatGPT was used as a support tool throughout the project for:
- Understanding the project requirements and specifications.
- Discussing the design and implementation of C modules.
- Debugging code and understanding compiler or test errors.
- Developing unit tests and scenario tests.
- Improving the Makefile and automating the test process.
- Reviewing project documentation.

All suggested code and changes were reviewed, implemented, and tested by me. I used the terminal to compile the program, run tests, and verify the results.

**GCC and Make**

GCC was used to compile the C source files. Make was used to automate the build process and run the test suite.

**AddressSanitizer and UndefinedBehaviorSanitizer**

These sanitizers were used when compiling the unit tests to help detect memory errors and undefined behavior.

**Git and GitHub**

Git was used for version control and GitHub for storing and managing the project repository.

## Example of an AI Tool Error

During the development of the test aggregation script, ChatGPT initially suggested a `sed` expression that incorrectly extracted the number of passed tests from the test summary. This caused an `Illegal number` error when running `make test`.

I identified the problem through the terminal output and replaced the parsing commands with `awk`, which correctly extracted the passed and failed counts.

The final test suite completed successfully with 304 tests passed and 0 failed.
