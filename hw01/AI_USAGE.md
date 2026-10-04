 # AI Usage for Homework 1

I used AI assistence to help understand better the assignments requirements and the concepts of bit manipulation functions, add some code structure.

AI assistence specifically was used for:
- understanding get_field(), set_field() and sign_extend()
- checking and improving test cases 
- understanding some link and compiler errors
- giving an idea of the README structure

I verified the generated code by compiling it with - gcc and running the test suite with - make test.

One error specific in the generated code was related to the 32-bit boundary cases. The original implementation used - 1u << width that caused incorrect behaviour when width=32 and test cases failed; I fixed the functions to handle this particular case separately and then verified that all 28 tests passed. 

I also checked the code and test results myself.