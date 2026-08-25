#ifndef DATE_UTILS_H
#define DATE_UTILS_H

#include <string>
using namespace std;
// Shared data/time utlity functions


// Leap Year = February got 29 days rather than 28 days
bool isLeapYear(int year);              

// Check date format
bool isValidDate(int day, int month, int year);

// Check time format
bool isValidTime(int hour, int minute);

// Parses DD/MM/YYYY
// Returns false and leaves output untouched if invalid
bool parseDate(const string &dateStr, int &day, int &month, int &year);

// Parses HH:MM
// Returns false if invalid
bool parseTime(const string &timeStr, int &hour, int &minute);

// Converts a valid (year,month,day) into day count
// e.g. 25/08/2026 = 739854 days, which means can do comparison
// otherwise, 28/12/2026 will be compare as SMALLER than 28/01/2026 because '0' < '2'
// different months have different max days, calculating differences is complex af
long long daysFromCivil(int year, int month, int day);

// Combines a valid date + time into a comparable value
// Returns true and set totalMinutes if both date & time valid, else false
bool toTotalMinutes(const string &dateStr, const string &timeStr, long long &totalMinutes);

// Reads the real system clock and returns "now" in minutes-since-epoch format
// same as toTotalMinutes for easier comparision
long long getCurrentTotalMinutes();

#endif