#include"common.h"
#include"STCF.h"
#include"FCFS.h"
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
	// shortestRemainingTimeFirst(v, m, k);
	// RoundRobin(v, tc, k, m);
	FCFS_Scheduler(v,k,m);
	return 0;
}