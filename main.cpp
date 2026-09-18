#include <iostream>
#include <string>
#include <thread>
#include <chrono>
using namespace std;

struct Registers {
    string status = "READY";
    string control = "IDLE";
    string data = "EMPTY";
};

class ControllerSimulator {
    Registers r;

public:
    void showControllers() {
        cout << "\n1. NVMe Controller\n"
             << "2. SATA Controller\n"
             << "3. Ethernet Controller\n"
             << "4. GPU Controller\n";
    }

    void io(const string& data) {
        r.status = "BUSY";
        r.control = "WRITE";
        r.data = data;

        cout << "\nControl: " << r.control
             << "\nData: " << r.data << '\n';

        this_thread::sleep_for(chrono::milliseconds(500));

        r.status = "READY";
        r.control = "IDLE";
        cout << "Operacija zavrsena - IRQ poslat procesoru.\n";
    }

    void dma(const string& data) {
        r.status = "DMA_TRANSFER";
        cout << "\nDMA prenos: " << data << '\n';

        this_thread::sleep_for(chrono::milliseconds(500));

        r.status = "READY";
        cout << "DMA prenos zavrsen - IRQ poslat procesoru.\n";
    }
};

int main() {
    ControllerSimulator sim;
    int izbor;

    do {
        cout << "\n--- SIMULACIJA KONTROLERA ---\n"
             << "1. Prikazi kontrolere\n"
             << "2. I/O operacija\n"
             << "3. DMA prenos\n"
             << "0. Izlaz\nIzbor: ";
        cin >> izbor;

        if (izbor == 1)
            sim.showControllers();
        else if (izbor == 2) {
            string data;
            cout << "Podatak: ";
            cin >> data;
            sim.io(data);
        }
        else if (izbor == 3) {
            string data;
            cout << "Blok podataka: ";
            cin >> data;
            sim.dma(data);
        }
    } while (izbor != 0);

    return 0;
}
