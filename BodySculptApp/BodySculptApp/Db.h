#pragma once
#include <string>
#include <pqxx/pqxx>

class Db {
public:
    explicit Db(std::string connStr) : connStr_(std::move(connStr)) {}

    // Create a new connection when needed
    pqxx::connection connect() const {
        return pqxx::connection(connStr_);
    }

private:
    std::string connStr_;
};

