# CSCE-548-Project
Full Stack Application Development


# Body Sculpt App – Phase 1

CSCE 548 – Database Systems  
Project 1: Database and Data Access Layer

## Overview
The Body Sculpt App is a console-based C++ application designed to track users’ body metrics as part of a larger body sculpting and fitness system. Phase 1 focuses on creating a high-quality PostgreSQL database schema and a functional C++ data access layer using libpqxx.

The application demonstrates full CRUD functionality through a menu-driven console interface and successfully retrieves data from the database.

## Technologies Used
- C++ (Visual Studio 2022)
- PostgreSQL
- libpqxx (7+)
- pgAdmin
- GitHub

## Database Design
The database contains 5 tables with properly defined primary keys and foreign key relationships. Constraints are used to enforce data integrity.

The users table includes the following columns:
- user_id (integer, primary key)
- first_name (character varying)
- last_name (character varying)
- email (character varying)
- height_cm (numeric)
- created_at (timestamp)

SQL scripts for creating tables and inserting test data are located in the sql/ directory.

## Features
- Console-based menu interface
- Full CRUD operations (Create, Read, Update, Delete)
- PostgreSQL database connectivity
- Data Access Layer (DAL) abstraction
- Proper handling of PostgreSQL numeric data types
- Error handling for database and runtime issues

## Project Structure
CSCE-548-Project/
├── sql/
│   ├── create_tables.sql
│   └── insert_test_data.sql
├── src/
│   ├── db/
│   │   └── Db.h
│   ├── models/
│   │   └── User.h
│   ├── dal/
│   │   ├── UserDAL.h
│   │   └── UserDAL.cpp
│   └── BodySculptApp.cpp
├── README.md
└── .gitignore

## How to Run
1. Ensure PostgreSQL is running and the database is created.
2. Update the connection string in BodySculptApp.cpp if necessary.
3. Build the project in Visual Studio.
4. Run the program using Ctrl + F5 (Start Without Debugging).
5. Use the console menu to retrieve and manage user records.

## Sample Console Output
--- USERS ---
1: Myra Walker | myra1@example.com | height_cm=165.1
2: Jordan Lee | jordan@example.com | height_cm=180.34
3: Casey Nguyen | casey@example.com | height_cm=172.72

## Notes
Visual Studio generated files (such as .vs/, .ipch/, and build outputs) are excluded using .gitignore. The database was populated with 50+ rows of test data. This phase establishes the foundation for future project phases.

## Author
Myra Walker
