# OS_Synchronization_Implementation

In this project, I implemented a sync algorithm with 10 binary semaphors. (Please check out the project definition file for more info.) This scheduler takes a “definition.txt” file with the processes, their codefile names and arrival times as input. All codefiles are also taken as input. The scheduler schedules the given processes according to Round Robin Algorithm with a quantum of 100 ms, and gives a output file, named as “output.txt”. Since there are conflict of use of resources, waiting queues of semaphors are also added. output_[Semaphor No].txt files show the changes in the waiting queues. If there is no change, file is empty.

Output.txt contains information about the ready queue and the track of time during the scheduling of processes. Until all processes are completed, scheduler works and outputs while keeping an eye of the synchronized movement of processes between valuable resources.

**Deliverable**

C++ source code named CmpE322_P2_2012402015.cpp

**How to compile it?**

g++ CmpE322_P2_2012402015.cpp -o CmpE322_P2_2012402015 -std=c++11

**How to run it?**

After making sure that you put definition file as “definition.txt” and codefiles as “x.code.txt”(x is a number) to the same directory with the source code, it is enough to run below command:

./CmpE322_P2_2012402015

**What to expect as output?**

The scheduler creates a file called “output.txt” in the same directory. Its content will look like as below:

0::HEAD-P1-TAIL

140::HEAD-P2-P1-TAIL

250::HEAD-P1-P3-P2-TAIL

290::HEAD-P3-P2-TAIL

390::HEAD-P2-P3-TAIL

550::HEAD-P3-P4-P2-TAIL

650::HEAD-P4-P2-P3-TAIL

790::HEAD-P2-P3-P4-TAIL

910::HEAD-P3-P4-P2-TAIL

1070::HEAD-P4-P2-P3-TAIL

1110::HEAD-P2-P3-TAIL

1210::HEAD-P3-P2-TAIL

1310::HEAD-P2-P3-TAIL

1320::HEAD-P3-TAIL

1400::HEAD—TAIL

**To understand how the scheduler works, please check out the comments provided in the source code.**

**For more info: mmervecerit@gmail.com**
