#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include "bicycle.h"

using namespace std;

Bicycle* findBicycleByID(vector<Bicycle> &bicycles, const string &id) {
    for (Bicycle &bike : bicycles) {
        if (bike.id == id) {
            return &bike;
        }
    }
    return nullptr;
}

string generateNextBicycleID(const vector<Bicycle> &bicycles) {
    int maxID = 0;
    for (const Bicycle& bike : bicycles) {
        if (bike.id.length() > 2 && bike.id.substr(0, 2) == "BK") {
            int currentID = stoi(bike.id.substr(2));
            if (currentID > maxID) maxID = currentID;
        }
    }
    stringstream ss;
    ss << "BK" << setfill('0') << setw(3) << (maxID + 1);
    return ss.str();
}

void bicycleMenu(vector<Bicycle> &bicycles, bool isStaff) {
    int choice;
    do {
        cout << "\n  +-------------------------------------------+\n";
        cout << "  |     BICYCLE REGISTRATION & INVENTORY      |\n";
        cout << "  +-------------------------------------------+\n";
        cout << "   1. View Bicycle Inventory\n";
        
        if (isStaff) {
            cout << "   2. Register New Bicycle\n";
            cout << "   3. Update Bicycle Status\n";
            cout << "   4. Remove / Retire Bicycle\n";
            cout << "   5. Generate Maintenance Report (Extra)\n";
        }
        cout << "   0. Back to Main Menu\n";
        cout << "  ---------------------------------------------\n";
        cout << "  Choice -> ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "  [!] Invalid input. Please enter a number.\n";
            continue;
        }

        if (choice == 1) {
            viewBicycleInventory(bicycles);
        } else if (isStaff && choice == 2) {
            registerNewBicycle(bicycles);
        } else if (isStaff && choice == 3) {
            updateBicycleStatus(bicycles);
        } else if (isStaff && choice == 4) {
            removeBicycle(bicycles);
        } else if (isStaff && choice == 5) {
            generateMaintenanceReport(bicycles);
        } else if (choice != 0) {
            cout << "  [!] Invalid choice or insufficient permissions.\n";
        }
    } while (choice != 0);
}

void registerNewBicycle(vector<Bicycle> &bicycles) {
    Bicycle newBike;
    newBike.id = generateNextBicycleID(bicycles);
    newBike.isAvailable = true;
    newBike.condition = "Good";

    cout << "\n  [Registering New Bicycle - " << newBike.id << "]\n";
    cout << "  Enter Brand -> ";
    getline(cin >> ws, newBike.brand);
    
    cout << "  Enter Model -> ";
    getline(cin >> ws, newBike.model);
    
    cout << "  Enter Type (Mountain/Road/Hybrid) -> ";
    getline(cin >> ws, newBike.type);
    
    cout << "  Enter Hourly Rate (RM) -> ";
    cin >> newBike.hourlyRate;

    bicycles.push_back(newBike);
    cout << "  [+] Successfully registered " << newBike.brand << " " << newBike.model << "!\n";
}

void viewBicycleInventory(const vector<Bicycle> &bicycles) {
    if (bicycles.empty()) {
        cout << "  [!] No bicycles found in the inventory.\n";
        return;
    }

    cout << fixed << setprecision(2);
    cout << "\n  +--------------------------------------------------------------------------------------------------+\n";
    cout << "  |                                     BICYCLE INVENTORY                                            |\n";
    cout << "  +-------+-----------------+-----------------+----------+-------------+-----------------+-----------+\n";
    cout << "  | ID    | Brand           | Model           | Type     | Hourly (RM) | Condition       | Status    |\n";
    cout << "  +-------+-----------------+-----------------+----------+-------------+-----------------+-----------+\n";
    
    for (const Bicycle &bike : bicycles) {
        string statusStr = bike.isAvailable ? "Available" : "Maintenance"; 
        
        cout << "  | " << left << setw(5) << bike.id
             << " | " << left << setw(15) << (bike.brand.length() > 15 ? bike.brand.substr(0, 12) + "..." : bike.brand)
             << " | " << left << setw(15) << (bike.model.length() > 15 ? bike.model.substr(0, 12) + "..." : bike.model)
             << " | " << left << setw(8)  << bike.type
             << " | " << right << setw(11) << bike.hourlyRate
             << " | " << left << setw(15) << bike.condition
             << " | " << left << setw(9)  << statusStr << " |\n";
    }
    cout << "  +-------+-----------------+-----------------+----------+-------------+-----------------+-----------+\n\n";
}

void updateBicycleStatus(vector<Bicycle> &bicycles) {
    string searchID;
    cout << "\n  Enter Bicycle ID to update (e.g., BK001) -> ";
    cin >> searchID;

    Bicycle *bike = findBicycleByID(bicycles, searchID);
    if (bike == nullptr) {
        cout << "  [!] Error: Bicycle ID '" << searchID << "' not found.\n";
        return;
    }

    cout << "\n  [Current Status of " << bike->id << "]\n";
    cout << "  Condition: " << bike->condition << "\n";
    cout << "  Available: " << (bike->isAvailable ? "Yes" : "No") << "\n";

    cout << "\n  Enter New Condition (e.g., Good, Needs Repair) -> ";
    getline(cin >> ws, bike->condition);

    char availChar;
    cout << "  Is it available for rent? (Y/N) -> ";
    cin >> availChar;
    
    if (tolower(availChar) == 'y') {
        bike->isAvailable = true;
    } else {
        bike->isAvailable = false;
    }

    cout << "  [+] Status for " << bike->id << " updated successfully.\n";
}

void removeBicycle(vector<Bicycle> &bicycles) {
    string searchID;
    cout << "\n  Enter Bicycle ID to remove -> ";
    cin >> searchID;

    for (auto it = bicycles.begin(); it != bicycles.end(); ++it) {
        if (it->id == searchID) {
            char confirm;
            cout << "  Are you sure you want to remove " << it->brand << " " << it->model << "? (Y/N) -> ";
            cin >> confirm;
            if (tolower(confirm) == 'y') {
                bicycles.erase(it);
                cout << "  [+] Bicycle removed from system.\n";
            } else {
                cout << "  [#] Removal cancelled.\n";
            }
            return;
        }
    }
    cout << "  [!] Error: Bicycle ID not found.\n";
}

// --- 附加功能 (Extra Feature)：生成维护与库存报告 ---
void generateMaintenanceReport(const vector<Bicycle> &bicycles) {
    int total = bicycles.size();
    int availableCount = 0;
    int maintenanceCount = 0;

    cout << "\n  +-------------------------------------------+\n";
    cout << "  |          SYSTEM MAINTENANCE REPORT        |\n";
    cout << "  +-------------------------------------------+\n";

    if (total == 0) {
        cout << "  | No bicycles in the system yet.            |\n";
        cout << "  +-------------------------------------------+\n\n";
        return;
    }

    for (const Bicycle &bike : bicycles) {
        if (bike.isAvailable) {
            availableCount++;
        } else {
            maintenanceCount++;
        }
    }

    double readiness = ((double)availableCount / total) * 100;

    cout << "  | Total Fleet Size    : " << left << setw(19) << total << " |\n";
    cout << "  | Ready for Rent      : " << left << setw(19) << availableCount << " |\n";
    cout << "  | Under Maintenance   : " << left << setw(19) << maintenanceCount << " |\n";
    cout << "  +-------------------------------------------+\n";
    cout << "  | Fleet Readiness     : " << left << setw(18) << fixed << setprecision(1) << readiness << "% |\n";
    cout << "  +-------------------------------------------+\n";
    cout << "  [DONE] Report generated successfully.\n\n";
}