#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <iomanip>
#include <cmath>
#include <climits>
using namespace std;

struct Process {
    int pid, at, bt, rt, ct, tat, wt;
    bool done;
};

void displayTable(vector<Process>& p, string algo) {
    cout << "\n" << string(80, '=') << "\nAlgorithm: " << algo << "\n" << string(80, '=') << "\n";
    cout << left << setw(8) << "PID" << setw(8) << "AT" << setw(8) << "BT" << setw(12) << "CT" << setw(8) << "TAT" << setw(8) << "WT" << "\n" << string(80, '-') << "\n";
    double twt = 0, ttat = 0;
    for (auto& x : p) {
        cout << left << setw(8) << x.pid << setw(8) << x.at << setw(8) << x.bt << setw(12) << x.ct << setw(8) << x.tat << setw(8) << x.wt << "\n";
        twt += x.wt; ttat += x.tat;
    }
    cout << string(80, '-') << "\n" << "Average WT: " << fixed << setprecision(2) << (twt / p.size()) << "\n" << "Average TAT: " << fixed << setprecision(2) << (ttat / p.size()) << "\n";
}

void sjf(vector<Process> p) {
    sort(p.begin(), p.end(), [](const Process& a, const Process& b) { return (a.at != b.at) ? a.at < b.at : a.bt < b.bt; });
    int t = 0; vector<bool> ex(p.size());
    for (int i = 0; i < p.size(); i++) {
        int idx = -1, minb = INT_MAX;
        for (int j = 0; j < p.size(); j++) if (!ex[j] && p[j].at <= t && p[j].bt < minb) minb = p[j].bt, idx = j;
        if (idx == -1) for (int j = 0; j < p.size(); j++) if (!ex[j]) { t = p[j].at; idx = j; break; }
        ex[idx] = true; t += p[idx].bt; p[idx].ct = t; p[idx].tat = p[idx].ct - p[idx].at; p[idx].wt = p[idx].tat - p[idx].bt;
    }
    sort(p.begin(), p.end(), [](const Process& a, const Process& b) { return a.pid < b.pid; });
    displayTable(p, "Shortest Job First (SJF)");
}

void srtf(vector<Process> p) {
    int t = 0, done = 0, n = p.size();
    while (done < n) {
        int idx = -1, minr = INT_MAX;
        for (int i = 0; i < n; i++) if (p[i].rt > 0 && p[i].at <= t && p[i].rt < minr) minr = p[i].rt, idx = i;
        if (idx == -1) {
            int na = INT_MAX;
            for (int i = 0; i < n; i++) if (p[i].rt > 0 && p[i].at > t) na = min(na, p[i].at);
            t = na; continue;
        }
        p[idx].rt--; t++;
        if (p[idx].rt == 0) { p[idx].ct = t; p[idx].tat = p[idx].ct - p[idx].at; p[idx].wt = p[idx].tat - p[idx].bt; done++; }
    }
    sort(p.begin(), p.end(), [](const Process& a, const Process& b) { return a.pid < b.pid; });
    displayTable(p, "Shortest Remaining Time First (SRTF)");
}

void rr(vector<Process> p, int q) {
    int t = 0, done = 0, n = p.size(); queue<int> que; vector<bool> add(n);
    int mint = INT_MAX;
    for (auto& x : p) mint = min(mint, x.at);
    t = mint;
    for (int i = 0; i < n; i++) if (p[i].at <= t) que.push(i), add[i] = true;
    while (done < n) {
        if (que.empty()) {
            int na = INT_MAX;
            for (int i = 0; i < n; i++) if (!add[i]) na = min(na, p[i].at);
            t = na;
            for (int i = 0; i < n; i++) if (!add[i] && p[i].at <= t) que.push(i), add[i] = true;
        }
        int idx = que.front(); que.pop();
        int et = min(q, p[idx].rt);
        t += et; p[idx].rt -= et;
        for (int i = 0; i < n; i++) if (!add[i] && p[i].at <= t) que.push(i), add[i] = true;
        if (p[idx].rt > 0) que.push(idx);
        else { p[idx].ct = t; p[idx].tat = p[idx].ct - p[idx].at; p[idx].wt = p[idx].tat - p[idx].bt; done++; }
    }
    sort(p.begin(), p.end(), [](const Process& a, const Process& b) { return a.pid < b.pid; });
    displayTable(p, "Round Robin (RR) - Quantum: " + to_string(q));
}

int calcQuantum(vector<Process>& p) {
    vector<int> bt;
    for (auto& x : p) bt.push_back(x.bt);
    sort(bt.begin(), bt.end());
    int n = bt.size();
    double med = (n % 2 == 0) ? (bt[n/2-1] + bt[n/2]) / 2.0 : bt[n/2];
    return max(1, (int)round(med));
}

void init(vector<Process>& p) {
    for (auto& x : p) x.rt = x.bt, x.ct = x.tat = x.wt = 0, x.done = false;
}

int main() {
    vector<Process> procs = {{1, 1, 53, 53, 0, 0, 0, false}, {2, 3, 43, 43, 0, 0, 0, false}, {3, 8, 18, 18, 0, 0, 0, false}, {4, 4, 16, 16, 0, 0, 0, false}, {5, 6, 24, 24, 0, 0, 0, false}, {6, 7, 73, 73, 0, 0, 0, false}, {7, 2, 99, 99, 0, 0, 0, false}, {8, 5, 27, 27, 0, 0, 0, false}};
    cout << "\n" << string(80, '=') << "\nCPU SCHEDULING ALGORITHMS\n" << string(80, '=') << "\nTotal Processes: " << procs.size() << "\n" << string(80, '-') << "\n";
    int q = calcQuantum(procs);
    cout << "Recommended Quantum Time (Median): " << q << "\n" << string(80, '=') << "\n\n";
    auto temp = procs; init(temp); sjf(temp);
    temp = procs; init(temp); srtf(temp);
    temp = procs; init(temp); rr(temp, q);
    cout << "\n" << string(80, '=') << "\n";
    return 0;
}
