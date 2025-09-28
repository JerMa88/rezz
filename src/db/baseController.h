#ifndef BASECONTROLLER_H
#define BASECONTROLLER_H

#include "database.h"
#include <string>
#include <vector>
#include <memory>

class BaseController {
protected:
    std::shared_ptr<DatabaseConnection> db;

public:
    BaseController();
    virtual ~BaseController() = default;

    virtual bool initializeDatabase();
    virtual std::string exportToJson() = 0;
    virtual std::string exportToCsv() = 0;
    virtual bool importFromJson(const std::string& jsonData) = 0;

protected:
    std::string escapeJsonString(const std::string& input) const;
    std::string escapeCsvField(const std::string& input) const;
    std::vector<std::string> splitCsvLine(const std::string& line) const;
    std::string getCurrentTimestamp() const;
    std::string formatDateForDb(const std::string& date) const;
};

#endif // BASECONTROLLER_H