# ENCE360

A concurrent web scraper which downloads all linked files for a particular website using a number of threads to provide efficient implementation.
The task is split into three parts which should each be completed in turn, then the final task will build on the first two to complete the web scraper.

Guidelines:

 No global variables (at all)

 No more than 3 levels of nesting in any function and less than 40 lines of code

 Use minimum amount of code required (No more than 200 lines for any part of the assignment and preferably less than 100)

 The program should not crash, hang or exit unexpectedly as a result of bad output from an external website

 Instead, errors should be handled in a way described by the interface in the header files

 Implement all code in C, not C++ or using external programs

 Code outside of http.c crawler.c and queue.c will not be marked (but you may implement small bug fixes provided the interface stays the same)

 All memory allocated during the programs execution should be free()'ed and all resources, files, sockets should be closed before the program finishes

 Comment code as necessary, excessive commenting is not required but anything not obvious should be documented
