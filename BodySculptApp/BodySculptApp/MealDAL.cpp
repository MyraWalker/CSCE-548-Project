// MealDAL.cpp  (libpqxx 7+ compatible)

#include "MealDAL.h"

#include <pqxx/pqxx>
#include <stdexcept>

// -------------------- helper --------------------
static Meal mealFromRow(const pqxx::row& r)
{
    Meal m{};
    m.meal_id = r["meal_id"].as<int>();
    m.user_id = r["user_id"].as<int>();
    m.meal_time = r["meal_time"].as<std::string>();
    m.meal_type = r["meal_type"].as<std::string>();

    // notes is nullable
    if (r["notes"].is_null())
        m.notes = std::nullopt;
    else
        m.notes = r["notes"].as<std::string>();

    return m;
}

// -------------------- CREATE --------------------
int MealDAL::create(const Meal& m)
{
    auto conn = db_.connect();
    pqxx::work tx(conn);

    pqxx::row r = tx.exec(
        pqxx::zview{
            "INSERT INTO meals (user_id, meal_time, meal_type, notes) "
            "VALUES ($1, $2, $3, $4) "
            "RETURNING meal_id"
        },
        pqxx::params{
            m.user_id,
            m.meal_time,
            m.meal_type,
            m.notes.has_value() ? m.notes.value() : nullptr
        }
    ).one_row();

    tx.commit();
    return r["meal_id"].as<int>();
}

// -------------------- READ ONE --------------------
std::optional<Meal> MealDAL::getById(int id)
{
    auto conn = db_.connect();
    pqxx::work tx(conn);

    pqxx::result res = tx.exec(
        pqxx::zview{
            "SELECT meal_id, user_id, meal_time, meal_type, notes "
            "FROM meals "
            "WHERE meal_id = $1"
        },
        pqxx::params{ id }
    );

    tx.commit();

    if (res.empty())
        return std::nullopt;

    return mealFromRow(res[0]);
}

// -------------------- READ ALL --------------------
std::vector<Meal> MealDAL::getAll()
{
    auto conn = db_.connect();
    pqxx::work tx(conn);

    pqxx::result res = tx.exec(
        pqxx::zview{
            "SELECT meal_id, user_id, meal_time, meal_type, notes "
            "FROM meals "
            "ORDER BY meal_time DESC"
        }
    );

    tx.commit();

    std::vector<Meal> meals;
    meals.reserve(res.size());

    for (const auto& row : res)
        meals.push_back(mealFromRow(row));

    return meals;
}

// -------------------- UPDATE --------------------
bool MealDAL::update(const Meal& m)
{
    auto conn = db_.connect();
    pqxx::work tx(conn);

    pqxx::result res = tx.exec(
        pqxx::zview{
            "UPDATE meals "
            "SET user_id = $1, meal_time = $2, meal_type = $3, notes = $4 "
            "WHERE meal_id = $5"
        },
        pqxx::params{
            m.user_id,
            m.meal_time,
            m.meal_type,
            m.notes.has_value() ? m.notes.value() : nullptr,
            m.meal_id
        }
    );

    tx.commit();
    return res.affected_rows() == 1;
}

// -------------------- DELETE --------------------
bool MealDAL::remove(int id)
{
    auto conn = db_.connect();
    pqxx::work tx(conn);

    pqxx::result res = tx.exec(
        pqxx::zview{ "DELETE FROM meals WHERE meal_id = $1" },
        pqxx::params{ id }
    );

    tx.commit();
    return res.affected_rows() == 1;
}
