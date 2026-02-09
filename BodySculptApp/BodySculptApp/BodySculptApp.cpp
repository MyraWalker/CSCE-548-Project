// BodySculptApp.cpp (complete)
// Build: libpqxx 7+
// Console UI for 5 tables: users, meals, meal_items, workouts, workout_entries
// Note: try/catch so SQL/connection errors print instead of aborting.

#include <iostream>
#include <string>
#include <vector>
#include <optional>
#include <limits>
#include <pqxx/pqxx>

#include "Db.h"
#include "UserDAL.h"
#include "MealDAL.h"
#include "MealItemDAL.h"
#include "WorkoutDAL.h"
#include "WorkoutEntryDAL.h"

// -------------------- Input Helpers --------------------
static void clearLine() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static int readInt(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        int v;
        if (std::cin >> v) {
            clearLine();
            return v;
        }
        std::cin.clear();
        clearLine();
        std::cout << "Invalid number. Try again.\n";
    }
}

static double readDouble(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        double v;
        if (std::cin >> v) {
            clearLine();
            return v;
        }
        std::cin.clear();
        clearLine();
        std::cout << "Invalid number. Try again.\n";
    }
}

static std::string readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string s;
    std::getline(std::cin, s);
    return s;
}

static std::optional<std::string> readOptionalLine(const std::string& prompt) {
    std::string s = readLine(prompt);
    if (s.empty()) return std::nullopt;
    return s;
}

// -------------------- Print Helpers --------------------
static void printUsers(const std::vector<User>& users) {
    std::cout << "\n--- USERS ---\n";
    for (const auto& u : users) {
        std::cout << u.user_id << ": "
            << u.first_name << " " << u.last_name
            << " | " << u.email
            << " | height_cm=" << u.height_cm << "\n";
    }
}

static void printMeals(const std::vector<Meal>& meals) {
    std::cout << "\n--- MEALS ---\n";
    for (const auto& m : meals) {
        std::cout << m.meal_id
            << " | user_id=" << m.user_id
            << " | time=" << m.meal_time
            << " | type=" << m.meal_type
            << " | notes=" << (m.notes ? *m.notes : "(null)")
            << "\n";
    }
}

static void printMealItems(const std::vector<MealItem>& items) {
    std::cout << "\n--- MEAL ITEMS ---\n";
    for (const auto& mi : items) {
        std::cout << mi.item_id
            << " | meal_id=" << mi.meal_id
            << " | " << mi.food_name
            << " | cal=" << mi.calories
            << " | P=" << mi.protein_g
            << " C=" << mi.carbs_g
            << " F=" << mi.fat_g
            << "\n";
    }
}

static void printWorkouts(const std::vector<Workout>& workouts) {
    std::cout << "\n--- WORKOUTS ---\n";
    for (const auto& w : workouts) {
        std::cout << w.workout_id
            << " | user_id=" << w.user_id
            << " | date=" << w.workout_date
            << " | duration_min=" << w.duration_min
            << " | notes=" << (w.notes ? *w.notes : "(null)")
            << "\n";
    }
}

static void printWorkoutEntries(const std::vector<WorkoutEntry>& entries) {
    std::cout << "\n--- WORKOUT ENTRIES ---\n";
    for (const auto& e : entries) {
        std::cout << e.entry_id
            << " | workout_id=" << e.workout_id
            << " | " << e.exercise_name
            << " | sets=" << e.sets
            << " reps=" << e.reps
            << " | weight_kg=" << e.weight_kg
            << "\n";
    }
}

// -------------------- Submenus --------------------
static void usersMenu(UserDAL& userDal) {
    while (true) {
        std::cout << "\n[USERS]\n"
            << "1) List all\n"
            << "2) Get by id\n"
            << "3) Create\n"
            << "4) Update\n"
            << "5) Delete\n"
            << "0) Back\n"
            << "Choice: ";

        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            clearLine();
            std::cout << "Invalid input.\n";
            continue;
        }
        clearLine();

        if (choice == 0) break;

        try {
            if (choice == 1) {
                printUsers(userDal.getAll());
            }
            else if (choice == 2) {
                int id = readInt("Enter user id: ");
                auto u = userDal.getById(id);
                if (!u) std::cout << "User not found.\n";
                else {
                    std::cout << u->user_id << ": " << u->first_name << " " << u->last_name
                        << " | " << u->email
                        << " | height_cm=" << u->height_cm << "\n";
                }
            }
            else if (choice == 3) {
                User u{};
                u.first_name = readLine("First name: ");
                u.last_name = readLine("Last name: ");
                u.email = readLine("Email: ");
                u.height_cm = readDouble("Height (cm): ");

                int newId = userDal.create(u);
                std::cout << "Created user_id = " << newId << "\n";
            }
            else if (choice == 4) {
                User u{};
                u.user_id = readInt("User id: ");
                u.first_name = readLine("First name: ");
                u.last_name = readLine("Last name: ");
                u.email = readLine("Email: ");
                u.height_cm = readDouble("Height (cm): ");

                std::cout << (userDal.update(u) ? "Updated.\n" : "Update failed.\n");
            }
            else if (choice == 5) {
                int id = readInt("User id: ");
                std::cout << (userDal.remove(id) ? "Deleted.\n" : "Delete failed.\n");
            }
            else {
                std::cout << "Invalid choice.\n";
            }
        }
        catch (const pqxx::sql_error& e) {
            std::cerr << "\nSQL error: " << e.what()
                << "\nQuery was: " << e.query() << "\n";
        }
        catch (const std::exception& e) {
            std::cerr << "\nError: " << e.what() << "\n";
        }
    }
}

static void mealsMenu(MealDAL& mealDal) {
    while (true) {
        std::cout << "\n[MEALS]\n"
            << "1) List all\n"
            << "2) Get by id\n"
            << "3) Create\n"
            << "4) Update\n"
            << "5) Delete\n"
            << "0) Back\n"
            << "Choice: ";

        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            clearLine();
            std::cout << "Invalid input.\n";
            continue;
        }
        clearLine();

        if (choice == 0) break;

        try {
            if (choice == 1) {
                printMeals(mealDal.getAll());
            }
            else if (choice == 2) {
                int id = readInt("Meal id: ");
                auto m = mealDal.getById(id);
                if (!m) std::cout << "Meal not found.\n";
                else {
                    std::cout << "Meal " << m->meal_id
                        << " | user_id=" << m->user_id
                        << " | time=" << m->meal_time
                        << " | type=" << m->meal_type
                        << " | notes=" << (m->notes ? *m->notes : "(null)")
                        << "\n";
                }
            }
            else if (choice == 3) {
                Meal m{};
                m.user_id = readInt("User id (FK): ");
                m.meal_time = readLine("Meal time (YYYY-MM-DD HH:MM:SS): ");
                m.meal_type = readLine("Meal type (breakfast/lunch/dinner/snack): ");
                m.notes = readOptionalLine("Notes (optional, press Enter for NULL): ");

                int newId = mealDal.create(m);
                std::cout << "Created meal_id = " << newId << "\n";
            }
            else if (choice == 4) {
                Meal m{};
                m.meal_id = readInt("Meal id: ");
                m.user_id = readInt("User id (FK): ");
                m.meal_time = readLine("Meal time (YYYY-MM-DD HH:MM:SS): ");
                m.meal_type = readLine("Meal type: ");
                m.notes = readOptionalLine("Notes (optional, press Enter for NULL): ");

                std::cout << (mealDal.update(m) ? "Updated.\n" : "Update failed.\n");
            }
            else if (choice == 5) {
                int id = readInt("Meal id: ");
                std::cout << (mealDal.remove(id) ? "Deleted.\n" : "Delete failed.\n");
            }
            else {
                std::cout << "Invalid choice.\n";
            }
        }
        catch (const pqxx::sql_error& e) {
            std::cerr << "\nSQL error: " << e.what()
                << "\nQuery was: " << e.query() << "\n";
        }
        catch (const std::exception& e) {
            std::cerr << "\nError: " << e.what() << "\n";
        }
    }
}

static void mealItemsMenu(MealItemDAL& dal) {
    while (true) {
        std::cout << "\n[MEAL ITEMS]\n"
            << "1) List all\n"
            << "2) Get by id\n"
            << "3) List by meal_id\n"
            << "4) Create\n"
            << "5) Update\n"
            << "6) Delete\n"
            << "0) Back\n"
            << "Choice: ";

        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            clearLine();
            std::cout << "Invalid input.\n";
            continue;
        }
        clearLine();

        if (choice == 0) break;

        try {
            if (choice == 1) {
                printMealItems(dal.getAll());
            }
            else if (choice == 2) {
                int id = readInt("Item id: ");
                auto mi = dal.getById(id);
                if (!mi) std::cout << "Item not found.\n";
                else {
                    std::cout << mi->item_id << " | meal_id=" << mi->meal_id
                        << " | " << mi->food_name << " | cal=" << mi->calories << "\n";
                }
            }
            else if (choice == 3) {
                int meal_id = readInt("Meal id (FK): ");
                printMealItems(dal.getByMealId(meal_id));
            }
            else if (choice == 4) {
                MealItem mi{};
                mi.meal_id = readInt("Meal id (FK): ");
                mi.food_name = readLine("Food name: ");
                mi.calories = readInt("Calories: ");
                mi.protein_g = readDouble("Protein (g): ");
                mi.carbs_g = readDouble("Carbs (g): ");
                mi.fat_g = readDouble("Fat (g): ");

                int newId = dal.create(mi);
                std::cout << "Created item_id = " << newId << "\n";
            }
            else if (choice == 5) {
                MealItem mi{};
                mi.item_id = readInt("Item id: ");
                mi.food_name = readLine("Food name: ");
                mi.calories = readInt("Calories: ");
                mi.protein_g = readDouble("Protein (g): ");
                mi.carbs_g = readDouble("Carbs (g): ");
                mi.fat_g = readDouble("Fat (g): ");

                std::cout << (dal.update(mi) ? "Updated.\n" : "Update failed.\n");
            }
            else if (choice == 6) {
                int id = readInt("Item id: ");
                std::cout << (dal.remove(id) ? "Deleted.\n" : "Delete failed.\n");
            }
            else {
                std::cout << "Invalid choice.\n";
            }
        }
        catch (const pqxx::sql_error& e) {
            std::cerr << "\nSQL error: " << e.what()
                << "\nQuery was: " << e.query() << "\n";
        }
        catch (const std::exception& e) {
            std::cerr << "\nError: " << e.what() << "\n";
        }
    }
}

static void workoutsMenu(WorkoutDAL& dal) {
    while (true) {
        std::cout << "\n[WORKOUTS]\n"
            << "1) List all\n"
            << "2) Get by id\n"
            << "3) List by user_id\n"
            << "4) Create\n"
            << "5) Update\n"
            << "6) Delete\n"
            << "0) Back\n"
            << "Choice: ";

        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            clearLine();
            std::cout << "Invalid input.\n";
            continue;
        }
        clearLine();

        if (choice == 0) break;

        try {
            if (choice == 1) {
                printWorkouts(dal.getAll());
            }
            else if (choice == 2) {
                int id = readInt("Workout id: ");
                auto w = dal.getById(id);
                if (!w) std::cout << "Workout not found.\n";
                else {
                    std::cout << w->workout_id << " | user_id=" << w->user_id
                        << " | date=" << w->workout_date
                        << " | duration_min=" << w->duration_min
                        << " | notes=" << (w->notes ? *w->notes : "(null)") << "\n";
                }
            }
            else if (choice == 3) {
                int uid = readInt("User id: ");
                printWorkouts(dal.getByUserId(uid));
            }
            else if (choice == 4) {
                Workout w{};
                w.user_id = readInt("User id (FK): ");
                w.workout_date = readLine("Workout date (YYYY-MM-DD): ");
                w.duration_min = readInt("Duration (min): ");
                w.notes = readOptionalLine("Notes (optional, press Enter for NULL): ");

                int newId = dal.create(w);
                std::cout << "Created workout_id = " << newId << "\n";
            }
            else if (choice == 5) {
                Workout w{};
                w.workout_id = readInt("Workout id: ");
                w.user_id = readInt("User id (FK): ");
                w.workout_date = readLine("Workout date (YYYY-MM-DD): ");
                w.duration_min = readInt("Duration (min): ");
                w.notes = readOptionalLine("Notes (optional, press Enter for NULL): ");

                std::cout << (dal.update(w) ? "Updated.\n" : "Update failed.\n");
            }
            else if (choice == 6) {
                int id = readInt("Workout id: ");
                std::cout << (dal.remove(id) ? "Deleted.\n" : "Delete failed.\n");
            }
            else {
                std::cout << "Invalid choice.\n";
            }
        }
        catch (const pqxx::sql_error& e) {
            std::cerr << "\nSQL error: " << e.what()
                << "\nQuery was: " << e.query() << "\n";
        }
        catch (const std::exception& e) {
            std::cerr << "\nError: " << e.what() << "\n";
        }
    }
}

static void workoutEntriesMenu(WorkoutEntryDAL& dal) {
    while (true) {
        std::cout << "\n[WORKOUT ENTRIES]\n"
            << "1) List all\n"
            << "2) Get by id\n"
            << "3) List by workout_id\n"
            << "4) Create\n"
            << "5) Update\n"
            << "6) Delete\n"
            << "0) Back\n"
            << "Choice: ";

        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            clearLine();
            std::cout << "Invalid input.\n";
            continue;
        }
        clearLine();

        if (choice == 0) break;

        try {
            if (choice == 1) {
                printWorkoutEntries(dal.getAll());
            }
            else if (choice == 2) {
                int id = readInt("Entry id: ");
                auto e = dal.getById(id);
                if (!e) std::cout << "Entry not found.\n";
                else {
                    std::cout << e->entry_id << " | workout_id=" << e->workout_id
                        << " | " << e->exercise_name
                        << " | sets=" << e->sets
                        << " reps=" << e->reps
                        << " | weight_kg=" << e->weight_kg << "\n";
                }
            }
            else if (choice == 3) {
                int wid = readInt("Workout id: ");
                printWorkoutEntries(dal.getByWorkoutId(wid));
            }
            else if (choice == 4) {
                WorkoutEntry e{};
                e.workout_id = readInt("Workout id (FK): ");
                e.exercise_name = readLine("Exercise name: ");
                e.sets = readInt("Sets: ");
                e.reps = readInt("Reps: ");
                e.weight_kg = readDouble("Weight (kg): ");

                int newId = dal.create(e);
                std::cout << "Created entry_id = " << newId << "\n";
            }
            else if (choice == 5) {
                WorkoutEntry e{};
                e.entry_id = readInt("Entry id: ");
                e.exercise_name = readLine("Exercise name: ");
                e.sets = readInt("Sets: ");
                e.reps = readInt("Reps: ");
                e.weight_kg = readDouble("Weight (kg): ");

                std::cout << (dal.update(e) ? "Updated.\n" : "Update failed.\n");
            }
            else if (choice == 6) {
                int id = readInt("Entry id: ");
                std::cout << (dal.remove(id) ? "Deleted.\n" : "Delete failed.\n");
            }
            else {
                std::cout << "Invalid choice.\n";
            }
        }
        catch (const pqxx::sql_error& e) {
            std::cerr << "\nSQL error: " << e.what()
                << "\nQuery was: " << e.query() << "\n";
        }
        catch (const std::exception& e) {
            std::cerr << "\nError: " << e.what() << "\n";
        }
    }
}

// -------------------- MAIN --------------------
int main() {
    // Use the "key=value" format (easiest on Windows)
    std::string connStr =
        "dbname=body_sculpt user=postgres password=postgresql host=localhost port=5432";

    Db db(connStr);

    UserDAL userDal(db);
    MealDAL mealDal(db);
    MealItemDAL mealItemDal(db);
    WorkoutDAL workoutDal(db);
    WorkoutEntryDAL workoutEntryDal(db);

    while (true) {
        std::cout << "\nBody Sculpt App (Phase 1)\n"
            << "1) Users\n"
            << "2) Meals\n"
            << "3) Meal Items\n"
            << "4) Workouts\n"
            << "5) Workout Entries\n"
            << "0) Exit\n"
            << "Choice: ";

        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            clearLine();
            std::cout << "Invalid input.\n";
            continue;
        }
        clearLine();

        if (choice == 0) break;

        if (choice == 1) usersMenu(userDal);
        else if (choice == 2) mealsMenu(mealDal);
        else if (choice == 3) mealItemsMenu(mealItemDal);
        else if (choice == 4) workoutsMenu(workoutDal);
        else if (choice == 5) workoutEntriesMenu(workoutEntryDal);
        else std::cout << "Invalid choice.\n";
    }

    return 0;
}
