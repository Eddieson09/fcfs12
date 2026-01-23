#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <iomanip>
#include <cmath>
#include <climits>

using namespace std;

// Process structure with short field names
struct Process {
    int pid, at, bt, rt, ct, tat, wt; 
    bool done; // completion flag
};

// Print scheduling results without table format
void printTable(vector<Process>& p, string algo) {
    cout << "\n" << "Algorithm: " << algo << "\n"; // Print algorithm name
    double twt = 0, ttat = 0; // Initialize totals
    for (auto& x : p) { // For each process
        cout << "PID: " << x.pid << " AT: " << x.at << " BT: " << x.bt  // Print process data
             << " CT: " << x.ct << " TAT: " << x.tat << " WT: " << x.wt << "\n"; // Continue printing
        twt += x.wt; ttat += x.tat; // Add to totals
    }
    cout << "Average WT: " << fixed << setprecision(2) << (twt / p.size()) << "\n"; // Print avg waiting time
    cout << "Average TAT: " << fixed << setprecision(2) << (ttat / p.size()) << "\n"; // Print avg turnaround time
}

// Shortest Job First - Non-preemptive algorithm
void sjf(vector<Process> p) {
    sort(p.begin(), p.end(), [](const Process& a, const Process& b) { // Sort by arrival time then burst time
        return (a.at != b.at) ? a.at < b.at : a.bt < b.bt; // Ternary operator for sorting
    });
    int t = 0; // Current time
    vector<bool> ex(p.size()); // Track executed processes
    for (int i = 0; i < p.size(); i++) { // For each process
        int idx = -1, minb = INT_MAX; // Initialize index and minimum burst
        for (int j = 0; j < p.size(); j++) // Search for best process
            if (!ex[j] && p[j].at <= t && p[j].bt < minb) minb = p[j].bt, idx = j; // Find min burst time
        if (idx == -1) { // If no process available
            for (int j = 0; j < p.size(); j++) // Find next arriving process
                if (!ex[j]) { t = p[j].at; idx = j; break; } // Jump to arrival time
        }
        ex[idx] = true; // Mark as executed
        t += p[idx].bt; // Add burst time to current time
        p[idx].ct = t; // Set completion time
        p[idx].tat = p[idx].ct - p[idx].at; // Calculate turnaround time
        p[idx].wt = p[idx].tat - p[idx].bt; // Calculate waiting time
    }
    sort(p.begin(), p.end(), [](const Process& a, const Process& b) { return a.pid < b.pid; }); // Sort by PID
    printTable(p, "Shortest Job First (SJF)"); // Display results
}

// Shortest Remaining Time First - Preemptive algorithm
void srtf(vector<Process> p) {
    int t = 0, done = 0, n = p.size(); // Initialize time, completion counter, size
    while (done < n) { // While not all processes completed
        int idx = -1, minr = INT_MAX; // Initialize index and minimum remaining
        for (int i = 0; i < n; i++) // Search all processes
            if (p[i].rt > 0 && p[i].at <= t && p[i].rt < minr) minr = p[i].rt, idx = i; // Find min remaining time
        if (idx == -1) { // If no process available
            int na = INT_MAX; // Next arrival
            for (int i = 0; i < n; i++) if (p[i].rt > 0 && p[i].at > t) na = min(na, p[i].at); // Find next arrival
            t = na; continue; // Jump to next arrival
        }
        p[idx].rt--; t++; // Decrement remaining time, increment time
        if (p[idx].rt == 0) { // If process completed
            p[idx].ct = t; // Set completion time
            p[idx].tat = p[idx].ct - p[idx].at; // Calculate turnaround time
            p[idx].wt = p[idx].tat - p[idx].bt; // Calculate waiting time
            done++; // Increment done counter
        }
    }
    sort(p.begin(), p.end(), [](const Process& a, const Process& b) { return a.pid < b.pid; }); // Sort by PID
    printTable(p, "Shortest Remaining Time First (SRTF)"); // Display results
}

// Round Robin - Time quantum based scheduling
void rr(vector<Process> p, int q) {
    int t = 0, done = 0, n = p.size(); // Initialize time, done counter, size
    queue<int> que; // Create queue for processes
    vector<bool> add(n); // Track added processes
    int mint = INT_MAX; // Minimum arrival time
    for (auto& x : p) mint = min(mint, x.at); // Find earliest arrival
    t = mint; // Set current time to first arrival
    for (int i = 0; i < n; i++) if (p[i].at <= t) que.push(i), add[i] = true; // Add initial processes
    while (done < n) { // While not all done
        if (que.empty()) { // If queue empty
            int na = INT_MAX; // Next arrival
            for (int i = 0; i < n; i++) if (!add[i]) na = min(na, p[i].at); // Find next arrival
            t = na; // Jump to next arrival
            for (int i = 0; i < n; i++) if (!add[i] && p[i].at <= t) que.push(i), add[i] = true; // Add arrived processes
        }
        int idx = que.front(); que.pop(); // Get front process from queue
        int et = min(q, p[idx].rt); // Calculate execution time (min of quantum and remaining)
        t += et; p[idx].rt -= et; // Update time and remaining time
        for (int i = 0; i < n; i++) if (!add[i] && p[i].at <= t) que.push(i), add[i] = true; // Add newly arrived
        if (p[idx].rt > 0) que.push(idx); // If still remaining, re-queue
        else { p[idx].ct = t; p[idx].tat = p[idx].ct - p[idx].at; p[idx].wt = p[idx].tat - p[idx].bt; done++; } // Mark completed
    }
    sort(p.begin(), p.end(), [](const Process& a, const Process& b) { return a.pid < b.pid; }); // Sort by PID
    printTable(p, "Round Robin (RR) - Quantum: " + to_string(q)); // Display results
}

// Calculate optimal quantum time using median
int calcQuantum(vector<Process>& p) {
    vector<int> bt; // Vector to store burst times
    for (auto& x : p) bt.push_back(x.bt); // Collect all burst times
    sort(bt.begin(), bt.end()); // Sort burst times
    int n = bt.size(); // Get size
    double med = (n % 2 == 0) ? (bt[n/2-1] + bt[n/2]) / 2.0 : bt[n/2]; // Calculate median (even/odd cases)
    return max(1, (int)round(med)); // Return rounded median, minimum 1
}

// Initialize process fields for scheduling
void init(vector<Process>& p) {
    for (auto& x : p) x.rt = x.bt, x.ct = x.tat = x.wt = 0, x.done = false; // Reset all timing fields
}

// Main function - entry point
int main() {
    // Create process array with predefined data (pid, arrival_time, burst_time, remaining_time, completion_time, tat, wt, done)
    vector<Process> procs = {
        {1, 1, 53, 53, 0, 0, 0, false}, // Process 1
        {2, 3, 43, 43, 0, 0, 0, false}, // Process 2
        {3, 8, 18, 18, 0, 0, 0, false}, // Process 3
        {4, 4, 16, 16, 0, 0, 0, false}, // Process 4
        {5, 6, 24, 24, 0, 0, 0, false}, // Process 5
        {6, 7, 73, 73, 0, 0, 0, false}, // Process 6
        {7, 2, 99, 99, 0, 0, 0, false}, // Process 7
        {8, 5, 27, 27, 0, 0, 0, false}  // Process 8
    };
    cout << "\nCPU SCHEDULING ALGORITHMS\n"; // Print title
    cout << "Total Processes: " << procs.size() << "\n"; // Print count
    int q = calcQuantum(procs); // Calculate optimal quantum time
    cout << "Recommended Quantum Time (Median): " << q << "\n\n"; // Print quantum
    
    auto temp = procs; init(temp); sjf(temp); // Run SJF
    temp = procs; init(temp); srtf(temp); // Run SRTF
    temp = procs; init(temp); rr(temp, q); // Run RR
    
    return 0; // Exit program
}
