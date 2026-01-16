#include <iostream>
    #include <iomanip>
    using namespace std;

    // Main function - Round Robin Scheduling Simulator
    int main() {
        // =============== VARIABLE DECLARATIONS ===============
        int n, i, completed = 0;                              // n=number of processes, i=counter, completed=finished count
        float current_time = 0, tq;                           // current_time=CPU clock, tq=time quantum
        float at[100], bt[100], rt[100], tat[100], wt[100];  // Arrival Time, Burst Time, Remaining Time, Turnaround Time, Waiting Time
        int gantt_process[500], gantt_count = 0;             // Gantt chart: process IDs and count
        float gantt_start[500], gantt_end[500];              // Gantt chart: start and end times
        float tatavg = 0, wtavg = 0;                         // Sums for average calculations

        // =============== INPUT SECTION ===============
        cout << "Enter the number of processes: ";
        cin >> n;

        // Get arrival time and burst time for each process
        for (i = 0; i < n; i++) {
            cout << "\n/// Process P" << i + 1 << " ///\n";
            cout << "Enter Arrival Time: ";
            cin >> at[i];
            cout << "Enter Burst Time: ";
            cin >> bt[i];

            rt[i] = bt[i];  // Initialize remaining time equal to burst time
        }

        // Get time quantum (time slice per process)
        cout << "\nEnter Time Quantum: ";
        cin >> tq;

        // =============== ROUND ROBIN SCHEDULING ALGORITHM ===============
        while (completed < n) {  // Continue until all processes finish
            int executed = 0;    // Flag: 0 if CPU is idle, 1 if process executes

            // Check each process to see if it can execute
            for (i = 0; i < n; i++) {
                // Check if process has arrived AND still needs CPU time
                if (at[i] <= current_time && rt[i] > 0) {
                    executed = 1;                  // Mark that CPU is not idle
                    int start_time = current_time; // Record when process starts

                    // ========== EXECUTION LOGIC ==========
                    if (rt[i] > tq) {
                        // Process needs more than one time quantum
                        current_time += tq;  // Give it one time quantum
                        rt[i] -= tq;         // Reduce remaining time
                    } else {
                        // Process can finish within time quantum
                        current_time += rt[i];  // Give remaining time
                        rt[i] = 0;              // Mark as finished

                        // Calculate metrics for completed process
                        tat[i] = current_time - at[i]; // TAT = Completion - Arrival
                        wt[i]  = tat[i] - bt[i];       // WT = TAT - Burst

                        // Add to totals for average calculation
                        tatavg += tat[i];
                        wtavg  += wt[i];

                        completed++;  // Increment finished process count
                    }

                    // Store execution information for Gantt chart
                    gantt_process[gantt_count] = i + 1;     // Process ID
                    gantt_start[gantt_count] = start_time;  // When it started
                    gantt_end[gantt_count] = current_time;  // When it ended
                    gantt_count++;                          // Increment step count
                }
            }

            // If no process executed this cycle, CPU is idle - advance time
            if (!executed) {
                current_time++;
            }
        }

        // =============== OUTPUT TABLE ===============
        cout << "\n==================================================================================================\n";
        // Print table header
        cout << left << setw(15) << "Process" << setw(20) << "AT (ms)" << setw(20) << "BT (ms)" 
            << setw(25) << "Turnaround Time (ms)" << "Waiting Time (ms)\n";
        cout << "==================================================================================================\n";

        // Print results for each process
        for (i = 0; i < n; i++) {
            cout << left << setw(15) << ("P" + to_string(i + 1))  // Process name
                << setw(20) << fixed << setprecision(2) << at[i]  // Arrival time
                << setw(20) << fixed << setprecision(2) << bt[i]  // Burst time
                << setw(25) << fixed << setprecision(2) << tat[i] // Turnaround time
                << fixed << setprecision(2) << wt[i] << "\n";     // Waiting time
        }

        // Print average metrics
        cout << "\n\nAverage Turnaround Time: " << fixed << setprecision(2) << tatavg / n << " ms";
        cout << "\nAverage Waiting Time: " << fixed << setprecision(2) << wtavg / n << " ms\n";

        // =============== GANTT CHART DISPLAY ===============
        cout << "\n\n//////////-------- GANTT CHART --------//////////\n";
        
        // Show which processes executed
        cout << "Processes: ";
        for (i = 0; i < gantt_count; i++) {
            cout << "| P" << gantt_process[i] << " ";
        }
        cout << "|\n";

        // Show timeline (time values with decimals)
        cout << "Time:      ";
        for (i = 0; i < gantt_count; i++) {
            cout << fixed << setprecision(2) << gantt_start[i];  // Show decimals
            cout << "    ";                                      // Space for readability
        }
        cout << fixed << setprecision(2) << gantt_end[gantt_count - 1] << "\n";  // Final end time

        return 0;
    }