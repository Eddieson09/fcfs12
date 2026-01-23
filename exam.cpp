#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <iomanip>
#include <cmath>
#include <climits>

using namespace std;

// Process structure
struct Process {
    int pid;                    // Process ID
    int arrival_time;          // AT
    int burst_time;            // BT
    int remaining_time;        // Remaining time
    int completion_time;       // CT
    int turnaround_time;       // TT
    int waiting_time;          // WT
    bool completed;            // Flag for completion
};

// Function to display the scheduling results
void displayTable(vector<Process>& processes, string algorithm_name) {
    cout << "\nAlgorithm: " << algorithm_name << endl;
    
    double total_wt = 0, total_tt = 0;
    
    for (const auto& p : processes) {
        cout << "PID: " << p.pid 
             << " AT: " << p.arrival_time 
             << " BT: " << p.burst_time 
             << " CT: " << p.completion_time 
             << " TT: " << p.turnaround_time 
             << " WT: " << p.waiting_time << endl;
        total_wt += p.waiting_time;
        total_tt += p.turnaround_time;
    }
    
    cout << "Average WT: " << fixed << setprecision(2) << (total_wt / processes.size()) << endl;
    cout << "Average TT: " << fixed << setprecision(2) << (total_tt / processes.size()) << endl;
}

// Shortest Job First (SJF) - Non-preemptive
void sjf(vector<Process>& processes) {
    vector<Process> temp = processes;
    sort(temp.begin(), temp.end(), [](const Process& a, const Process& b) {
        if (a.arrival_time != b.arrival_time)
            return a.arrival_time < b.arrival_time;
        return a.burst_time < b.burst_time;
    });
    
    int current_time = 0;
    vector<bool> executed(temp.size(), false);
    
    for (int i = 0; i < temp.size(); i++) {
        int idx = -1;
        int min_burst = INT_MAX;
        
        // Find process with minimum burst time that has arrived
        for (int j = 0; j < temp.size(); j++) {
            if (!executed[j] && temp[j].arrival_time <= current_time && temp[j].burst_time < min_burst) {
                min_burst = temp[j].burst_time;
                idx = j;
            }
        }
        
        // If no process available, jump to next arrival
        if (idx == -1) {
            for (int j = 0; j < temp.size(); j++) {
                if (!executed[j]) {
                    current_time = temp[j].arrival_time;
                    idx = j;
                    break;
                }
            }
        }
        
        executed[idx] = true;
        current_time += temp[idx].burst_time;
        temp[idx].completion_time = current_time;
        temp[idx].turnaround_time = temp[idx].completion_time - temp[idx].arrival_time;
        temp[idx].waiting_time = temp[idx].turnaround_time - temp[idx].burst_time;
    }
    
    // Sort back to original PID order for display
    sort(temp.begin(), temp.end(), [](const Process& a, const Process& b) {
        return a.pid < b.pid;
    });
    
    displayTable(temp, "Shortest Job First (SJF)");
}

// Shortest Remaining Time First (SRTF) - Preemptive
void srtf(vector<Process>& processes) {
    vector<Process> temp = processes;
    int current_time = 0;
    int completed = 0;
    int n = temp.size();
    
    while (completed < n) {
        int idx = -1;
        int min_remaining = INT_MAX;
        
        // Find process with minimum remaining time that has arrived
        for (int i = 0; i < n; i++) {
            if (temp[i].remaining_time > 0 && temp[i].arrival_time <= current_time) {
                if (temp[i].remaining_time < min_remaining) {
                    min_remaining = temp[i].remaining_time;
                    idx = i;
                }
            }
        }
        
        // If no process available, jump to next arrival
        if (idx == -1) {
            int next_arrival = INT_MAX;
            for (int i = 0; i < n; i++) {
                if (temp[i].remaining_time > 0 && temp[i].arrival_time > current_time) {
                    next_arrival = min(next_arrival, temp[i].arrival_time);
                }
            }
            current_time = next_arrival;
            continue;
        }
        
        // Execute for 1 unit of time
        temp[idx].remaining_time--;
        current_time++;
        
        if (temp[idx].remaining_time == 0) {
            temp[idx].completion_time = current_time;
            temp[idx].turnaround_time = temp[idx].completion_time - temp[idx].arrival_time;
            temp[idx].waiting_time = temp[idx].turnaround_time - temp[idx].burst_time;
            completed++;
        }
    }
    
    // Sort back to original PID order for display
    sort(temp.begin(), temp.end(), [](const Process& a, const Process& b) {
        return a.pid < b.pid;
    });
    
    displayTable(temp, "Shortest Remaining Time First (SRTF)");
}

// Round Robin (RR)
void roundRobin(vector<Process>& processes, int quantum) {
    vector<Process> temp = processes;
    int current_time = 0;
    queue<int> rr_queue;
    vector<bool> added(temp.size(), false);
    int n = temp.size();
    int completed = 0;
    
    // Find the minimum arrival time
    int min_arrival = INT_MAX;
    for (const auto& p : temp) {
        min_arrival = min(min_arrival, p.arrival_time);
    }
    current_time = min_arrival;
    
    // Add initial processes that have arrived
    for (int i = 0; i < n; i++) {
        if (temp[i].arrival_time <= current_time) {
            rr_queue.push(i);
            added[i] = true;
        }
    }
    
    while (completed < n) {
        if (rr_queue.empty()) {
            // Find next arriving process
            int next_arrival = INT_MAX;
            for (int i = 0; i < n; i++) {
                if (!added[i]) {
                    next_arrival = min(next_arrival, temp[i].arrival_time);
                }
            }
            current_time = next_arrival;
            for (int i = 0; i < n; i++) {
                if (!added[i] && temp[i].arrival_time <= current_time) {
                    rr_queue.push(i);
                    added[i] = true;
                }
            }
        }
        
        int idx = rr_queue.front();
        rr_queue.pop();
        
        int execute_time = min(quantum, temp[idx].remaining_time);
        current_time += execute_time;
        temp[idx].remaining_time -= execute_time;
        
        // Add newly arrived processes
        for (int i = 0; i < n; i++) {
            if (!added[i] && temp[i].arrival_time <= current_time) {
                rr_queue.push(i);
                added[i] = true;
            }
        }
        
        if (temp[idx].remaining_time > 0) {
            rr_queue.push(idx);
        } else {
            temp[idx].completion_time = current_time;
            temp[idx].turnaround_time = temp[idx].completion_time - temp[idx].arrival_time;
            temp[idx].waiting_time = temp[idx].turnaround_time - temp[idx].burst_time;
            completed++;
        }
    }
    
    // Sort back to original PID order for display
    sort(temp.begin(), temp.end(), [](const Process& a, const Process& b) {
        return a.pid < b.pid;
    });
    
    displayTable(temp, "Round Robin (RR) - Quantum: " + to_string(quantum));
}

// Calculate optimal quantum time using mean
int calculateOptimalQuantum(vector<Process>& processes) {
    double sum = 0;
    for (const auto& p : processes) {
        sum += p.burst_time;
    }
    double mean = sum / processes.size();
    int quantum = round(mean);
    return (quantum > 0) ? quantum : 1;
}

// Initialize process remaining time and reset fields
void initializeProcesses(vector<Process>& processes) {
    for (auto& p : processes) {
        p.remaining_time = p.burst_time;
        p.completion_time = 0;
        p.turnaround_time = 0;
        p.waiting_time = 0;
        p.completed = false;
    }
}

int main() {
    // Process data - predefined
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
    
    cout << "\nCPU SCHEDULING ALGORITHMS" << endl;
    cout << "Total Processes: " << processes.size() << endl;
    
    // Calculate optimal quantum time
    int optimal_quantum = calculateOptimalQuantum(processes);
    cout << "Recommended Quantum Time (Mean): " << optimal_quantum << endl;
    cout << endl;
    
    // SJF
    vector<Process> temp = processes;
    initializeProcesses(temp);
    sjf(temp);
    
    // SRTF
    temp = processes;
    initializeProcesses(temp);
    srtf(temp);
    
    // Round Robin
    temp = processes;
    initializeProcesses(temp);
    roundRobin(temp, optimal_quantum);
    
    return 0;
}
