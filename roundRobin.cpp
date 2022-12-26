#include"common.h"

void print(ll st, ll en, ll pid, bool fin = 0,bool io=0) {
	if (~pid)
		cout << "process with id " << pid << " from " << st << " to " << en << "\n";
	else
		cout << "free cpu from " << st << " " << en << "\n";
	if (io)
	cout << "Process left cpu for IO purpose.......\n";
	if (fin)
		cout << "Process is treminated....\n";
}
void metaDataDisplay(map<ll, processMetaData>procData, ll numberOfProcesses) {
	ll totalTurnAround = 0, totalResponse = 0;
	for (auto& i : procData) {
		cout << "process with pid = " << i.first << "\n";
		cout << "turn around time = " << " " << i.second.turnAroundTime() << " "
			<< "Response Time = " << i.second.responseTime() << "\n";
		totalTurnAround += i.second.turnAroundTime();
		totalResponse += i.second.responseTime();
	}
	cout << "\nAverage TurnAround Time = " << totalTurnAround / numberOfProcesses << " "
		<< "Average Response Time = " << totalResponse / numberOfProcesses << "\n";
}
void RoundRobin(const vector<Process>& v, ll timeSlice, ll insTime, ll ioTime) {
	if(v.empty())return cout<< "There are no processes",void();
	priority_queue<Process>scheduler;
	map<ll, processMetaData>procData;
	for (auto& i : v) {
		procData[i.pId].arrival = i.readyTime;
		scheduler.push(i);
	}
	ll curTime = 0;

	while (scheduler.size())
	{
		Process curProcess = scheduler.top();
		scheduler.pop();
		if (curProcess.readyTime > curTime) {
			print(curTime, curProcess.readyTime - 1, -1);
			curTime = curProcess.readyTime;
		}
		ll st = curTime, en = 0, ok = 0;
		for (int i = 0; i < timeSlice and curProcess.insIdx < curProcess.numberOfInstructions; i+=insTime, curTime+=insTime, curProcess.insIdx++) {
			if (procData[curProcess.pId].firstTun == -1)procData[curProcess.pId].firstTun = curTime;
			if (curProcess.insType[curProcess.insIdx] == 0) //cpu instruction
				continue;
			//io instruction
			en = curTime;

			curProcess.readyTime = curTime + ioTime;
			curProcess.insIdx++;
			ok = 1;
			print(st, en, curProcess.pId, curProcess.insIdx == curProcess.numberOfInstructions,1);
			if (curProcess.insIdx != curProcess.numberOfInstructions)
				scheduler.push(curProcess);
			curTime+= insTime;
			break;
		}
		en = curTime;
		if (curProcess.insIdx == curProcess.numberOfInstructions)
			procData[curProcess.pId].completion = curProcess.readyTime;
		if(!ok)
			print(st, en, curProcess.pId, curProcess.insIdx == curProcess.numberOfInstructions);
	}
	metaDataDisplay(procData, v.size());
}








/*

10 100 1
0 84 77 81
1 33 3 104
2 80 54 151
3 64 86 169
4 65 7 242
5 60 19 267
6 41 26 350
7 45 3 368
8 59 84 427
9 33 20 499




*/
