#ifndef BICYCLE_H
#define BICYCLE_H

#include <vector>
#include <string>
#include "globals.h"

using namespace std;

void bicycleMenu(vector<Bicycle> &bicycles, bool isStaff);


void registerNewBicycle(vector<Bicycle> &bicycles);
void viewBicycleInventory(const vector<Bicycle> &bicycles);
void updateBicycleStatus(vector<Bicycle> &bicycles);
void removeBicycle(vector<Bicycle> &bicycles);
void generateMaintenanceReport(const vector<Bicycle> &bicycles);

Bicycle* findBicycleByID(vector<Bicycle> &bicycles, const string &id);
string generateNextBicycleID(const vector<Bicycle> &bicycles);

#endif