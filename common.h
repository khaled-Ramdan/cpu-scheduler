#include<bits/stdc++.h>
using namespace std;
typedef long long ll;


#ifndef _common_H
#define _common_H
struct Process {
	ll readyTime, pId, numberOfInstructions, IOPercent, insIdx;
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
	bool operator< (const Process& p) const {
		return p.readyTime < this->readyTime;
	}
};
void print(ll st, ll en, ll pid, bool fin,bool io);
struct processMetaData
{
	ll  arrival = -1, completion = -1, firstTun = -1;
	ll responseTime() {
		return firstTun - arrival;
	}
	ll turnAroundTime() {
		return completion - arrival;
	}
};
void metaDataDisplay(map<ll, processMetaData>procData, ll numberOfProcesses);
void RoundRobin(const vector<Process>& v, ll timeSlice, ll insTime, ll ioTime);
#endif
