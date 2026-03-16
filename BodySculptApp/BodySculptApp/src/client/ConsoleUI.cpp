#include "ConsoleUI.h"
#include "ApiClient.h"
#include <iostream>
#include <limits>
#include <string>

void ConsoleUI::Run()
{
    ApiClient api;
    int choice = -1;

    while (choice != 0)
    {
        std::cout << "\n===== BodySculptApp Console Client =====\n";
        std::cout << "1. Create User\n";
        std::cout << "2. Get All Users\n";
        std::cout << "3. Create Workout\n";
        std::cout << "4. Get All Workouts\n";
        std::cout << "5. Create Workout Entry\n";
        std::cout << "6. Get Workout Entries\n";
        std::cout << "7. Create Meal\n";
        std::cout << "8. Get All Meals\n";
        std::cout << "9. Create Meal Item\n";
        std::cout << "10. Get Meal Items\n";
        std::cout << "0. Exit\n";
        std::cout << "Enter choice: ";
        std::cin >> choice;

        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input.\n";
            continue;
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 1)
        {
            std::string firstName, lastName, email;
            double heightCm;

            std::cout << "First name: ";
            std::getline(std::cin, firstName);

            std::cout << "Last name: ";
            std::getline(std::cin, lastName);

            std::cout << "Email: ";
            std::getline(std::cin, email);

            std::cout << "Height (cm): ";
            std::cin >> heightCm;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            std::cout << api.CreateUser(firstName, lastName, email, heightCm) << "\n";
        }
        else if (choice == 2)
        {
            std::cout << api.GetUsers() << "\n";
        }
        else if (choice == 3)
        {
            int userId, durationMin;
            std::string workoutDate, notes;

            std::cout << "User ID: ";
            std::cin >> userId;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            std::cout << "Workout date (YYYY-MM-DD): ";
            std::getline(std::cin, workoutDate);

            std::cout << "Duration (min): ";
            std::cin >> durationMin;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            std::cout << "Notes: ";
            std::getline(std::cin, notes);

            std::cout << api.CreateWorkout(userId, workoutDate, durationMin, notes) << "\n";
        }
        else if (choice == 4)
        {
            std::cout << api.GetWorkouts() << "\n";
        }
        else if (choice == 5)
        {
            int workoutId, sets, reps;
            double weightKg;
            std::string exerciseName;

            std::cout << "Workout ID: ";
            std::cin >> workoutId;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            std::cout << "Exercise name: ";
            std::getline(std::cin, exerciseName);

            std::cout << "Sets: ";
            std::cin >> sets;

            std::cout << "Reps: ";
            std::cin >> reps;

            std::cout << "Weight (kg): ";
            std::cin >> weightKg;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            std::cout << api.CreateWorkoutEntry(workoutId, exerciseName, sets, reps, weightKg) << "\n";
        }
        else if (choice == 6)
        {
            int workoutId;

            std::cout << "Workout ID: ";
            std::cin >> workoutId;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            std::cout << api.GetWorkoutEntries(workoutId) << "\n";
        }
        else if (choice == 7)
        {
            int userId;
            std::string mealTime, mealType, notes;

            std::cout << "User ID: ";
            std::cin >> userId;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            std::cout << "Meal time (YYYY-MM-DD HH:MM:SS): ";
            std::getline(std::cin, mealTime);

            std::cout << "Meal type (breakfast/lunch/dinner/snack): ";
            std::getline(std::cin, mealType);

            std::cout << "Notes: ";
            std::getline(std::cin, notes);

            std::cout << api.CreateMeal(userId, mealTime, mealType, notes) << "\n";
        }
        else if (choice == 8)
        {
            std::cout << api.GetMeals() << "\n";
        }
        else if (choice == 9)
        {
            int mealId, calories;
            double proteinG, carbsG, fatG;
            std::string foodName;

            std::cout << "Meal ID: ";
            std::cin >> mealId;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            std::cout << "Food name: ";
            std::getline(std::cin, foodName);

            std::cout << "Calories: ";
            std::cin >> calories;

            std::cout << "Protein (g): ";
            std::cin >> proteinG;

            std::cout << "Carbs (g): ";
            std::cin >> carbsG;

            std::cout << "Fat (g): ";
            std::cin >> fatG;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            std::cout << api.CreateMealItem(mealId, foodName, calories, proteinG, carbsG, fatG) << "\n";
        }
        else if (choice == 10)
        {
            int mealId;

            std::cout << "Meal ID: ";
            std::cin >> mealId;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            std::cout << api.GetMealItems(mealId) << "\n";
        }
        else if (choice == 0)
        {
            std::cout << "Exiting...\n";
        }
        else
        {
            std::cout << "Invalid choice.\n";
        }
    }
}