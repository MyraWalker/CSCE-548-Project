// MealItemDAL.cpp  (libpqxx 7+ compatible)

#include "MealItemDAL.h"

#include <pqxx/pqxx>
#include <stdexcept>

// Build a MealItem from a pqxx::row
static MealItem mealItemFromRow(const pqxx::row& r)
{
    MealItem mi{};
    mi.item_id = r["item_id"].as<int>();
    mi.meal_id = r["meal_id"].as<int>();
    mi.food_name = r["food_name"].as<std::string>();
    mi.calories = r["calories"].as<int>();
    mi.protein_g = r["protein_g"].as<double>();
    mi.carbs_g = r["carbs_g"].as<double>();
    mi.fat_g = r["fat_g"].as<double>();
    return mi;
}

// -------------------- CREATE --------------------
int MealItemDAL::create(const MealItem& mi)
{
    auto conn = db_.connect();
    pqxx::work tx(conn);

    pqxx::row r = tx.exec(
        pqxx::zview{
            "INSERT INTO meal_items "
            "(meal_id, food_name, calories, protein_g, carbs_g, fat_g) "
            "VALUES ($1, $2, $3, $4, $5, $6) "
            "RETURNING item_id"
        },
        pqxx::params{
            mi.meal_id,
            mi.food_name,
            mi.calories,
            mi.protein_g,
            mi.carbs_g,
            mi.fat_g
        }
    ).one_row();

    tx.commit();
    return r["item_id"].as<int>();
}

// -------------------- READ ONE --------------------
std::optional<MealItem> MealItemDAL::getById(int id)
{
    auto conn = db_.connect();
    pqxx::work tx(conn);

    pqxx::result res = tx.exec(
        pqxx::zview{
            "SELECT item_id, meal_id, food_name, calories, protein_g, carbs_g, fat_g "
            "FROM meal_items "
            "WHERE item_id = $1"
        },
        pqxx::params{ id }
    );

    tx.commit();

    if (res.empty()) return std::nullopt;
    return mealItemFromRow(res[0]);
}

// -------------------- READ ALL --------------------
std::vector<MealItem> MealItemDAL::getAll()
{
    auto conn = db_.connect();
    pqxx::work tx(conn);

    pqxx::result res = tx.exec(
        pqxx::zview{
            "SELECT item_id, meal_id, food_name, calories, protein_g, carbs_g, fat_g "
            "FROM meal_items "
            "ORDER BY item_id"
        }
    );

    tx.commit();

    std::vector<MealItem> items;
    items.reserve(res.size());
    for (const auto& row : res)
        items.push_back(mealItemFromRow(row));

    return items;
}

// -------------------- READ BY MEAL --------------------
std::vector<MealItem> MealItemDAL::getByMealId(int meal_id)
{
    auto conn = db_.connect();
    pqxx::work tx(conn);

    pqxx::result res = tx.exec(
        pqxx::zview{
            "SELECT item_id, meal_id, food_name, calories, protein_g, carbs_g, fat_g "
            "FROM meal_items "
            "WHERE meal_id = $1 "
            "ORDER BY item_id"
        },
        pqxx::params{ meal_id }
    );

    tx.commit();

    std::vector<MealItem> items;
    items.reserve(res.size());
    for (const auto& row : res)
        items.push_back(mealItemFromRow(row));

    return items;
}

// -------------------- UPDATE --------------------
bool MealItemDAL::update(const MealItem& mi)
{
    auto conn = db_.connect();
    pqxx::work tx(conn);

    pqxx::result res = tx.exec(
        pqxx::zview{
            "UPDATE meal_items "
            "SET food_name = $1, calories = $2, protein_g = $3, carbs_g = $4, fat_g = $5 "
            "WHERE item_id = $6"
        },
        pqxx::params{
            mi.food_name,
            mi.calories,
            mi.protein_g,
            mi.carbs_g,
            mi.fat_g,
            mi.item_id
        }
    );

    tx.commit();
    return res.affected_rows() == 1;
}

// -------------------- DELETE --------------------
bool MealItemDAL::remove(int id)
{
    auto conn = db_.connect();
    pqxx::work tx(conn);

    pqxx::result res = tx.exec(
        pqxx::zview{ "DELETE FROM meal_items WHERE item_id = $1" },
        pqxx::params{ id }
    );

    tx.commit();
    return res.affected_rows() == 1;
}
