#pragma once
#include "../models/Meal.h"
#include "../models/MealItem.h"
#include <vector>
#include <optional>

class MealService
{
public:
    std::vector<Meal> GetAllMeals();
    std::optional<Meal> GetMealById(int mealId);
    bool CreateMeal(const Meal& meal);
    bool UpdateMeal(const Meal& meal);
    bool DeleteMeal(int mealId);

    std::vector<MealItem> GetItemsForMeal(int mealId);
    std::optional<MealItem> GetMealItemById(int itemId);
    bool CreateMealItem(const MealItem& item);
    bool UpdateMealItem(const MealItem& item);
    bool DeleteMealItem(int itemId);
};