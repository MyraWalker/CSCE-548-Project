#include "HttpServer.h"
#include "../business/UserService.h"
#include "../business/WorkoutService.h"
#include "../business/MealService.h"
#include "../models/User.h"
#include "../models/Workout.h"
#include "../models/WorkoutEntry.h"
#include "../models/Meal.h"
#include "../models/MealItem.h"
#include "../utils/Logger.h"
#include "httplib.h"
#include <sstream>
#include <string>
#include <exception>

static std::string getJsonValue(const std::string& body, const std::string& key)
{
    std::string pattern = "\"" + key + "\":";
    size_t start = body.find(pattern);
    if (start == std::string::npos) return "";

    start += pattern.length();

    while (start < body.size() && (body[start] == ' ' || body[start] == '\"'))
    {
        start++;
    }

    size_t end = start;
    while (end < body.size() &&
        body[end] != '\"' &&
        body[end] != ',' &&
        body[end] != '}')
    {
        end++;
    }

    return body.substr(start, end - start);
}

void HttpServer::Start()
{
    httplib::Server svr;

    // USERS
    svr.Get("/users", [](const httplib::Request&, httplib::Response& res)
        {
            Logger::Info("GET /users");

            UserService service;
            auto users = service.GetAllUsers();

            std::ostringstream out;
            out << "[\n";

            for (size_t i = 0; i < users.size(); i++)
            {
                out << "  { "
                    << "\"user_id\": " << users[i].user_id << ", "
                    << "\"first_name\": \"" << users[i].first_name << "\", "
                    << "\"last_name\": \"" << users[i].last_name << "\", "
                    << "\"email\": \"" << users[i].email << "\", "
                    << "\"height_cm\": " << users[i].height_cm
                    << " }";

                if (i + 1 < users.size()) out << ",";
                out << "\n";
            }

            out << "]";
            res.set_content(out.str(), "application/json");
        });

    svr.Post("/users", [](const httplib::Request& req, httplib::Response& res)
        {
            try
            {
                Logger::Info("POST /users");

                User user;
                user.first_name = getJsonValue(req.body, "first_name");
                user.last_name = getJsonValue(req.body, "last_name");
                user.email = getJsonValue(req.body, "email");

                std::string heightText = getJsonValue(req.body, "height_cm");
                user.height_cm = heightText.empty() ? 0.0 : std::stod(heightText);

                UserService service;
                bool success = service.CreateUser(user);

                if (success)
                {
                    res.status = 201;
                    res.set_content("{\"success\":true,\"message\":\"User created successfully\"}", "application/json");
                }
                else
                {
                    res.status = 400;
                    res.set_content("{\"success\":false,\"message\":\"User creation failed\"}", "application/json");
                }
            }
            catch (const std::exception& ex)
            {
                Logger::Error(std::string("POST /users exception: ") + ex.what());
                res.status = 500;
                res.set_content("{\"success\":false,\"message\":\"Server error while creating user\"}", "application/json");
            }
        });

    // WORKOUTS
    svr.Get("/workouts", [](const httplib::Request&, httplib::Response& res)
        {
            Logger::Info("GET /workouts");

            WorkoutService service;
            auto workouts = service.GetAllWorkouts();

            std::ostringstream out;
            out << "[\n";

            for (size_t i = 0; i < workouts.size(); i++)
            {
                out << "  { "
                    << "\"workout_id\": " << workouts[i].workout_id << ", "
                    << "\"user_id\": " << workouts[i].user_id << ", "
                    << "\"workout_date\": \"" << workouts[i].workout_date << "\", "
                    << "\"duration_min\": " << workouts[i].duration_min << ", "
                    << "\"notes\": \"" << (workouts[i].notes ? *workouts[i].notes : "") << "\""
                    << " }";

                if (i + 1 < workouts.size()) out << ",";
                out << "\n";
            }

            out << "]";
            res.set_content(out.str(), "application/json");
        });

    svr.Post("/workouts", [](const httplib::Request& req, httplib::Response& res)
        {
            try
            {
                Logger::Info("POST /workouts");

                Workout workout;
                workout.user_id = std::stoi(getJsonValue(req.body, "user_id"));
                workout.workout_date = getJsonValue(req.body, "workout_date");
                workout.duration_min = std::stoi(getJsonValue(req.body, "duration_min"));
                workout.notes = getJsonValue(req.body, "notes");

                WorkoutService service;
                bool success = service.CreateWorkout(workout);

                if (success)
                {
                    res.status = 201;
                    res.set_content("{\"success\":true,\"message\":\"Workout created successfully\"}", "application/json");
                }
                else
                {
                    res.status = 400;
                    res.set_content("{\"success\":false,\"message\":\"Workout creation failed\"}", "application/json");
                }
            }
            catch (const std::exception& ex)
            {
                Logger::Error(std::string("POST /workouts exception: ") + ex.what());
                res.status = 500;
                res.set_content("{\"success\":false,\"message\":\"Server error while creating workout\"}", "application/json");
            }
        });

    // WORKOUT ENTRIES
    svr.Get(R"(/workouts/(\d+)/entries)", [](const httplib::Request& req, httplib::Response& res)
        {
            int workoutId = std::stoi(req.matches[1]);

            Logger::Info("GET /workouts/{id}/entries");

            WorkoutService service;
            auto entries = service.GetEntriesForWorkout(workoutId);

            std::ostringstream out;
            out << "[\n";

            for (size_t i = 0; i < entries.size(); i++)
            {
                out << "  { "
                    << "\"entry_id\": " << entries[i].entry_id << ", "
                    << "\"workout_id\": " << entries[i].workout_id << ", "
                    << "\"exercise_name\": \"" << entries[i].exercise_name << "\", "
                    << "\"sets\": " << entries[i].sets << ", "
                    << "\"reps\": " << entries[i].reps << ", "
                    << "\"weight_kg\": " << entries[i].weight_kg
                    << " }";

                if (i + 1 < entries.size()) out << ",";
                out << "\n";
            }

            out << "]";
            res.set_content(out.str(), "application/json");
        });

    svr.Post(R"(/workouts/(\d+)/entries)", [](const httplib::Request& req, httplib::Response& res)
        {
            try
            {
                int workoutId = std::stoi(req.matches[1]);

                Logger::Info("POST /workouts/{id}/entries");

                WorkoutEntry entry;
                entry.workout_id = workoutId;
                entry.exercise_name = getJsonValue(req.body, "exercise_name");
                entry.sets = std::stoi(getJsonValue(req.body, "sets"));
                entry.reps = std::stoi(getJsonValue(req.body, "reps"));
                entry.weight_kg = std::stod(getJsonValue(req.body, "weight_kg"));

                WorkoutService service;
                bool success = service.CreateWorkoutEntry(entry);

                if (success)
                {
                    res.status = 201;
                    res.set_content("{\"success\":true,\"message\":\"Workout entry created successfully\"}", "application/json");
                }
                else
                {
                    res.status = 400;
                    res.set_content("{\"success\":false,\"message\":\"Workout entry creation failed\"}", "application/json");
                }
            }
            catch (const std::exception& ex)
            {
                Logger::Error(std::string("POST /workouts/{id}/entries exception: ") + ex.what());
                res.status = 500;
                res.set_content("{\"success\":false,\"message\":\"Server error while creating workout entry\"}", "application/json");
            }
        });

    // MEALS
    svr.Get("/meals", [](const httplib::Request&, httplib::Response& res)
        {
            Logger::Info("GET /meals");

            MealService service;
            auto meals = service.GetAllMeals();

            std::ostringstream out;
            out << "[\n";

            for (size_t i = 0; i < meals.size(); i++)
            {
                out << "  { "
                    << "\"meal_id\": " << meals[i].meal_id << ", "
                    << "\"user_id\": " << meals[i].user_id << ", "
                    << "\"meal_time\": \"" << meals[i].meal_time << "\", "
                    << "\"meal_type\": \"" << meals[i].meal_type << "\", "
                    << "\"notes\": \"" << (meals[i].notes ? *meals[i].notes : "") << "\""
                    << " }";

                if (i + 1 < meals.size()) out << ",";
                out << "\n";
            }

            out << "]";
            res.set_content(out.str(), "application/json");
        });

    svr.Post("/meals", [](const httplib::Request& req, httplib::Response& res)
        {
            try
            {
                Logger::Info("POST /meals");

                Meal meal;
                meal.user_id = std::stoi(getJsonValue(req.body, "user_id"));
                meal.meal_time = getJsonValue(req.body, "meal_time");
                meal.meal_type = getJsonValue(req.body, "meal_type");
                meal.notes = getJsonValue(req.body, "notes");

                MealService service;
                bool success = service.CreateMeal(meal);

                if (success)
                {
                    res.status = 201;
                    res.set_content("{\"success\":true,\"message\":\"Meal created successfully\"}", "application/json");
                }
                else
                {
                    res.status = 400;
                    res.set_content("{\"success\":false,\"message\":\"Meal creation failed\"}", "application/json");
                }
            }
            catch (const std::exception& ex)
            {
                Logger::Error(std::string("POST /meals exception: ") + ex.what());
                res.status = 500;
                res.set_content("{\"success\":false,\"message\":\"Server error while creating meal\"}", "application/json");
            }
        });

    // MEAL ITEMS
    svr.Get(R"(/meals/(\d+)/items)", [](const httplib::Request& req, httplib::Response& res)
        {
            int mealId = std::stoi(req.matches[1]);

            Logger::Info("GET /meals/{id}/items");

            MealService service;
            auto items = service.GetItemsForMeal(mealId);

            std::ostringstream out;
            out << "[\n";

            for (size_t i = 0; i < items.size(); i++)
            {
                out << "  { "
                    << "\"item_id\": " << items[i].item_id << ", "
                    << "\"meal_id\": " << items[i].meal_id << ", "
                    << "\"food_name\": \"" << items[i].food_name << "\", "
                    << "\"calories\": " << items[i].calories << ", "
                    << "\"protein_g\": " << items[i].protein_g << ", "
                    << "\"carbs_g\": " << items[i].carbs_g << ", "
                    << "\"fat_g\": " << items[i].fat_g
                    << " }";

                if (i + 1 < items.size()) out << ",";
                out << "\n";
            }

            out << "]";
            res.set_content(out.str(), "application/json");
        });

    svr.Post(R"(/meals/(\d+)/items)", [](const httplib::Request& req, httplib::Response& res)
        {
            try
            {
                int mealId = std::stoi(req.matches[1]);

                Logger::Info("POST /meals/{id}/items");

                MealItem item;
                item.meal_id = mealId;
                item.food_name = getJsonValue(req.body, "food_name");
                item.calories = std::stoi(getJsonValue(req.body, "calories"));
                item.protein_g = std::stod(getJsonValue(req.body, "protein_g"));
                item.carbs_g = std::stod(getJsonValue(req.body, "carbs_g"));
                item.fat_g = std::stod(getJsonValue(req.body, "fat_g"));

                MealService service;
                bool success = service.CreateMealItem(item);

                if (success)
                {
                    res.status = 201;
                    res.set_content("{\"success\":true,\"message\":\"Meal item created successfully\"}", "application/json");
                }
                else
                {
                    res.status = 400;
                    res.set_content("{\"success\":false,\"message\":\"Meal item creation failed\"}", "application/json");
                }
            }
            catch (const std::exception& ex)
            {
                Logger::Error(std::string("POST /meals/{id}/items exception: ") + ex.what());
                res.status = 500;
                res.set_content("{\"success\":false,\"message\":\"Server error while creating meal item\"}", "application/json");
            }
        });

    Logger::Info("Server started on port 8080");
    svr.listen("localhost", 8080);
}