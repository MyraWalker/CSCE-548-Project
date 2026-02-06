// BodySculptApp.cpp (complete)
// Build: libpqxx 7+
// Note: This file includes try/catch so SQL/connection errors print instead of aborting.

#include <iostream>
#include <string>
#include <vector>
#include <pqxx/pqxx>

#include "Db.h"
#include "UserDAL.h"

static void printUsers(const std::vector<User>& users) {
    std::cout << "\n--- USERS ---\n";
    for (const auto& u : users) {
        std::cout << u.user_id << ": "
            << u.first_name << " " << u.last_name
            << " | " << u.email
            << " | height_cm=" << u.height_cm << "\n";
    }
}

int main() {
    // Use the "key=value" format (easiest on Windows)
    std::string connStr =
        "dbname=body_sculpt user=postgres password=postgresql host=localhost port=5432";

    Db db(connStr);
    UserDAL userDal(db);

    while (true) {
        std::cout << "\nBody Sculpt App (Phase 1)\n"
            << "1) List all users\n"
            << "2) Get user by id\n"
            << "3) Create user\n"
            << "4) Update user\n"
            << "5) Delete user\n"
            << "0) Exit\n"
            << "Choice: ";

        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(100000, '\n');
            std::cout << "Invalid input.\n";
            continue;
        }

        if (choice == 0) break;

        try {
            if (choice == 1) {
                auto users = userDal.getAll();
                printUsers(users);

            }
            else if (choice == 2) {
                int id;
                std::cout << "Enter user id: ";
                std::cin >> id;

                auto u = userDal.getById(id);
                if (!u) {
                    std::cout << "User not found.\n";
                }
                else {
                    std::cout << u->user_id << ": " << u->first_name << " " << u->last_name
                        << " | " << u->email
                        << " | height_cm=" << u->height_cm << "\n";
                }

            }
            else if (choice == 3) {
                User u{};
                std::cout << "First name: "; std::cin >> u.first_name;
                std::cout << "Last name: ";  std::cin >> u.last_name;
                std::cout << "Email: ";      std::cin >> u.email;
                std::cout << "Height (cm): "; std::cin >> u.height_cm;

                int newId = userDal.create(u);
                std::cout << "Created user_id = " << newId << "\n";

            }
            else if (choice == 4) {
                User u{};
                std::cout << "User id: ";     std::cin >> u.user_id;
                std::cout << "First name: ";  std::cin >> u.first_name;
                std::cout << "Last name: ";   std::cin >> u.last_name;
                std::cout << "Email: ";       std::cin >> u.email;
                std::cout << "Height (cm): "; std::cin >> u.height_cm;

                std::cout << (userDal.update(u) ? "Updated.\n" : "Update failed.\n");

            }
            else if (choice == 5) {
                int id;
                std::cout << "User id: ";
                std::cin >> id;

                std::cout << (userDal.remove(id) ? "Deleted.\n" : "Delete failed.\n");

            }
            else {
                std::cout << "Invalid choice.\n";
            }
        }
        catch (const pqxx::sql_error& e) {
            std::cerr << "\nSQL error: " << e.what()
                << "\nQuery was: " << e.query() << "\n";
        }
        catch (const std::exception& e) {
            std::cerr << "\nError: " << e.what() << "\n";
        }
    }

    return 0;
}
