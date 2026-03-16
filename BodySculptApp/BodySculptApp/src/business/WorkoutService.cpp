#include "WorkoutService.h"
#include "../dal/WorkoutDAL.h"
#include "../dal/WorkoutEntryDAL.h"
#include "../db/Db.h"
#include "../utils/Logger.h"

std::vector<Workout> WorkoutService::GetAllWorkouts()
{
    Logger::Info("Fetching all workouts");

    // PostgreSQL connection string
    Db db("host=localhost port=5432 dbname=body_sculpt user=postgres password=postgresql");
    WorkoutDAL dal(db);

    return dal.getAll();
}

bool WorkoutService::CreateWorkout(const Workout& workout)
{
    if (workout.user_id <= 0)
    {
        Logger::Warn("Workout creation failed: invalid user_id");
        return false;
    }

    if (workout.workout_date.empty())
    {
        Logger::Warn("Workout creation failed: missing workout_date");
        return false;
    }

    if (workout.duration_min <= 0)
    {
        Logger::Warn("Workout creation failed: invalid duration");
        return false;
    }

    Logger::Info("Creating workout");

    // PostgreSQL connection string
    Db db("host=localhost port=5432 dbname=body_sculpt user=postgres password=postgresql");
    WorkoutDAL dal(db);

    int newId = dal.create(workout);
    return newId > 0;
}

std::vector<WorkoutEntry> WorkoutService::GetEntriesForWorkout(int workoutId)
{
    if (workoutId <= 0)
    {
        Logger::Warn("GetEntriesForWorkout failed: invalid workoutId");
        return {};
    }

    Logger::Info("Fetching workout entries");

    // PostgreSQL connection string
    Db db("host=localhost port=5432 dbname=body_sculpt user=postgres password=postgresql");
    WorkoutEntryDAL dal(db);

    return dal.getByWorkoutId(workoutId);
}

bool WorkoutService::CreateWorkoutEntry(const WorkoutEntry& entry)
{
    if (entry.workout_id <= 0)
    {
        Logger::Warn("Workout entry creation failed: invalid workout_id");
        return false;
    }

    if (entry.exercise_name.empty())
    {
        Logger::Warn("Workout entry creation failed: missing exercise_name");
        return false;
    }

    if (entry.sets < 0)
    {
        Logger::Warn("Workout entry creation failed: sets cannot be negative");
        return false;
    }

    if (entry.reps < 0)
    {
        Logger::Warn("Workout entry creation failed: reps cannot be negative");
        return false;
    }

    if (entry.weight_kg < 0)
    {
        Logger::Warn("Workout entry creation failed: weight cannot be negative");
        return false;
    }

    Logger::Info("Creating workout entry");

    // PostgreSQL connection string
    Db db("host=localhost port=5432 dbname=body_sculpt user=postgres password=postgresql");
    WorkoutEntryDAL dal(db);

    int newId = dal.create(entry);
    return newId > 0;
}