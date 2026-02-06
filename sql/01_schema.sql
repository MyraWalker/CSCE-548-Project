DROP TABLE IF EXISTS meal_items;
DROP TABLE IF EXISTS meals;
DROP TABLE IF EXISTS workout_entries;
DROP TABLE IF EXISTS workouts;
DROP TABLE IF EXISTS users;

CREATE TABLE users (
  user_id        SERIAL PRIMARY KEY,
  first_name     VARCHAR(50) NOT NULL,
  last_name      VARCHAR(50) NOT NULL,
  email          VARCHAR(120) NOT NULL UNIQUE,
  height_cm      NUMERIC(5,2) CHECK (height_cm BETWEEN 50 AND 260),
  created_at     TIMESTAMP NOT NULL DEFAULT NOW()
);

CREATE TABLE workouts (
  workout_id     SERIAL PRIMARY KEY,
  user_id        INT NOT NULL REFERENCES users(user_id) ON DELETE CASCADE,
  workout_date   DATE NOT NULL,
  duration_min   INT NOT NULL CHECK (duration_min BETWEEN 1 AND 600),
  notes          VARCHAR(255)
);

CREATE TABLE workout_entries (
  entry_id       SERIAL PRIMARY KEY,
  workout_id     INT NOT NULL REFERENCES workouts(workout_id) ON DELETE CASCADE,
  exercise_name  VARCHAR(80) NOT NULL,
  sets           INT NOT NULL CHECK (sets BETWEEN 1 AND 50),
  reps           INT NOT NULL CHECK (reps BETWEEN 1 AND 200),
  weight_kg      NUMERIC(6,2) NOT NULL CHECK (weight_kg BETWEEN 0 AND 500)
);

CREATE TABLE meals (
  meal_id        SERIAL PRIMARY KEY,
  user_id        INT NOT NULL REFERENCES users(user_id) ON DELETE CASCADE,
  meal_time      TIMESTAMP NOT NULL,
  meal_type      VARCHAR(20) NOT NULL CHECK (meal_type IN ('breakfast','lunch','dinner','snack')),
  notes          VARCHAR(255)
);

CREATE TABLE meal_items (
  item_id        SERIAL PRIMARY KEY,
  meal_id        INT NOT NULL REFERENCES meals(meal_id) ON DELETE CASCADE,
  food_name      VARCHAR(80) NOT NULL,
  calories       INT NOT NULL CHECK (calories BETWEEN 0 AND 5000),
  protein_g      NUMERIC(6,2) NOT NULL CHECK (protein_g BETWEEN 0 AND 500),
  carbs_g        NUMERIC(6,2) NOT NULL CHECK (carbs_g BETWEEN 0 AND 500),
  fat_g          NUMERIC(6,2) NOT NULL CHECK (fat_g BETWEEN 0 AND 500)
);

CREATE INDEX idx_workouts_user_id ON workouts(user_id);
CREATE INDEX idx_meals_user_id ON meals(user_id);
