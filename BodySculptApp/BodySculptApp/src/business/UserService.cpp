#include "UserService.h"
#include "../dal/UserDAL.h"
#include "../db/Db.h"
#include "../utils/Logger.h"
#include <optional>

std::vector<User> UserService::GetAllUsers()
{
    Logger::Info("Fetching all users");

    Db db("host=localhost port=5432 dbname=body_sculpt user=postgres password=postgresql");
    UserDAL dal(db);

    return dal.getAll();
}

std::optional<User> UserService::GetUserById(int userId)
{
    if (userId <= 0)
    {
        Logger::Warn("GetUserById failed: invalid userId");
        return std::nullopt;
    }

    Logger::Info("Fetching user by id");

    Db db("host=localhost port=5432 dbname=body_sculpt user=postgres password=postgresql");
    UserDAL dal(db);

    return dal.getById(userId);
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

bool UserService::UpdateUser(const User& user)
{
    if (user.user_id <= 0)
    {
        Logger::Warn("User update failed: invalid user_id");
        return false;
    }

    if (user.email.find('@') == std::string::npos)
    {
        Logger::Warn("User update failed: invalid email");
        return false;
    }

    if (user.first_name.empty() || user.last_name.empty())
    {
        Logger::Warn("User update failed: first or last name is blank");
        return false;
    }

    if (user.height_cm <= 0)
    {
        Logger::Warn("User update failed: invalid height");
        return false;
    }

    Logger::Info("Updating user");

    Db db("host=localhost port=5432 dbname=body_sculpt user=postgres password=postgresql");
    UserDAL dal(db);

    return dal.update(user);
}

bool UserService::DeleteUser(int userId)
{
    if (userId <= 0)
    {
        Logger::Warn("User delete failed: invalid userId");
        return false;
    }

    Logger::Info("Deleting user");

    Db db("host=localhost port=5432 dbname=body_sculpt user=postgres password=postgresql");
    UserDAL dal(db);

    return dal.remove(userId);
}