# CSCE-548-Project
Full Stack Application Development


# Body Sculpt App – Phase 1
CSCE 548 – Database Systems
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
