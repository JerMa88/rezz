#include "database.h"
#include <iostream>
#include <sstream>
#include <algorithm>

std::shared_ptr<DatabaseConnection> DatabaseConnection::instance = nullptr;

DatabaseConnection::DatabaseConnection()
    : connection(nullptr), connected(false) {}

DatabaseConnection::DatabaseConnection(const std::string& host, const std::string& port,
                                     const std::string& dbname, const std::string& user,
                                     const std::string& password)
    : connection(nullptr), connected(false) {
    buildConnectionString(host, port, dbname, user, password);
}

DatabaseConnection::~DatabaseConnection() {
    disconnect();
}

void DatabaseConnection::buildConnectionString(const std::string& host, const std::string& port,
                                             const std::string& dbname, const std::string& user,
                                             const std::string& password) {
    std::ostringstream oss;
    oss << "host=" << host << " port=" << port << " dbname=" << dbname
        << " user=" << user << " password=" << password;
    connectionString = oss.str();
}

bool DatabaseConnection::connect() {
    if (connectionString.empty()) {
        connectionString = "host=localhost port=5432 dbname=rezz_db user=postgres password=postgres";
    }

    connection = PQconnectdb(connectionString.c_str());

    if (PQstatus(connection) != CONNECTION_OK) {
        std::cerr << "Database connection failed: " << PQerrorMessage(connection) << std::endl;
        PQfinish(connection);
        connection = nullptr;
        connected = false;
        return false;
    }

    connected = true;
    return true;
}

bool DatabaseConnection::connect(const std::string& host, const std::string& port,
                               const std::string& dbname, const std::string& user,
                               const std::string& password) {
    buildConnectionString(host, port, dbname, user, password);
    return connect();
}

void DatabaseConnection::disconnect() {
    if (connection) {
        PQfinish(connection);
        connection = nullptr;
        connected = false;
    }
}

bool DatabaseConnection::isConnected() const {
    return connected && connection && PQstatus(connection) == CONNECTION_OK;
}

PGresult* DatabaseConnection::executeQuery(const std::string& query) {
    if (!isConnected()) {
        std::cerr << "Database not connected" << std::endl;
        return nullptr;
    }

    PGresult* result = PQexec(connection, query.c_str());

    if (PQresultStatus(result) != PGRES_TUPLES_OK && PQresultStatus(result) != PGRES_COMMAND_OK) {
        std::cerr << "Query execution failed: " << PQerrorMessage(connection) << std::endl;
        PQclear(result);
        return nullptr;
    }

    return result;
}

PGresult* DatabaseConnection::executeParameterizedQuery(const std::string& query,
                                                       const std::vector<std::string>& params) {
    if (!isConnected()) {
        std::cerr << "Database not connected" << std::endl;
        return nullptr;
    }

    std::vector<const char*> paramValues;
    for (const auto& param : params) {
        paramValues.push_back(param.c_str());
    }

    PGresult* result = PQexecParams(connection, query.c_str(), params.size(),
                                   nullptr, paramValues.data(), nullptr, nullptr, 0);

    if (PQresultStatus(result) != PGRES_TUPLES_OK && PQresultStatus(result) != PGRES_COMMAND_OK) {
        std::cerr << "Parameterized query execution failed: " << PQerrorMessage(connection) << std::endl;
        PQclear(result);
        return nullptr;
    }

    return result;
}

bool DatabaseConnection::executeNonQuery(const std::string& query) {
    PGresult* result = executeQuery(query);
    if (result) {
        PQclear(result);
        return true;
    }
    return false;
}

bool DatabaseConnection::executeParameterizedNonQuery(const std::string& query,
                                                     const std::vector<std::string>& params) {
    PGresult* result = executeParameterizedQuery(query, params);
    if (result) {
        PQclear(result);
        return true;
    }
    return false;
}

std::string DatabaseConnection::escapeString(const std::string& input) {
    if (!isConnected()) {
        return input;
    }

    char* escaped = new char[input.length() * 2 + 1];
    PQescapeStringConn(connection, escaped, input.c_str(), input.length(), nullptr);
    std::string result(escaped);
    delete[] escaped;
    return result;
}

std::string DatabaseConnection::getLastError() const {
    if (connection) {
        return std::string(PQerrorMessage(connection));
    }
    return "No connection";
}

bool DatabaseConnection::beginTransaction() {
    return executeNonQuery("BEGIN");
}

bool DatabaseConnection::commitTransaction() {
    return executeNonQuery("COMMIT");
}

bool DatabaseConnection::rollbackTransaction() {
    return executeNonQuery("ROLLBACK");
}

std::shared_ptr<DatabaseConnection> DatabaseConnection::getInstance() {
    if (!instance) {
        instance = std::make_shared<DatabaseConnection>();
    }
    return instance;
}

void DatabaseConnection::setInstance(std::shared_ptr<DatabaseConnection> instance) {
    DatabaseConnection::instance = instance;
}

DatabaseResult::DatabaseResult(PGresult* result)
    : result(result), currentRow(-1), numRows(0), numFields(0) {
    if (result) {
        numRows = PQntuples(result);
        numFields = PQnfields(result);
    }
}

DatabaseResult::~DatabaseResult() {
    if (result) {
        PQclear(result);
    }
}

bool DatabaseResult::isValid() const {
    return result != nullptr;
}

int DatabaseResult::getRowCount() const {
    return numRows;
}

int DatabaseResult::getFieldCount() const {
    return numFields;
}

bool DatabaseResult::next() {
    if (!result || currentRow >= numRows - 1) {
        return false;
    }
    currentRow++;
    return true;
}

void DatabaseResult::reset() {
    currentRow = -1;
}

std::string DatabaseResult::getString(int fieldIndex) const {
    if (!result || currentRow < 0 || currentRow >= numRows || fieldIndex < 0 || fieldIndex >= numFields) {
        return "";
    }

    if (PQgetisnull(result, currentRow, fieldIndex)) {
        return "";
    }

    return std::string(PQgetvalue(result, currentRow, fieldIndex));
}

std::string DatabaseResult::getString(const std::string& fieldName) const {
    int fieldIndex = getFieldIndex(fieldName);
    return getString(fieldIndex);
}

int DatabaseResult::getInt(int fieldIndex) const {
    std::string value = getString(fieldIndex);
    if (value.empty()) {
        return 0;
    }
    try {
        return std::stoi(value);
    } catch (const std::exception&) {
        return 0;
    }
}

int DatabaseResult::getInt(const std::string& fieldName) const {
    int fieldIndex = getFieldIndex(fieldName);
    return getInt(fieldIndex);
}

double DatabaseResult::getDouble(int fieldIndex) const {
    std::string value = getString(fieldIndex);
    if (value.empty()) {
        return 0.0;
    }
    try {
        return std::stod(value);
    } catch (const std::exception&) {
        return 0.0;
    }
}

double DatabaseResult::getDouble(const std::string& fieldName) const {
    int fieldIndex = getFieldIndex(fieldName);
    return getDouble(fieldIndex);
}

bool DatabaseResult::getBool(int fieldIndex) const {
    std::string value = getString(fieldIndex);
    return value == "t" || value == "true" || value == "1";
}

bool DatabaseResult::getBool(const std::string& fieldName) const {
    int fieldIndex = getFieldIndex(fieldName);
    return getBool(fieldIndex);
}

bool DatabaseResult::isNull(int fieldIndex) const {
    if (!result || currentRow < 0 || currentRow >= numRows || fieldIndex < 0 || fieldIndex >= numFields) {
        return true;
    }
    return PQgetisnull(result, currentRow, fieldIndex);
}

bool DatabaseResult::isNull(const std::string& fieldName) const {
    int fieldIndex = getFieldIndex(fieldName);
    return isNull(fieldIndex);
}

std::string DatabaseResult::getFieldName(int fieldIndex) const {
    if (!result || fieldIndex < 0 || fieldIndex >= numFields) {
        return "";
    }
    return std::string(PQfname(result, fieldIndex));
}

int DatabaseResult::getFieldIndex(const std::string& fieldName) const {
    if (!result) {
        return -1;
    }

    for (int i = 0; i < numFields; i++) {
        if (getFieldName(i) == fieldName) {
            return i;
        }
    }
    return -1;
}

std::vector<std::map<std::string, std::string>> DatabaseResult::getAllRows() const {
    std::vector<std::map<std::string, std::string>> rows;

    if (!result) {
        return rows;
    }

    for (int row = 0; row < numRows; row++) {
        std::map<std::string, std::string> rowData;
        for (int field = 0; field < numFields; field++) {
            std::string fieldName = getFieldName(field);
            std::string value = "";
            if (!PQgetisnull(result, row, field)) {
                value = std::string(PQgetvalue(result, row, field));
            }
            rowData[fieldName] = value;
        }
        rows.push_back(rowData);
    }

    return rows;
}

std::map<std::string, std::string> DatabaseResult::getCurrentRow() const {
    std::map<std::string, std::string> rowData;

    if (!result || currentRow < 0 || currentRow >= numRows) {
        return rowData;
    }

    for (int field = 0; field < numFields; field++) {
        std::string fieldName = getFieldName(field);
        std::string value = "";
        if (!PQgetisnull(result, currentRow, field)) {
            value = std::string(PQgetvalue(result, currentRow, field));
        }
        rowData[fieldName] = value;
    }

    return rowData;
}