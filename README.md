# 🏋️ BodySculptApp – Full Stack Fitness Tracking System

**Course:** CSCE 548  
**Student:** Myra Walker  
**Semester:** Spring 2026  

---

# 📌 Project Overview

The **BodySculptApp** is a full-stack application designed to help users track their workouts and diet information to support fitness and body sculpting goals. The system allows users to create, update, and retrieve workout and meal data, including detailed entries such as exercises performed and nutritional information.

This project demonstrates a complete **n-tier architecture**, consisting of:

- Database Layer (PostgreSQL)
- Data Access Layer (DAL)
- Business Layer
- Service/API Layer
- Client/Frontend Layer

The application was developed using **C++**, with a web-based frontend built using **HTML, CSS, and JavaScript**.

---

# 🛠️ Technology Stack

- **Programming Language:** C++
- **Database:** PostgreSQL
- **Database Tool:** pgAdmin
- **Backend Framework:** Custom HTTP Server using cpp-httplib
- **Frontend:** HTML, CSS, JavaScript
- **IDE:** Visual Studio Community 2026
- **Version Control:** GitHub
- **Logging:** Custom Logger (C++)

---

# 🏗️ System Architecture

The application follows a layered architecture:

### 1. Database Layer
Stores all persistent data using PostgreSQL tables:
- users
- meals
- meal_items
- workouts
- workout_entries

### 2. Data Access Layer (DAL)
Handles all direct database operations:
- SQL queries
- CRUD operations
- Database connections

### 3. Business Layer
Implements application logic:
- Validates data
- Coordinates operations between layers
- Calls DAL methods

### 4. Service Layer (API)
- Hosted via HTTP server
- Exposes endpoints for frontend interaction
- Handles requests and responses

### 5. Client Layer (Frontend)
- Web interface (HTML/JS)
- Sends requests to API
- Displays results to user

---

# 🗄️ Database Schema

## Tables

### users
- user_id (PK)
- first_name
- last_name
- email
- height_cm
- created_at

### meals
- meal_id (PK)
- user_id (FK)
- meal_time
- meal_type
- notes

### meal_items
- item_id (PK)
- meal_id (FK)
- food_name
- calories
- protein_g
- carbs_g
- fat_g

### workouts
- workout_id (PK)
- user_id (FK)
- workout_date
- duration_min
- notes

### workout_entries
- entry_id (PK)
- workout_id (FK)
- exercise_name
- sets
- reps
- weight_kg

## Relationships
- Users → Meals (1:M)
- Users → Workouts (1:M)
- Meals → MealItems (1:M)
- Workouts → WorkoutEntries (1:M)

---

# 📁 Project Structure

BodySculptApp/  
│  
├── frontend/  
│   ├── index.html  
│   ├── app.js  
│   └── style.css  
│  
├── src/  
│   ├── models/  
│   ├── dal/  
│   ├── business/  
│   ├── services/  
│   ├── client/  
│   ├── db/  
│   └── utils/  
│  
├── BodySculptApp.cpp  
└── README.md  

---

# ⚙️ Setup & Installation

## 1. Clone Repository

git clone <repo-url>

---

## 2. Install Prerequisites

- Visual Studio Community 2026 (with C++ workload)
- PostgreSQL
- pgAdmin
- Web browser (Chrome recommended)

---

## 3. Setup Database

1. Open pgAdmin  
2. Create a new database  
3. Run SQL scripts:  
   - Create tables  
   - Insert test data  
4. Verify tables exist:  
   - users  
   - meals  
   - meal_items  
   - workouts  
   - workout_entries  

---

## 4. Configure Database Connection

Update database credentials in:

src/db/Db.h

Include:
- Database name  
- Username  
- Password  
- Port (default: 5432)

---

## 5. Build the Project

1. Open solution file:

..\GitHub\CSCE-548-Project\BodySculptApp\BodySculptApp.slnx  

2. Build:
- Build → Build Solution

---

## 6. Run Backend Server

1. Click Local Windows Debugger (green play button)  
2. Backend console will appear  
3. DO NOT exit the console (server will shut down)  

### Verify Server

Open browser and go to:

http://localhost:8080/health  

Expected response:

{"success":true,"message":"Server is running"}

---

## 7. Run Frontend Web Application

Open browser and go to:

http://localhost:8080/ui/index.html  

Expected result:
- BodySculptApp web application loads successfully

---

# 🧪 System Testing

The system was tested end-to-end:

Frontend → API → Business → DAL → Database → Frontend

## ✔ Create (Insert)
- Add user via frontend  
- Confirm database insertion  

## ✔ Update
- Modify user  
- Confirm database update  

## ✔ Get All
- Retrieve all records  
- Display in frontend  

## ✔ Get By ID
- Retrieve single record  
- Confirm accuracy  

## ✔ Data Flow Validation
- All layers communicate correctly  
- Data persists in database  

---

# 🚀 Deployment Notes

To run successfully:

1. Ensure PostgreSQL is running  
2. Start backend server  
3. Open frontend via /ui/index.html  
4. Perform CRUD operations  

---

# 🧠 AI Usage & Reflection

## Prompt Used for Project 4

The following prompt guided development:

In previous chats labeled: Branch · Full Stack - CSCE 548 – Project, CSCE 548 - Project 2 Development, and CSCE 548 - Project 3 Web Client, you helped me complete phase 1-3...

---

## Changes Made to AI-Generated Code

- Fixed missing CRUD methods  
- Corrected database queries  
- Adjusted project structure  
- Improved API endpoints  
- Fixed frontend-backend communication  
- Resolved build/configuration issues  
- Added logging functionality  
- Verified full data flow  

---

## Effectiveness of AI Tool

AI was effective for:
- Generating boilerplate code  
- Structuring architecture  
- Accelerating development  

Manual work required:
- Debugging  
- Integration  
- Validation  

---

## Errors Encountered

- Database connection issues  
- Missing CRUD operations  
- API routing issues  
- Build configuration problems  
- Frontend path issues  

---

# ✅ Project Status

✔ Full n-tier architecture implemented  
✔ Backend services hosted  
✔ Frontend functional via /ui route  
✔ End-to-end system tested  
✔ Deployment documentation completed  

---

# 📎 Submission

- GitHub Repository (this repo)  
- System Test & Deployment PDF submitted to Blackboard  

---

# 👩‍💻 Author

Myra Walker  
CSCE 548 – Full Stack Development  
Spring 2026  
