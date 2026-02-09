// WorkoutDAL.cpp  (libpqxx 7+ compatible)

#include "WorkoutDAL.h"

#include <pqxx/pqxx>
#include <stdexcept>

// Build a Workout from a pqxx::row
static Workout workoutFromRow(const pqxx::row& r)
{
    Workout w{};
    w.workout_id = r["workout_id"].as<int>();
    w.user_id = r["user_id"].as<int>();
    w.workout_date = r["workout_date"].as<std::string>();
    w.duration_min = r["duration_min"].as<int>();

    if (r["notes"].is_null()) w.notes = std::nullopt;
    else w.notes = r["notes"].as<std::string>();

    return w;
}

// -------------------- CREATE --------------------
int WorkoutDAL::create(const Workout& w)
{
    auto conn = db_.connect();
    pqxx::work tx(conn);

    // Use $4 for notes; pass nullptr if no notes
    pqxx::row r = tx.exec(
        pqxx::zview{
            "INSERT INTO workouts (user_id, workout_date, duration_min, notes) "
            "VALUES ($1, $2, $3, $4) "
            "RETURNING workout_id"
        },
        pqxx::params{
            w.user_id,
            w.workout_date, // text -> date coercion
            w.duration_min,
            w.notes.has_value() ? w.notes.value() : nullptr
        }
    ).one_row();

    tx.commit();
    return r["workout_id"].as<int>();
}

// -------------------- READ ONE --------------------
std::optional<Workout> WorkoutDAL::getById(int id)
{
    auto conn = db_.connect();
    pqxx::work tx(conn);

    pqxx::result res = tx.exec(
        pqxx::zview{
            "SELECT workout_id, user_id, workout_date, duration_min, notes "
            "FROM workouts "
            "WHERE workout_id = $1"
        },
        pqxx::params{ id }
    );

    tx.commit();

    if (res.empty()) return std::nullopt;
    return workoutFromRow(res[0]);
}

// -------------------- READ ALL --------------------
std::vector<Workout> WorkoutDAL::getAll()
{
    auto conn = db_.connect();
    pqxx::work tx(conn);

    pqxx::result res = tx.exec(
        pqxx::zview{
            "SELECT workout_id, user_id, workout_date, duration_min, notes "
            "FROM workouts "
            "ORDER BY workout_date DESC"
        }
    );

    tx.commit();

    std::vector<Workout> workouts;
    workouts.reserve(res.size());
    for (const auto& row : res)
        workouts.push_back(workoutFromRow(row));

    return workouts;
}

// -------------------- READ BY USER --------------------
std::vector<Workout> WorkoutDAL::getByUserId(int user_id)
{
    auto conn = db_.connect();
    pqxx::work tx(conn);

    pqxx::result res = tx.exec(
        pqxx::zview{
            "SELECT workout_id, user_id, workout_date, duration_min, notes "
            "FROM workouts "
            "WHERE user_id = $1 "
            "ORDER BY workout_date DESC"
        },
        pqxx::params{ user_id }
    );

    tx.commit();

    std::vector<Workout> workouts;
    workouts.reserve(res.size());
    for (const auto& row : res)
        workouts.push_back(workoutFromRow(row));

    return workouts;
}

// -------------------- UPDATE --------------------
bool WorkoutDAL::update(const Workout& w)
{
    auto conn = db_.connect();
    pqxx::work tx(conn);

    pqxx::result res = tx.exec(
        pqxx::zview{
            "UPDATE workouts "
            "SET user_id = $1, workout_date = $2, duration_min = $3, notes = $4 "
            "WHERE workout_id = $5"
        },
        pqxx::params{
            w.user_id,
            w.workout_date,
            w.duration_min,
            w.notes.has_value() ? w.notes.value() : nullptr,
            w.workout_id
        }
    );

    tx.commit();
    return res.affected_rows() == 1;
}

// -------------------- DELETE --------------------
bool WorkoutDAL::remove(int id)
{
    auto conn = db_.connect();
    pqxx::work tx(conn);

    pqxx::result res = tx.exec(
        pqxx::zview{ "DELETE FROM workouts WHERE workout_id = $1" },
        pqxx::params{ id }
    );

    tx.commit();
    return res.affected_rows() == 1;
}
