#pragma once
#include <vector>
#include <optional>

#include "Db.h"
#include "MealItem.h"

class MealItemDAL {
public:
    explicit MealItemDAL(const Db& db) : db_(db) {}

    int create(const MealItem& mi);
    std::optional<MealItem> getById(int id);
    std::vector<MealItem> getAll();
    std::vector<MealItem> getByMealId(int meal_id); // helpful because of FK
    bool update(const MealItem& mi);
    bool remove(int id);

private:
    const Db& db_;
};
