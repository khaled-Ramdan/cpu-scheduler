#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

// Ranges 

// number of Processes
const int num_of_processes_lower = 10;
const int num_of_processes_upper = 10;

// IO waiting time
const int io_waiting_time_lower = 10;
const int io_waiting_time_upper = 10;

// instruction execution time
const int instruction_execution_time_lower = 1;
const int instruction_execution_time_upper = 1;

// instruction count
const int instruction_count_lower = 5;
const int instruction_count_upper = 100;

// IO percentage
const int io_percentage_lower = 0;
const int io_percentage_upper = 100;

// arrival of process
int start_lower = 0;
int start_upper = 100;
const int diff_between_lower_and_upper = 100;

// PID Length
const int PIDLENGTH = 5;

// get random integer from lower to upper
int random_value(int lower, int upper)
{
    return (rand() % (upper - lower + 1)) + lower;
}

// create the PID where the PID length is PIDLENGTH
string getPID(int x)
{
    string pid = "";
    
    // make the integer as a string
    while (x)
    {
        char digit = x % 10 + '0';
        pid = digit + pid;
        x /= 10;
    }

    // append Zero at right of the pid untill it reaches to the PIDLENGTH
    while (pid.size() < PIDLENGTH)
    {
        pid = "0" + pid;
    }

    return pid;
}

int main(char *argv[], int argc)
{

    fstream dataSet;
    
    // open the 'Data_set.txt' File if exist or create it if not exist
    dataSet.open("Data_Set.txt", ios::out);   

    // if file creatation failed    
    if(!dataSet)
    {
        cout << "File Creatation failed"; 
        return 0;
    } 

    // if reaches here the file is opened or created

    // Use current time as seed for random generator
    srand(time(0));
    
    // get the number of processes
    int number_of_processes = random_value(num_of_processes_lower, num_of_processes_upper);
    // appen number of processes in the data set
    dataSet << number_of_processes << ' ';

    // get and appen the IO waiting time in the data set
    dataSet << random_value(io_waiting_time_lower, io_waiting_time_upper) << ' ';

    // get and appen the instruction execution time in the data set
    dataSet << random_value(instruction_execution_time_lower, instruction_execution_time_upper) << '\n';
    
    // loop to create random values for each process
    for (int i = 0; i < number_of_processes; i++)
    {

        // append the pid for this instruction
        dataSet << getPID(i) << ' ';

        // get and append the instruction count for this process
        dataSet << random_value(instruction_count_lower, instruction_count_upper) << ' ';

        // get and append the IO percentage for this process
        dataSet << random_value(io_percentage_lower, io_percentage_upper) << ' ';

        // get and append  the arrival time for this process
        int newStart = random_value(start_lower, start_upper);
        dataSet << newStart << '\n';
        // update the lower and upper for the start
        start_lower = newStart;
        start_upper = start_lower + diff_between_lower_and_upper;

    }

    // close the data set file
    dataSet.close();

    // if it reaches here data set is completed

    return 0;
}