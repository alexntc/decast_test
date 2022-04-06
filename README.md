Test task
========================================

About
--------------

This project contains test task.

HowTo
--------------

This code can be build in different ways, here is the example for GCC toolchain.

#### Example build with GCC

    gcc -Wall -g ./decast_test.c -o ./decast_text_exec

#### Running the program

While executing from command string, there must be 2 arguments (**PROGRAM NAME** and **INPUT STRING**), example:

    ./decast_test_exec B1-B9-Z9

#### Wrong data input

If you'll put wrong token string as **INPUT STRING**, the program will show the problem occured.
