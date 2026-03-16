// WorkoutEntryDAL.cpp  (libpqxx 7+ compatible)

#include "WorkoutEntryDAL.h"

#include <pqxx/pqxx>
#include <stdexcept>

// Build a WorkoutEntry from a pqxx::row
static WorkoutEntry workoutEntryFromRow(const pqxx::row& r)
{
    WorkoutEntry e{};
    e.entry_id = r["entry_id"].as<int>();
    e.workout_id = r["workout_id"].as<int>();
    e.exercise_name = r["exercise_name"].as<std::string>();
    e.sets = r["sets"].as<int>();
    e.reps = r["reps"].as<int>();
    e.weight_kg = r["weight_kg"].as<double>();
    return e;
}

// -------------------- CREATE --------------------
int WorkoutEntryDAL::create(const WorkoutEntry& e)
{
    auto conn = db_.connect();
    pqxx::work tx(conn);

    pqxx::row r = tx.exec(
        pqxx::zview{
            "INSERT INTO workout_entries "
            "(workout_id, exercise_name, sets, reps, weight_kg) "
            "VALUES ($1, $2, $3, $4, $5) "
            "RETURNING entry_id"
        },
        pqxx::params{
            e.workout_id,
            e.exercise_name,
            e.sets,
            e.reps,
            e.weight_kg
        }
    ).one_row();

    tx.commit();
    return r["entry_id"].as<int>();
}

// -------------------- READ ONE --------------------
std::optional<WorkoutEntry> WorkoutEntryDAL::getById(int id)
{
    auto conn = db_.connect();
    pqxx::work tx(conn);

    pqxx::result res = tx.exec(
        pqxx::zview{
            "SELECT entry_id, workout_id, exercise_name, sets, reps, weight_kg "
            "FROM workout_entries "
            "WHERE entry_id = $1"
        },
        pqxx::params{ id }
    );

    tx.commit();

    if (res.empty()) return std::nullopt;
    return workoutEntryFromRow(res[0]);
}

// -------------------- READ ALL --------------------
std::vector<WorkoutEntry> WorkoutEntryDAL::getAll()
{
    auto conn = db_.connect();
    pqxx::work tx(conn);

    pqxx::result res = tx.exec(
        pqxx::zview{
            "SELECT entry_id, workout_id, exercise_name, sets, reps, weight_kg "
            "FROM workout_entries "
            "ORDER BY entry_id"
        }
    );

    tx.commit();

    std::vector<WorkoutEntry> entries;
    entries.reserve(res.size());
    for (const auto& row : res)
        entries.push_back(workoutEntryFromRow(row));

    return entries;
}

// -------------------- READ BY WORKOUT --------------------
std::vector<WorkoutEntry> WorkoutEntryDAL::getByWorkoutId(int workout_id)
{
    auto conn = db_.connect();
    pqxx::work tx(conn);

    pqxx::result res = tx.exec(
        pqxx::zview{
            "SELECT entry_id, workout_id, exercise_name, sets, reps, weight_kg "
            "FROM workout_entries "
            "WHERE workout_id = $1 "
            "ORDER BY entry_id"
        },
        pqxx::params{ workout_id }
    );

    tx.commit();

    std::vector<WorkoutEntry> entries;
    entries.reserve(res.size());
    for (const auto& row : res)
        entries.push_back(workoutEntryFromRow(row));

    return entries;
}

// -------------------- UPDATE --------------------
bool WorkoutEntryDAL::update(const WorkoutEntry& e)
{
    auto conn = db_.connect();
    pqxx::work tx(conn);

    pqxx::result res = tx.exec(
        pqxx::zview{
            "UPDATE workout_entries "
            "SET exercise_name = $1, sets = $2, reps = $3, weight_kg = $4 "
            "WHERE entry_id = $5"
        },
        pqxx::params{
            e.exercise_name,
            e.sets,
            e.reps,
            e.weight_kg,
            e.entry_id
        }
    );

    tx.commit();
    return res.affected_rows() == 1;
}

// -------------------- DELETE --------------------
bool WorkoutEntryDAL::remove(int id)
{
    auto conn = db_.connect();
    pqxx::work tx(conn);

    pqxx::result res = tx.exec(
        pqxx::zview{ "DELETE FROM workout_entries WHERE entry_id = $1" },
        pqxx::params{ id }
    );

    tx.commit();
    return res.affected_rows() == 1;
}
