#include <iostream>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <vector>

using namespace std;

struct Process {

    // info for each Process
	string PID;
    int number_of_instructions, io_percent, ready_time;
    // instruction Type vector -> to know the type for the instruction where 0 => cpu and 1 => io
    vector<bool>instruction_type; 

    // constructor of this struct
	Process(string pid, int instructionCount, int iopercent, int arrivalTime) {
		PID = pid;
		number_of_instructions = instructionCount;
		io_percent = iopercent;
		ready_time = arrivalTime;
        // resize the instruction_type vector (instruction_type.size() = number_of_instructions) and assign each index by 0
		instruction_type.assign(number_of_instructions, 0); 
		// call randomizeIO function to determine which instruction is cpu or io
        randomizeIO();
	}

    // determine each instruction is cpu or io
	void randomizeIO() {
        // get the number of io intructions depend on io percentage
        int ioCount = io_percent * number_of_instructions / 100;
		// make the first ioCount in the instructions is io instruction
        for (int i = 0; i < ioCount; i++)
		{
            instruction_type[i] = 1;    // means this instruction is io instruction
        }
        // shuffle all instructions randomly
		random_shuffle(instruction_type.begin(), instruction_type.end());
	}

    // sort the processes according to its ready_time
	bool operator< (const Process& p) const {
		return p.ready_time < this->ready_time;
	}
};

int main()
{
    fstream dataSet;
    
    // open the 'Data_set.txt' File 
    dataSet.open("Data_Set.txt", ios::in);   

    // if file Opening failed    
    if(!dataSet)
    {
        cout << "File Opening failed"; 
        return 0;
    } 

    // if reaches here the file is opened

    // get the number of processes
    int number_of_processes;
    dataSet >> number_of_processes;

    // get the IO waiting time
    int io_waiting_time;
    dataSet >> io_waiting_time;

    // get the instruction execution time
    int instruction_execution_time;
    dataSet >> instruction_execution_time;
    
    // vector to store all processes
    vector<Process>process;

    // loop to get info for each process
    for (int i = 0; i < number_of_processes; i++)
    {
        string PID;
        int number_of_instructions, io_percent, arrival_time;

        // get the pid for this instruction
        dataSet >> PID;

        // get the instruction count for this process
        dataSet >> number_of_instructions;

        // get the IO percentage for this process
        dataSet >> io_percent;

        // get the arrival time for this process
        dataSet >> arrival_time;
 
        // push this process in process vector
        process.push_back(Process(PID, number_of_instructions, io_percent, arrival_time));
    }

    // close the data set file
    dataSet.close();

    // if it reaches here data set is uploaded

}
