#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "ds/resume/resume.h"

class ResumeBuilder {
private:
    Resume resume;

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

    std::string escapeJson(const std::string& str) {
        std::string escaped;
        for (char c : str) {
            if (c == '"') escaped += "\\\"";
            else if (c == '\\') escaped += "\\\\";
            else if (c == '\n') escaped += "\\n";
            else if (c == '\r') escaped += "\\r";
            else if (c == '\t') escaped += "\\t";
            else escaped += c;
        }
        return escaped;
    }

    std::string vectorToJsonArray(const std::vector<std::string>& vec) {
        std::ostringstream oss;
        oss << "[";
        for (size_t i = 0; i < vec.size(); ++i) {
            if (i > 0) oss << ",";
            oss << "\"" << escapeJson(vec[i]) << "\"";
        }
        oss << "]";
        return oss.str();
    }

public:
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

    void collectSkills() {
        std::cout << "\n=== SKILLS ===\n";
        Skills& skills = resume.getSkills();

        std::string skillType = promptString("Enter skill category (e.g., Programming Languages, Tools, etc.): ");
        skills.setSkillType(skillType);

        while (true) {
            std::string skillName = promptString("Enter skill name (or press Enter to finish skills): ");
            if (skillName.empty()) break;

            std::cout << "Skill levels: 1=Beginner, 2=Intermediate, 3=Advanced, 4=Expert\n";
            int level = promptInt("Enter skill level (1-4): ");
            if (level < 1 || level > 4) level = 1;

            int years = promptInt("Years of experience with this skill: ");

            SkillLevel skillLevel = static_cast<SkillLevel>(level);
            Skill skill(skillName, years, skillLevel);
            skills.addSkill(skill);

            std::cout << "Added skill: " << skillName << " (Level: " << level << ", Years: " << years << ")\n";
        }
    }

    void collectEducation() {
        std::cout << "\n=== EDUCATION ===\n";

        while (true) {
            std::string institution = promptString("Enter institution name (or press Enter to finish education): ");
            if (institution.empty()) break;

            std::string degree = promptString("Enter degree (e.g., Bachelor of Science): ");
            std::string field = promptString("Enter field of study: ");
            std::string startDate = promptString("Enter start date (YYYY-MM): ");
            std::string endDate = promptString("Enter end date (YYYY-MM or 'Present'): ");

            float gpa = promptFloat("Enter GPA (or press Enter to skip): ");

            std::vector<std::string> courses;
            std::cout << "Enter relevant courses (press Enter on empty line to finish):\n";
            while (true) {
                std::string course = promptString("Course: ");
                if (course.empty()) break;
                courses.push_back(course);
            }

            Education edu(institution, degree, field, startDate, endDate, gpa, courses);
            resume.getEducation() = edu;

            std::cout << "Added education: " << degree << " from " << institution << "\n";
            break; // For now, just handle one education entry
        }
    }

    void collectExperiences() {
        std::cout << "\n=== EXPERIENCES ===\n";
        Experiences& experiences = resume.getExperiences();

        while (true) {
            std::string expName = promptString("Enter experience/technology name (or press Enter to finish): ");
            if (expName.empty()) break;

            std::cout << "Experience levels: 1=Beginner, 2=Intermediate, 3=Advanced, 4=Expert, 5=Master\n";
            int level = promptInt("Enter experience level (1-5): ");
            if (level < 1 || level > 5) level = 1;

            ExperienceLevel expLevel = static_cast<ExperienceLevel>(level);
            Experience exp(expName, expLevel);
            experiences.addExperience(exp);

            std::cout << "Added experience: " << expName << " (Level: " << level << ")\n";
        }
    }

    void collectProjects() {
        std::cout << "\n=== PROJECTS ===\n";
        Projects& projects = resume.getProjects();

        while (true) {
            std::string projectName = promptString("Enter project name (or press Enter to finish projects): ");
            if (projectName.empty()) break;

            std::string description = promptString("Enter project description: ");
            std::string url = promptString("Enter project URL (optional): ");
            std::string course = promptString("Enter associated course (optional): ");
            std::string startDate = promptString("Enter start date (YYYY-MM): ");
            std::string endDate = promptString("Enter end date (YYYY-MM): ");

            std::vector<Skill> skillsUsed;
            std::cout << "Enter skills used in this project (press Enter on empty line to finish):\n";
            while (true) {
                std::string skillName = promptString("Skill name: ");
                if (skillName.empty()) break;

                int level = promptInt("Skill level (1-4): ");
                if (level < 1 || level > 4) level = 1;
                int years = promptInt("Years of experience: ");

                SkillLevel skillLevel = static_cast<SkillLevel>(level);
                Skill skill(skillName, years, skillLevel);
                skillsUsed.push_back(skill);
            }

            Project project(projectName, description, url, skillsUsed, course, startDate, endDate);
            projects.addProject(project);

            std::cout << "Added project: " << projectName << "\n";
        }
    }

    void collectOtherSections() {
        std::cout << "\n=== OTHER SECTIONS ===\n";

        // Languages
        std::cout << "\n--- Languages ---\n";
        Languages& languages = resume.getLanguages();
        while (true) {
            std::string langName = promptString("Enter language (or press Enter to finish): ");
            if (langName.empty()) break;

            std::cout << "Proficiency levels: 1=Elementary, 2=Limited Working, 3=Professional Working, 4=Full Professional, 5=Native\n";
            int proficiency = promptInt("Enter proficiency level (1-5): ");
            if (proficiency < 1 || proficiency > 5) proficiency = 1;

            LanguageProficiency prof = static_cast<LanguageProficiency>(proficiency);
            Language lang(langName, prof);
            languages.addLanguage(lang);

            std::cout << "Added language: " << langName << "\n";
        }

        // Awards
        std::cout << "\n--- Awards ---\n";
        Awards& awards = resume.getAwards();
        while (true) {
            std::string awardName = promptString("Enter award name (or press Enter to finish): ");
            if (awardName.empty()) break;

            std::string date = promptString("Enter award date (YYYY-MM): ");
            std::string issuer = promptString("Enter award issuer: ");
            std::string description = promptString("Enter award description (optional): ");

            Award award(awardName, date, issuer, description);
            awards.addAward(award);

            std::cout << "Added award: " << awardName << "\n";
        }

        // Certifications
        std::cout << "\n--- Certifications ---\n";
        Certifications& certifications = resume.getCertifications();
        while (true) {
            std::string certName = promptString("Enter certification name (or press Enter to finish): ");
            if (certName.empty()) break;

            std::string authority = promptString("Enter certifying authority: ");
            std::string license = promptString("Enter license number (optional): ");
            std::string url = promptString("Enter verification URL (optional): ");
            std::string issueDate = promptString("Enter issue date (YYYY-MM): ");

            bool doesNotExpire = promptYesNo("Does this certification never expire?");
            std::string expDate = "";
            if (!doesNotExpire) {
                expDate = promptString("Enter expiration date (YYYY-MM): ");
            }

            Certification cert(certName, authority, license, url, issueDate, expDate, doesNotExpire);
            certifications.addCertification(cert);

            std::cout << "Added certification: " << certName << "\n";
        }
    }

    std::string generateJSON() {
        std::ostringstream json;
        json << "{\n";
        json << "  \"id\": " << resume.getId() << ",\n";
        json << "  \"name\": \"" << escapeJson(resume.getName()) << "\",\n";
        json << "  \"email\": \"" << escapeJson(resume.getEmail()) << "\",\n";
        json << "  \"city\": \"" << escapeJson(resume.getCity()) << "\",\n";
        json << "  \"phone\": \"" << escapeJson(resume.getPhone()) << "\",\n";
        json << "  \"linkedin\": \"" << escapeJson(resume.getLinkedin()) << "\",\n";
        json << "  \"website\": \"" << escapeJson(resume.getWebsite()) << "\",\n";
        json << "  \"interests\": \"" << escapeJson(resume.getInterests()) << "\",\n";

        // Skills
        Skills& skills = resume.getSkills();
        json << "  \"skills\": {\n";
        json << "    \"type\": \"" << escapeJson(skills.getSkillType()) << "\",\n";
        json << "    \"count\": " << skills.getSkillCount() << ",\n";
        json << "    \"list\": [";

        std::vector<Skill> skillList = skills.getAllSkills();
        for (size_t i = 0; i < skillList.size(); ++i) {
            if (i > 0) json << ",";
            json << "\n      {\n";
            json << "        \"name\": \"" << escapeJson(skillList[i].getName()) << "\",\n";
            json << "        \"years\": " << skillList[i].getYearsOfExperience() << ",\n";
            json << "        \"level\": " << static_cast<int>(skillList[i].getLevel()) << "\n";
            json << "      }";
        }
        json << "\n    ]\n";
        json << "  },\n";

        // Education
        Education& education = resume.getEducation();
        json << "  \"education\": {\n";
        json << "    \"institution\": \"" << escapeJson(education.getInstitutionName()) << "\",\n";
        json << "    \"degree\": \"" << escapeJson(education.getDegree()) << "\",\n";
        json << "    \"field\": \"" << escapeJson(education.getFieldOfStudy()) << "\",\n";
        json << "    \"start_date\": \"" << escapeJson(education.getStartDate()) << "\",\n";
        json << "    \"end_date\": \"" << escapeJson(education.getEndDate()) << "\",\n";
        json << "    \"gpa\": " << education.getGPA() << ",\n";
        json << "    \"courses\": " << vectorToJsonArray(education.getCourses()) << "\n";
        json << "  },\n";

        // Experiences
        Experiences& experiences = resume.getExperiences();
        std::vector<Experience> expList = experiences.getExperiences();
        json << "  \"experiences\": [";
        for (size_t i = 0; i < expList.size(); ++i) {
            if (i > 0) json << ",";
            json << "\n    {\n";
            json << "      \"name\": \"" << escapeJson(expList[i].getName()) << "\",\n";
            json << "      \"level\": " << static_cast<int>(expList[i].getLevel()) << "\n";
            json << "    }";
        }
        json << "\n  ],\n";

        // Projects
        Projects& projects = resume.getProjects();
        std::vector<Project> projectList = projects.getProjects();
        json << "  \"projects\": [";
        for (size_t i = 0; i < projectList.size(); ++i) {
            if (i > 0) json << ",";
            json << "\n    {\n";
            json << "      \"name\": \"" << escapeJson(projectList[i].getName()) << "\",\n";
            json << "      \"description\": \"" << escapeJson(projectList[i].getDescription()) << "\",\n";
            json << "      \"url\": \"" << escapeJson(projectList[i].getUrl()) << "\",\n";
            json << "      \"course\": \"" << escapeJson(projectList[i].getCourse()) << "\",\n";
            json << "      \"start_date\": \"" << escapeJson(projectList[i].getStartDate()) << "\",\n";
            json << "      \"end_date\": \"" << escapeJson(projectList[i].getEndDate()) << "\",\n";
            json << "      \"skills_used\": [";

            std::vector<Skill> projSkills = projectList[i].getSkillsUsed();
            for (size_t j = 0; j < projSkills.size(); ++j) {
                if (j > 0) json << ",";
                json << "\n        {\n";
                json << "          \"name\": \"" << escapeJson(projSkills[j].getName()) << "\",\n";
                json << "          \"years\": " << projSkills[j].getYearsOfExperience() << ",\n";
                json << "          \"level\": " << static_cast<int>(projSkills[j].getLevel()) << "\n";
                json << "        }";
            }
            json << "\n      ]\n";
            json << "    }";
        }
        json << "\n  ],\n";

        // Languages
        Languages& languages = resume.getLanguages();
        std::vector<Language> langList = languages.getLanguages();
        json << "  \"languages\": [";
        for (size_t i = 0; i < langList.size(); ++i) {
            if (i > 0) json << ",";
            json << "\n    {\n";
            json << "      \"name\": \"" << escapeJson(langList[i].getName()) << "\",\n";
            json << "      \"proficiency\": " << static_cast<int>(langList[i].getProficiency()) << "\n";
            json << "    }";
        }
        json << "\n  ],\n";

        // Awards
        Awards& awards = resume.getAwards();
        std::vector<Award> awardList = awards.getAwards();
        json << "  \"awards\": [";
        for (size_t i = 0; i < awardList.size(); ++i) {
            if (i > 0) json << ",";
            json << "\n    {\n";
            json << "      \"name\": \"" << escapeJson(awardList[i].getName()) << "\",\n";
            json << "      \"date\": \"" << escapeJson(awardList[i].getDate()) << "\",\n";
            json << "      \"issuer\": \"" << escapeJson(awardList[i].getIssuer()) << "\",\n";
            json << "      \"description\": \"" << escapeJson(awardList[i].getDescription()) << "\"\n";
            json << "    }";
        }
        json << "\n  ],\n";

        // Certifications
        Certifications& certifications = resume.getCertifications();
        std::vector<Certification> certList = certifications.getCertifications();
        json << "  \"certifications\": [";
        for (size_t i = 0; i < certList.size(); ++i) {
            if (i > 0) json << ",";
            json << "\n    {\n";
            json << "      \"name\": \"" << escapeJson(certList[i].getName()) << "\",\n";
            json << "      \"authority\": \"" << escapeJson(certList[i].getAuthority()) << "\",\n";
            json << "      \"license_number\": \"" << escapeJson(certList[i].getLicenseNumber()) << "\",\n";
            json << "      \"url\": \"" << escapeJson(certList[i].getUrl()) << "\",\n";
            json << "      \"issue_date\": \"" << escapeJson(certList[i].getIssueDate()) << "\",\n";
            json << "      \"expiration_date\": \"" << escapeJson(certList[i].getExpirationDate()) << "\",\n";
            json << "      \"does_not_expire\": " << (certList[i].isDoesNotExpire() ? "true" : "false") << "\n";
            json << "    }";
        }
        json << "\n  ]\n";

        json << "}";
        return json.str();
    }

    void saveToFile(const std::string& filename) {
        std::string jsonContent = generateJSON();
        std::ofstream file(filename);
        if (file.is_open()) {
            file << jsonContent;
            file.close();
            std::cout << "\nResume saved to " << filename << "\n";
        } else {
            std::cerr << "Error: Could not open file " << filename << " for writing.\n";
        }
    }

    void run() {
        std::cout << "=================================\n";
        std::cout << "    RESUME BUILDER CLI\n";
        std::cout << "=================================\n\n";
        std::cout << "This tool will help you create a comprehensive resume.\n";
        std::cout << "Press Enter to skip optional fields.\n\n";

        collectBasicInfo();
        collectSkills();
        collectEducation();
        collectExperiences();
        collectProjects();
        collectOtherSections();

        std::cout << "\n=== REVIEW & SAVE ===\n";
        std::cout << "Resume collection complete!\n";

        std::string filename = promptString("Enter filename for JSON output (default: resume.json): ");
        if (filename.empty()) {
            filename = "resume.json";
        }
        if (filename.find(".json") == std::string::npos) {
            filename += ".json";
        }

        saveToFile(filename);

        std::cout << "\nThank you for using Resume Builder CLI!\n";
        std::cout << "Your resume has been saved in JSON format, ready for database import.\n";
    }
};

int main() {
    ResumeBuilder builder;
    builder.run();
    return 0;
}