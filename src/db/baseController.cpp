#include "baseController.h"
#include <iostream>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <algorithm>

BaseController::BaseController() {
    db = DatabaseConnection::getInstance();
}

bool BaseController::initializeDatabase() {
    if (!db->isConnected()) {
        if (!db->connect()) {
            std::cerr << "Failed to connect to database" << std::endl;
            return false;
        }
    }
    return true;
}

std::string BaseController::escapeJsonString(const std::string& input) const {
    std::string escaped = input;

    std::replace(escaped.begin(), escaped.end(), '\\', '/');

    size_t pos = 0;
    while ((pos = escaped.find('"', pos)) != std::string::npos) {
        escaped.replace(pos, 1, "\\\"");
        pos += 2;
    }

    pos = 0;
    while ((pos = escaped.find('\n', pos)) != std::string::npos) {
        escaped.replace(pos, 1, "\\n");
        pos += 2;
    }

    pos = 0;
    while ((pos = escaped.find('\r', pos)) != std::string::npos) {
        escaped.replace(pos, 1, "\\r");
        pos += 2;
    }

    pos = 0;
    while ((pos = escaped.find('\t', pos)) != std::string::npos) {
        escaped.replace(pos, 1, "\\t");
        pos += 2;
    }

    return escaped;
}

std::string BaseController::escapeCsvField(const std::string& input) const {
    std::string escaped = input;

    if (escaped.find(',') != std::string::npos ||
        escaped.find('"') != std::string::npos ||
        escaped.find('\n') != std::string::npos ||
        escaped.find('\r') != std::string::npos) {

        size_t pos = 0;
        while ((pos = escaped.find('"', pos)) != std::string::npos) {
            escaped.replace(pos, 1, "\"\"");
            pos += 2;
        }

        escaped = "\"" + escaped + "\"";
    }

    return escaped;
}

std::vector<std::string> BaseController::splitCsvLine(const std::string& line) const {
    std::vector<std::string> fields;
    std::string field;
    bool inQuotes = false;

    for (size_t i = 0; i < line.length(); i++) {
        char c = line[i];

        if (c == '"') {
            if (inQuotes && i + 1 < line.length() && line[i + 1] == '"') {
                field += '"';
                i++;
            } else {
                inQuotes = !inQuotes;
            }
        } else if (c == ',' && !inQuotes) {
            fields.push_back(field);
            field.clear();
        } else {
            field += c;
        }
    }

    fields.push_back(field);
    return fields;
}

std::string BaseController::getCurrentTimestamp() const {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

std::string BaseController::formatDateForDb(const std::string& date) const {
    if (date.empty()) {
        return "NULL";
    }
    return "'" + date + "'";
}