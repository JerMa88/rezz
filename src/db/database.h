#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <memory>
#include <vector>
#include <map>
#include <libpq-fe.h>

class DatabaseConnection {
private:
    PGconn* connection;
    std::string connectionString;
    bool connected;

public:
    DatabaseConnection();
    DatabaseConnection(const std::string& host, const std::string& port,
                      const std::string& dbname, const std::string& user,
                      const std::string& password);
    ~DatabaseConnection();

    bool connect();
    bool connect(const std::string& host, const std::string& port,
                const std::string& dbname, const std::string& user,
                const std::string& password);
    void disconnect();
    bool isConnected() const;

    PGresult* executeQuery(const std::string& query);
    PGresult* executeParameterizedQuery(const std::string& query,
                                       const std::vector<std::string>& params);

    bool executeNonQuery(const std::string& query);
    bool executeParameterizedNonQuery(const std::string& query,
                                     const std::vector<std::string>& params);

    std::string escapeString(const std::string& input);
    std::string getLastError() const;

    bool beginTransaction();
    bool commitTransaction();
    bool rollbackTransaction();

    static std::shared_ptr<DatabaseConnection> getInstance();
    static void setInstance(std::shared_ptr<DatabaseConnection> instance);

private:
    static std::shared_ptr<DatabaseConnection> instance;
    void buildConnectionString(const std::string& host, const std::string& port,
                              const std::string& dbname, const std::string& user,
                              const std::string& password);
};

class DatabaseResult {
private:
    PGresult* result;
    int currentRow;
    int numRows;
    int numFields;

public:
    DatabaseResult(PGresult* result);
    ~DatabaseResult();

    bool isValid() const;
    int getRowCount() const;
    int getFieldCount() const;

    bool next();
    void reset();

    std::string getString(int fieldIndex) const;
    std::string getString(const std::string& fieldName) const;
    int getInt(int fieldIndex) const;
    int getInt(const std::string& fieldName) const;
    double getDouble(int fieldIndex) const;
    double getDouble(const std::string& fieldName) const;
    bool getBool(int fieldIndex) const;
    bool getBool(const std::string& fieldName) const;

    bool isNull(int fieldIndex) const;
    bool isNull(const std::string& fieldName) const;

    std::string getFieldName(int fieldIndex) const;
    int getFieldIndex(const std::string& fieldName) const;

    std::vector<std::map<std::string, std::string>> getAllRows() const;
    std::map<std::string, std::string> getCurrentRow() const;
};

#endif // DATABASE_H