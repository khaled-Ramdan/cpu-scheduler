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
	int n, m, k, tc = 200;
	cin >> n >> m >> k;
	vector<Process>v(n);
	for (int i = 0; i < n; i++) {
		int a, b, c, d;
		cin >> a >> b >> c >> d;
		v[i] = Process(a, b, c, d);
	}
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
			FCFS_Scheduler(v,k,m);
			break;
		case '2':
			// SJF_Scheduler(v,k,m);
			break;
		case '3':
			shortestRemainingTimeFirst(v, m, k);
			break;
		case '4':
			RoundRobin(v, tc, k, m);
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
