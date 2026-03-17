#pragma once
#include "../models/User.h"
#include <vector>
#include <optional>

class UserService
{
public:
    std::vector<User> GetAllUsers();
    std::optional<User> GetUserById(int userId);
    bool CreateUser(const User& user);
    bool UpdateUser(const User& user);
    bool DeleteUser(int userId);
};