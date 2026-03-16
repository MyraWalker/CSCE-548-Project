#pragma once
#include <string>
#include <optional>

struct Workout {
    int workout_id{};
    int user_id{};
    std::string workout_date;          // date as "YYYY-MM-DD"
    int duration_min{};
    std::optional<std::string> notes;  // nullable
};

