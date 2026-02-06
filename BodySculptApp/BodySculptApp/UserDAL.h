#pragma once
#include <vector>
#include <optional>
#include "Db.h"
#include "User.h"

class UserDAL {
public:
    explicit UserDAL(const Db& db) : db_(db) {}

    int create(const User& u);
    std::optional<User> getById(int id);
    std::vector<User> getAll();
    bool update(const User& u);
    bool remove(int id);

private:
    const Db& db_;
};
