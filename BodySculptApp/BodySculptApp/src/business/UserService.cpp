#include "UserService.h"
#include "../dal/UserDAL.h"
#include "../db/Db.h"
#include "../utils/Logger.h"

std::vector<User> UserService::GetAllUsers()
{
    Logger::Info("Fetching all users");

    Db db("host=localhost port=5432 dbname=body_sculpt user=postgres password=postgresql");
    UserDAL dal(db);

    return dal.getAll();
}

bool UserService::CreateUser(const User& user)
{
    if (user.email.find('@') == std::string::npos)
    {
        Logger::Warn("Invalid email attempted");
        return false;
    }

    if (user.first_name.empty() || user.last_name.empty())
    {
        Logger::Warn("User creation failed: first or last name is blank");
        return false;
    }

    if (user.height_cm <= 0)
    {
        Logger::Warn("User creation failed: invalid height");
        return false;
    }

    Logger::Info("Creating user");

    Db db("host=localhost port=5432 dbname=body_sculpt user=postgres password=postgresql");
    UserDAL dal(db);

    int newId = dal.create(user);
    return newId > 0;
}