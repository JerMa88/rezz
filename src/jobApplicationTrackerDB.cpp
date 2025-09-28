#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <memory>
#include "ds/resume/jobApplication.h"
#include "db/jobApplicationController.h"

class JobApplicationTrackerDB {
private:
    std::unique_ptr<JobApplicationController> dbController;
    std::string jsonFilename;
    std::string csvFilename;

    std::string getCurrentDate() {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d");
        return ss.str();
    }

    std::string generateApplicationId() {
        auto now = std::chrono::system_clock::now();
        auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
        return "APP_" + std::to_string(timestamp);
    }

    void saveToJsonFile() {
        std::string jsonData = dbController->exportToJson();
        std::ofstream file(jsonFilename);
        if (file.is_open()) {
            file << jsonData;
            file.close();
            std::cout << "Data saved to JSON file: " << jsonFilename << std::endl;
        } else {
            std::cerr << "Error: Could not save to JSON file: " << jsonFilename << std::endl;
        }
    }

    void exportToCsvFile() {
        std::string csvData = dbController->exportToCsv();
        std::ofstream file(csvFilename);
        if (file.is_open()) {
            file << csvData;
            file.close();
            std::cout << "Data exported to CSV file: " << csvFilename << std::endl;
        } else {
            std::cerr << "Error: Could not export to CSV file: " << csvFilename << std::endl;
        }
    }

public:
    JobApplicationTrackerDB(const std::string& jsonFilename = "job_applications.json",
                           const std::string& csvFilename = "job_applications.csv")
        : jsonFilename(jsonFilename), csvFilename(csvFilename) {
        dbController = std::make_unique<JobApplicationController>();
    }

    void addApplication() {
        std::string jobId, jobTitle, company;
        std::cout << "\n=== Add New Job Application ===\n";

        std::cout << "Enter Job ID: ";
        std::getline(std::cin, jobId);

        std::cout << "Enter Job Title: ";
        std::getline(std::cin, jobTitle);

        std::cout << "Enter Company: ";
        std::getline(std::cin, company);

        std::string applicationId = generateApplicationId();
        std::string dateApplied = getCurrentDate();

        JobApplication app(applicationId, jobId, jobTitle, company, dateApplied);

        std::string input;
        std::cout << "Enter Contact Name (optional): ";
        std::getline(std::cin, input);
        if (!input.empty()) app.setContactName(input);

        std::cout << "Enter Contact Email (optional): ";
        std::getline(std::cin, input);
        if (!input.empty()) app.setContactEmail(input);

        std::cout << "Enter Contact Phone (optional): ";
        std::getline(std::cin, input);
        if (!input.empty()) app.setContactPhone(input);

        std::cout << "Enter Application URL (optional): ";
        std::getline(std::cin, input);
        if (!input.empty()) app.setApplicationUrl(input);

        std::cout << "Enter Expected Salary (optional): ";
        std::getline(std::cin, input);
        if (!input.empty()) app.setExpectedSalary(input);

        std::cout << "Enter Referral Source (optional): ";
        std::getline(std::cin, input);
        if (!input.empty()) app.setReferralSource(input);

        std::cout << "Enter Application Method (e.g., LinkedIn, Company Website) (optional): ";
        std::getline(std::cin, input);
        if (!input.empty()) app.setApplicationMethod(input);

        std::cout << "Enter Comments/Notes (optional): ";
        std::getline(std::cin, input);
        if (!input.empty()) app.setComments(input);

        if (dbController->createJobApplication(app)) {
            std::cout << "\nApplication added successfully to database! Application ID: " << applicationId << std::endl;
            saveToJsonFile();
        } else {
            std::cout << "\nFailed to add application to database." << std::endl;
        }
    }

    void updateApplicationStatus() {
        auto applications = dbController->getAllJobApplications();
        if (applications.empty()) {
            std::cout << "No applications found." << std::endl;
            return;
        }

        std::cout << "\n=== Update Application Status ===\n";
        listApplications();

        std::string applicationId;
        std::cout << "\nEnter Application ID to update: ";
        std::getline(std::cin, applicationId);

        auto app = dbController->getJobApplicationById(applicationId);
        if (!app) {
            std::cout << "Application not found." << std::endl;
            return;
        }

        std::cout << "\nSelect new status:\n";
        std::cout << "1. APPLIED\n";
        std::cout << "2. REVIEWING\n";
        std::cout << "3. INTERVIEW_SCHEDULED\n";
        std::cout << "4. INTERVIEWING\n";
        std::cout << "5. WAITING_RESPONSE\n";
        std::cout << "6. OFFER_RECEIVED\n";
        std::cout << "7. REJECTED\n";
        std::cout << "8. WITHDRAWN\n";
        std::cout << "9. ACCEPTED\n";
        std::cout << "Enter choice (1-9): ";

        int choice;
        std::cin >> choice;
        std::cin.ignore();

        ApplicationStatus newStatus = static_cast<ApplicationStatus>(choice);
        app->setStatus(newStatus);

        if (choice == 3 || choice == 4) {
            std::string interviewDate;
            std::cout << "Enter interview date (YYYY-MM-DD): ";
            std::getline(std::cin, interviewDate);
            if (!interviewDate.empty()) {
                app->addInterviewDate(interviewDate);
            }
        }

        if (choice == 6) {
            std::string salaryOffered;
            std::cout << "Enter salary offered (optional): ";
            std::getline(std::cin, salaryOffered);
            if (!salaryOffered.empty()) {
                app->setSalaryOffered(salaryOffered);
            }
        }

        std::string notes;
        std::cout << "Add any notes (optional): ";
        std::getline(std::cin, notes);
        if (!notes.empty()) {
            app->setNotes(app->getNotes() + "\n" + getCurrentDate() + ": " + notes);
        }

        if (dbController->updateJobApplication(*app)) {
            std::cout << "Application status updated successfully in database!" << std::endl;
            saveToJsonFile();
        } else {
            std::cout << "Failed to update application in database." << std::endl;
        }
    }

    void listApplications() {
        auto applications = dbController->getAllJobApplications();
        if (applications.empty()) {
            std::cout << "No applications found." << std::endl;
            return;
        }

        std::cout << "\n=== Job Applications (Total: " << applications.size() << ") ===\n";
        std::cout << std::left << std::setw(15) << "App ID"
                  << std::setw(25) << "Job Title"
                  << std::setw(20) << "Company"
                  << std::setw(12) << "Date Applied"
                  << std::setw(20) << "Status" << std::endl;
        std::cout << std::string(92, '-') << std::endl;

        for (const auto& app : applications) {
            std::cout << std::left << std::setw(15) << app->getApplicationId().substr(4, 10)
                      << std::setw(25) << app->getJobTitle().substr(0, 24)
                      << std::setw(20) << app->getCompany().substr(0, 19)
                      << std::setw(12) << app->getDateApplied()
                      << std::setw(20) << JobApplication::statusToString(app->getStatus()) << std::endl;
        }
    }

    void viewApplicationDetails() {
        auto applications = dbController->getAllJobApplications();
        if (applications.empty()) {
            std::cout << "No applications found." << std::endl;
            return;
        }

        listApplications();

        std::string applicationId;
        std::cout << "\nEnter Application ID to view details: ";
        std::getline(std::cin, applicationId);

        auto app = dbController->getJobApplicationById(applicationId);
        if (!app) {
            std::cout << "Application not found." << std::endl;
            return;
        }

        std::cout << "\n=== Application Details ===\n";
        std::cout << "Application ID: " << app->getApplicationId() << std::endl;
        std::cout << "Job ID: " << app->getJobId() << std::endl;
        std::cout << "Job Title: " << app->getJobTitle() << std::endl;
        std::cout << "Company: " << app->getCompany() << std::endl;
        std::cout << "Date Applied: " << app->getDateApplied() << std::endl;
        std::cout << "Status: " << JobApplication::statusToString(app->getStatus()) << std::endl;
        std::cout << "Contact Name: " << app->getContactName() << std::endl;
        std::cout << "Contact Email: " << app->getContactEmail() << std::endl;
        std::cout << "Contact Phone: " << app->getContactPhone() << std::endl;
        std::cout << "Application URL: " << app->getApplicationUrl() << std::endl;
        std::cout << "Expected Salary: " << app->getExpectedSalary() << std::endl;
        std::cout << "Salary Offered: " << app->getSalaryOffered() << std::endl;
        std::cout << "Referral Source: " << app->getReferralSource() << std::endl;
        std::cout << "Application Method: " << app->getApplicationMethod() << std::endl;
        std::cout << "Response Deadline: " << app->getResponseDeadline() << std::endl;

        const auto& interviewDates = app->getInterviewDates();
        if (!interviewDates.empty()) {
            std::cout << "Interview Dates: ";
            for (size_t i = 0; i < interviewDates.size(); ++i) {
                std::cout << interviewDates[i];
                if (i < interviewDates.size() - 1) std::cout << ", ";
            }
            std::cout << std::endl;
        }

        std::cout << "Comments: " << app->getComments() << std::endl;
        std::cout << "Notes: " << app->getNotes() << std::endl;
    }

    void searchApplications() {
        std::cout << "\n=== Search Applications ===\n";
        std::cout << "1. Search by Company\n";
        std::cout << "2. Search by Status\n";
        std::cout << "3. Search by Date Range\n";
        std::cout << "Enter choice (1-3): ";

        int choice;
        std::cin >> choice;
        std::cin.ignore();

        std::vector<std::unique_ptr<JobApplication>> results;

        switch (choice) {
            case 1: {
                std::string company;
                std::cout << "Enter company name (partial match): ";
                std::getline(std::cin, company);
                results = dbController->getJobApplicationsByCompany(company);
                break;
            }
            case 2: {
                std::cout << "Select status:\n";
                std::cout << "1. APPLIED\n2. REVIEWING\n3. INTERVIEW_SCHEDULED\n4. INTERVIEWING\n";
                std::cout << "5. WAITING_RESPONSE\n6. OFFER_RECEIVED\n7. REJECTED\n8. WITHDRAWN\n9. ACCEPTED\n";
                std::cout << "Enter choice (1-9): ";
                int statusChoice;
                std::cin >> statusChoice;
                std::cin.ignore();
                ApplicationStatus status = static_cast<ApplicationStatus>(statusChoice);
                results = dbController->getJobApplicationsByStatus(status);
                break;
            }
            case 3: {
                std::string startDate, endDate;
                std::cout << "Enter start date (YYYY-MM-DD): ";
                std::getline(std::cin, startDate);
                std::cout << "Enter end date (YYYY-MM-DD): ";
                std::getline(std::cin, endDate);
                results = dbController->getJobApplicationsByDateRange(startDate, endDate);
                break;
            }
            default:
                std::cout << "Invalid choice." << std::endl;
                return;
        }

        if (results.empty()) {
            std::cout << "No applications found matching your criteria." << std::endl;
            return;
        }

        std::cout << "\n=== Search Results (" << results.size() << " found) ===\n";
        std::cout << std::left << std::setw(15) << "App ID"
                  << std::setw(25) << "Job Title"
                  << std::setw(20) << "Company"
                  << std::setw(12) << "Date Applied"
                  << std::setw(20) << "Status" << std::endl;
        std::cout << std::string(92, '-') << std::endl;

        for (const auto& app : results) {
            std::cout << std::left << std::setw(15) << app->getApplicationId().substr(4, 10)
                      << std::setw(25) << app->getJobTitle().substr(0, 24)
                      << std::setw(20) << app->getCompany().substr(0, 19)
                      << std::setw(12) << app->getDateApplied()
                      << std::setw(20) << JobApplication::statusToString(app->getStatus()) << std::endl;
        }
    }

    void deleteApplication() {
        auto applications = dbController->getAllJobApplications();
        if (applications.empty()) {
            std::cout << "No applications found." << std::endl;
            return;
        }

        listApplications();

        std::string applicationId;
        std::cout << "\nEnter Application ID to delete: ";
        std::getline(std::cin, applicationId);

        auto app = dbController->getJobApplicationById(applicationId);
        if (!app) {
            std::cout << "Application not found." << std::endl;
            return;
        }

        std::cout << "Are you sure you want to delete the application for \""
                  << app->getJobTitle() << "\" at \"" << app->getCompany() << "\"? (y/N): ";

        std::string confirmation;
        std::getline(std::cin, confirmation);

        if (confirmation == "y" || confirmation == "Y" || confirmation == "yes" || confirmation == "Yes") {
            if (dbController->deleteJobApplication(applicationId)) {
                std::cout << "Application deleted successfully from database!" << std::endl;
                saveToJsonFile();
            } else {
                std::cout << "Failed to delete application from database." << std::endl;
            }
        } else {
            std::cout << "Delete operation cancelled." << std::endl;
        }
    }

    void showStatistics() {
        std::cout << "\n=== Application Statistics ===\n";

        int totalCount = dbController->getJobApplicationCount();
        std::cout << "Total Applications: " << totalCount << std::endl;

        if (totalCount == 0) {
            return;
        }

        std::cout << "\nBy Status:\n";
        std::cout << "Applied: " << dbController->getJobApplicationCountByStatus(ApplicationStatus::APPLIED) << std::endl;
        std::cout << "Reviewing: " << dbController->getJobApplicationCountByStatus(ApplicationStatus::REVIEWING) << std::endl;
        std::cout << "Interview Scheduled: " << dbController->getJobApplicationCountByStatus(ApplicationStatus::INTERVIEW_SCHEDULED) << std::endl;
        std::cout << "Interviewing: " << dbController->getJobApplicationCountByStatus(ApplicationStatus::INTERVIEWING) << std::endl;
        std::cout << "Waiting Response: " << dbController->getJobApplicationCountByStatus(ApplicationStatus::WAITING_RESPONSE) << std::endl;
        std::cout << "Offer Received: " << dbController->getJobApplicationCountByStatus(ApplicationStatus::OFFER_RECEIVED) << std::endl;
        std::cout << "Rejected: " << dbController->getJobApplicationCountByStatus(ApplicationStatus::REJECTED) << std::endl;
        std::cout << "Withdrawn: " << dbController->getJobApplicationCountByStatus(ApplicationStatus::WITHDRAWN) << std::endl;
        std::cout << "Accepted: " << dbController->getJobApplicationCountByStatus(ApplicationStatus::ACCEPTED) << std::endl;
    }

    void exportData() {
        std::cout << "\n=== Export Data ===\n";
        std::cout << "1. Export to JSON\n";
        std::cout << "2. Export to CSV\n";
        std::cout << "3. Export Both\n";
        std::cout << "Enter choice (1-3): ";

        int choice;
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            case 1:
                saveToJsonFile();
                break;
            case 2:
                exportToCsvFile();
                break;
            case 3:
                saveToJsonFile();
                exportToCsvFile();
                break;
            default:
                std::cout << "Invalid choice." << std::endl;
        }
    }

    void showMenu() {
        int choice;
        do {
            std::cout << "\n=== Job Application Tracker with Database ===\n";
            std::cout << "1. Add New Application\n";
            std::cout << "2. List All Applications\n";
            std::cout << "3. View Application Details\n";
            std::cout << "4. Update Application Status\n";
            std::cout << "5. Search Applications\n";
            std::cout << "6. Delete Application\n";
            std::cout << "7. Show Statistics\n";
            std::cout << "8. Export Data\n";
            std::cout << "9. Exit\n";
            std::cout << "Enter your choice (1-9): ";

            std::cin >> choice;
            std::cin.ignore();

            switch (choice) {
                case 1:
                    addApplication();
                    break;
                case 2:
                    listApplications();
                    break;
                case 3:
                    viewApplicationDetails();
                    break;
                case 4:
                    updateApplicationStatus();
                    break;
                case 5:
                    searchApplications();
                    break;
                case 6:
                    deleteApplication();
                    break;
                case 7:
                    showStatistics();
                    break;
                case 8:
                    exportData();
                    break;
                case 9:
                    std::cout << "Exiting Job Application Tracker. Goodbye!" << std::endl;
                    break;
                default:
                    std::cout << "Invalid choice. Please try again." << std::endl;
            }
        } while (choice != 9);
    }
};

int main() {
    std::cout << "=== REZZ Job Application Tracker with Database ===\n";
    std::cout << "Connecting to PostgreSQL database...\n";

    auto db = DatabaseConnection::getInstance();
    if (!db->connect()) {
        std::cerr << "Failed to connect to database. Please ensure PostgreSQL is running and the database is set up.\n";
        std::cerr << "Using default connection: host=localhost port=5432 dbname=rezz_db user=postgres\n";
        return 1;
    }

    std::cout << "Database connected successfully!\n";

    JobApplicationTrackerDB tracker("job_applications_db.json", "job_applications_db.csv");
    tracker.showMenu();

    return 0;
}