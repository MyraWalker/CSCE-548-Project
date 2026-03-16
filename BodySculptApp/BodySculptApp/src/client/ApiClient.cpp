#include "ApiClient.h"
#include "../services/httplib.h"
#include <sstream>
#include <string>

static const std::string BASE_URL = "http://localhost:8080";

std::string ApiClient::GetUsers()
{
    httplib::Client cli(BASE_URL);
    auto res = cli.Get("/users");
    return res ? res->body : "Error: could not connect to server.";
}

std::string ApiClient::CreateUser(const std::string& firstName,
    const std::string& lastName,
    const std::string& email,
    double heightCm)
{
    httplib::Client cli(BASE_URL);

    std::ostringstream body;
    body << "{"
        << "\"first_name\":\"" << firstName << "\","
        << "\"last_name\":\"" << lastName << "\","
        << "\"email\":\"" << email << "\","
        << "\"height_cm\":" << heightCm
        << "}";

    auto res = cli.Post("/users", body.str(), "application/json");
    return res ? res->body : "Error creating user.";
}

std::string ApiClient::GetWorkouts()
{
    httplib::Client cli(BASE_URL);
    auto res = cli.Get("/workouts");
    return res ? res->body : "Error: could not retrieve workouts.";
}

std::string ApiClient::CreateWorkout(int userId,
    const std::string& workoutDate,
    int durationMin,
    const std::string& notes)
{
    httplib::Client cli(BASE_URL);

    std::ostringstream body;
    body << "{"
        << "\"user_id\":" << userId << ","
        << "\"workout_date\":\"" << workoutDate << "\","
        << "\"duration_min\":" << durationMin << ","
        << "\"notes\":\"" << notes << "\""
        << "}";

    auto res = cli.Post("/workouts", body.str(), "application/json");
    return res ? res->body : "Error creating workout.";
}

std::string ApiClient::GetWorkoutEntries(int workoutId)
{
    httplib::Client cli(BASE_URL);
    std::string route = "/workouts/" + std::to_string(workoutId) + "/entries";
    auto res = cli.Get(route.c_str());
    return res ? res->body : "Error retrieving workout entries.";
}

std::string ApiClient::CreateWorkoutEntry(int workoutId,
    const std::string& exerciseName,
    int sets,
    int reps,
    double weightKg)
{
    httplib::Client cli(BASE_URL);
    std::string route = "/workouts/" + std::to_string(workoutId) + "/entries";

    std::ostringstream body;
    body << "{"
        << "\"exercise_name\":\"" << exerciseName << "\","
        << "\"sets\":" << sets << ","
        << "\"reps\":" << reps << ","
        << "\"weight_kg\":" << weightKg
        << "}";

    auto res = cli.Post(route.c_str(), body.str(), "application/json");
    return res ? res->body : "Error creating workout entry.";
}

std::string ApiClient::GetMeals()
{
    httplib::Client cli(BASE_URL);
    auto res = cli.Get("/meals");
    return res ? res->body : "Error retrieving meals.";
}

std::string ApiClient::CreateMeal(int userId,
    const std::string& mealTime,
    const std::string& mealType,
    const std::string& notes)
{
    httplib::Client cli(BASE_URL);

    std::ostringstream body;
    body << "{"
        << "\"user_id\":" << userId << ","
        << "\"meal_time\":\"" << mealTime << "\","
        << "\"meal_type\":\"" << mealType << "\","
        << "\"notes\":\"" << notes << "\""
        << "}";

    auto res = cli.Post("/meals", body.str(), "application/json");
    return res ? res->body : "Error creating meal.";
}

std::string ApiClient::GetMealItems(int mealId)
{
    httplib::Client cli(BASE_URL);
    std::string route = "/meals/" + std::to_string(mealId) + "/items";
    auto res = cli.Get(route.c_str());
    return res ? res->body : "Error retrieving meal items.";
}

std::string ApiClient::CreateMealItem(int mealId,
    const std::string& foodName,
    int calories,
    double proteinG,
    double carbsG,
    double fatG)
{
    httplib::Client cli(BASE_URL);
    std::string route = "/meals/" + std::to_string(mealId) + "/items";

    std::ostringstream body;
    body << "{"
        << "\"food_name\":\"" << foodName << "\","
        << "\"calories\":" << calories << ","
        << "\"protein_g\":" << proteinG << ","
        << "\"carbs_g\":" << carbsG << ","
        << "\"fat_g\":" << fatG
        << "}";

    auto res = cli.Post(route.c_str(), body.str(), "application/json");
    return res ? res->body : "Error creating meal item.";
}