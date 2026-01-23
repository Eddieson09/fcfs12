#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <iomanip>
#include <cmath>
#include <climits>
// Use standard namespace
using namespace std;

// Process structure to hold process information
struct Process {
    int pid;                    // Process ID - unique identifier for each process
    int arrival_time;          // AT - time when process arrives in queue
    int burst_time;            // BT - total CPU time needed for process
    int remaining_time;        // Remaining time - CPU time still needed
    int completion_time;       // CT - time when process finishes execution
    int turnaround_time;       // TAT - completion time minus arrival time
    int waiting_time;          // WT - turnaround time minus burst time
    bool completed;            // Flag to mark if process is completed
};

// Function to display the scheduling table with results
void displayTable(vector<Process>& processes, string algorithm_name) {
    // Print a separator line
    cout << "\n" << string(80, '=') << endl;
    // Print the algorithm name
    cout << "Algorithm: " << algorithm_name << endl;
    // Print another separator line
    cout << string(80, '=') << endl;
    
    // Print the table headers with fixed width columns
    cout << left << setw(8) << "PID" 
         << setw(8) << "AT" 
         << setw(8) << "BT" 
         << setw(12) << "CT" 
         << setw(8) << "TAT" 
         << setw(8) << "WT" << endl;
    // Print a separator line under headers
    cout << string(80, '-') << endl;
    
    // Initialize variables to accumulate total waiting time and turnaround time
    double total_wt = 0, total_tt = 0;
    
    // Loop through each process to display its data
    for (const auto& p : processes) {
        // Print process data in formatted columns
        cout << left << setw(8) << p.pid 
             << setw(8) << p.arrival_time 
             << setw(8) << p.burst_time 
             << setw(12) << p.completion_time 
             << setw(8) << p.turnaround_time 
             << setw(8) << p.waiting_time << endl;
        // Add to total waiting time
        total_wt += p.waiting_time;
        // Add to total turnaround time
        total_tt += p.turnaround_time;
    }
    
    // Print separator line
    cout << string(80, '-') << endl;
    // Calculate and print average waiting time
    cout << "Average WT: " << fixed << setprecision(2) << (total_wt / processes.size()) << endl;
    // Calculate and print average turnaround time
    cout << "Average TT: " << fixed << setprecision(2) << (total_tt / processes.size()) << endl;
}

// Shortest Job First (SJF) - Non-preemptive scheduling algorithm
void sjf(vector<Process>& processes) {
    // Create a temporary copy of processes for manipulation
    vector<Process> temp = processes;
    // Sort processes by arrival time, then by burst time (for tie-breaking)
    sort(temp.begin(), temp.end(), [](const Process& a, const Process& b) {
        // If arrival times are different, sort by arrival time
        if (a.arrival_time != b.arrival_time)
            return a.arrival_time < b.arrival_time;
        // Otherwise, sort by burst time
        return a.burst_time < b.burst_time;
    });
    
    // Initialize current time to 0
    int current_time = 0;
    // Track which processes have been executed
    vector<bool> executed(temp.size(), false);
    
    // Loop through all processes
    for (int i = 0; i < temp.size(); i++) {
        // Initialize index to -1 (not found)
        int idx = -1;
        // Initialize minimum burst time to maximum value
        int min_burst = INT_MAX;
        
        // Find process with minimum burst time that has arrived
        for (int j = 0; j < temp.size(); j++) {
            // Check if process hasn't been executed AND has arrived AND has minimum burst time
            if (!executed[j] && temp[j].arrival_time <= current_time && temp[j].burst_time < min_burst) {
                // Update minimum burst time
                min_burst = temp[j].burst_time;
                // Update index to current process
                idx = j;
            }
        }
        
        // If no process available, jump to next arrival time
        if (idx == -1) {
            // Search for next unexecuted process
            for (int j = 0; j < temp.size(); j++) {
                // If process hasn't been executed
                if (!executed[j]) {
                    // Set current time to this process's arrival time
                    current_time = temp[j].arrival_time;
                    // Set index to this process
                    idx = j;
                    // Exit loop
                    break;
                }
            }
        }
        
        // Mark process as executed
        executed[idx] = true;
        // Add burst time to current time
        current_time += temp[idx].burst_time;
        // Set completion time
        temp[idx].completion_time = current_time;
        // Calculate turnaround time (completion - arrival)
        temp[idx].turnaround_time = temp[idx].completion_time - temp[idx].arrival_time;
        // Calculate waiting time (turnaround - burst)
        temp[idx].waiting_time = temp[idx].turnaround_time - temp[idx].burst_time;
    }
    
    // Sort back to original PID order for display
    sort(temp.begin(), temp.end(), [](const Process& a, const Process& b) {
        // Sort by process ID in ascending order
        return a.pid < b.pid;
    });
    
    // Display the scheduling results
    displayTable(temp, "Shortest Job First (SJF)");
}

// Shortest Remaining Time First (SRTF) - Preemptive scheduling algorithm
void srtf(vector<Process>& processes) {
    // Create a temporary copy of processes
    vector<Process> temp = processes;
    // Initialize current time to 0
    int current_time = 0;
    // Initialize completed process counter
    int completed = 0;
    // Store total number of processes
    int n = temp.size();
    
    // Loop until all processes are completed
    while (completed < n) {
        // Initialize index to -1 (not found)
        int idx = -1;
        // Initialize minimum remaining time to maximum value
        int min_remaining = INT_MAX;
        
        // Find process with minimum remaining time that has arrived
        for (int i = 0; i < n; i++) {
            // Check if process has remaining time AND has arrived
            if (temp[i].remaining_time > 0 && temp[i].arrival_time <= current_time) {
                // If this process has less remaining time
                if (temp[i].remaining_time < min_remaining) {
                    // Update minimum remaining time
                    min_remaining = temp[i].remaining_time;
                    // Update index to current process
                    idx = i;
                }
            }
        }
        
        // If no process available, jump to next arrival time
        if (idx == -1) {
            // Initialize next arrival to maximum value
            int next_arrival = INT_MAX;
            // Search for next arriving process
            for (int i = 0; i < n; i++) {
                // If process still has work AND hasn't arrived yet
                if (temp[i].remaining_time > 0 && temp[i].arrival_time > current_time) {
                    // Update next arrival time to minimum
                    next_arrival = min(next_arrival, temp[i].arrival_time);
                }
            }
            // Jump to next arrival time
            current_time = next_arrival;
            // Skip to next iteration
            continue;
        }
        
        // Execute process for 1 unit of time
        temp[idx].remaining_time--;
        // Increment current time
        current_time++;
        
        // If process is now complete
        if (temp[idx].remaining_time == 0) {
            // Set completion time
            temp[idx].completion_time = current_time;
            // Calculate turnaround time (completion - arrival)
            temp[idx].turnaround_time = temp[idx].completion_time - temp[idx].arrival_time;
            // Calculate waiting time (turnaround - burst)
            temp[idx].waiting_time = temp[idx].turnaround_time - temp[idx].burst_time;
            // Increment completed counter
            completed++;
        }
    }
    
    // Sort back to original PID order for display
    sort(temp.begin(), temp.end(), [](const Process& a, const Process& b) {
        // Sort by process ID in ascending order
        return a.pid < b.pid;
    });
    
    // Display the scheduling results
    displayTable(temp, "Shortest Remaining Time First (SRTF)");
}

// Round Robin (RR) scheduling algorithm with time quantum
void roundRobin(vector<Process>& processes, int quantum) {
    // Create a temporary copy of processes
    vector<Process> temp = processes;
    // Initialize current time to 0
    int current_time = 0;
    // Create a queue to hold process indices
    queue<int> rr_queue;
    // Track which processes have been added to queue
    vector<bool> added(temp.size(), false);
    // Store total number of processes
    int n = temp.size();
    // Initialize completed process counter
    int completed = 0;
    
    // Find the minimum arrival time
    int min_arrival = INT_MAX;
    // Loop through all processes
    for (const auto& p : temp) {
        // Find minimum arrival time
        min_arrival = min(min_arrival, p.arrival_time);
    }
    // Set current time to first arrival
    current_time = min_arrival;
    
    // Add initial processes that have arrived
    for (int i = 0; i < n; i++) {
        // If process has arrived by current time
        if (temp[i].arrival_time <= current_time) {
            // Add process to queue
            rr_queue.push(i);
            // Mark as added
            added[i] = true;
        }
    }
    
    // Loop until all processes are completed
    while (completed < n) {
        // If queue is empty
        if (rr_queue.empty()) {
            // Find next arriving process
            int next_arrival = INT_MAX;
            // Search for next unscheduled process
            for (int i = 0; i < n; i++) {
                // If process hasn't been added yet
                if (!added[i]) {
                    // Update next arrival time
                    next_arrival = min(next_arrival, temp[i].arrival_time);
                }
            }
            // Jump to next arrival time
            current_time = next_arrival;
            // Add newly arrived processes
            for (int i = 0; i < n; i++) {
                // If process hasn't been added AND has arrived
                if (!added[i] && temp[i].arrival_time <= current_time) {
                    // Add process to queue
                    rr_queue.push(i);
                    // Mark as added
                    added[i] = true;
                }
            }
        }
        
        // Get front process from queue
        int idx = rr_queue.front();
        // Remove from front of queue
        rr_queue.pop();
        
        // Calculate execution time (minimum of quantum and remaining time)
        int execute_time = min(quantum, temp[idx].remaining_time);
        // Add execution time to current time
        current_time += execute_time;
        // Reduce remaining time by execution time
        temp[idx].remaining_time -= execute_time;
        
        // Add newly arrived processes
        for (int i = 0; i < n; i++) {
            // If process hasn't been added AND has arrived
            if (!added[i] && temp[i].arrival_time <= current_time) {
                // Add process to queue
                rr_queue.push(i);
                // Mark as added
                added[i] = true;
            }
        }
        
        // If process still has remaining time
        if (temp[idx].remaining_time > 0) {
            // Add process back to end of queue
            rr_queue.push(idx);
        } else {
            // Set completion time
            temp[idx].completion_time = current_time;
            // Calculate turnaround time (completion - arrival)
            temp[idx].turnaround_time = temp[idx].completion_time - temp[idx].arrival_time;
            // Calculate waiting time (turnaround - burst)
            temp[idx].waiting_time = temp[idx].turnaround_time - temp[idx].burst_time;
            // Increment completed counter
            completed++;
        }
    }
    
    // Sort back to original PID order for display
    sort(temp.begin(), temp.end(), [](const Process& a, const Process& b) {
        // Sort by process ID in ascending order
        return a.pid < b.pid;
    });
    
    // Display the scheduling results with quantum time
    displayTable(temp, "Round Robin (RR) - Quantum: " + to_string(quantum));
}

// Calculate optimal quantum time using median of burst times
int calculateOptimalQuantum(vector<Process>& processes) {
    // Create a vector to store burst times
    vector<int> burst_times;
    // Loop through all processes
    for (const auto& p : processes) {
        // Add burst time to vector
        burst_times.push_back(p.burst_time);
    }
    // Sort burst times in ascending order
    sort(burst_times.begin(), burst_times.end());
    
    // Store the size of burst times vector
    int n = burst_times.size();
    // Declare median variable
    double median;
    // If number of elements is even
    if (n % 2 == 0) {
        // Median is average of two middle elements
        median = (burst_times[n/2 - 1] + burst_times[n/2]) / 2.0;
    } else {
        // If odd, median is the middle element
        median = burst_times[n/2];
    }
    
    // Round median to nearest integer
    int quantum = round(median);
    // Return quantum (at least 1 to avoid division by zero)
    return (quantum > 0) ? quantum : 1;
}

// Initialize process remaining time and reset fields for scheduling
void initializeProcesses(vector<Process>& processes) {
    // Loop through each process
    for (auto& p : processes) {
        // Set remaining time equal to burst time
        p.remaining_time = p.burst_time;
        // Reset completion time to 0
        p.completion_time = 0;
        // Reset turnaround time to 0
        p.turnaround_time = 0;
        // Reset waiting time to 0
        p.waiting_time = 0;
        // Reset completed flag to false
        p.completed = false;
    }
}

// Main function - entry point of the program
int main() {
    // Create a vector of processes with predefined data
    vector<Process> processes = {
       
        {1, 1, 53},
        {2, 3, 43},
        {3, 8, 18},
        {4, 4, 16},
        {5, 6, 24},
        {6, 7, 73},
        {7, 2, 99},
        {8, 5, 27}
    };
    
    // Print a separator line
    cout << "\n" << string(80, '=') << endl;
    // Print the title
    cout << "CPU SCHEDULING ALGORITHMS" << endl;
    // Print another separator line
    cout << string(80, '=') << endl;
    // Print total number of processes
    cout << "Total Processes: " << processes.size() << endl;
    // Print a divider line
    cout << string(80, '-') << endl;
    
    // Calculate optimal quantum time based on median burst time
    int optimal_quantum = calculateOptimalQuantum(processes);
    // Print the recommended quantum time
    cout << "Recommended Quantum Time (Median): " << optimal_quantum << endl;
    // Print separator line
    cout << string(80, '=') << "\n" << endl;
    
    // Execute SJF algorithm
    // Create a copy of processes
    vector<Process> temp = processes;
    // Initialize process fields for SJF
    initializeProcesses(temp);
    // Run SJF scheduling
    sjf(temp);
    
    // Execute SRTF algorithm
    // Create a copy of processes
    temp = processes;
    // Initialize process fields for SRTF
    initializeProcesses(temp);
    // Run SRTF scheduling
    srtf(temp);
    
    // Execute Round Robin algorithm
    // Create a copy of processes
    temp = processes;
    // Initialize process fields for RR
    initializeProcesses(temp);
    // Run Round Robin scheduling with calculated quantum
    roundRobin(temp, optimal_quantum);
    
    // Print final separator line
    cout << "\n" << string(80, '=') << endl;
    
    // Return 0 to indicate successful execution
    return 0;
}
