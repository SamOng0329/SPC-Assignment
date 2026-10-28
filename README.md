# Bicycle Rental Management System 🚲

A console-based C++ application for running a bicycle rental shop: customers, bicycle inventory, bookings, and billing, all driven by a role-based login and text menus.

## What it does

This is a menu-driven simulation of a bike rental shop's back office. On startup you log in as staff, log in as a customer, or register as a new customer. Depending on your role, you get a different set of menus:

| Role | Can do |
|---|---|
| **Customer** | Browse bicycles, create/view/modify/cancel own bookings, pay for bookings and view payment records, view and update own profile |
| **Staff** | Manage customers (view, search, update, delete, filter by tier), register/update/retire bicycles, generate the fleet maintenance report, view the full booking schedule, view payment records, generate the revenue & statistics report |

All data lives in memory for the duration of a run (there is no database or file persistence) and is seeded with a few demo customers, bicycles and staff accounts on startup, so you can log in and try things out immediately.

## Try it out

- Demo staff login: `chongwc` / `chongwc123`
- Demo staff login: `natalie` / `natalie456`
- Demo customer login (Regular): `tanml` / `meiling123`
- Demo customer login (Premium): `chanjj` / `junjie456`

(See `loadSampleData()` in `main.cpp` for the full seed list.)

## Project structure

```
bicycle-rental/
├── SPC.vcxproj               # Visual Studio project file
├── SPC.vcxproj.filters
├── README.md
└── source files
    ├── main.cpp              # Entry point: gateway menu, staff/customer login, role menus, sample data
    ├── globals.h             # Shared constants + structs (Staff, Customer, CustomerLogin, Bicycle, Booking, Payment)
    │
    ├── customer.h / .cpp     # Registration, search, update, delete, filter by tier, customer management menu
    ├── bicycle.h / .cpp      # Inventory view, register/update/remove bicycles, maintenance report
    ├── booking.h / .cpp      # Create/view/modify/cancel bookings, schedule view, slot-conflict checks
    ├── payment.h / .cpp      # Payment processing, receipts, payment history, revenue report
    └── date_utils.h / .cpp   # Date/time parsing, validation and minute-based comparison
```

Each feature area is a header/source pair, with shared types and constants kept in `globals.h`.

## Data model at a glance

- **`Customer`** is a rental customer profile (ID, contact details, `memberType` of Regular or Premium, total rentals, active flag). **`CustomerLogin`** holds the username/password and links to a customer through `customerID`.
- **`Staff`** is a staff account with its own login credentials.
- **`Bicycle`** has a brand, model, type (Mountain / Road / Hybrid), condition, hourly rate and an availability flag. IDs are auto-generated (`BK001`, `BK002`, ...).
- **`Booking`** ties a customer to a bicycle for a start date/time and a duration of 1 to 72 hours, and carries the whole pricing breakdown plus a status of `Active`, `Completed` or `Cancelled`.
- **`Payment`** records the method, amount paid, total and change for a completed booking.
- **`date_utils`** converts `DD/MM/YYYY` + `HH:MM` into minutes since 1970, which makes comparing dates and detecting overlapping booking slots simple.

## Pricing rules

```
Subtotal = hourly rate x duration
Discount = 10% of subtotal (Premium members only)
SST      = 6% of (subtotal - discount)
Total    = (subtotal - discount) + SST + RM 20.00 damage fee (if damage reported)
```

Reporting damage at payment time also marks the bicycle as unavailable with the condition "Needs Repair".

## Building

**Visual Studio:** open `SPC.vcxproj`, then build (`Ctrl+Shift+B`).

**g++ / clang++:**

```bash
g++ -std=c++11 -o bicycle_rental main.cpp customer.cpp bicycle.cpp booking.cpp payment.cpp date_utils.cpp
```

## Running

```bash
./bicycle_rental        # Linux / macOS
bicycle_rental.exe      # Windows
```

## Known limitations

- **No persistence**: all data resets every time the program restarts (it is re-seeded by `loadSampleData()`).
- **Plaintext passwords**: credentials are stored and compared as plain strings, which is fine for a class project or demo but not for real use.
- **Console only**: input handling is text-based, so unexpected input may still need extra validation in some menus.
- Build artifacts (`.exe`, `.obj`, `.pdb`) should be kept out of the repo with a `.gitignore`.
