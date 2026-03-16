#include "MealService.h"
#include "../dal/MealDAL.h"
#include "../dal/MealItemDAL.h"
#include "../db/Db.h"
#include "../utils/Logger.h"
#include <algorithm>
#include <cctype>
#include <string>

static std::string toLowerCopy(const std::string& value)
{
    std::string result = value;
    std::transform(result.begin(), result.end(), result.begin(),
        [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return result;
}

std::vector<Meal> MealService::GetAllMeals()
{
    Logger::Info("Fetching all meals");

    // PostgreSQL connection string
    Db db("host=localhost port=5432 dbname=body_sculpt user=postgres password=postgresql");
    MealDAL dal(db);

    return dal.getAll();
}

bool MealService::CreateMeal(const Meal& meal)
{
    if (meal.user_id <= 0)
    {
        Logger::Warn("Meal creation failed: invalid user_id");
        return false;
    }

    if (meal.meal_time.empty())
    {
        Logger::Warn("Meal creation failed: missing meal_time");
        return false;
    }

    if (meal.meal_type.empty())
    {
        Logger::Warn("Meal creation failed: meal_type is blank");
        return false;
    }

    std::string normalizedMealType = toLowerCopy(meal.meal_type);

    if (normalizedMealType != "breakfast" &&
        normalizedMealType != "lunch" &&
        normalizedMealType != "dinner" &&
        normalizedMealType != "snack")
    {
        Logger::Warn("Meal creation failed: invalid meal_type");
        return false;
    }

    Logger::Info("Creating meal");

    // PostgreSQL connection string
    Db db("host=localhost port=5432 dbname=body_sculpt user=postgres password=postgresql");
    MealDAL dal(db);

    Meal mealToCreate = meal;
    mealToCreate.meal_type = normalizedMealType;

    int newId = dal.create(mealToCreate);
    return newId > 0;
}

std::vector<MealItem> MealService::GetItemsForMeal(int mealId)
{
    if (mealId <= 0)
    {
        Logger::Warn("GetItemsForMeal failed: invalid mealId");
        return {};
    }

    Logger::Info("Fetching meal items");

    // PostgreSQL connection string
    Db db("host=localhost port=5432 dbname=body_sculpt user=postgres password=postgresql");
    MealItemDAL dal(db);

    return dal.getByMealId(mealId);
}

bool MealService::CreateMealItem(const MealItem& item)
{
    if (item.meal_id <= 0)
    {
        Logger::Warn("Meal item creation failed: invalid meal_id");
        return false;
    }

    if (item.food_name.empty())
    {
        Logger::Warn("Meal item creation failed: missing food_name");
        return false;
    }

    if (item.calories < 0 || item.protein_g < 0 || item.carbs_g < 0 || item.fat_g < 0)
    {
        Logger::Warn("Meal item creation failed: negative nutrition values");
        return false;
    }

    Logger::Info("Creating meal item");

    // PostgreSQL connection string
    Db db("host=localhost port=5432 dbname=body_sculpt user=postgres password=postgresql");
    MealItemDAL dal(db);

    int newId = dal.create(item);
    return newId > 0;
}