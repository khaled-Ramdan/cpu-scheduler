#include"common.h"
#include"STCF.h"
#include"FCFS.h"
void menu()
{
		cout<<"\nchoose one:\n";
		cout<<"\t1- go to first come first serve\n";
		cout<<"\t2- go to shortest jop first\n";
		cout<<"\t3- go to shortest time to complelion first\n";
		cout<<"\t4- go to round robin\n";
		cout<<"\t5- go to multi level feedback queue\n";
		cout<<"\t6-	to exit\n";
}
int main() {
	srand(time(0));
	fstream dataSet;
        int number_of_processes,io_waiting_time ,instruction_execution_time;
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
    dataSet >> number_of_processes;

    // get the IO waiting time
    dataSet >> io_waiting_time;

    // get the instruction execution time
    dataSet >> instruction_execution_time;
    
    // vector to store all processes
    vector<Process>v(number_of_processes);

    // loop to get info for each process
    for (int i = 0; i < number_of_processes; i++)
    {
        int PID;
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
		v[i]=Process(PID,number_of_instructions,io_percent,arrival_time);
    }

    // close the data set file
    dataSet.close();

	int tc;
    // if it reaches here data set is uploaded
	// int n, m, k, tc = 200;
	// cin >> n >> m >> k;
	// vector<Process>v(n);
	// for (int i = 0; i < n; i++) {
	// 	int a, b, c, d;
	// 	cin >> a >> b >> c >> d;
	// 	v[i] = Process(a, b, c, d);
	// }
	bool on=1;
	while(on)
	{
		menu();
		bool ok=1;
		char z;
		while(ok)
		{
			cout<<"enter umber from 1->6:";
			cin>>z;
			if(z<='6'&&z>='1')ok=0;
		}
		switch (z)
		{
		case '1':
			FCFS_Scheduler(v,instruction_execution_time,io_waiting_time);
			break;
		case '2':
			// SJF_Scheduler(v,k,m);
			break;
		case '3':
			shortestRemainingTimeFirst(v, io_waiting_time, instruction_execution_time);
			break;
		case '4':
			cout<<"enter slice time:";
			cin>>tc;
			RoundRobin(v, tc, instruction_execution_time, io_waiting_time);
			break;
		case '5':
			// MLFQ(v,k,m);
			break;
		
		default:
		on=0;
			break;
		}
		if(on)
		{
			cout<<"1- to return to the main menu\n";
			cout<<"0- to exit\n";
			ok=1;
			while(ok)
			{
				cout<<"enter umber from 0->1:";
				cin>>z;
				if(z<='1'&&z>='0')ok=0;
			}
			switch (z)
			{
			case '0':
				on=0;
				break;
			
			default:
				break;
			}
		}
	}
	// shortestRemainingTimeFirst(v, m, k);
	// RoundRobin(v, tc, k, m);
	// FCFS_Scheduler(v,k,m);
	return 0;
}
