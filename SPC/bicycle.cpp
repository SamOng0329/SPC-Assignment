#include <iostream>
#include <iomanip>
#include <sstream>
#include <cctype>
#include "bicycle.h"
 
using namespace std;

Bicycle *findBicycleByID(vector<Bicycle> &bicycles, const string &id){
    for(auto &bicycle : bicycles){
        if (bicycle.id == id){
            return &bicycle;    // if found return the entire bicycle data
        }
    }
    // if no found, return null
    return nullptr;
}