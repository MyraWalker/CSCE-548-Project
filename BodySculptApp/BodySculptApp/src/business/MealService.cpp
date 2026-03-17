#include "MealService.h"
#include "../dal/MealDAL.h"
#include "../dal/MealItemDAL.h"
#include "../db/Db.h"
#include "../utils/Logger.h"
#include <algorithm>
#include <cctype>
#include <string>
#include <optional>

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

    Db db("host=localhost port=5432 dbname=body_sculpt user=postgres password=postgresql");
    MealDAL dal(db);

    return dal.getAll();
}

std::optional<Meal> MealService::GetMealById(int mealId)
{
    if (mealId <= 0)
    {
        Logger::Warn("GetMealById failed: invalid mealId");
        return std::nullopt;
    }

    Logger::Info("Fetching meal by id");

    Db db("host=localhost port=5432 dbname=body_sculpt user=postgres password=postgresql");
    MealDAL dal(db);

    return dal.getById(mealId);
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

    Db db("host=localhost port=5432 dbname=body_sculpt user=postgres password=postgresql");
    MealDAL dal(db);

    Meal mealToCreate = meal;
    mealToCreate.meal_type = normalizedMealType;

    int newId = dal.create(mealToCreate);
    return newId > 0;
}

bool MealService::UpdateMeal(const Meal& meal)
{
    if (meal.meal_id <= 0)
    {
        Logger::Warn("Meal update failed: invalid meal_id");
        return false;
    }

    if (meal.user_id <= 0)
    {
        Logger::Warn("Meal update failed: invalid user_id");
        return false;
    }

    if (meal.meal_time.empty())
    {
        Logger::Warn("Meal update failed: missing meal_time");
        return false;
    }

    if (meal.meal_type.empty())
    {
        Logger::Warn("Meal update failed: meal_type is blank");
        return false;
    }

    std::string normalizedMealType = toLowerCopy(meal.meal_type);

    if (normalizedMealType != "breakfast" &&
        normalizedMealType != "lunch" &&
        normalizedMealType != "dinner" &&
        normalizedMealType != "snack")
    {
        Logger::Warn("Meal update failed: invalid meal_type");
        return false;
    }

    Logger::Info("Updating meal");

    Db db("host=localhost port=5432 dbname=body_sculpt user=postgres password=postgresql");
    MealDAL dal(db);

    Meal mealToUpdate = meal;
    mealToUpdate.meal_type = normalizedMealType;

    return dal.update(mealToUpdate);
}

bool MealService::DeleteMeal(int mealId)
{
    if (mealId <= 0)
    {
        Logger::Warn("Meal delete failed: invalid mealId");
        return false;
    }

    Logger::Info("Deleting meal");

    Db db("host=localhost port=5432 dbname=body_sculpt user=postgres password=postgresql");
    MealDAL dal(db);

    return dal.remove(mealId);
}

std::vector<MealItem> MealService::GetItemsForMeal(int mealId)
{
    if (mealId <= 0)
    {
        Logger::Warn("GetItemsForMeal failed: invalid mealId");
        return {};
    }

    Logger::Info("Fetching meal items");

    Db db("host=localhost port=5432 dbname=body_sculpt user=postgres password=postgresql");
    MealItemDAL dal(db);

    return dal.getByMealId(mealId);
}

std::optional<MealItem> MealService::GetMealItemById(int itemId)
{
    if (itemId <= 0)
    {
        Logger::Warn("GetMealItemById failed: invalid itemId");
        return std::nullopt;
    }

    Logger::Info("Fetching meal item by id");

    Db db("host=localhost port=5432 dbname=body_sculpt user=postgres password=postgresql");
    MealItemDAL dal(db);

    return dal.getById(itemId);
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

    Db db("host=localhost port=5432 dbname=body_sculpt user=postgres password=postgresql");
    MealItemDAL dal(db);

    int newId = dal.create(item);
    return newId > 0;
}

bool MealService::UpdateMealItem(const MealItem& item)
{
    if (item.item_id <= 0)
    {
        Logger::Warn("Meal item update failed: invalid item_id");
        return false;
    }

    if (item.food_name.empty())
    {
        Logger::Warn("Meal item update failed: missing food_name");
        return false;
    }

    if (item.calories < 0 || item.protein_g < 0 || item.carbs_g < 0 || item.fat_g < 0)
    {
        Logger::Warn("Meal item update failed: negative nutrition values");
        return false;
    }

    Logger::Info("Updating meal item");

    Db db("host=localhost port=5432 dbname=body_sculpt user=postgres password=postgresql");
    MealItemDAL dal(db);

    return dal.update(item);
}

bool MealService::DeleteMealItem(int itemId)
{
    if (itemId <= 0)
    {
        Logger::Warn("Meal item delete failed: invalid itemId");
        return false;
    }

    Logger::Info("Deleting meal item");

    Db db("host=localhost port=5432 dbname=body_sculpt user=postgres password=postgresql");
    MealItemDAL dal(db);

    return dal.remove(itemId);
}