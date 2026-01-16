#include <iostream>          // For input and output
#include <iomanip>           // For setting width and precision
using namespace std;         // Use standard namespace

int main() {
    int n;                   // Variable to store number of processes
    cout << "\nEnter number of processes: ";  // Ask user for number of processes
    cin >> n;                // Read number of processes

    float burstTime[n];      // Array to store burst time for each process
    float waitingTime[n], turnaroundTime[n];  // Arrays for waiting and turnaround times
    int process[n];          // Array to store process numbers

    // Input burst times manually from the user
    for (int i = 0; i < n; i++) {           // Loop through each process
        process[i] = i + 1;                 // Assign process number (P1, P2, P3...)
        cout << "Enter burst time for process " << (i + 1) << " -- ";  // Ask for burst time
        cin >> burstTime[i];                // Read and store burst time
    }

    // Sort by burst time using bubble sort (Shortest Job First)
    for (int i = 0; i < n - 1; i++) {      // Outer loop for each position
        for (int j = i + 1; j < n; j++) {  // Inner loop to compare elements
            if (burstTime[i] > burstTime[j]) {  // If current is greater than next
                swap(burstTime[i], burstTime[j]);  // Swap burst times
                swap(process[i], process[j]);      // Swap process numbers to keep track
            }
        }
    }

    // Calculation for waiting time
    waitingTime[0] = 0;     // First process has no waiting time
    for (int i = 1; i < n; i++) {           // For all other processes
        // Waiting time = previous waiting time + previous burst time
        waitingTime[i] = waitingTime[i - 1] + burstTime[i - 1];
    }

    // Calculation for turnaround time
    for (int i = 0; i < n; i++) {           // For each process
        // Turnaround time = waiting time + burst time (total time from arrival to completion)
        turnaroundTime[i] = waitingTime[i] + burstTime[i];
    }

    // Calculate average waiting and turnaround times
    float totalWaiting = 0, totalTurnaround = 0;  // Variables to store totals
    for (int i = 0; i < n; i++) {           // Add all times together
        totalWaiting += waitingTime[i];     // Add this process's waiting time
        totalTurnaround += turnaroundTime[i];  // Add this process's turnaround time
    }

    // Display the output
    cout << "\n\n=== SJF (Shortest Job First) Scheduling ===\n";  // Print title
    cout << fixed << setprecision(2);       // Set decimal places to 2
    cout << left << setw(12) << "Process"   // Print "Process" column header
         << right << setw(12) << "BT"       // Print "BT" (Burst Time) column header
         << right << setw(12) << "WT"       // Print "WT" (Waiting Time) column header
         << right << setw(12) << "TAT\n";   // Print "TAT" (Turnaround Time) column header
    cout << string(48, '-') << endl;        // Print separator line

    // Print each process with its values
    for (int i = 0; i < n; i++) {           // Loop through each process
        cout << left << setw(12) << ("P" + to_string(process[i]))  // Print process name (P1, P2...)
             << right << setw(12) << burstTime[i]                   // Print burst time
             << right << setw(12) << waitingTime[i]                 // Print waiting time
             << right << setw(12) << turnaroundTime[i] << endl;     // Print turnaround time
    }

    cout << string(48, '-') << endl;        // Print separator line
    cout << "\nAverage Waiting Time = " << totalWaiting / n;  // Calculate and print average waiting time
    cout << "\nAverage Turnaround Time = " << totalTurnaround / n << endl;  // Calculate and print average turnaround time

    return 0;               // End program successfully
}
