#include <cctype>
#include <ctime>
#include "date_utils.h"

using namespace std;

bool isLeapYear(int year){
    // 1. If year is not divisible by 4 = common year
    // 2. If year is divisible by 4 but also divisible by 100 = not a leap year (century exception)
    // 3. If year is divisible by 400 = leap year again (special correction for century years)
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

bool isValidDate(int day, int month, int year){
    if(year < 2000 || year > 2100) return false;
    if(month < 1 || month > 12) return false;

    // static bc it will be allocated once for lifetime of the program
    // without static, it will be recreated everytime function runs
    static const int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int maxDay = daysInMonth[month - 1];
    if (month == 2 && isLeapYear(year)) maxDay = 29;

    return day >= 1 && day <= maxDay;
}

bool isValidTime(int hour, int minute){
    return hour >= 0 && hour <= 23 && minute >= 0 && minute <= 59;
}

// Splits DD/MM/YYYY
bool parseDate(const string &dateStr, int &day, int &month, int &year){
    if(dateStr.length() != 10){
        return false;
    }
    if(dateStr[2] != '/' && dateStr[5] != '/'){
        return false;
    }

    for(int i = 0; i < 10; i++){
        if(i == 2 || i == 5) continue;  // skip index 2 and 5 bc slashes already checked
        // unsigned char stores value 0-255, then covert to letters based on ASCII code
        if(!isdigit(static_cast<unsigned char>(dateStr[i]))) return false;
    }

    // stoi = string to int
    int d = stoi(dateStr.substr(0, 2));
    int m = stoi(dateStr.substr(3, 2));
    int y = stoi(dateStr.substr(6, 4));

    if(!isValidDate(d, m ,y)) return false;

    day = d;
    month = m;
    year = y;
    return true;
}

// Splits HH:MM
bool parseTime(const string &timeStr, int &hour, int &minute){
    if(timeStr.length() != 5){
        return false;
    }
    if(timeStr[2] != ':'){
        return false;
    }

    for(int i = 0; i < 5; i++){
        if(i == 2) continue;    // skips ':' cuz already checked
        if(!isdigit(static_cast<unsigned char>(timeStr[i]))) return false;
    }

    int h = stoi(timeStr.substr(0, 2));
    int m = stoi(timeStr.substr(3, 2));

    if(!isValidTime(h, m)) return false;

    hour = h;
    minute = m;
    return true;
}

// Converts date (y,m,d) into single number (total days since 01/01/1970)
// So it is easy to compare dates (date1 < date2) and calculate RENTAL DURATIONS
long long daysFromCivil(int year, int month, int day){
    // If month is JAN or FEB = end of the previous year
    // so leap day (FEB 29) is always the last day of year
    // PURPOSE: Start checks on March 1st, prevent special condition checks mid-year
    year = year - month <= 2;

    // handles negative/historical years safely
    // in 400 years, there are 97 leap years
    // PURPOSE: Find which 400-year cycle (era) the year belongs to (each cycle = 146,097)
    long long era = (year >= 0 ? year : year - 399) / 400;

    // get year's position within the 400-year cycle (0 - 399)
    // unsigned = can changes int types(int, short, long) so can store NEGATIVE values
    // 0 - 399
    unsigned yoe = static_cast<unsigned>(year - era * 400);                     // Year Of Era

    // Since March = month 0, number of days in consecutive months follows repeating pattern
    // 31 + 30 + 31 + 30 + 31 = 153 days
    // 0 - 365
    unsigned doy = (153 * (month + (month > 2 ? -3 : 9)) + 2) / 5 + day - 1;    // Date of Year

    // yoe * 365   = standard days per year
    // + yoe / 4   = adds leap year (every 4 years)
    // - yoe / 100 = removes century non leap years (every 100 years)
    // + doy       = adds the current day within the year
    // PURPOSE: calculate total days from the start of 400-year era up to this specific date
    // 0 - 146096
    unsigned doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;                       // Date of Era

    // combine ERAs and DAYS, then offset so Day 0 presents 01/01/1970
    // era * 146097 + doe = get total days since Year 0
    // - 719468 (total number of days between Year 0 and 01/01/1970) so 01/01/1970 returns 0
    // dates AFTER 1970 return POSITIVE NUM, EARLEIR dates return NEGATIVE NUM
    return era * 146097 + static_cast<long long>(doe) - 719468;
}

// Combines date string and time string
// into a single minute count (minutes since 01/01/1970 00:00)
// returns true if parsing success, or false if date/time invalid
bool toTotalMinutes(const string &dateStr, const string &timeStr, long long &totalMinutes){
    int day, month, year, hour, minute;

    if(!parseDate(dateStr, day, month, year)) return false;
    if(!parseTime(timeStr, hour, minute)) return false;

    long long days = daysFromCivil(year, month, day);
    // 1440 = 24 days * 60 minutes
    // LL(long long) = forces 64-bit integer math to prevent overflow
    totalMinutes = days * 1440LL + hour * 60LL + minute;
    return true;
}

long long getCurrentTotalMinutes() {
    time_t now = time(nullptr);
    tm* localNow = localtime(&now);
 
    int year = localNow->tm_year + 1900;
    int month = localNow->tm_mon + 1;
    int day = localNow->tm_mday;
    int hour = localNow->tm_hour;
    int minute = localNow->tm_min;
 
    long long days = daysFromCivil(year, month, day);
    return days * 1440LL + hour * 60LL + minute;
}