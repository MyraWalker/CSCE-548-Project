const API_BASE = "";

// ----------------------------
// TAB SWITCHING
// ----------------------------
document.querySelectorAll(".tab-button").forEach(button => {
    button.addEventListener("click", () => {
        document.querySelectorAll(".tab-button").forEach(btn => btn.classList.remove("active"));
        document.querySelectorAll(".tab-content").forEach(tab => tab.classList.remove("active"));

        button.classList.add("active");
        document.getElementById(button.dataset.tab).classList.add("active");
    });
});

// ----------------------------
// COMMON HELPERS
// ----------------------------
function showResult(elementId, data) {
    document.getElementById(elementId).textContent =
        typeof data === "string" ? data : JSON.stringify(data, null, 2);
}

async function handleResponse(response) {
    const text = await response.text();

    try {
        const json = text ? JSON.parse(text) : {};
        if (!response.ok) {
            throw new Error(json.message || `HTTP ${response.status}`);
        }
        return json;
    } catch (err) {
        if (!response.ok) {
            throw new Error(text || `HTTP ${response.status}`);
        }
        return text;
    }
}

async function apiRequest(url, options = {}) {
    const response = await fetch(url, {
        headers: {
            "Content-Type": "application/json"
        },
        ...options
    });

    return handleResponse(response);
}

function getValue(id) {
    return document.getElementById(id).value.trim();
}

function requireId(idValue, fieldName = "ID") {
    if (!idValue) {
        throw new Error(`${fieldName} is required.`);
    }
}

function parseNumberOrNull(value) {
    return value === "" ? null : Number(value);
}

// ----------------------------
// USERS
// ----------------------------
function clearUserForm() {
    document.getElementById("user_id").value = "";
    document.getElementById("user_first_name").value = "";
    document.getElementById("user_last_name").value = "";
    document.getElementById("user_email").value = "";
    document.getElementById("user_height_cm").value = "";
}

function getUserPayload() {
    return {
        first_name: getValue("user_first_name"),
        last_name: getValue("user_last_name"),
        email: getValue("user_email"),
        height_cm: parseNumberOrNull(getValue("user_height_cm"))
    };
}

async function getAllUsers() {
    try {
        const data = await apiRequest(`${API_BASE}/users`);
        showResult("users_result", data);
    } catch (err) {
        showResult("users_result", err.message);
    }
}

async function getUserById() {
    try {
        const id = getValue("user_id_search");
        requireId(id, "User ID");

        const data = await apiRequest(`${API_BASE}/users/${id}`);
        showResult("users_result", data);

        if (data && !Array.isArray(data)) {
            document.getElementById("user_id").value = data.user_id ?? "";
            document.getElementById("user_first_name").value = data.first_name ?? "";
            document.getElementById("user_last_name").value = data.last_name ?? "";
            document.getElementById("user_email").value = data.email ?? "";
            document.getElementById("user_height_cm").value = data.height_cm ?? "";
        }
    } catch (err) {
        showResult("users_result", err.message);
    }
}

async function addUser() {
    try {
        const payload = getUserPayload();

        const data = await apiRequest(`${API_BASE}/users`, {
            method: "POST",
            body: JSON.stringify(payload)
        });

        showResult("users_result", data);
        await getAllUsers();
    } catch (err) {
        showResult("users_result", err.message);
    }
}

async function updateUser() {
    try {
        const id = getValue("user_id");
        requireId(id, "User ID");

        const payload = getUserPayload();

        const data = await apiRequest(`${API_BASE}/users/${id}`, {
            method: "PUT",
            body: JSON.stringify(payload)
        });

        showResult("users_result", data);
        await getAllUsers();
    } catch (err) {
        showResult("users_result", err.message);
    }
}

async function deleteUser() {
    try {
        const id = getValue("user_id") || getValue("user_id_search");
        requireId(id, "User ID");

        const data = await apiRequest(`${API_BASE}/users/${id}`, {
            method: "DELETE"
        });

        showResult("users_result", data);
        clearUserForm();
        await getAllUsers();
    } catch (err) {
        showResult("users_result", err.message);
    }
}

// ----------------------------
// WORKOUTS
// ----------------------------
function clearWorkoutForm() {
    document.getElementById("workout_id").value = "";
    document.getElementById("workout_user_id").value = "";
    document.getElementById("workout_date").value = "";
    document.getElementById("workout_duration_min").value = "";
    document.getElementById("workout_notes").value = "";
}

function getWorkoutPayload() {
    return {
        user_id: parseNumberOrNull(getValue("workout_user_id")),
        workout_date: getValue("workout_date"),
        duration_min: parseNumberOrNull(getValue("workout_duration_min")),
        notes: getValue("workout_notes")
    };
}

async function getAllWorkouts() {
    try {
        const data = await apiRequest(`${API_BASE}/workouts`);
        showResult("workouts_result", data);
    } catch (err) {
        showResult("workouts_result", err.message);
    }
}

async function getWorkoutById() {
    try {
        const id = getValue("workout_id_search");
        requireId(id, "Workout ID");

        const data = await apiRequest(`${API_BASE}/workouts/${id}`);
        showResult("workouts_result", data);

        if (data && !Array.isArray(data)) {
            document.getElementById("workout_id").value = data.workout_id ?? "";
            document.getElementById("workout_user_id").value = data.user_id ?? "";
            document.getElementById("workout_date").value = data.workout_date ?? "";
            document.getElementById("workout_duration_min").value = data.duration_min ?? "";
            document.getElementById("workout_notes").value = data.notes ?? "";
        }
    } catch (err) {
        showResult("workouts_result", err.message);
    }
}

async function addWorkout() {
    try {
        const payload = getWorkoutPayload();

        const data = await apiRequest(`${API_BASE}/workouts`, {
            method: "POST",
            body: JSON.stringify(payload)
        });

        showResult("workouts_result", data);
        await getAllWorkouts();
    } catch (err) {
        showResult("workouts_result", err.message);
    }
}

async function updateWorkout() {
    try {
        const id = getValue("workout_id");
        requireId(id, "Workout ID");

        const payload = getWorkoutPayload();

        const data = await apiRequest(`${API_BASE}/workouts/${id}`, {
            method: "PUT",
            body: JSON.stringify(payload)
        });

        showResult("workouts_result", data);
        await getAllWorkouts();
    } catch (err) {
        showResult("workouts_result", err.message);
    }
}

async function deleteWorkout() {
    try {
        const id = getValue("workout_id") || getValue("workout_id_search");
        requireId(id, "Workout ID");

        const data = await apiRequest(`${API_BASE}/workouts/${id}`, {
            method: "DELETE"
        });

        showResult("workouts_result", data);
        clearWorkoutForm();
        await getAllWorkouts();
    } catch (err) {
        showResult("workouts_result", err.message);
    }
}

// ----------------------------
// WORKOUT ENTRIES
// ----------------------------
// Your current backend supports:
//   GET    /workouts/{workoutId}/entries
//   POST   /workouts/{workoutId}/entries
//
// It does NOT yet support:
//   GET    /workout_entries
//   GET    /workout_entries/{id}
//   PUT    /workout_entries/{id}
//   DELETE /workout_entries/{id}
// ----------------------------
function clearWorkoutEntryForm() {
    document.getElementById("workout_entry_id").value = "";
    document.getElementById("workout_entry_workout_id").value = "";
    document.getElementById("workout_entry_exercise_name").value = "";
    document.getElementById("workout_entry_sets").value = "";
    document.getElementById("workout_entry_reps").value = "";
    document.getElementById("workout_entry_weight_kg").value = "";
}

function getWorkoutEntryPayload() {
    return {
        exercise_name: getValue("workout_entry_exercise_name"),
        sets: parseNumberOrNull(getValue("workout_entry_sets")),
        reps: parseNumberOrNull(getValue("workout_entry_reps")),
        weight_kg: parseNumberOrNull(getValue("workout_entry_weight_kg"))
    };
}

async function getAllWorkoutEntries() {
    try {
        const workoutId = getValue("workout_entry_workout_id") || getValue("workout_id") || getValue("workout_id_search");
        requireId(workoutId, "Workout ID for entries");

        const data = await apiRequest(`${API_BASE}/workouts/${workoutId}/entries`);
        showResult("workout_entries_result", data);
    } catch (err) {
        showResult("workout_entries_result", err.message);
    }
}

async function getWorkoutEntryById() {
    showResult(
        "workout_entries_result",
        "This backend does not currently support GET /workout_entries/{id}. Use Get All with a Workout ID to view entries for that workout."
    );
}

async function addWorkoutEntry() {
    try {
        const workoutId = getValue("workout_entry_workout_id");
        requireId(workoutId, "Workout ID");

        const payload = getWorkoutEntryPayload();

        const data = await apiRequest(`${API_BASE}/workouts/${workoutId}/entries`, {
            method: "POST",
            body: JSON.stringify(payload)
        });

        showResult("workout_entries_result", data);
        await getAllWorkoutEntries();
    } catch (err) {
        showResult("workout_entries_result", err.message);
    }
}

async function updateWorkoutEntry() {
    showResult(
        "workout_entries_result",
        "This backend does not currently support PUT /workout_entries/{id}. Update route must be added in C++ first."
    );
}

async function deleteWorkoutEntry() {
    showResult(
        "workout_entries_result",
        "This backend does not currently support DELETE /workout_entries/{id}. Delete route must be added in C++ first."
    );
}

// ----------------------------
// MEALS
// ----------------------------
function clearMealForm() {
    document.getElementById("meal_id").value = "";
    document.getElementById("meal_user_id").value = "";
    document.getElementById("meal_time").value = "";
    document.getElementById("meal_type").value = "";
    document.getElementById("meal_notes").value = "";
}

function getMealPayload() {
    return {
        user_id: parseNumberOrNull(getValue("meal_user_id")),
        meal_time: getValue("meal_time"),
        meal_type: getValue("meal_type"),
        notes: getValue("meal_notes")
    };
}

async function getAllMeals() {
    try {
        const data = await apiRequest(`${API_BASE}/meals`);
        showResult("meals_result", data);
    } catch (err) {
        showResult("meals_result", err.message);
    }
}

async function getMealById() {
    try {
        const id = getValue("meal_id_search");
        requireId(id, "Meal ID");

        const data = await apiRequest(`${API_BASE}/meals/${id}`);
        showResult("meals_result", data);

        if (data && !Array.isArray(data)) {
            document.getElementById("meal_id").value = data.meal_id ?? "";
            document.getElementById("meal_user_id").value = data.user_id ?? "";
            document.getElementById("meal_time").value = data.meal_time ?? "";
            document.getElementById("meal_type").value = data.meal_type ?? "";
            document.getElementById("meal_notes").value = data.notes ?? "";
        }
    } catch (err) {
        showResult("meals_result", err.message);
    }
}

async function addMeal() {
    try {
        const payload = getMealPayload();

        const data = await apiRequest(`${API_BASE}/meals`, {
            method: "POST",
            body: JSON.stringify(payload)
        });

        showResult("meals_result", data);
        await getAllMeals();
    } catch (err) {
        showResult("meals_result", err.message);
    }
}

async function updateMeal() {
    try {
        const id = getValue("meal_id");
        requireId(id, "Meal ID");

        const payload = getMealPayload();

        const data = await apiRequest(`${API_BASE}/meals/${id}`, {
            method: "PUT",
            body: JSON.stringify(payload)
        });

        showResult("meals_result", data);
        await getAllMeals();
    } catch (err) {
        showResult("meals_result", err.message);
    }
}

async function deleteMeal() {
    try {
        const id = getValue("meal_id") || getValue("meal_id_search");
        requireId(id, "Meal ID");

        const data = await apiRequest(`${API_BASE}/meals/${id}`, {
            method: "DELETE"
        });

        showResult("meals_result", data);
        clearMealForm();
        await getAllMeals();
    } catch (err) {
        showResult("meals_result", err.message);
    }
}

// ----------------------------
// MEAL ITEMS
// ----------------------------
// Your current backend supports:
//   GET    /meals/{mealId}/items
//   GET    /meal_items/{id}
//   POST   /meals/{mealId}/items
//   PUT    /meal_items/{id}
//   DELETE /meal_items/{id}
// ----------------------------
function clearMealItemForm() {
    document.getElementById("meal_item_id").value = "";
    document.getElementById("meal_item_meal_id").value = "";
    document.getElementById("meal_item_food_name").value = "";
    document.getElementById("meal_item_calories").value = "";
    document.getElementById("meal_item_protein_g").value = "";
    document.getElementById("meal_item_carbs_g").value = "";
    document.getElementById("meal_item_fat_g").value = "";
}

function getMealItemPayload(includeMealId = true) {
    const payload = {
        food_name: getValue("meal_item_food_name"),
        calories: parseNumberOrNull(getValue("meal_item_calories")),
        protein_g: parseNumberOrNull(getValue("meal_item_protein_g")),
        carbs_g: parseNumberOrNull(getValue("meal_item_carbs_g")),
        fat_g: parseNumberOrNull(getValue("meal_item_fat_g"))
    };

    if (includeMealId) {
        payload.meal_id = parseNumberOrNull(getValue("meal_item_meal_id"));
    }

    return payload;
}

async function getAllMealItems() {
    try {
        const mealId = getValue("meal_item_meal_id") || getValue("meal_id") || getValue("meal_id_search");
        requireId(mealId, "Meal ID for items");

        const data = await apiRequest(`${API_BASE}/meals/${mealId}/items`);
        showResult("meal_items_result", data);
    } catch (err) {
        showResult("meal_items_result", err.message);
    }
}

async function getMealItemById() {
    try {
        const id = getValue("meal_item_id_search");
        requireId(id, "Meal Item ID");

        const data = await apiRequest(`${API_BASE}/meal_items/${id}`);
        showResult("meal_items_result", data);

        if (data && !Array.isArray(data)) {
            document.getElementById("meal_item_id").value = data.item_id ?? "";
            document.getElementById("meal_item_meal_id").value = data.meal_id ?? "";
            document.getElementById("meal_item_food_name").value = data.food_name ?? "";
            document.getElementById("meal_item_calories").value = data.calories ?? "";
            document.getElementById("meal_item_protein_g").value = data.protein_g ?? "";
            document.getElementById("meal_item_carbs_g").value = data.carbs_g ?? "";
            document.getElementById("meal_item_fat_g").value = data.fat_g ?? "";
        }
    } catch (err) {
        showResult("meal_items_result", err.message);
    }
}

async function addMealItem() {
    try {
        const mealId = getValue("meal_item_meal_id");
        requireId(mealId, "Meal ID");

        const payload = getMealItemPayload(false);

        const data = await apiRequest(`${API_BASE}/meals/${mealId}/items`, {
            method: "POST",
            body: JSON.stringify(payload)
        });

        showResult("meal_items_result", data);
        await getAllMealItems();
    } catch (err) {
        showResult("meal_items_result", err.message);
    }
}

async function updateMealItem() {
    try {
        const id = getValue("meal_item_id");
        requireId(id, "Meal Item ID");

        const payload = getMealItemPayload(true);

        const data = await apiRequest(`${API_BASE}/meal_items/${id}`, {
            method: "PUT",
            body: JSON.stringify(payload)
        });

        showResult("meal_items_result", data);
        await getMealItemById();
    } catch (err) {
        showResult("meal_items_result", err.message);
    }
}

async function deleteMealItem() {
    try {
        const id = getValue("meal_item_id") || getValue("meal_item_id_search");
        requireId(id, "Meal Item ID");

        const data = await apiRequest(`${API_BASE}/meal_items/${id}`, {
            method: "DELETE"
        });

        showResult("meal_items_result", data);
        clearMealItemForm();
    } catch (err) {
        showResult("meal_items_result", err.message);
    }
}