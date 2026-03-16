#pragma once
#include "../models/Meal.h"
#include "../models/MealItem.h"
#include <vector>

class MealService
{
public:
    std::vector<Meal> GetAllMeals();
    bool CreateMeal(const Meal& meal);

    std::vector<MealItem> GetItemsForMeal(int mealId);
    bool CreateMealItem(const MealItem& item);
};