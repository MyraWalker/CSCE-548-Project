#include "WorkoutService.h"
#include "../dal/WorkoutDAL.h"
#include "../dal/WorkoutEntryDAL.h"
#include "../db/Db.h"
#include "../utils/Logger.h"
#include <optional>

std::vector<Workout> WorkoutService::GetAllWorkouts()
{
    Logger::Info("Fetching all workouts");

    Db db("host=localhost port=5432 dbname=body_sculpt user=postgres password=postgresql");
    WorkoutDAL dal(db);

    return dal.getAll();
}

std::optional<Workout> WorkoutService::GetWorkoutById(int workoutId)
{
    if (workoutId <= 0)
    {
        Logger::Warn("GetWorkoutById failed: invalid workoutId");
        return std::nullopt;
    }

    Logger::Info("Fetching workout by id");

    Db db("host=localhost port=5432 dbname=body_sculpt user=postgres password=postgresql");
    WorkoutDAL dal(db);

    return dal.getById(workoutId);
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

    Db db("host=localhost port=5432 dbname=body_sculpt user=postgres password=postgresql");
    WorkoutDAL dal(db);

    int newId = dal.create(workout);
    return newId > 0;
}

bool WorkoutService::UpdateWorkout(const Workout& workout)
{
    if (workout.workout_id <= 0)
    {
        Logger::Warn("Workout update failed: invalid workout_id");
        return false;
    }

    if (workout.user_id <= 0)
    {
        Logger::Warn("Workout update failed: invalid user_id");
        return false;
    }

    if (workout.workout_date.empty())
    {
        Logger::Warn("Workout update failed: missing workout_date");
        return false;
    }

    if (workout.duration_min <= 0)
    {
        Logger::Warn("Workout update failed: invalid duration");
        return false;
    }

    Logger::Info("Updating workout");

    Db db("host=localhost port=5432 dbname=body_sculpt user=postgres password=postgresql");
    WorkoutDAL dal(db);

    return dal.update(workout);
}

bool WorkoutService::DeleteWorkout(int workoutId)
{
    if (workoutId <= 0)
    {
        Logger::Warn("Workout delete failed: invalid workoutId");
        return false;
    }

    Logger::Info("Deleting workout");

    Db db("host=localhost port=5432 dbname=body_sculpt user=postgres password=postgresql");
    WorkoutDAL dal(db);

    return dal.remove(workoutId);
}

std::vector<WorkoutEntry> WorkoutService::GetEntriesForWorkout(int workoutId)
{
    if (workoutId <= 0)
    {
        Logger::Warn("GetEntriesForWorkout failed: invalid workoutId");
        return {};
    }

    Logger::Info("Fetching workout entries");

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

    Db db("host=localhost port=5432 dbname=body_sculpt user=postgres password=postgresql");
    WorkoutEntryDAL dal(db);

    int newId = dal.create(entry);
    return newId > 0;
}