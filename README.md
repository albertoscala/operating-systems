# Operating Systems

## [Homework 1 - Basic C programming](https://github.com/albertoscala/operating-systems/blob/main/homework1/homework1.c)

You are asked to make a program in C that calculates the differences between two text files. The specifications of the
program are as follows:
1. The program receives as input the name of two text files, henceforth referred to as F1 and F2,
and a set of options, listed in Step 3.
2. The program, once started, loads the two text files into memory in blocks of 50
lines, which are processed before reading the next lines of the file.
3. The program calculates the diff between the i-th line of file F1 and the i-th line of file F2,
returning an output that depends on the options passed from the command line, which are

   - **-g**: if F1 and F2 are different in at least one line returns the string 
     `F1 and F2 are different`. The names of the processed files should be returned instead of F1 and F2.
   - **-s**: if F1 and F2 are equal in all rows returns the string `F1 and F2 are
     equal`. The names of the processed files should be returned instead of F1 and F2.
   - **-d**: returns the numbers of different rows
   - **-u**: returns the numbers of the equal rows
   - **-v**: combined with **-d** or **-u** prints lines that differ or are the same. The
   output format must be<br><br>
   ```
   #Line, FILENAME F1: <F1 line that differs or is the same>
   #Line, FILENAME F2: <F2 line that differs or is the same>
   ```
   - **-o FILENAME_OUTPUT**: redirects the output to a text file.

*The -g and -s options can be combined; the -d and -u options are mutually
exclusive; the -o option can always be used.*

4. In case the two files have different lengths, in case the
-d, -u and -v options, the comparison must be made for the number of rows contained in the
smaller file (the one with the smallest number of rows).

### Tests

The program should work properly in the following cases:

  1. The input consists of two equal rows (test with 5 different pairs of rows of lengths
  greater and less than 50 rows)
  2. The input consists of two different files (test with 5 different pairs of files of lengths
  greater and less than 50 lines)
  3. Both input files, or either one, does not exist
  4. One of the two input files is empty
  5. The two input files have different lengths (test with 5 different pairs of files)

## Homework 2 - System programming

You are asked to realize, with the C language, a server program (henceforth server) and a
client program (henceforth client) that meet the following specifications:
1. The server receives requests from remote clients via a TPC/IP socket
2. The client requires the server to perform an arithmetic operation and return the result. The
requests made by clients thus contain an arithmetic operation and its
operands
3. The server calculates the requested operation and returns the result to the client via socket
TCP/IP.
Additional detail specifications are:
4. The operations that the server must be able to calculate are: **+**, **-**, <strong>*</strong>, **/**
5. The message sent by the client has the following structure

<p align="center"><code>[operation, Operand1, Operand2]</code></p>

6. The response message sent from the server to the client has the following structure

<p align="center"><code>[timestamp receiving request, timestamp sending response, operation result]</code></p>

7. The server must be able to handle multiple clients concurrently, i.e., it is
multithreaded
8. The server writes into a single log the operations performed, results, timestamps, id of the
client (e.g., IP address). Concurrent access to the log must be guaranteed and managed.
9. The client must allow the user to enter an operation by means of a string such as.
`Operando1 operation Operando2`.
10. The client must transform the input string into the specified message format
11. When the client receives a response to a request, it must display the result
of the operation and the service time, i.e. `timestamp sending response - timestamp
receiving request`
12. The client, once activated, must allow as many operations to be entered as the user
wishes, until explicitly terminated by the client itself. The user, cannot request
a new operation before receiving a response to the previous request.

***NOTE**: implementation of a graphical user interface is not required.*

### Tests

The program should work properly in the following cases:
- A + B, A - B, A * B, A/B where A and B are real numbers other than 0 (calculate at least 5
operations for each operator)
- A/B, where B is' zero.
- The user-supplied input string is not correctly formatted, i.e., one of the
operands is missing or is not a number, the operator is absent or is not one of the provided
- The client process can't connect to the server process because, for example, the
server is not active
- Server process terminates unexpectedly
