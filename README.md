# cpu-scheduler
## tools
* c
* c++ 
* makefile

## Program menu 
  - first come first serve
  - shortest jop first
  - shortest time to complelion first
  - go to round robin
  - go to multi level feedback queue

## Data set
 Using external program, data-set is created with random values to be as the program input
 
## Input
 The input is taken from a text file *`Data_set.txt`* directly and the choosen algrithm is applied to it
 
## Output
 the output is displayed on the console or terminal showing the state of the processor with time
 
## main Idea
 1- First we construct an object or struct for the "process" which holds data about each process 
 
 2- We know the number of instructions for each process and IO percent 
 
 3- Construct an  array which holds the type of each instruction (IO instruction or cpu instruction)
 
 4- Get number of instructions which are cpu instructions from the IO percent 
 
 5- Mark the n elements in the array with 1 *`IO Instruction`* where n is the number of instrutions which are IO instrucions
 
 6- Randomize the array

After these steps we have now an array for each process with size = number of instructions for this process with random type. each value could be cpu instrution or io instruction.
now we apply the wanted  scheduling algorithm on this object
