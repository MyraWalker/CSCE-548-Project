#pragma once
#include <string>

class ApiClient
{
public:
    std::string GetUsers();
    std::string CreateUser(const std::string& firstName,
        const std::string& lastName,
        const std::string& email,
        double heightCm);

    std::string GetWorkouts();
    std::string CreateWorkout(int userId,
        const std::string& workoutDate,
        int durationMin,
        const std::string& notes);

    std::string GetWorkoutEntries(int workoutId);
    std::string CreateWorkoutEntry(int workoutId,
        const std::string& exerciseName,
        int sets,
        int reps,
        double weightKg);

    std::string GetMeals();
    std::string CreateMeal(int userId,
        const std::string& mealTime,
        const std::string& mealType,
        const std::string& notes);

    std::string GetMealItems(int mealId);
    std::string CreateMealItem(int mealId,
        const std::string& foodName,
        int calories,
        double proteinG,
        double carbsG,
        double fatG);
};