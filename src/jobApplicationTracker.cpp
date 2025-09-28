#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include "ds/resume/jobApplication.h"

class JobApplicationTracker {
private:
    std::vector<JobApplication> applications;
    std::string filename;

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

    void saveToJson() {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file for writing: " << filename << std::endl;
            return;
        }

        file << "{\n";
        file << "  \"applications\": [\n";

        for (size_t i = 0; i < applications.size(); ++i) {
            const auto& app = applications[i];
            file << "    {\n";
            file << "      \"applicationId\": \"" << app.getApplicationId() << "\",\n";
            file << "      \"jobId\": \"" << app.getJobId() << "\",\n";
            file << "      \"jobTitle\": \"" << app.getJobTitle() << "\",\n";
            file << "      \"company\": \"" << app.getCompany() << "\",\n";
            file << "      \"dateApplied\": \"" << app.getDateApplied() << "\",\n";
            file << "      \"status\": \"" << JobApplication::statusToString(app.getStatus()) << "\",\n";
            file << "      \"contactName\": \"" << app.getContactName() << "\",\n";
            file << "      \"contactEmail\": \"" << app.getContactEmail() << "\",\n";
            file << "      \"contactPhone\": \"" << app.getContactPhone() << "\",\n";
            file << "      \"comments\": \"" << app.getComments() << "\",\n";
            file << "      \"applicationUrl\": \"" << app.getApplicationUrl() << "\",\n";
            file << "      \"salaryOffered\": \"" << app.getSalaryOffered() << "\",\n";
            file << "      \"expectedSalary\": \"" << app.getExpectedSalary() << "\",\n";
            file << "      \"responseDeadline\": \"" << app.getResponseDeadline() << "\",\n";
            file << "      \"referralSource\": \"" << app.getReferralSource() << "\",\n";
            file << "      \"applicationMethod\": \"" << app.getApplicationMethod() << "\",\n";
            file << "      \"notes\": \"" << app.getNotes() << "\",\n";

            file << "      \"interviewDates\": [";
            const auto& interviewDates = app.getInterviewDates();
            for (size_t j = 0; j < interviewDates.size(); ++j) {
                file << "\"" << interviewDates[j] << "\"";
                if (j < interviewDates.size() - 1) file << ", ";
            }
            file << "],\n";

            file << "      \"followUpDates\": [";
            const auto& followUpDates = app.getFollowUpDates();
            for (size_t j = 0; j < followUpDates.size(); ++j) {
                file << "\"" << followUpDates[j] << "\"";
                if (j < followUpDates.size() - 1) file << ", ";
            }
            file << "]\n";

            file << "    }";
            if (i < applications.size() - 1) file << ",";
            file << "\n";
        }

        file << "  ]\n";
        file << "}\n";
        file.close();
        std::cout << "Applications saved to " << filename << std::endl;
    }

public:
    JobApplicationTracker(const std::string& filename = "job_applications.json")
        : filename(filename) {}

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

        applications.push_back(app);
        saveToJson();

        std::cout << "\nApplication added successfully! Application ID: " << applicationId << std::endl;
    }

    void updateApplicationStatus() {
        if (applications.empty()) {
            std::cout << "No applications found." << std::endl;
            return;
        }

        std::cout << "\n=== Update Application Status ===\n";
        listApplications();

        std::string applicationId;
        std::cout << "\nEnter Application ID to update: ";
        std::getline(std::cin, applicationId);

        auto it = std::find_if(applications.begin(), applications.end(),
            [&applicationId](const JobApplication& app) {
                return app.getApplicationId() == applicationId;
            });

        if (it == applications.end()) {
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
        it->setStatus(newStatus);

        if (choice == 3 || choice == 4) {
            std::string interviewDate;
            std::cout << "Enter interview date (YYYY-MM-DD): ";
            std::getline(std::cin, interviewDate);
            if (!interviewDate.empty()) {
                it->addInterviewDate(interviewDate);
            }
        }

        if (choice == 6) {
            std::string salaryOffered;
            std::cout << "Enter salary offered (optional): ";
            std::getline(std::cin, salaryOffered);
            if (!salaryOffered.empty()) {
                it->setSalaryOffered(salaryOffered);
            }
        }

        std::string notes;
        std::cout << "Add any notes (optional): ";
        std::getline(std::cin, notes);
        if (!notes.empty()) {
            it->setNotes(it->getNotes() + "\n" + getCurrentDate() + ": " + notes);
        }

        saveToJson();
        std::cout << "Application status updated successfully!" << std::endl;
    }

    void listApplications() {
        if (applications.empty()) {
            std::cout << "No applications found." << std::endl;
            return;
        }

        std::cout << "\n=== Job Applications ===\n";
        std::cout << std::left << std::setw(15) << "App ID"
                  << std::setw(25) << "Job Title"
                  << std::setw(20) << "Company"
                  << std::setw(12) << "Date Applied"
                  << std::setw(20) << "Status" << std::endl;
        std::cout << std::string(92, '-') << std::endl;

        for (const auto& app : applications) {
            std::cout << std::left << std::setw(15) << app.getApplicationId().substr(4, 10)
                      << std::setw(25) << app.getJobTitle().substr(0, 24)
                      << std::setw(20) << app.getCompany().substr(0, 19)
                      << std::setw(12) << app.getDateApplied()
                      << std::setw(20) << JobApplication::statusToString(app.getStatus()) << std::endl;
        }
    }

    void viewApplicationDetails() {
        if (applications.empty()) {
            std::cout << "No applications found." << std::endl;
            return;
        }

        listApplications();

        std::string applicationId;
        std::cout << "\nEnter Application ID to view details: ";
        std::getline(std::cin, applicationId);

        auto it = std::find_if(applications.begin(), applications.end(),
            [&applicationId](const JobApplication& app) {
                return app.getApplicationId() == applicationId;
            });

        if (it == applications.end()) {
            std::cout << "Application not found." << std::endl;
            return;
        }

        const auto& app = *it;
        std::cout << "\n=== Application Details ===\n";
        std::cout << "Application ID: " << app.getApplicationId() << std::endl;
        std::cout << "Job ID: " << app.getJobId() << std::endl;
        std::cout << "Job Title: " << app.getJobTitle() << std::endl;
        std::cout << "Company: " << app.getCompany() << std::endl;
        std::cout << "Date Applied: " << app.getDateApplied() << std::endl;
        std::cout << "Status: " << JobApplication::statusToString(app.getStatus()) << std::endl;
        std::cout << "Contact Name: " << app.getContactName() << std::endl;
        std::cout << "Contact Email: " << app.getContactEmail() << std::endl;
        std::cout << "Contact Phone: " << app.getContactPhone() << std::endl;
        std::cout << "Application URL: " << app.getApplicationUrl() << std::endl;
        std::cout << "Expected Salary: " << app.getExpectedSalary() << std::endl;
        std::cout << "Salary Offered: " << app.getSalaryOffered() << std::endl;
        std::cout << "Referral Source: " << app.getReferralSource() << std::endl;
        std::cout << "Application Method: " << app.getApplicationMethod() << std::endl;
        std::cout << "Response Deadline: " << app.getResponseDeadline() << std::endl;

        const auto& interviewDates = app.getInterviewDates();
        if (!interviewDates.empty()) {
            std::cout << "Interview Dates: ";
            for (size_t i = 0; i < interviewDates.size(); ++i) {
                std::cout << interviewDates[i];
                if (i < interviewDates.size() - 1) std::cout << ", ";
            }
            std::cout << std::endl;
        }

        std::cout << "Comments: " << app.getComments() << std::endl;
        std::cout << "Notes: " << app.getNotes() << std::endl;
    }

    void showMenu() {
        int choice;
        do {
            std::cout << "\n=== Job Application Tracker ===\n";
            std::cout << "1. Add New Application\n";
            std::cout << "2. List All Applications\n";
            std::cout << "3. View Application Details\n";
            std::cout << "4. Update Application Status\n";
            std::cout << "5. Exit\n";
            std::cout << "Enter your choice (1-5): ";

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
                    std::cout << "Exiting Job Application Tracker. Goodbye!" << std::endl;
                    break;
                default:
                    std::cout << "Invalid choice. Please try again." << std::endl;
            }
        } while (choice != 5);
    }
};

int main() {
    JobApplicationTracker tracker("job_applications.json");
    tracker.showMenu();
    return 0;
}