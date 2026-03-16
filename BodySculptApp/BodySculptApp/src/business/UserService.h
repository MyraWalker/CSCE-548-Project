#pragma once
#include "../models/User.h"
#include <vector>

class UserService
{
public:
    std::vector<User> GetAllUsers();
    bool CreateUser(const User& user);
};