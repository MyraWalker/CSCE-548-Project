#pragma once
#include <string>

struct WorkoutEntry {
    int entry_id{};
    int workout_id{};
    std::string exercise_name;
    int sets{};
    int reps{};
    double weight_kg{}; // numeric(6,2)
};

