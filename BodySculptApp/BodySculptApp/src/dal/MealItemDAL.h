#pragma once
#include <vector>
#include <optional>
#include "../db/Db.h"
#include "../models/MealItem.h"

class MealItemDAL
{
public:
    explicit MealItemDAL(const Db& db) : db_(db) {}

    int create(const MealItem& mi);
    std::optional<MealItem> getById(int id);
    std::vector<MealItem> getAll();
    std::vector<MealItem> getByMealId(int meal_id);
    bool update(const MealItem& mi);
    bool remove(int id);

private:
    const Db& db_;
};