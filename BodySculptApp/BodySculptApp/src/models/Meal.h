#pragma once
#include <string>
#include <optional>

struct Meal {
    int meal_id{};
    int user_id{};
    std::string meal_time;            // timestamp without time zone
    std::string meal_type;            // breakfast/lunch/dinner/snack
    std::optional<std::string> notes; // nullable
};

