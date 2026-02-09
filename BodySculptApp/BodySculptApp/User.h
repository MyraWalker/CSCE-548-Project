#pragma once
#include <string>

struct User {
    int user_id{};
    std::string first_name;
    std::string last_name;
    std::string email;
    double height_cm{}; // numeric in PostgreSQL ? double in C++
};
