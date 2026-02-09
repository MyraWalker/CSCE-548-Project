#pragma once
#include <vector>
#include <optional>

#include "Db.h"
#include "Meal.h"

class MealDAL {
public:
    explicit MealDAL(const Db& db) : db_(db) {}

    int create(const Meal& m);
    std::optional<Meal> getById(int id);
    std::vector<Meal> getAll();
    bool update(const Meal& m);
    bool remove(int id);

private:
    const Db& db_;
};

