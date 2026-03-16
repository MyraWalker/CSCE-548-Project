// UserDAL.cpp  (libpqxx 7+ compatible)

#include "UserDAL.h"

#include <pqxx/pqxx>
#include <stdexcept>
#include <vector>
#include <optional>

// Build a User from a pqxx::row
static User userFromRow(const pqxx::row& r)
{
    User u{};
    u.user_id = r["user_id"].as<int>();
    u.first_name = r["first_name"].as<std::string>();
    u.last_name = r["last_name"].as<std::string>();
    u.email = r["email"].as<std::string>();
    u.height_cm = r["height_cm"].as<double>();
    return u;
}

// -------------------- CREATE --------------------
int UserDAL::create(const User& u)
{
    auto conn = db_.connect();
    pqxx::work tx(conn);

    pqxx::row r = tx
        .exec(
            pqxx::zview{
                "INSERT INTO users (first_name, last_name, email, height_cm) "
                "VALUES ($1, $2, $3, $4) "
                "RETURNING user_id"
            },
            pqxx::params{ u.first_name, u.last_name, u.email, u.height_cm }
        )
        .one_row();

    tx.commit();
    return r["user_id"].as<int>();
}

// -------------------- READ ONE --------------------
std::optional<User> UserDAL::getById(int id)
{
    auto conn = db_.connect();
    pqxx::work tx(conn);

    pqxx::result res = tx.exec(
        pqxx::zview{
            "SELECT user_id, first_name, last_name, email, height_cm "
            "FROM users "
            "WHERE user_id = $1"
        },
        pqxx::params{ id }
    );

    tx.commit();

    if (res.empty()) return std::nullopt;
    return userFromRow(res[0]);
}

// -------------------- READ ALL --------------------
std::vector<User> UserDAL::getAll()
{
    auto conn = db_.connect();
    pqxx::work tx(conn);

    pqxx::result res = tx.exec(
        pqxx::zview{
            "SELECT user_id, first_name, last_name, email, height_cm "
            "FROM users "
            "ORDER BY user_id"
        }
    );

    tx.commit();

    std::vector<User> users;
    users.reserve(res.size());
    for (const auto& row : res)
        users.push_back(userFromRow(row));

    return users;
}

// -------------------- UPDATE --------------------
bool UserDAL::update(const User& u)
{
    auto conn = db_.connect();
    pqxx::work tx(conn);

    pqxx::result res = tx.exec(
        pqxx::zview{
            "UPDATE users "
            "SET first_name = $1, last_name = $2, email = $3, height_cm = $4 "
            "WHERE user_id = $5"
        },
        pqxx::params{ u.first_name, u.last_name, u.email, u.height_cm, u.user_id }
    );

    tx.commit();

    // affected_rows() tells you if any row actually updated
    return res.affected_rows() == 1;
}

// -------------------- DELETE --------------------
bool UserDAL::remove(int id)
{
    auto conn = db_.connect();
    pqxx::work tx(conn);

    pqxx::result res = tx.exec(
        pqxx::zview{ "DELETE FROM users WHERE user_id = $1" },
        pqxx::params{ id }
    );

    tx.commit();
    return res.affected_rows() == 1;
}
