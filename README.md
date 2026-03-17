# CSCE-548-Project
Full Stack Application Development


# Body Sculpt App – Phase 1
CSCE 548 – Building Secure Software
Project 1: Database and Data Access Layer

## Overview
- The Body Sculpt App is a console-based C++ application designed to track users’ workouts, meals, and body metrics as part of a larger body sculpting and fitness system.
- Phase 1 focuses on creating a high-quality PostgreSQL database schema and a functional C++ Data Access Layer (DAL) using libpqxx (7+).
- The application demonstrates full CRUD functionality across multiple related tables through a menu-driven console interface and successfully retrieves and manages data       from the database.

## Technologies Used
- C++ (Visual Studio 2022)
- PostgreSQL
- libpqxx (7+)
- pgAdmin
- GitHub

## Database Design
The database contains 5 tables with properly defined primary keys, foreign key relationships, and constraints to enforce data integrity.

## Tables
- users
-- user_id (integer, primary key)
-- first_name (character varying)
-- last_name (character varying)
-- email (character varying)
-- height_cm (numeric)
-- created_at (timestamp)

- meals
-- meal_id (integer, primary key)
-- user_id (foreign key → users.user_id)
-- meal_time (timestamp)
-- meal_type (character varying)
-- notes (character varying, nullable)

- meal_items
-- item_id (integer, primary key)
-- meal_id (foreign key → meals.meal_id)
-- food_name (character varying)
-- calories (integer)
-- protein_g (numeric)
-- carbs_g (numeric)
-- fat_g (numeric)

-workouts
-- workout_id (integer, primary key)
-- user_id (foreign key → users.user_id)
-- workout_date (date)
-- duration_min (integer)
-- notes (character varying, nullable)

- workout_entries
-- entry_id (integer, primary key)
-- workout_id (foreign key → workouts.workout_id)
-- exercise_name (character varying)
-- sets (integer)
-- reps (integer)
-- weight_kg (numeric)

NOTE: SQL scripts for creating tables and inserting test data are located in the sql/ directory.

## Features
- Console-based menu interface
- Full CRUD operations (Create, Read, Update, Delete) across all five tables
- PostgreSQL database connectivity using libpqxx
- Data Access Layer (DAL) abstraction for each table
- Proper handling of PostgreSQL numeric and nullable data types
- Demonstration of foreign key relationships through filtered queries
- Error handling for SQL and runtime exceptions

## Project Structure
CSCE-548-Project/
├── sql/
│ ├── create_tables.sql
│ └── insert_test_data.sql
├── src/
│ ├── db/
│ │ └── Db.h
│ ├── models/
│ │ ├── User.h
│ │ ├── Meal.h
│ │ ├── MealItem.h
│ │ ├── Workout.h
│ │ └── WorkoutEntry.h
│ ├── dal/
│ │ ├── UserDAL.h
│ │ ├── UserDAL.cpp
│ │ ├── MealDAL.h
│ │ ├── MealDAL.cpp
│ │ ├── MealItemDAL.h
│ │ ├── MealItemDAL.cpp
│ │ ├── WorkoutDAL.h
│ │ ├── WorkoutDAL.cpp
│ │ ├── WorkoutEntryDAL.h
│ │ └── WorkoutEntryDAL.cpp
│ └── BodySculptApp.cpp
├── README.md
└── .gitignore

## How to Run
1) Ensure PostgreSQL is running and the body_sculpt database is created.
2) Execute the SQL scripts in the sql/ directory to create tables and insert test data.
3) Update the PostgreSQL connection string in BodySculptApp.cpp if necessary.
4) Build the project in Visual Studio.
5) Run the program using Ctrl + F5 (Start Without Debugging).
6) Use the console menus to retrieve and manage records across all tables.

## Sample Console Output
--- USERS ---
1: Myra Walker | myra1@example.com
 | height_cm=165.1
2: Jordan Lee | jordan@example.com
 | height_cm=180.34
 
--- MEALS ---
3 | user_id=1 | time=2024-10-01 08:30:00 | type=breakfast | notes=Oatmeal

--- WORKOUTS ---
5 | user_id=1 | date=2024-10-02 | duration_min=60 | notes=Leg day

## Notes
Visual Studio generated files (such as .vs/, .ipch/, and build outputs) are excluded using .gitignore. The database was populated with 50+ rows of test data to demonstrate functionality. This phase establishes a solid foundation for future phases involving business logic, analytics, and user interface expansion.

## Author
Myra Walker




# Body Sculpt App – Phase 2
CSCE 548 – Building Secure Software
Project 2: Business Logic and Service Layers

## Overview

BodySculptApp is a backend fitness tracking application developed in C++ as part of **CSCE 548**. The system allows users to track workouts and meals in order to monitor their fitness progress.

The application implements a layered architecture consisting of:

- Models
- Data Access Layer (DAL)
- Business Logic Layer (Services)
- HTTP Service Layer
- Console Client for testing

The backend communicates with a **PostgreSQL database** using **libpqxx** and exposes services through a lightweight HTTP server built with **cpp-httplib**.

---

## Features

The application supports the following functionality:

### User Management
- Create users
- Retrieve all users

### Workout Tracking
- Create workouts
- Retrieve workouts
- Create workout entries
- Retrieve workout entries

### Meal Tracking
- Create meals
- Retrieve meals
- Create meal items
- Retrieve meal items

### Logging
The system logs application events such as server startup and HTTP requests to:


app.log


---

## Technologies Used

- **C++**
- **PostgreSQL**
- **libpqxx** (PostgreSQL C++ client library)
- **cpp-httplib** (HTTP server)
- **Visual Studio**
- **GitHub**

---

## Project Architecture


src
│
├── business
│ ├── UserService
│ ├── WorkoutService
│ └── MealService
│
├── dal
│ ├── UserDAL
│ ├── WorkoutDAL
│ ├── WorkoutEntryDAL
│ ├── MealDAL
│ └── MealItemDAL
│
├── models
│ ├── User
│ ├── Workout
│ ├── WorkoutEntry
│ ├── Meal
│ └── MealItem
│
├── server
│ └── HttpServer
│
├── client
│ ├── ApiClient
│ └── ConsoleUI
│
└── utils
└── Logger


### Models
Represent database entities such as users, workouts, meals, and related records.

### Data Access Layer (DAL)
Handles database queries and maps PostgreSQL results to C++ objects.

### Business Layer
Implements validation and business rules before calling the DAL.

### HTTP Service Layer
Exposes REST-style endpoints that interact with the business layer.

### Console Client
Provides a menu-driven interface for testing the services.

---

## HTTP Endpoints

The application exposes the following endpoints:


GET /users
POST /users

GET /workouts
POST /workouts

GET /workouts/{id}/entries
POST /workouts/{id}/entries

GET /meals
POST /meals

GET /meals/{id}/items
POST /meals/{id}/items


---

## Database Tables

The PostgreSQL database contains the following tables:

### Users


user_id
first_name
last_name
email
height_cm


### Workouts


workout_id
user_id
workout_date
duration_min
notes


### Workout Entries


entry_id
workout_id
exercise_name
sets
reps
weight_kg


### Meals


meal_id
user_id
meal_time
meal_type
notes


### Meal Items


item_id
meal_id
food_name
calories
protein_g
carbs_g
fat_g


---

## Business Rules

The service layer validates inputs before inserting records into the database.

### User Rules
- First name cannot be blank
- Last name cannot be blank
- Email must contain "@"
- Height must be positive

### Workout Rules
- User must exist
- Duration must be positive
- Workout date must be provided

### Workout Entry Rules
- Workout ID must exist
- Sets and reps must be positive
- Weight cannot be negative

### Meal Rules
Allowed meal types:


breakfast
lunch
dinner
snack


Additional rules:

- Meal time must be provided
- User ID must exist

### Meal Item Rules
- Food name cannot be blank
- Nutrition values cannot be negative

---

## Running the Application

### 1. Start the server

Run the main application:


BodySculptApp.exe


The server starts on:


http://localhost:8080


---

### 2. Use the Console Client

The console client provides a menu for testing the services:


Create User

Get All Users

Create Workout

Get All Workouts

Create Workout Entry

Get Workout Entries

Create Meal

Get All Meals

Create Meal Item

Get Meal Items


---

## Logging

Application events are logged to:


app.log


Example log output:


[INFO] Application starting
[INFO] Server started on port 8080
[INFO] POST /users
[INFO] GET /users
[INFO] POST /workouts
[INFO] POST /meals


---

## Testing

All endpoints were tested using the console client. Testing included:

- Creating records
- Retrieving records
- Verifying database inserts
- Confirming logging output

Screenshots of console tests and database results are included in the project submission.

---

## Repository

This project is hosted on GitHub as part of the **CSCE 548 coursework**.


https://github.com/
<your-username>/CSCE-548-Project


---

## Author

Myra Walker  
CSCE 548 – Full Stack Development




# Body Sculpt App – Phase 3
CSCE 548 – Building Secure Software
Project 3: Backend and Frontend Expansion

## **Overview**
Phase 3 of the Body Sculpt App extends the project into a full-stack application by integrating a frontend user interface with the backend REST API developed in previous phases. This phase enables users to interact with the system through a web-based UI while maintaining proper communication with the backend and PostgreSQL database.

---

## **Objectives**
- Integrate frontend with backend API
- Enable full CRUD operations through a web interface
- Ensure consistent JSON communication
- Resolve CORS issues using same-origin setup
- Validate end-to-end functionality (UI → Backend → Database)

---

## **Technologies Used**
- **Frontend**
  - HTML
  - CSS
  - JavaScript (Fetch API)

- **Backend**
  - C++
  - cpp-httplib (HTTP server)

- **Database**
  - PostgreSQL

- **Tools**
  - Visual Studio
  - pgAdmin
  - Browser DevTools (Network/Console)

---

## **Project Structure**


BodySculptApp/
│
├── src/
│ ├── business/
│ ├── data/
│ ├── models/
│ ├── services/
│ ├── utils/
│ └── client/
│
├── frontend/
│ ├── index.html
│ ├── style.css
│ └── app.js
│
├── app.log
└── README.md


---

## **How It Works**

### **Frontend**
- Provides UI for interacting with the system
- Sends HTTP requests using `fetch()`
- Displays results from backend responses

### **Backend**
- Handles REST API endpoints
- Processes requests and interacts with database
- Returns JSON responses

### **Database**
- Stores all application data
- Maintains relationships using foreign keys

---

## **Running the Application**

### **1. Start Backend**
- Open project in Visual Studio
- Run using **Local Windows Debugger**
- Server runs at:

http://localhost:8080


### **2. Open Frontend**

http://localhost:8080/ui/index.html


---

## **API Endpoints**

### **Users**
- `GET /users`
- `GET /users/{id}`
- `POST /users`
- `PUT /users/{id}`
- `DELETE /users/{id}`

### **Workouts**
- `GET /workouts`
- `GET /workouts/{id}`
- `POST /workouts`
- `PUT /workouts/{id}`
- `DELETE /workouts/{id}`

### **Workout Entries**
- `GET /workouts/{id}/entries`
- `POST /workouts/{id}/entries`

### **Meals**
- `GET /meals`
- `GET /meals/{id}`
- `POST /meals`
- `PUT /meals/{id}`
- `DELETE /meals/{id}`

### **Meal Items**
- `GET /meals/{id}/items`
- `GET /meal_items/{id}`
- `POST /meals/{id}/items`
- `PUT /meal_items/{id}`
- `DELETE /meal_items/{id}`

---

## **Frontend–Backend Integration**

- Frontend is served through backend at `/ui`
- This ensures **same-origin requests**
- Eliminates CORS errors
- Uses JSON for all request/response data

Example:
```javascript
fetch("/users", {
  method: "POST",
  headers: { "Content-Type": "application/json" },
  body: JSON.stringify(userData)
});
Features Implemented
Users

Create, read, update, delete users

Workouts

Manage workouts linked to users

Workout Entries

Add and retrieve workout entries

Meals

Manage meals linked to users

Meal Items

Add, update, delete meal items

Testing
Testing Order

Users

Workouts

Workout Entries

Meals

Meal Items

Validation Methods

Frontend UI actions

Backend logs (app.log)

PostgreSQL database verification

Browser DevTools (Network tab)

Issues Encountered & Solutions
CORS Errors

Problem: "Failed to fetch"

Solution: Serve frontend via backend (/ui route)

File Path Issues

Problem: Frontend files not found

Solution: Corrected absolute file path

404 Errors

Problem: Incorrect routes or file names

Solution: Standardized endpoint and file paths

JSON Mismatch

Problem: Field inconsistencies

Solution: Matched frontend/backend field names

Results

Full frontend-backend integration achieved

All CRUD operations working through UI

Data correctly stored and retrieved from database

No runtime or communication errors

Future Improvements

Add authentication/login system

Improve UI/UX design

Add input validation

Implement filtering and search

Deploy application to cloud

Notes

Ensure PostgreSQL is running before starting backend

Use correct URL:

http://localhost:8080/ui/index.html

Avoid using .htm (must be .html)
