#pragma once
#include <vector>
#include <optional>

#include "Db.h"
#include "Workout.h"

class WorkoutDAL {
public:
    explicit WorkoutDAL(const Db& db) : db_(db) {}

    int create(const Workout& w);
    std::optional<Workout> getById(int id);
    std::vector<Workout> getAll();
    std::vector<Workout> getByUserId(int user_id); // helpful because of FK
    bool update(const Workout& w);
    bool remove(int id);

private:
    const Db& db_;
};

