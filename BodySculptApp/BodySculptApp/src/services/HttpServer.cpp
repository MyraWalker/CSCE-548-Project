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
#include <fstream>

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

static void addCorsHeaders(httplib::Response& res)
{
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
    res.set_header("Access-Control-Allow-Headers", "Content-Type");
    res.set_header("Access-Control-Allow-Private-Network", "true");
}

static bool readFileText(const std::string& path, std::string& content)
{
    std::ifstream file(path, std::ios::in | std::ios::binary);
    if (!file.is_open()) return false;

    std::ostringstream ss;
    ss << file.rdbuf();
    content = ss.str();
    return true;
}

static std::string userToJson(const User& user)
{
    std::ostringstream out;
    out << "{ "
        << "\"user_id\": " << user.user_id << ", "
        << "\"first_name\": \"" << user.first_name << "\", "
        << "\"last_name\": \"" << user.last_name << "\", "
        << "\"email\": \"" << user.email << "\", "
        << "\"height_cm\": " << user.height_cm
        << " }";
    return out.str();
}

static std::string workoutToJson(const Workout& workout)
{
    std::ostringstream out;
    out << "{ "
        << "\"workout_id\": " << workout.workout_id << ", "
        << "\"user_id\": " << workout.user_id << ", "
        << "\"workout_date\": \"" << workout.workout_date << "\", "
        << "\"duration_min\": " << workout.duration_min << ", "
        << "\"notes\": \"" << (workout.notes ? *workout.notes : "") << "\""
        << " }";
    return out.str();
}

static std::string mealToJson(const Meal& meal)
{
    std::ostringstream out;
    out << "{ "
        << "\"meal_id\": " << meal.meal_id << ", "
        << "\"user_id\": " << meal.user_id << ", "
        << "\"meal_time\": \"" << meal.meal_time << "\", "
        << "\"meal_type\": \"" << meal.meal_type << "\", "
        << "\"notes\": \"" << (meal.notes ? *meal.notes : "") << "\""
        << " }";
    return out.str();
}

static std::string mealItemToJson(const MealItem& item)
{
    std::ostringstream out;
    out << "{ "
        << "\"item_id\": " << item.item_id << ", "
        << "\"meal_id\": " << item.meal_id << ", "
        << "\"food_name\": \"" << item.food_name << "\", "
        << "\"calories\": " << item.calories << ", "
        << "\"protein_g\": " << item.protein_g << ", "
        << "\"carbs_g\": " << item.carbs_g << ", "
        << "\"fat_g\": " << item.fat_g
        << " }";
    return out.str();
}

void HttpServer::Start()
{
    httplib::Server svr;

    // Add CORS headers to every request before routing
    svr.set_pre_routing_handler([](const httplib::Request&, httplib::Response& res)
        {
            addCorsHeaders(res);
            return httplib::Server::HandlerResponse::Unhandled;
        });

    // Handle browser preflight requests
    svr.Options(R"(.*)", [](const httplib::Request&, httplib::Response& res)
        {
            addCorsHeaders(res);
            res.status = 200;
        });

    // Add CORS headers after route handling too
    svr.set_post_routing_handler([](const httplib::Request&, httplib::Response& res)
        {
            addCorsHeaders(res);
        });

    // Add CORS headers to framework-generated error responses
    svr.set_error_handler([](const httplib::Request&, httplib::Response& res)
        {
            addCorsHeaders(res);

            if (res.body.empty())
            {
                std::ostringstream out;
                out << "{\"success\":false,\"message\":\"HTTP " << res.status << "\"}";
                res.set_content(out.str(), "application/json");
            }
        });

    // Add CORS headers to unhandled exceptions
    svr.set_exception_handler([](const httplib::Request&, httplib::Response& res, std::exception_ptr ep)
        {
            addCorsHeaders(res);

            try
            {
                if (ep) std::rethrow_exception(ep);
            }
            catch (const std::exception& ex)
            {
                Logger::Error(std::string("Unhandled server exception: ") + ex.what());
                res.status = 500;
                res.set_content("{\"success\":false,\"message\":\"Unhandled server exception\"}", "application/json");
            }
            catch (...)
            {
                Logger::Error("Unhandled unknown server exception");
                res.status = 500;
                res.set_content("{\"success\":false,\"message\":\"Unhandled unknown exception\"}", "application/json");
            }
        });

    // FRONTEND FILES
    const std::string frontendDir = "C:/Users/myraw/Documents/GitHub/CSCE-548-Project/BodySculptApp/BodySculptApp/frontend";

    svr.Get("/ui", [frontendDir](const httplib::Request&, httplib::Response& res)
        {
            std::string content;
            if (!readFileText(frontendDir + "/index.html", content))
            {
                res.status = 404;
                res.set_content("{\"success\":false,\"message\":\"index.html not found\"}", "application/json");
                return;
            }
            res.set_content(content, "text/html; charset=UTF-8");
        });

    svr.Get("/ui/", [frontendDir](const httplib::Request&, httplib::Response& res)
        {
            std::string content;
            if (!readFileText(frontendDir + "/index.html", content))
            {
                res.status = 404;
                res.set_content("{\"success\":false,\"message\":\"index.html not found\"}", "application/json");
                return;
            }
            res.set_content(content, "text/html; charset=UTF-8");
        });

    svr.Get("/ui/index.html", [frontendDir](const httplib::Request&, httplib::Response& res)
        {
            std::string content;
            if (!readFileText(frontendDir + "/index.html", content))
            {
                res.status = 404;
                res.set_content("{\"success\":false,\"message\":\"index.html not found\"}", "application/json");
                return;
            }
            res.set_content(content, "text/html; charset=UTF-8");
        });

    svr.Get("/ui/style.css", [frontendDir](const httplib::Request&, httplib::Response& res)
        {
            std::string content;
            if (!readFileText(frontendDir + "/style.css", content))
            {
                res.status = 404;
                res.set_content("{\"success\":false,\"message\":\"style.css not found\"}", "application/json");
                return;
            }
            res.set_content(content, "text/css; charset=UTF-8");
        });

    svr.Get("/ui/app.js", [frontendDir](const httplib::Request&, httplib::Response& res)
        {
            std::string content;
            if (!readFileText(frontendDir + "/app.js", content))
            {
                res.status = 404;
                res.set_content("{\"success\":false,\"message\":\"app.js not found\"}", "application/json");
                return;
            }
            res.set_content(content, "application/javascript; charset=UTF-8");
        });

    // HEALTH
    svr.Get("/health", [](const httplib::Request&, httplib::Response& res)
        {
            Logger::Info("GET /health");
            addCorsHeaders(res);
            res.set_content("{\"success\":true,\"message\":\"Server is running\"}", "application/json");
        });

    // USERS
    svr.Get("/users", [](const httplib::Request&, httplib::Response& res)
        {
            Logger::Info("GET /users");
            addCorsHeaders(res);

            UserService service;
            auto users = service.GetAllUsers();

            std::ostringstream out;
            out << "[\n";
            for (size_t i = 0; i < users.size(); i++)
            {
                out << "  " << userToJson(users[i]);
                if (i + 1 < users.size()) out << ",";
                out << "\n";
            }
            out << "]";

            res.set_content(out.str(), "application/json");
        });

    svr.Get(R"(/users/(\d+))", [](const httplib::Request& req, httplib::Response& res)
        {
            try
            {
                Logger::Info("GET /users/{id}");
                addCorsHeaders(res);

                int userId = std::stoi(req.matches[1]);

                UserService service;
                auto user = service.GetUserById(userId);

                if (!user.has_value())
                {
                    res.status = 404;
                    res.set_content("{\"success\":false,\"message\":\"User not found\"}", "application/json");
                    return;
                }

                res.set_content(userToJson(user.value()), "application/json");
            }
            catch (const std::exception& ex)
            {
                Logger::Error(std::string("GET /users/{id} exception: ") + ex.what());
                addCorsHeaders(res);
                res.status = 500;
                res.set_content("{\"success\":false,\"message\":\"Server error while fetching user\"}", "application/json");
            }
        });

    svr.Post("/users", [](const httplib::Request& req, httplib::Response& res)
        {
            try
            {
                Logger::Info("POST /users");
                addCorsHeaders(res);

                User user;
                user.first_name = getJsonValue(req.body, "first_name");
                user.last_name = getJsonValue(req.body, "last_name");
                user.email = getJsonValue(req.body, "email");

                std::string heightText = getJsonValue(req.body, "height_cm");
                user.height_cm = heightText.empty() ? 0.0 : std::stod(heightText);

                Logger::Info("POST /users parsed request body successfully");

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
                addCorsHeaders(res);
                res.status = 500;
                res.set_content("{\"success\":false,\"message\":\"Server error while creating user\"}", "application/json");
            }
        });

    svr.Put(R"(/users/(\d+))", [](const httplib::Request& req, httplib::Response& res)
        {
            try
            {
                Logger::Info("PUT /users/{id}");
                addCorsHeaders(res);

                int userId = std::stoi(req.matches[1]);

                User user;
                user.user_id = userId;
                user.first_name = getJsonValue(req.body, "first_name");
                user.last_name = getJsonValue(req.body, "last_name");
                user.email = getJsonValue(req.body, "email");

                std::string heightText = getJsonValue(req.body, "height_cm");
                user.height_cm = heightText.empty() ? 0.0 : std::stod(heightText);

                UserService service;
                bool success = service.UpdateUser(user);

                if (success)
                {
                    res.set_content("{\"success\":true,\"message\":\"User updated successfully\"}", "application/json");
                }
                else
                {
                    res.status = 400;
                    res.set_content("{\"success\":false,\"message\":\"User update failed\"}", "application/json");
                }
            }
            catch (const std::exception& ex)
            {
                Logger::Error(std::string("PUT /users/{id} exception: ") + ex.what());
                addCorsHeaders(res);
                res.status = 500;
                res.set_content("{\"success\":false,\"message\":\"Server error while updating user\"}", "application/json");
            }
        });

    svr.Delete(R"(/users/(\d+))", [](const httplib::Request& req, httplib::Response& res)
        {
            try
            {
                Logger::Info("DELETE /users/{id}");
                addCorsHeaders(res);

                int userId = std::stoi(req.matches[1]);

                UserService service;
                bool success = service.DeleteUser(userId);

                if (success)
                {
                    res.set_content("{\"success\":true,\"message\":\"User deleted successfully\"}", "application/json");
                }
                else
                {
                    res.status = 404;
                    res.set_content("{\"success\":false,\"message\":\"User delete failed or user not found\"}", "application/json");
                }
            }
            catch (const std::exception& ex)
            {
                Logger::Error(std::string("DELETE /users/{id} exception: ") + ex.what());
                addCorsHeaders(res);
                res.status = 500;
                res.set_content("{\"success\":false,\"message\":\"Server error while deleting user\"}", "application/json");
            }
        });

    // WORKOUTS
    svr.Get("/workouts", [](const httplib::Request&, httplib::Response& res)
        {
            Logger::Info("GET /workouts");
            addCorsHeaders(res);

            WorkoutService service;
            auto workouts = service.GetAllWorkouts();

            std::ostringstream out;
            out << "[\n";
            for (size_t i = 0; i < workouts.size(); i++)
            {
                out << "  " << workoutToJson(workouts[i]);
                if (i + 1 < workouts.size()) out << ",";
                out << "\n";
            }
            out << "]";

            res.set_content(out.str(), "application/json");
        });

    svr.Get(R"(/workouts/(\d+))", [](const httplib::Request& req, httplib::Response& res)
        {
            try
            {
                Logger::Info("GET /workouts/{id}");
                addCorsHeaders(res);

                int workoutId = std::stoi(req.matches[1]);

                WorkoutService service;
                auto workout = service.GetWorkoutById(workoutId);

                if (!workout.has_value())
                {
                    res.status = 404;
                    res.set_content("{\"success\":false,\"message\":\"Workout not found\"}", "application/json");
                    return;
                }

                res.set_content(workoutToJson(workout.value()), "application/json");
            }
            catch (const std::exception& ex)
            {
                Logger::Error(std::string("GET /workouts/{id} exception: ") + ex.what());
                addCorsHeaders(res);
                res.status = 500;
                res.set_content("{\"success\":false,\"message\":\"Server error while fetching workout\"}", "application/json");
            }
        });

    svr.Post("/workouts", [](const httplib::Request& req, httplib::Response& res)
        {
            try
            {
                Logger::Info("POST /workouts");
                addCorsHeaders(res);

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
                addCorsHeaders(res);
                res.status = 500;
                res.set_content("{\"success\":false,\"message\":\"Server error while creating workout\"}", "application/json");
            }
        });

    svr.Put(R"(/workouts/(\d+))", [](const httplib::Request& req, httplib::Response& res)
        {
            try
            {
                Logger::Info("PUT /workouts/{id}");
                addCorsHeaders(res);

                int workoutId = std::stoi(req.matches[1]);

                Workout workout;
                workout.workout_id = workoutId;
                workout.user_id = std::stoi(getJsonValue(req.body, "user_id"));
                workout.workout_date = getJsonValue(req.body, "workout_date");
                workout.duration_min = std::stoi(getJsonValue(req.body, "duration_min"));
                workout.notes = getJsonValue(req.body, "notes");

                WorkoutService service;
                bool success = service.UpdateWorkout(workout);

                if (success)
                {
                    res.set_content("{\"success\":true,\"message\":\"Workout updated successfully\"}", "application/json");
                }
                else
                {
                    res.status = 400;
                    res.set_content("{\"success\":false,\"message\":\"Workout update failed\"}", "application/json");
                }
            }
            catch (const std::exception& ex)
            {
                Logger::Error(std::string("PUT /workouts/{id} exception: ") + ex.what());
                addCorsHeaders(res);
                res.status = 500;
                res.set_content("{\"success\":false,\"message\":\"Server error while updating workout\"}", "application/json");
            }
        });

    svr.Delete(R"(/workouts/(\d+))", [](const httplib::Request& req, httplib::Response& res)
        {
            try
            {
                Logger::Info("DELETE /workouts/{id}");
                addCorsHeaders(res);

                int workoutId = std::stoi(req.matches[1]);

                WorkoutService service;
                bool success = service.DeleteWorkout(workoutId);

                if (success)
                {
                    res.set_content("{\"success\":true,\"message\":\"Workout deleted successfully\"}", "application/json");
                }
                else
                {
                    res.status = 404;
                    res.set_content("{\"success\":false,\"message\":\"Workout delete failed or workout not found\"}", "application/json");
                }
            }
            catch (const std::exception& ex)
            {
                Logger::Error(std::string("DELETE /workouts/{id} exception: ") + ex.what());
                addCorsHeaders(res);
                res.status = 500;
                res.set_content("{\"success\":false,\"message\":\"Server error while deleting workout\"}", "application/json");
            }
        });

    // WORKOUT ENTRIES
    svr.Get(R"(/workouts/(\d+)/entries)", [](const httplib::Request& req, httplib::Response& res)
        {
            int workoutId = std::stoi(req.matches[1]);

            Logger::Info("GET /workouts/{id}/entries");
            addCorsHeaders(res);

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
                addCorsHeaders(res);

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
                addCorsHeaders(res);
                res.status = 500;
                res.set_content("{\"success\":false,\"message\":\"Server error while creating workout entry\"}", "application/json");
            }
        });

    // MEALS
    svr.Get("/meals", [](const httplib::Request&, httplib::Response& res)
        {
            Logger::Info("GET /meals");
            addCorsHeaders(res);

            MealService service;
            auto meals = service.GetAllMeals();

            std::ostringstream out;
            out << "[\n";
            for (size_t i = 0; i < meals.size(); i++)
            {
                out << "  " << mealToJson(meals[i]);
                if (i + 1 < meals.size()) out << ",";
                out << "\n";
            }
            out << "]";

            res.set_content(out.str(), "application/json");
        });

    svr.Get(R"(/meals/(\d+))", [](const httplib::Request& req, httplib::Response& res)
        {
            try
            {
                Logger::Info("GET /meals/{id}");
                addCorsHeaders(res);

                int mealId = std::stoi(req.matches[1]);

                MealService service;
                auto meal = service.GetMealById(mealId);

                if (!meal.has_value())
                {
                    res.status = 404;
                    res.set_content("{\"success\":false,\"message\":\"Meal not found\"}", "application/json");
                    return;
                }

                res.set_content(mealToJson(meal.value()), "application/json");
            }
            catch (const std::exception& ex)
            {
                Logger::Error(std::string("GET /meals/{id} exception: ") + ex.what());
                addCorsHeaders(res);
                res.status = 500;
                res.set_content("{\"success\":false,\"message\":\"Server error while fetching meal\"}", "application/json");
            }
        });

    svr.Post("/meals", [](const httplib::Request& req, httplib::Response& res)
        {
            try
            {
                Logger::Info("POST /meals");
                addCorsHeaders(res);

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
                addCorsHeaders(res);
                res.status = 500;
                res.set_content("{\"success\":false,\"message\":\"Server error while creating meal\"}", "application/json");
            }
        });

    svr.Put(R"(/meals/(\d+))", [](const httplib::Request& req, httplib::Response& res)
        {
            try
            {
                Logger::Info("PUT /meals/{id}");
                addCorsHeaders(res);

                int mealId = std::stoi(req.matches[1]);

                Meal meal;
                meal.meal_id = mealId;
                meal.user_id = std::stoi(getJsonValue(req.body, "user_id"));
                meal.meal_time = getJsonValue(req.body, "meal_time");
                meal.meal_type = getJsonValue(req.body, "meal_type");
                meal.notes = getJsonValue(req.body, "notes");

                MealService service;
                bool success = service.UpdateMeal(meal);

                if (success)
                {
                    res.set_content("{\"success\":true,\"message\":\"Meal updated successfully\"}", "application/json");
                }
                else
                {
                    res.status = 400;
                    res.set_content("{\"success\":false,\"message\":\"Meal update failed\"}", "application/json");
                }
            }
            catch (const std::exception& ex)
            {
                Logger::Error(std::string("PUT /meals/{id} exception: ") + ex.what());
                addCorsHeaders(res);
                res.status = 500;
                res.set_content("{\"success\":false,\"message\":\"Server error while updating meal\"}", "application/json");
            }
        });

    svr.Delete(R"(/meals/(\d+))", [](const httplib::Request& req, httplib::Response& res)
        {
            try
            {
                Logger::Info("DELETE /meals/{id}");
                addCorsHeaders(res);

                int mealId = std::stoi(req.matches[1]);

                MealService service;
                bool success = service.DeleteMeal(mealId);

                if (success)
                {
                    res.set_content("{\"success\":true,\"message\":\"Meal deleted successfully\"}", "application/json");
                }
                else
                {
                    res.status = 404;
                    res.set_content("{\"success\":false,\"message\":\"Meal delete failed or meal not found\"}", "application/json");
                }
            }
            catch (const std::exception& ex)
            {
                Logger::Error(std::string("DELETE /meals/{id} exception: ") + ex.what());
                addCorsHeaders(res);
                res.status = 500;
                res.set_content("{\"success\":false,\"message\":\"Server error while deleting meal\"}", "application/json");
            }
        });

    // MEAL ITEMS
    svr.Get(R"(/meals/(\d+)/items)", [](const httplib::Request& req, httplib::Response& res)
        {
            int mealId = std::stoi(req.matches[1]);

            Logger::Info("GET /meals/{id}/items");
            addCorsHeaders(res);

            MealService service;
            auto items = service.GetItemsForMeal(mealId);

            std::ostringstream out;
            out << "[\n";
            for (size_t i = 0; i < items.size(); i++)
            {
                out << "  " << mealItemToJson(items[i]);
                if (i + 1 < items.size()) out << ",";
                out << "\n";
            }
            out << "]";

            res.set_content(out.str(), "application/json");
        });

    svr.Get(R"(/meal_items/(\d+))", [](const httplib::Request& req, httplib::Response& res)
        {
            try
            {
                Logger::Info("GET /meal_items/{id}");
                addCorsHeaders(res);

                int itemId = std::stoi(req.matches[1]);

                MealService service;
                auto item = service.GetMealItemById(itemId);

                if (!item.has_value())
                {
                    res.status = 404;
                    res.set_content("{\"success\":false,\"message\":\"Meal item not found\"}", "application/json");
                    return;
                }

                res.set_content(mealItemToJson(item.value()), "application/json");
            }
            catch (const std::exception& ex)
            {
                Logger::Error(std::string("GET /meal_items/{id} exception: ") + ex.what());
                addCorsHeaders(res);
                res.status = 500;
                res.set_content("{\"success\":false,\"message\":\"Server error while fetching meal item\"}", "application/json");
            }
        });

    svr.Post(R"(/meals/(\d+)/items)", [](const httplib::Request& req, httplib::Response& res)
        {
            try
            {
                int mealId = std::stoi(req.matches[1]);

                Logger::Info("POST /meals/{id}/items");
                addCorsHeaders(res);

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
                addCorsHeaders(res);
                res.status = 500;
                res.set_content("{\"success\":false,\"message\":\"Server error while creating meal item\"}", "application/json");
            }
        });

    svr.Put(R"(/meal_items/(\d+))", [](const httplib::Request& req, httplib::Response& res)
        {
            try
            {
                Logger::Info("PUT /meal_items/{id}");
                addCorsHeaders(res);

                int itemId = std::stoi(req.matches[1]);

                MealItem item;
                item.item_id = itemId;

                std::string mealIdText = getJsonValue(req.body, "meal_id");
                item.meal_id = mealIdText.empty() ? 0 : std::stoi(mealIdText);

                item.food_name = getJsonValue(req.body, "food_name");
                item.calories = std::stoi(getJsonValue(req.body, "calories"));
                item.protein_g = std::stod(getJsonValue(req.body, "protein_g"));
                item.carbs_g = std::stod(getJsonValue(req.body, "carbs_g"));
                item.fat_g = std::stod(getJsonValue(req.body, "fat_g"));

                MealService service;
                bool success = service.UpdateMealItem(item);

                if (success)
                {
                    res.set_content("{\"success\":true,\"message\":\"Meal item updated successfully\"}", "application/json");
                }
                else
                {
                    res.status = 400;
                    res.set_content("{\"success\":false,\"message\":\"Meal item update failed\"}", "application/json");
                }
            }
            catch (const std::exception& ex)
            {
                Logger::Error(std::string("PUT /meal_items/{id} exception: ") + ex.what());
                addCorsHeaders(res);
                res.status = 500;
                res.set_content("{\"success\":false,\"message\":\"Server error while updating meal item\"}", "application/json");
            }
        });

    svr.Delete(R"(/meal_items/(\d+))", [](const httplib::Request& req, httplib::Response& res)
        {
            try
            {
                Logger::Info("DELETE /meal_items/{id}");
                addCorsHeaders(res);

                int itemId = std::stoi(req.matches[1]);

                MealService service;
                bool success = service.DeleteMealItem(itemId);

                if (success)
                {
                    res.set_content("{\"success\":true,\"message\":\"Meal item deleted successfully\"}", "application/json");
                }
                else
                {
                    res.status = 404;
                    res.set_content("{\"success\":false,\"message\":\"Meal item delete failed or item not found\"}", "application/json");
                }
            }
            catch (const std::exception& ex)
            {
                Logger::Error(std::string("DELETE /meal_items/{id} exception: ") + ex.what());
                addCorsHeaders(res);
                res.status = 500;
                res.set_content("{\"success\":false,\"message\":\"Server error while deleting meal item\"}", "application/json");
            }
        });

    Logger::Info("Server started on port 8080");
    svr.listen("localhost", 8080);
}