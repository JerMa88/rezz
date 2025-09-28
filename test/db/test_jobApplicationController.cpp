#include <iostream>
#include <cassert>
#include <memory>
#include <vector>
#include "../../src/db/jobApplicationController.h"
#include "../../src/ds/resume/jobApplication.h"

class JobApplicationControllerTest {
private:
    std::unique_ptr<JobApplicationController> controller;
    std::vector<std::string> testApplicationIds;

public:
    JobApplicationControllerTest() {
        controller = std::make_unique<JobApplicationController>();

        auto db = DatabaseConnection::getInstance();
        if (!db->connect("localhost", "5432", "rezz_test_db", "postgres", "postgres")) {
            std::cerr << "Warning: Could not connect to test database. Using default connection." << std::endl;
            if (!db->connect()) {
                std::cerr << "Error: Could not connect to any database!" << std::endl;
                exit(1);
            }
        }
    }

    ~JobApplicationControllerTest() {
        cleanupTestData();
    }

    void cleanupTestData() {
        for (const auto& id : testApplicationIds) {
            controller->deleteJobApplication(id);
        }
        testApplicationIds.clear();
    }

    void testCreateJobApplication() {
        std::cout << "Testing JobApplication creation..." << std::endl;

        JobApplication app("TEST_APP_001", "JOB_001", "Software Engineer", "TechCorp", "2024-01-15");
        app.setContactName("John Doe");
        app.setContactEmail("john.doe@techcorp.com");
        app.setExpectedSalary("100000");
        app.setStatus(ApplicationStatus::APPLIED);
        app.addInterviewDate("2024-02-01");
        app.addFollowUpDate("2024-02-10");

        bool result = controller->createJobApplication(app);
        assert(result);
        testApplicationIds.push_back("TEST_APP_001");

        bool duplicate = controller->createJobApplication(app);
        assert(!duplicate);

        std::cout << "✓ JobApplication creation tests passed" << std::endl;
    }

    void testGetJobApplication() {
        std::cout << "Testing JobApplication retrieval..." << std::endl;

        auto app = controller->getJobApplicationById("TEST_APP_001");
        assert(app != nullptr);
        assert(app->getApplicationId() == "TEST_APP_001");
        assert(app->getJobTitle() == "Software Engineer");
        assert(app->getCompany() == "TechCorp");
        assert(app->getContactName() == "John Doe");
        assert(app->getStatus() == ApplicationStatus::APPLIED);

        auto interviewDates = app->getInterviewDates();
        assert(interviewDates.size() == 1);
        assert(interviewDates[0] == "2024-02-01");

        auto followUpDates = app->getFollowUpDates();
        assert(followUpDates.size() == 1);
        assert(followUpDates[0] == "2024-02-10");

        auto nonExistent = controller->getJobApplicationById("NON_EXISTENT");
        assert(nonExistent == nullptr);

        std::cout << "✓ JobApplication retrieval tests passed" << std::endl;
    }

    void testUpdateJobApplication() {
        std::cout << "Testing JobApplication update..." << std::endl;

        auto app = controller->getJobApplicationById("TEST_APP_001");
        assert(app != nullptr);

        app->setStatus(ApplicationStatus::INTERVIEW_SCHEDULED);
        app->setContactPhone("+1-555-1234");
        app->setSalaryOffered("105000");
        app->addInterviewDate("2024-02-15");

        bool result = controller->updateJobApplication(*app);
        assert(result);

        auto updatedApp = controller->getJobApplicationById("TEST_APP_001");
        assert(updatedApp != nullptr);
        assert(updatedApp->getStatus() == ApplicationStatus::INTERVIEW_SCHEDULED);
        assert(updatedApp->getContactPhone() == "+1-555-1234");
        assert(updatedApp->getSalaryOffered() == "105000");

        auto interviewDates = updatedApp->getInterviewDates();
        assert(interviewDates.size() == 2);

        std::cout << "✓ JobApplication update tests passed" << std::endl;
    }

    void testGetJobApplicationsByCompany() {
        std::cout << "Testing JobApplication search by company..." << std::endl;

        JobApplication app2("TEST_APP_002", "JOB_002", "Data Scientist", "TechCorp", "2024-01-20");
        bool result = controller->createJobApplication(app2);
        assert(result);
        testApplicationIds.push_back("TEST_APP_002");

        JobApplication app3("TEST_APP_003", "JOB_003", "DevOps Engineer", "DataFlow", "2024-01-25");
        result = controller->createJobApplication(app3);
        assert(result);
        testApplicationIds.push_back("TEST_APP_003");

        auto techCorpApps = controller->getJobApplicationsByCompany("TechCorp");
        assert(techCorpApps.size() == 2);

        auto dataFlowApps = controller->getJobApplicationsByCompany("DataFlow");
        assert(dataFlowApps.size() == 1);

        auto partialMatch = controller->getJobApplicationsByCompany("Tech");
        assert(partialMatch.size() == 2);

        std::cout << "✓ JobApplication search by company tests passed" << std::endl;
    }

    void testGetJobApplicationsByStatus() {
        std::cout << "Testing JobApplication search by status..." << std::endl;

        auto appliedApps = controller->getJobApplicationsByStatus(ApplicationStatus::APPLIED);
        assert(appliedApps.size() >= 2);

        auto interviewApps = controller->getJobApplicationsByStatus(ApplicationStatus::INTERVIEW_SCHEDULED);
        assert(interviewApps.size() >= 1);

        auto rejectedApps = controller->getJobApplicationsByStatus(ApplicationStatus::REJECTED);
        assert(rejectedApps.size() == 0);

        std::cout << "✓ JobApplication search by status tests passed" << std::endl;
    }

    void testGetJobApplicationsByDateRange() {
        std::cout << "Testing JobApplication search by date range..." << std::endl;

        auto rangeApps = controller->getJobApplicationsByDateRange("2024-01-01", "2024-01-31");
        assert(rangeApps.size() >= 3);

        auto narrowRange = controller->getJobApplicationsByDateRange("2024-01-15", "2024-01-20");
        assert(narrowRange.size() >= 2);

        auto futureRange = controller->getJobApplicationsByDateRange("2024-12-01", "2024-12-31");
        assert(futureRange.size() == 0);

        std::cout << "✓ JobApplication search by date range tests passed" << std::endl;
    }

    void testUpdateJobApplicationStatus() {
        std::cout << "Testing JobApplication status update..." << std::endl;

        bool result = controller->updateJobApplicationStatus("TEST_APP_002", ApplicationStatus::REVIEWING);
        assert(result);

        auto app = controller->getJobApplicationById("TEST_APP_002");
        assert(app != nullptr);
        assert(app->getStatus() == ApplicationStatus::REVIEWING);

        std::cout << "✓ JobApplication status update tests passed" << std::endl;
    }

    void testAddInterviewAndFollowUpDates() {
        std::cout << "Testing interview and follow-up date addition..." << std::endl;

        bool result = controller->addInterviewDate("TEST_APP_003", "2024-02-05");
        assert(result);

        result = controller->addFollowUpDate("TEST_APP_003", "2024-02-12");
        assert(result);

        auto app = controller->getJobApplicationById("TEST_APP_003");
        assert(app != nullptr);

        auto interviewDates = app->getInterviewDates();
        assert(interviewDates.size() == 1);
        assert(interviewDates[0] == "2024-02-05");

        auto followUpDates = app->getFollowUpDates();
        assert(followUpDates.size() == 1);
        assert(followUpDates[0] == "2024-02-12");

        std::cout << "✓ Interview and follow-up date addition tests passed" << std::endl;
    }

    void testGetJobApplicationCount() {
        std::cout << "Testing JobApplication count..." << std::endl;

        int totalCount = controller->getJobApplicationCount();
        assert(totalCount >= 3);

        int appliedCount = controller->getJobApplicationCountByStatus(ApplicationStatus::APPLIED);
        assert(appliedCount >= 1);

        int reviewingCount = controller->getJobApplicationCountByStatus(ApplicationStatus::REVIEWING);
        assert(reviewingCount >= 1);

        int interviewCount = controller->getJobApplicationCountByStatus(ApplicationStatus::INTERVIEW_SCHEDULED);
        assert(interviewCount >= 1);

        std::cout << "✓ JobApplication count tests passed" << std::endl;
    }

    void testJobApplicationExists() {
        std::cout << "Testing JobApplication existence check..." << std::endl;

        assert(controller->jobApplicationExists("TEST_APP_001"));
        assert(controller->jobApplicationExists("TEST_APP_002"));
        assert(controller->jobApplicationExists("TEST_APP_003"));
        assert(!controller->jobApplicationExists("NON_EXISTENT"));

        std::cout << "✓ JobApplication existence check tests passed" << std::endl;
    }

    void testExportToJson() {
        std::cout << "Testing JSON export..." << std::endl;

        std::string jsonData = controller->exportToJson();
        assert(!jsonData.empty());
        assert(jsonData.find("applications") != std::string::npos);
        assert(jsonData.find("TEST_APP_001") != std::string::npos);
        assert(jsonData.find("Software Engineer") != std::string::npos);
        assert(jsonData.find("TechCorp") != std::string::npos);

        std::cout << "✓ JSON export tests passed" << std::endl;
    }

    void testExportToCsv() {
        std::cout << "Testing CSV export..." << std::endl;

        std::string csvData = controller->exportToCsv();
        assert(!csvData.empty());
        assert(csvData.find("Application ID") != std::string::npos);
        assert(csvData.find("Job Title") != std::string::npos);
        assert(csvData.find("Company") != std::string::npos);
        assert(csvData.find("TEST_APP_001") != std::string::npos);
        assert(csvData.find("Software Engineer") != std::string::npos);

        std::cout << "✓ CSV export tests passed" << std::endl;
    }

    void testDeleteJobApplication() {
        std::cout << "Testing JobApplication deletion..." << std::endl;

        assert(controller->jobApplicationExists("TEST_APP_003"));

        bool result = controller->deleteJobApplication("TEST_APP_003");
        assert(result);

        assert(!controller->jobApplicationExists("TEST_APP_003"));

        testApplicationIds.erase(
            std::remove(testApplicationIds.begin(), testApplicationIds.end(), "TEST_APP_003"),
            testApplicationIds.end()
        );

        bool duplicateDelete = controller->deleteJobApplication("TEST_APP_003");
        assert(duplicateDelete);

        std::cout << "✓ JobApplication deletion tests passed" << std::endl;
    }

    void testDeleteJobApplicationsByCompany() {
        std::cout << "Testing JobApplication deletion by company..." << std::endl;

        JobApplication app4("TEST_APP_004", "JOB_004", "QA Engineer", "TestCorp", "2024-02-01");
        JobApplication app5("TEST_APP_005", "JOB_005", "Product Manager", "TestCorp", "2024-02-02");

        bool result = controller->createJobApplication(app4);
        assert(result);
        testApplicationIds.push_back("TEST_APP_004");

        result = controller->createJobApplication(app5);
        assert(result);
        testApplicationIds.push_back("TEST_APP_005");

        auto testCorpApps = controller->getJobApplicationsByCompany("TestCorp");
        assert(testCorpApps.size() == 2);

        result = controller->deleteJobApplicationsByCompany("TestCorp");
        assert(result);

        testCorpApps = controller->getJobApplicationsByCompany("TestCorp");
        assert(testCorpApps.size() == 0);

        testApplicationIds.erase(
            std::remove(testApplicationIds.begin(), testApplicationIds.end(), "TEST_APP_004"),
            testApplicationIds.end()
        );
        testApplicationIds.erase(
            std::remove(testApplicationIds.begin(), testApplicationIds.end(), "TEST_APP_005"),
            testApplicationIds.end()
        );

        std::cout << "✓ JobApplication deletion by company tests passed" << std::endl;
    }

    void testEdgeCases() {
        std::cout << "Testing edge cases..." << std::endl;

        JobApplication emptyApp;
        emptyApp.setApplicationId("TEST_EMPTY");
        emptyApp.setJobTitle("Empty Job");
        emptyApp.setCompany("Empty Corp");
        emptyApp.setDateApplied("2024-01-01");

        bool result = controller->createJobApplication(emptyApp);
        assert(result);
        testApplicationIds.push_back("TEST_EMPTY");

        auto retrievedEmpty = controller->getJobApplicationById("TEST_EMPTY");
        assert(retrievedEmpty != nullptr);
        assert(retrievedEmpty->getContactName().empty());
        assert(retrievedEmpty->getContactEmail().empty());
        assert(retrievedEmpty->getInterviewDates().empty());

        JobApplication longDataApp("TEST_LONG_DATA", "JOB_LONG",
                                  std::string(300, 'A'),
                                  std::string(300, 'B'),
                                  "2024-01-01");
        longDataApp.setComments(std::string(1000, 'C'));
        longDataApp.setNotes(std::string(2000, 'D'));

        result = controller->createJobApplication(longDataApp);
        assert(result);
        testApplicationIds.push_back("TEST_LONG_DATA");

        auto retrievedLong = controller->getJobApplicationById("TEST_LONG_DATA");
        assert(retrievedLong != nullptr);
        assert(retrievedLong->getJobTitle().length() == 300);

        std::cout << "✓ Edge case tests passed" << std::endl;
    }

    void runAllTests() {
        std::cout << "=== JobApplicationController Database Tests ===" << std::endl;

        try {
            testCreateJobApplication();
            testGetJobApplication();
            testUpdateJobApplication();
            testGetJobApplicationsByCompany();
            testGetJobApplicationsByStatus();
            testGetJobApplicationsByDateRange();
            testUpdateJobApplicationStatus();
            testAddInterviewAndFollowUpDates();
            testGetJobApplicationCount();
            testJobApplicationExists();
            testExportToJson();
            testExportToCsv();
            testDeleteJobApplication();
            testDeleteJobApplicationsByCompany();
            testEdgeCases();

            std::cout << "\n✓ All JobApplicationController database tests passed successfully!" << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Test failed with exception: " << e.what() << std::endl;
            cleanupTestData();
            exit(1);
        }
    }
};

int main() {
    std::cout << "Starting JobApplicationController database tests..." << std::endl;
    std::cout << "Note: This test requires a PostgreSQL database to be running." << std::endl;
    std::cout << "Expected connection: host=localhost port=5432 dbname=rezz_test_db user=postgres" << std::endl;

    JobApplicationControllerTest test;
    test.runAllTests();

    return 0;
}