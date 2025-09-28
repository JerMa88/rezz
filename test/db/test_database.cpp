#include <iostream>
#include <cassert>
#include <memory>
#include <vector>
#include "../../src/db/database.h"

class DatabaseTest {
private:
    std::shared_ptr<DatabaseConnection> db;

public:
    DatabaseTest() {
        db = DatabaseConnection::getInstance();
    }

    void testDatabaseConnection() {
        std::cout << "Testing database connection..." << std::endl;

        bool connected = db->connect("localhost", "5432", "rezz_test_db", "postgres", "postgres");
        if (!connected) {
            std::cout << "Warning: Could not connect to test database. Trying default connection..." << std::endl;
            connected = db->connect();
        }

        assert(connected);
        assert(db->isConnected());

        std::cout << "✓ Database connection tests passed" << std::endl;
    }

    void testSimpleQuery() {
        std::cout << "Testing simple query execution..." << std::endl;

        PGresult* result = db->executeQuery("SELECT 1 as test_value, 'hello' as test_string");
        assert(result != nullptr);

        DatabaseResult dbResult(result);
        assert(dbResult.isValid());
        assert(dbResult.getRowCount() == 1);
        assert(dbResult.getFieldCount() == 2);

        assert(dbResult.next());
        assert(dbResult.getInt("test_value") == 1);
        assert(dbResult.getString("test_string") == "hello");

        assert(!dbResult.next());

        std::cout << "✓ Simple query tests passed" << std::endl;
    }

    void testParameterizedQuery() {
        std::cout << "Testing parameterized query execution..." << std::endl;

        std::string query = "SELECT $1::text as param1, $2::int as param2, $3::boolean as param3";
        std::vector<std::string> params = {"test_string", "42", "true"};

        PGresult* result = db->executeParameterizedQuery(query, params);
        assert(result != nullptr);

        DatabaseResult dbResult(result);
        assert(dbResult.isValid());
        assert(dbResult.next());

        assert(dbResult.getString("param1") == "test_string");
        assert(dbResult.getInt("param2") == 42);
        assert(dbResult.getBool("param3") == true);

        std::cout << "✓ Parameterized query tests passed" << std::endl;
    }

    void testCreateTestTable() {
        std::cout << "Testing table creation and manipulation..." << std::endl;

        std::string dropQuery = "DROP TABLE IF EXISTS test_table";
        bool result = db->executeNonQuery(dropQuery);
        assert(result);

        std::string createQuery = R"(
            CREATE TABLE test_table (
                id SERIAL PRIMARY KEY,
                name VARCHAR(255) NOT NULL,
                age INTEGER,
                active BOOLEAN DEFAULT true,
                created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
            )
        )";
        result = db->executeNonQuery(createQuery);
        assert(result);

        std::cout << "✓ Table creation tests passed" << std::endl;
    }

    void testInsertData() {
        std::cout << "Testing data insertion..." << std::endl;

        std::string insertQuery = "INSERT INTO test_table (name, age, active) VALUES ($1, $2, $3)";
        std::vector<std::string> params1 = {"John Doe", "30", "true"};
        std::vector<std::string> params2 = {"Jane Smith", "25", "false"};
        std::vector<std::string> params3 = {"Bob Johnson", "35", "true"};

        bool result = db->executeParameterizedNonQuery(insertQuery, params1);
        assert(result);

        result = db->executeParameterizedNonQuery(insertQuery, params2);
        assert(result);

        result = db->executeParameterizedNonQuery(insertQuery, params3);
        assert(result);

        std::cout << "✓ Data insertion tests passed" << std::endl;
    }

    void testSelectData() {
        std::cout << "Testing data selection..." << std::endl;

        PGresult* result = db->executeQuery("SELECT * FROM test_table ORDER BY id");
        assert(result != nullptr);

        DatabaseResult dbResult(result);
        assert(dbResult.isValid());
        assert(dbResult.getRowCount() == 3);

        int count = 0;
        while (dbResult.next()) {
            count++;
            assert(!dbResult.getString("name").empty());
            assert(dbResult.getInt("age") > 0);
            assert(!dbResult.getString("created_at").empty());
        }
        assert(count == 3);

        result = db->executeQuery("SELECT COUNT(*) as total FROM test_table WHERE active = true");
        assert(result != nullptr);

        DatabaseResult countResult(result);
        assert(countResult.next());
        assert(countResult.getInt("total") == 2);

        std::cout << "✓ Data selection tests passed" << std::endl;
    }

    void testUpdateData() {
        std::cout << "Testing data update..." << std::endl;

        std::string updateQuery = "UPDATE test_table SET age = $1 WHERE name = $2";
        std::vector<std::string> params = {"31", "John Doe"};

        bool result = db->executeParameterizedNonQuery(updateQuery, params);
        assert(result);

        std::string selectQuery = "SELECT age FROM test_table WHERE name = $1";
        std::vector<std::string> selectParams = {"John Doe"};

        PGresult* selectResult = db->executeParameterizedQuery(selectQuery, selectParams);
        assert(selectResult != nullptr);

        DatabaseResult dbResult(selectResult);
        assert(dbResult.next());
        assert(dbResult.getInt("age") == 31);

        std::cout << "✓ Data update tests passed" << std::endl;
    }

    void testDeleteData() {
        std::cout << "Testing data deletion..." << std::endl;

        std::string deleteQuery = "DELETE FROM test_table WHERE name = $1";
        std::vector<std::string> params = {"Jane Smith"};

        bool result = db->executeParameterizedNonQuery(deleteQuery, params);
        assert(result);

        PGresult* countResult = db->executeQuery("SELECT COUNT(*) as total FROM test_table");
        assert(countResult != nullptr);

        DatabaseResult dbResult(countResult);
        assert(dbResult.next());
        assert(dbResult.getInt("total") == 2);

        std::cout << "✓ Data deletion tests passed" << std::endl;
    }

    void testTransactions() {
        std::cout << "Testing database transactions..." << std::endl;

        assert(db->beginTransaction());

        std::string insertQuery = "INSERT INTO test_table (name, age) VALUES ($1, $2)";
        std::vector<std::string> params = {"Transaction Test", "40"};
        bool result = db->executeParameterizedNonQuery(insertQuery, params);
        assert(result);

        assert(db->rollbackTransaction());

        PGresult* checkResult = db->executeQuery("SELECT COUNT(*) as total FROM test_table WHERE name = 'Transaction Test'");
        assert(checkResult != nullptr);

        DatabaseResult dbResult(checkResult);
        assert(dbResult.next());
        assert(dbResult.getInt("total") == 0);

        assert(db->beginTransaction());
        result = db->executeParameterizedNonQuery(insertQuery, params);
        assert(result);
        assert(db->commitTransaction());

        checkResult = db->executeQuery("SELECT COUNT(*) as total FROM test_table WHERE name = 'Transaction Test'");
        assert(checkResult != nullptr);

        DatabaseResult dbResult2(checkResult);
        assert(dbResult2.next());
        assert(dbResult2.getInt("total") == 1);

        std::cout << "✓ Transaction tests passed" << std::endl;
    }

    void testDatabaseResult() {
        std::cout << "Testing DatabaseResult functionality..." << std::endl;

        PGresult* result = db->executeQuery("SELECT id, name, age, active FROM test_table ORDER BY id");
        assert(result != nullptr);

        DatabaseResult dbResult(result);

        assert(dbResult.getFieldName(0) == "id");
        assert(dbResult.getFieldName(1) == "name");
        assert(dbResult.getFieldName(2) == "age");
        assert(dbResult.getFieldName(3) == "active");

        assert(dbResult.getFieldIndex("id") == 0);
        assert(dbResult.getFieldIndex("name") == 1);
        assert(dbResult.getFieldIndex("age") == 2);
        assert(dbResult.getFieldIndex("active") == 3);
        assert(dbResult.getFieldIndex("nonexistent") == -1);

        auto allRows = dbResult.getAllRows();
        assert(allRows.size() == 3);
        assert(allRows[0]["name"] == "John Doe");
        assert(allRows[0]["age"] == "31");

        dbResult.reset();
        assert(dbResult.next());
        auto currentRow = dbResult.getCurrentRow();
        assert(currentRow["name"] == "John Doe");

        std::cout << "✓ DatabaseResult tests passed" << std::endl;
    }

    void testErrorHandling() {
        std::cout << "Testing error handling..." << std::endl;

        PGresult* result = db->executeQuery("SELECT * FROM nonexistent_table");
        assert(result == nullptr);

        std::string errorMsg = db->getLastError();
        assert(!errorMsg.empty());

        std::string badQuery = "INSERT INTO test_table (name, age) VALUES ($1, $2, $3)";
        std::vector<std::string> params = {"Test", "25"};
        result = db->executeParameterizedQuery(badQuery, params);
        assert(result == nullptr);

        std::cout << "✓ Error handling tests passed" << std::endl;
    }

    void testStringEscaping() {
        std::cout << "Testing string escaping..." << std::endl;

        std::string testString = "Test with 'single quotes' and \"double quotes\"";
        std::string escaped = db->escapeString(testString);
        assert(!escaped.empty());

        std::string insertQuery = "INSERT INTO test_table (name, age) VALUES ($1, $2)";
        std::vector<std::string> params = {testString, "30"};
        bool result = db->executeParameterizedNonQuery(insertQuery, params);
        assert(result);

        std::string selectQuery = "SELECT name FROM test_table WHERE name = $1";
        PGresult* selectResult = db->executeParameterizedQuery(selectQuery, params);
        assert(selectResult != nullptr);

        DatabaseResult dbResult(selectResult);
        assert(dbResult.next());
        assert(dbResult.getString("name") == testString);

        std::cout << "✓ String escaping tests passed" << std::endl;
    }

    void cleanupTestTable() {
        std::cout << "Cleaning up test table..." << std::endl;
        db->executeNonQuery("DROP TABLE IF EXISTS test_table");
    }

    void runAllTests() {
        std::cout << "=== Database Connection and Operations Tests ===" << std::endl;

        try {
            testDatabaseConnection();
            testSimpleQuery();
            testParameterizedQuery();
            testCreateTestTable();
            testInsertData();
            testSelectData();
            testUpdateData();
            testDeleteData();
            testTransactions();
            testDatabaseResult();
            testErrorHandling();
            testStringEscaping();
            cleanupTestTable();

            std::cout << "\n✓ All database tests passed successfully!" << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Test failed with exception: " << e.what() << std::endl;
            cleanupTestTable();
            exit(1);
        }
    }
};

int main() {
    std::cout << "Starting database connection and operations tests..." << std::endl;
    std::cout << "Note: This test requires a PostgreSQL database to be running." << std::endl;
    std::cout << "Expected connection: host=localhost port=5432 dbname=rezz_test_db user=postgres" << std::endl;

    DatabaseTest test;
    test.runAllTests();

    return 0;
}