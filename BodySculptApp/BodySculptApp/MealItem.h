#pragma once
#include <string>

struct MealItem {
    int item_id{};
    int meal_id{};
    std::string food_name;
    int calories{};
    double protein_g{}; // numeric(6,2)
    double carbs_g{}; // numeric(6,2)
    double fat_g{}; // numeric(6,2)
};

