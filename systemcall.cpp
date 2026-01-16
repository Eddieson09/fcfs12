// Include necessary header files
#include <iostream>      // For input/output operations
#include <stdlib.h>      // For exit() function
#include <unistd.h>      // For fork(), getpid(), getppid() system calls
#include <sys/wait.h>    // For wait() function
using namespace std;

// Main function to demonstrate process creation using fork()
int main(void)
{
    // First fork: Create P2 from P1 (Original Parent)
    int pid1 = fork();
    if (pid1 < 0) {
        cout << "Fork failed\n";
        exit(1);
    }
    else if (pid1 == 0) {
        // P2 process: First child of P1
        int pid2 = fork();
        if (pid2 < 0) {
            cout << "Fork failed\n";
            exit(1);
        }
        else if (pid2 == 0) {
            // P3 process: Second child of P2
            int pid3 = fork();
            if (pid3 < 0) {
                cout << "Fork failed\n";
                exit(1);
            }
            else if (pid3 == 0) {
                // P4 process: Third child of P3
                int pid4 = fork();
                if (pid4 < 0) {
                    cout << "Fork failed\n";
                    exit(1);
                }
                else if (pid4 == 0) {
                    // P5 process: Fourth child of P4 (Latest child - outputs first)
                    cout << "P5 (4th Child) - PID: " << getpid() << ", Parent: " << getppid() << "\n";
                    exit(0);
                }
                else {
                    // P4 waits for P5 to complete, then outputs
                    wait(NULL);
                    cout << "P4 (3rd Child) - PID: " << getpid() << ", Parent: " << getppid() << "\n";
                    exit(0);
                }
            }
            else {
                // P3 waits for P4 to complete, then outputs
                wait(NULL);
                cout << "P3 (2nd Child) - PID: " << getpid() << ", Parent: " << getppid() << "\n";
                exit(0);
            }
        }
        else {
            // P2 waits for P3 to complete, then outputs
            wait(NULL);
            cout << "P2 (1st Child) - PID: " << getpid() << ", Parent: " << getppid() << "\n";
            exit(0);
        }
    }
    else {
        // P1 process: Original parent waits for P2 to complete, then outputs
        wait(NULL);
        cout << "P1 (Parent) - PID: " << getpid() << "\n";
    }
    
    return 0;
}
