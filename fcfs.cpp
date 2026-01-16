#include<iostream>                                           // Include input/output library
#include<iomanip>                                            // Include input/output manipulation library for formatting
using namespace std;                                          // Use standard namespace

int main()                                                    // Main function starts here
{
    int bt[10], at[10], wt[10], tat[10], ct[10], n, i;      // Declare arrays for burst time, arrival time, waiting time, turnaround time, completion time and variables n, i
    float wtavg=0, tatavg=0;                                  // Initialize average waiting time and average turnaround time to 0
    cout << "\nEnter number of processes ";                   // Display prompt to ask user for number of processes
    cin>>n;                                                   // Read number of processes from user

    for(i=0; i<n; i++)                                        // Loop through each process
    {
        cout << "\nEnter arrival time for process " << i+1 << " -- ";  // Display prompt for arrival time
        cin>>at[i];                                           // Read arrival time for process i
        cout << "Enter burst time for process " << i+1 << " -- ";     // Display prompt for burst time
        cin>>bt[i];                                           // Read burst time for process i
    }
    
    ct[0]=at[0]+bt[0];                                        // Calculate completion time of first process: arrival + burst
    wt[0]=0;                                                  // First process has no waiting time (it comes first)
    tat[0]=ct[0]-at[0];                                       // Calculate turnaround time of first process: completion - arrival

    for(i=1; i<n; i++)                                        // Loop through remaining processes
    {
        ct[i]=max(ct[i-1], at[i]) + bt[i];                   // Completion time = max(previous completion, current arrival) + burst time
        wt[i]=ct[i]-at[i]-bt[i];                             // Waiting time = completion time - arrival time - burst time
        tat[i]=ct[i]-at[i];                                  // Turnaround time = completion time - arrival time
    }
    
    for(i=0; i<n; i++)                                        // Loop through all processes to calculate averages
    {
        wtavg += wt[i];                                       // Add waiting time to total waiting time
        tatavg += tat[i];                                     // Add turnaround time to total turnaround time
    }
    
    cout << "\n\n" << left << setw(10) << "Process" << setw(15) << "Arrival" << setw(12) << "Burst"   // Print table header with column names and formatting
         << setw(12) << "Waiting" << setw(12) << "Turnaround" << endl;  // Continue printing header columns
    cout << string(61, '-') << endl;                          // Print separator line with 61 dashes
    
    for(i=0; i<n; i++)                                        // Loop through each process to print its data
    {
        cout << left << setw(10) << ("P" + to_string(i+1))   // Print process name (P1, P2, etc.) with formatting
             << setw(15) << at[i]                             // Print arrival time with formatting
             << setw(12) << bt[i]                             // Print burst time with formatting
             << setw(12) << wt[i]                             // Print waiting time with formatting
             << setw(12) << tat[i] << endl;                   // Print turnaround time with formatting and newline
    }
    
    cout << string(61, '-') << endl;                          // Print separator line with 61 dashes
    cout << "\nAverage Waiting Time = " << wtavg/n;           // Calculate and print average waiting time (total / number of processes)
    cout << "\nAverage Turnaround Time = " << tatavg/n << endl;  // Calculate and print average turnaround time (total / number of processes)

    return 0;                                                 // Return 0 to indicate successful program execution

}
