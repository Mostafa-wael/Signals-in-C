# Signals-in-C
C program to search for a given value from a list (array) of numbers as follows:
---
Parent (Main) Process: 
1.	The parent (main) process is called with the following arguments:
- A search integer value X. 
- List of integers,
  Example: ./main.out 5 7 8 5 3 2 1 6 0 
  , Explanation: 5 is the search value. The list of integers are 7, 8, 5, 3, 2, 1, 6, 0
2.	The parent creates an array and fills it with the list of integer values from the command line. 
3.	It prints the process information (i.e. process ID), For example: I am the parent, PID = 1233.
4.	It forks two children in parallel and then sleeps for 5 seconds. 
5.	It waits for both children to exit before it can terminate.
6.	Before termination, it prints “Value Not Found”. 

Each Child Process: 
1.	It prints the process information (i.e. process ID and parent process ID), For example: I am the first/second child, PID = 1234, PPID = 1233
2.	It searches the first or the second half of the array for “X”. The first child should search in the first half, the second child should search in the second half. In case of a list with an odd number of elements, break the tie by assigning the middle number to any of the two halves. 
3.	If the search item is found:
- The child process sends a signal SIGUSR1 to the parent to inform it that it has found that item X. 
- The child process sends the position of “X” to the parent as exit code. 
4.	If the search item is not found:
- The child process sleeps for 3 seconds.
- It prints a termination message “Child 1 (or 2) terminates” and then terminates. 
