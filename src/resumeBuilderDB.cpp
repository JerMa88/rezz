#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <memory>
#include "ds/resume/resume.h"
#include "db/resumeController.h"

class ResumeBuilderDB {
private:
    Resume resume;
    std::unique_ptr<ResumeController> dbController;
    std::string jsonFilename;

    std::string promptString(const std::string& prompt) {
        std::string input;
        std::cout << prompt;
        std::getline(std::cin, input);
        return input;
    }

    int promptInt(const std::string& prompt) {
        std::string input;
        std::cout << prompt;
        std::getline(std::cin, input);
        if (input.empty()) return 0;
        try {
            return std::stoi(input);
        } catch (const std::exception&) {
            return 0;
        }
    }

    float promptFloat(const std::string& prompt) {
        std::string input;
        std::cout << prompt;
        std::getline(std::cin, input);
        if (input.empty()) return -1.0f;
        try {
            return std::stof(input);
        } catch (const std::exception&) {
            return -1.0f;
        }
    }

    bool promptYesNo(const std::string& prompt) {
        std::string input;
        std::cout << prompt << " (y/n): ";
        std::getline(std::cin, input);
        return (input == "y" || input == "Y" || input == "yes" || input == "Yes");
    }

    void saveToJsonFile() {
        std::string jsonData = dbController->exportToJson();
        std::ofstream file(jsonFilename);
        if (file.is_open()) {
            file << jsonData;
            file.close();
            std::cout << "Resume data saved to JSON file: " << jsonFilename << std::endl;
        } else {
            std::cerr << "Error: Could not save to JSON file: " << jsonFilename << std::endl;
        }
    }

public:
    ResumeBuilderDB(const std::string& jsonFilename = "resumes_db.json")
        : jsonFilename(jsonFilename) {
        dbController = std::make_unique<ResumeController>();
    }

    void collectBasicInfo() {
        std::cout << "\n=== BASIC INFORMATION ===\n";

        int id = promptInt("Enter your ID (or press Enter for auto-generated): ");
        resume.setId(id);

        std::string name = promptString("Enter your full name: ");
        resume.setName(name);

        std::string email = promptString("Enter your email: ");
        resume.setEmail(email);

        std::string city = promptString("Enter your city, state (e.g., San Francisco, CA): ");
        resume.setCity(city);

        std::string phone = promptString("Enter your phone number: ");
        resume.setPhone(phone);

        std::string linkedin = promptString("Enter your LinkedIn URL (optional): ");
        resume.setLinkedin(linkedin);

        std::string website = promptString("Enter your website/portfolio URL (optional): ");
        resume.setWebsite(website);

        std::string interests = promptString("Enter your interests (optional): ");
        resume.setInterests(interests);
    }

    void createNewResume() {
        std::cout << "\n=== Create New Resume ===\n";

        resume = Resume();
        collectBasicInfo();

        if (dbController->createResume(resume)) {
            std::cout << "\nResume created successfully in database!" << std::endl;
            saveToJsonFile();
        } else {
            std::cout << "\nFailed to create resume in database." << std::endl;
        }
    }

    void loadExistingResume() {
        std::cout << "\n=== Load Existing Resume ===\n";
        std::cout << "1. Load by ID\n";
        std::cout << "2. Load by Email\n";
        std::cout << "3. Search by Name\n";
        std::cout << "Enter choice (1-3): ";

        int choice;
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            case 1: {
                int id = promptInt("Enter Resume ID: ");
                auto loadedResume = dbController->getResumeById(id);
                if (loadedResume) {
                    resume = *loadedResume;
                    std::cout << "Resume loaded successfully!" << std::endl;
                    displayResumeInfo();
                } else {
                    std::cout << "Resume not found." << std::endl;
                }
                break;
            }
            case 2: {
                std::string email = promptString("Enter Email: ");
                auto loadedResume = dbController->getResumeByEmail(email);
                if (loadedResume) {
                    resume = *loadedResume;
                    std::cout << "Resume loaded successfully!" << std::endl;
                    displayResumeInfo();
                } else {
                    std::cout << "Resume not found." << std::endl;
                }
                break;
            }
            case 3: {
                std::string name = promptString("Enter Name (partial match): ");
                auto resumes = dbController->getResumesByName(name);
                if (resumes.empty()) {
                    std::cout << "No resumes found." << std::endl;
                } else {
                    std::cout << "\nFound " << resumes.size() << " resume(s):\n";
                    for (size_t i = 0; i < resumes.size(); ++i) {
                        std::cout << (i + 1) << ". " << resumes[i]->getName()
                                  << " (" << resumes[i]->getEmail() << ")" << std::endl;
                    }

                    int selection = promptInt("Select resume number: ");
                    if (selection > 0 && selection <= static_cast<int>(resumes.size())) {
                        resume = *resumes[selection - 1];
                        std::cout << "Resume loaded successfully!" << std::endl;
                        displayResumeInfo();
                    } else {
                        std::cout << "Invalid selection." << std::endl;
                    }
                }
                break;
            }
            default:
                std::cout << "Invalid choice." << std::endl;
        }
    }

    void updateResume() {
        if (resume.getId() == 0) {
            std::cout << "No resume loaded. Please create or load a resume first." << std::endl;
            return;
        }

        std::cout << "\n=== Update Resume ===\n";
        std::cout << "1. Update Basic Information\n";
        std::cout << "2. Update Complete Resume\n";
        std::cout << "Enter choice (1-2): ";

        int choice;
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            case 1: {
                std::cout << "\nUpdating basic information...\n";
                std::string name = promptString("Enter your full name [" + resume.getName() + "]: ");
                if (!name.empty()) resume.setName(name);

                std::string email = promptString("Enter your email [" + resume.getEmail() + "]: ");
                if (!email.empty()) resume.setEmail(email);

                std::string city = promptString("Enter your city [" + resume.getCity() + "]: ");
                if (!city.empty()) resume.setCity(city);

                std::string phone = promptString("Enter your phone [" + resume.getPhone() + "]: ");
                if (!phone.empty()) resume.setPhone(phone);

                if (dbController->updateResumeBasicInfo(resume.getId(), resume.getName(),
                                                       resume.getEmail(), resume.getCity(), resume.getPhone())) {
                    std::cout << "Basic information updated successfully!" << std::endl;
                    saveToJsonFile();
                } else {
                    std::cout << "Failed to update basic information." << std::endl;
                }
                break;
            }
            case 2: {
                collectBasicInfo();
                if (dbController->updateResume(resume)) {
                    std::cout << "Resume updated successfully!" << std::endl;
                    saveToJsonFile();
                } else {
                    std::cout << "Failed to update resume." << std::endl;
                }
                break;
            }
            default:
                std::cout << "Invalid choice." << std::endl;
        }
    }

    void deleteResume() {
        if (resume.getId() == 0) {
            std::cout << "No resume loaded. Please load a resume first." << std::endl;
            return;
        }

        std::cout << "Are you sure you want to delete the resume for \""
                  << resume.getName() << "\" (" << resume.getEmail() << ")? (y/N): ";

        std::string confirmation;
        std::getline(std::cin, confirmation);

        if (confirmation == "y" || confirmation == "Y" || confirmation == "yes" || confirmation == "Yes") {
            if (dbController->deleteResume(resume.getId())) {
                std::cout << "Resume deleted successfully!" << std::endl;
                resume = Resume(); // Clear loaded resume
                saveToJsonFile();
            } else {
                std::cout << "Failed to delete resume." << std::endl;
            }
        } else {
            std::cout << "Delete operation cancelled." << std::endl;
        }
    }

    void listAllResumes() {
        auto resumes = dbController->getAllResumes();
        if (resumes.empty()) {
            std::cout << "No resumes found in database." << std::endl;
            return;
        }

        std::cout << "\n=== All Resumes (Total: " << resumes.size() << ") ===\n";
        std::cout << std::left << std::setw(5) << "ID"
                  << std::setw(25) << "Name"
                  << std::setw(30) << "Email"
                  << std::setw(20) << "City" << std::endl;
        std::cout << std::string(80, '-') << std::endl;

        for (const auto& res : resumes) {
            std::cout << std::left << std::setw(5) << res->getId()
                      << std::setw(25) << res->getName().substr(0, 24)
                      << std::setw(30) << res->getEmail().substr(0, 29)
                      << std::setw(20) << res->getCity().substr(0, 19) << std::endl;
        }
    }

    void displayResumeInfo() {
        if (resume.getId() == 0) {
            std::cout << "No resume loaded." << std::endl;
            return;
        }

        std::cout << "\n=== Resume Information ===\n";
        std::cout << "ID: " << resume.getId() << std::endl;
        std::cout << "Name: " << resume.getName() << std::endl;
        std::cout << "Email: " << resume.getEmail() << std::endl;
        std::cout << "City: " << resume.getCity() << std::endl;
        std::cout << "Phone: " << resume.getPhone() << std::endl;
        std::cout << "LinkedIn: " << resume.getLinkedin() << std::endl;
        std::cout << "Website: " << resume.getWebsite() << std::endl;
        std::cout << "Interests: " << resume.getInterests() << std::endl;
    }

    void searchResumes() {
        std::cout << "\n=== Search Resumes ===\n";
        std::cout << "1. Search by Name\n";
        std::cout << "2. Search by Email\n";
        std::cout << "Enter choice (1-2): ";

        int choice;
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            case 1: {
                std::string name = promptString("Enter name (partial match): ");
                auto resumes = dbController->getResumesByName(name);
                if (resumes.empty()) {
                    std::cout << "No resumes found." << std::endl;
                } else {
                    std::cout << "\nFound " << resumes.size() << " resume(s):\n";
                    for (const auto& res : resumes) {
                        std::cout << "ID: " << res->getId() << ", Name: " << res->getName()
                                  << ", Email: " << res->getEmail() << std::endl;
                    }
                }
                break;
            }
            case 2: {
                std::string email = promptString("Enter email: ");
                auto resume = dbController->getResumeByEmail(email);
                if (resume) {
                    std::cout << "\nResume found:\n";
                    std::cout << "ID: " << resume->getId() << ", Name: " << resume->getName()
                              << ", Email: " << resume->getEmail() << std::endl;
                } else {
                    std::cout << "No resume found with that email." << std::endl;
                }
                break;
            }
            default:
                std::cout << "Invalid choice." << std::endl;
        }
    }

    void showStatistics() {
        std::cout << "\n=== Resume Statistics ===\n";
        int totalCount = dbController->getResumeCount();
        std::cout << "Total Resumes: " << totalCount << std::endl;
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
            case 1: {
                saveToJsonFile();
                break;
            }
            case 2: {
                std::string csvData = dbController->exportToCsv();
                std::string csvFilename = "resumes_db.csv";
                std::ofstream file(csvFilename);
                if (file.is_open()) {
                    file << csvData;
                    file.close();
                    std::cout << "Data exported to CSV file: " << csvFilename << std::endl;
                } else {
                    std::cerr << "Error: Could not export to CSV file: " << csvFilename << std::endl;
                }
                break;
            }
            case 3: {
                saveToJsonFile();
                std::string csvData = dbController->exportToCsv();
                std::string csvFilename = "resumes_db.csv";
                std::ofstream file(csvFilename);
                if (file.is_open()) {
                    file << csvData;
                    file.close();
                    std::cout << "Data exported to both JSON and CSV files." << std::endl;
                } else {
                    std::cerr << "Error: Could not export to CSV file." << std::endl;
                }
                break;
            }
            default:
                std::cout << "Invalid choice." << std::endl;
        }
    }

    void showMenu() {
        int choice;
        do {
            std::cout << "\n=== Resume Builder with Database ===\n";
            std::cout << "1. Create New Resume\n";
            std::cout << "2. Load Existing Resume\n";
            std::cout << "3. Update Current Resume\n";
            std::cout << "4. Display Current Resume\n";
            std::cout << "5. Delete Current Resume\n";
            std::cout << "6. List All Resumes\n";
            std::cout << "7. Search Resumes\n";
            std::cout << "8. Show Statistics\n";
            std::cout << "9. Export Data\n";
            std::cout << "10. Exit\n";
            std::cout << "Enter your choice (1-10): ";

            std::cin >> choice;
            std::cin.ignore();

            switch (choice) {
                case 1:
                    createNewResume();
                    break;
                case 2:
                    loadExistingResume();
                    break;
                case 3:
                    updateResume();
                    break;
                case 4:
                    displayResumeInfo();
                    break;
                case 5:
                    deleteResume();
                    break;
                case 6:
                    listAllResumes();
                    break;
                case 7:
                    searchResumes();
                    break;
                case 8:
                    showStatistics();
                    break;
                case 9:
                    exportData();
                    break;
                case 10:
                    std::cout << "Exiting Resume Builder. Goodbye!" << std::endl;
                    break;
                default:
                    std::cout << "Invalid choice. Please try again." << std::endl;
            }
        } while (choice != 10);
    }
};

int main() {
    std::cout << "=== REZZ Resume Builder with Database ===\n";
    std::cout << "Connecting to PostgreSQL database...\n";

    auto db = DatabaseConnection::getInstance();
    if (!db->connect()) {
        std::cerr << "Failed to connect to database. Please ensure PostgreSQL is running and the database is set up.\n";
        std::cerr << "Using default connection: host=localhost port=5432 dbname=rezz_db user=postgres\n";
        return 1;
    }

    std::cout << "Database connected successfully!\n";

    ResumeBuilderDB builder("resumes_db.json");
    builder.showMenu();

    return 0;
}