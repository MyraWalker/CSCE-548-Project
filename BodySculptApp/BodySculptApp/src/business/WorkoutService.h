#pragma once
#include "../models/Workout.h"
#include "../models/WorkoutEntry.h"
#include <vector>
#include <optional>

class WorkoutService
{
public:
    std::vector<Workout> GetAllWorkouts();
    std::optional<Workout> GetWorkoutById(int workoutId);
    bool CreateWorkout(const Workout& workout);
    bool UpdateWorkout(const Workout& workout);
    bool DeleteWorkout(int workoutId);

    std::vector<WorkoutEntry> GetEntriesForWorkout(int workoutId);
    bool CreateWorkoutEntry(const WorkoutEntry& entry);
};