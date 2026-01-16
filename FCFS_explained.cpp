/*
=== FCFS (First Come First Served) Scheduling Algorithm ===

KEY CONCEPT:
- Processes execute in the order they arrive
- Each process runs completely before the next one starts
- The CPU is never idle if there are processes waiting

IMPORTANT FORMULAS:
1. Completion Time (CT): When a process finishes
   - ct[0] = at[0] + bt[0]  (First process: arrival + burst time)
   - ct[i] = max(ct[i-1], at[i]) + bt[i]  (Later processes: wait for CPU if not ready)

2. Waiting Time (WT): Time spent waiting in queue
   - wt[i] = ct[i] - at[i] - bt[i]
   
3. Turnaround Time (TAT): Total time from arrival to completion
   - tat[i] = ct[i] - at[i]
*/

#include<iostream>
#include<iomanip>
using namespace std;

int main()
{
    int n, i;
    cout << "Enter number of processes: ";
    cin >> n;
    
    // Arrays to store process data
    int at[n];    // Arrival Time - when process arrives
    int bt[n];    // Burst Time - how long process takes to execute
    int ct[n];    // Completion Time - when process finishes
    int wt[n];    // Waiting Time - time spent waiting
    int tat[n];   // Turnaround Time - total time from arrival to completion
    
    float wtavg = 0, tatavg = 0;  // Average waiting and turnaround times
    
    // Input: Get arrival and burst time for each process
    for(i = 0; i < n; i++)
    {
        cout << "\nProcess " << i+1 << ":\n";
        cout << "  Arrival Time: ";
        cin >> at[i];
        cout << "  Burst Time: ";
        cin >> bt[i];
    }
    
    // STEP 1: Calculate for first process
    ct[0] = at[0] + bt[0];        // Completion time = arrival + burst
    wt[0] = 0;                    // First process doesn't wait (it goes first)
    tat[0] = ct[0] - at[0];       // Turnaround = completion - arrival
    
    // STEP 2: Calculate for remaining processes
    for(i = 1; i < n; i++)
    {
        // Completion time: previous process completion + current burst time
        // But if current process arrives after previous completes, we can start immediately
        ct[i] = max(ct[i-1], at[i]) + bt[i];
        
        // Waiting time: how long did this process wait?
        wt[i] = ct[i] - at[i] - bt[i];
        
        // Turnaround time: from arrival to completion
        tat[i] = ct[i] - at[i];
    }
    
    // Calculate average waiting and turnaround times
    for(i = 0; i < n; i++)
    {
        wtavg += wt[i];
        tatavg += tat[i];
    }
    
    // STEP 3: Display results in table format
    cout << "\n\n" << string(70, '=') << endl;
    cout << left << setw(12) << "Process" << setw(12) << "Arrival" << setw(12) 
         << "Burst" << setw(12) << "Completion" << setw(12) << "Waiting" 
         << setw(12) << "Turnaround" << endl;
    cout << string(70, '=') << endl;
    
    for(i = 0; i < n; i++)
    {
        cout << left << setw(12) << ("P" + to_string(i+1)) 
             << setw(12) << at[i] 
             << setw(12) << bt[i]
             << setw(12) << ct[i]
             << setw(12) << wt[i] 
             << setw(12) << tat[i] << endl;
    }
    
    cout << string(70, '=') << endl;
    cout << "\nAverage Waiting Time = " << wtavg/n;
    cout << "\nAverage Turnaround Time = " << tatavg/n << endl;
    
    return 0;
}

/*
EXAMPLE:
Input:
  2 processes
  P1: Arrival=0, Burst=5
  P2: Arrival=1, Burst=3

Calculation:
  P1: CT=0+5=5, WT=0, TAT=5-0=5
  P2: CT=max(5,1)+3=8, WT=8-1-3=4, TAT=8-1=7
  
Output Table:
  P1: Arrival=0, Burst=5, Completion=5, Waiting=0, Turnaround=5
  P2: Arrival=1, Burst=3, Completion=8, Waiting=4, Turnaround=7
*/
