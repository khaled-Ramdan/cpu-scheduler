#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

struct Process {
	ll readyTime, pId, numberOfInstructions, IOPercent, insIdx,turnaround_time,responce_time;
	vector<bool>insType;//0=>cpu - 1=> io
	Process() { readyTime = 0; pId = 0; numberOfInstructions = 0; IOPercent = 0; insIdx = 0; };
	Process(ll id, ll instructionCount, ll iopercent, ll arrivalTime) {
		pId = id;
		numberOfInstructions = instructionCount;
		IOPercent = iopercent;
		readyTime = arrivalTime;
		insIdx = 0;
		insType.assign(numberOfInstructions, 0);
		randomizeIO();
	}
	void randomizeIO() {
		ll ioCount = IOPercent * numberOfInstructions / 100;
		for (int i = 0; i < ioCount; i++)
			insType[i] = 1;
		random_shuffle(insType.begin(), insType.end());
	}

};

void FCFS_Scheduler(Process *ptr, int n , int m,int k);
void FCFS_sortProcesses(Process *ptr, int n);
double calcTurnaroundTime(Process *ptr, int n,int waiting_time, int ins_time);
double calcResponceTime(Process *ptr, int n,int waiting_time);



int main() {
	srand(time(0));
	int n, m, k;
	cin >> n >> m >> k;
	Process *ptr ;
	for (int i = 0; i < n; i++) {
		
		cin >> (*ptr).pId >> (*ptr).numberOfInstructions >> (*ptr).IOPercent >>(*ptr).readyTime;
		
	}
	
	FCFS_Scheduler(ptr,n,m,k);
	
	
	return 0;
}


void FCFS_sortProcesses(Process *ptr, int n)
{
    for (int i = 0; i < n; i++) {
        for (int j = i+1; j < n; j++) {
            if (ptr[i].readyTime > ptr[j].readyTime) {
                Process temp = ptr[i];
                ptr[i] = ptr[j];
                ptr[j] = temp;
            }
        }
    }
}


double calcTurnaroundTime(Process *ptr, int n,int waiting_time, int ins_time){
    
   double sum=0 ,counter=0;
   
        for(int i=0;i < n;i++)
        {
             ptr[i].turnaround_time = waiting_time + ins_time;
            
            counter++;
            sum += ptr[i].turnaround_time;

        }
    
    return sum/counter ;
}

double calcResponceTime(Process *ptr, int n,int waiting_time){
    
   double sum=0 ,counter=0;
   
        for(int i=0;i < n;i++)
        {
             ptr[i].responce_time = waiting_time - ptr[i].readyTime;
            
            counter++;
            sum += ptr[i].responce_time;

        }
    
    return sum/counter;
}



void FCFS_Scheduler(Process *ptr, int n , int m ,int k){
    
    FCFS_sortProcesses(ptr,n);
    
    double avrg_turn = calcTurnaroundTime(ptr,n,m,k);
    double avrg_res  = calcResponceTime(ptr,n,m);
    
    for(int i=0;i<n;i++)
    {

        cout <<"id"<< ptr[i].pId;
        cout <<"turn around time "<< ptr[i].turnaround_time;
        cout <<"responce time "<< ptr[i].responce_time;
    }
    
    cout << "Average turn around time \n"<< avrg_turn ;
    cout << "Average responce time \n"<< avrg_res ;
    
}




