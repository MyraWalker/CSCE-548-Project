#pragma once
#include "../models/Workout.h"
#include "../models/WorkoutEntry.h"
#include <vector>

class WorkoutService
{
public:
    std::vector<Workout> GetAllWorkouts();
    bool CreateWorkout(const Workout& workout);

    std::vector<WorkoutEntry> GetEntriesForWorkout(int workoutId);
    bool CreateWorkoutEntry(const WorkoutEntry& entry);
};
