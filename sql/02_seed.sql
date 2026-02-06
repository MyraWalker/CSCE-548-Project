INSERT INTO users (first_name, last_name, email, height_cm) VALUES
('Myra','Walker','myra1@example.com',165.10),
('Jordan','Lee','jordan@example.com',180.34),
('Casey','Nguyen','casey@example.com',172.72),
('Avery','Smith','avery@example.com',158.00),
('Riley','Johnson','riley@example.com',190.50);

INSERT INTO workouts (user_id, workout_date, duration_min, notes) VALUES
(1,'2026-01-20',45,'Upper body strength'),
(1,'2026-01-22',35,'Cardio + core'),
(1,'2026-01-24',55,'Lower body strength'),
(2,'2026-01-21',40,'Push day'),
(2,'2026-01-23',50,'Pull day'),
(3,'2026-01-20',30,'Light cardio'),
(3,'2026-01-25',60,'Full body'),
(4,'2026-01-22',45,'HIIT'),
(5,'2026-01-23',70,'Leg day'),
(5,'2026-01-26',35,'Recovery workout');

INSERT INTO workout_entries (workout_id, exercise_name, sets, reps, weight_kg) VALUES
(1,'Bench Press',4,8,50),(1,'Dumbbell Row',3,10,22.5),(1,'Shoulder Press',3,8,20),
(2,'Running',1,20,0),(2,'Plank',3,60,0),(2,'Bicycle Crunch',3,20,0),
(3,'Squat',4,6,70),(3,'Romanian Deadlift',3,8,60),(3,'Calf Raise',4,12,40),
(4,'Incline Press',4,8,45),(4,'Tricep Pushdown',3,12,25),(4,'Lateral Raise',3,12,7.5),
(5,'Lat Pulldown',4,10,50),(5,'Barbell Curl',3,10,25),(5,'Face Pull',3,12,20),
(6,'Cycling',1,30,0),(6,'Stretching',2,10,0),(6,'Walking',1,15,0),
(7,'Deadlift',4,5,90),(7,'Pull-up',3,6,0),(7,'Push-up',3,12,0),
(8,'Burpees',3,12,0),(8,'Jump Rope',1,10,0),(8,'Mountain Climbers',3,20,0),
(9,'Leg Press',4,10,140),(9,'Hamstring Curl',3,12,45),(9,'Lunge',3,10,20),
(10,'Yoga Flow',1,30,0),(10,'Foam Roll',1,10,0),(10,'Mobility Drills',2,10,0);

INSERT INTO meals (user_id, meal_time, meal_type, notes) VALUES
(1,'2026-01-20 08:10:00','breakfast','High protein'),
(1,'2026-01-20 12:30:00','lunch','Balanced meal'),
(1,'2026-01-20 19:00:00','dinner','Post-workout'),
(2,'2026-01-21 09:00:00','breakfast',''),
(2,'2026-01-21 13:00:00','lunch',''),
(3,'2026-01-22 10:30:00','snack',''),
(3,'2026-01-22 18:45:00','dinner',''),
(4,'2026-01-23 07:50:00','breakfast',''),
(5,'2026-01-23 12:10:00','lunch',''),
(5,'2026-01-23 20:15:00','dinner','');

INSERT INTO meal_items (meal_id, food_name, calories, protein_g, carbs_g, fat_g) VALUES
(1,'Greek Yogurt',140,17,10,4),(1,'Banana',105,1,27,0),(1,'Peanut Butter',190,7,8,16),
(2,'Chicken Breast',220,40,0,5),(2,'Rice',205,4,45,1),(2,'Broccoli',55,4,11,0),
(3,'Salmon',300,34,0,18),(3,'Sweet Potato',180,4,41,0),(3,'Salad',80,2,10,4),
(4,'Oatmeal',150,5,27,3),(4,'Milk',120,8,12,5),(4,'Blueberries',40,0,10,0),
(5,'Turkey Sandwich',350,25,35,12),(5,'Apple',95,0,25,0),(5,'Almonds',170,6,6,15),
(6,'Protein Bar',210,20,25,7),(6,'Coffee',5,0,1,0),(6,'Orange',62,1,15,0),
(7,'Pasta',320,12,60,4),(7,'Ground Beef',280,26,0,20),(7,'Tomato Sauce',70,2,14,1),
(8,'Eggs',140,12,1,10),(8,'Toast',120,4,22,2),(8,'Avocado',160,2,9,15),
(9,'Tuna',180,35,0,2),(9,'Crackers',130,3,20,4),(9,'Grapes',60,0,15,0),
(10,'Steak',350,30,0,25),(10,'Asparagus',30,3,5,0),(10,'Potatoes',200,4,44,0);
