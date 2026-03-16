#pragma once
#include <vector>
#include <optional>

#include "Db.h"
#include "WorkoutEntry.h"

class WorkoutEntryDAL {
public:
    explicit WorkoutEntryDAL(const Db& db) : db_(db) {}

    int create(const WorkoutEntry& e);
    std::optional<WorkoutEntry> getById(int id);
    std::vector<WorkoutEntry> getAll();
    std::vector<WorkoutEntry> getByWorkoutId(int workout_id); // helpful because of FK
    bool update(const WorkoutEntry& e);
    bool remove(int id);

private:
    const Db& db_;
};

