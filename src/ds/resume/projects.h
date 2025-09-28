#include <string>
#include <vector>
#include "skill.h"

class Project {
private:
    std::string name;
    std::string description;
    std::string url; // Optional URL to the project
    std::vector<Skill> skillsUsed; // List of skills used in the project
    std::string course;
    std::string startDate; // Date or duration of the project
    std::string endDate;   // Date or duration of the project
public:
    // Default constructor
    Project() : name(""), description(""), url(""), course(""), startDate(""), endDate("") {}
    // Parameterized constructor
    Project(const std::string& name, const std::string& description, const std::string& url,
            const std::vector<Skill>& skillsUsed, const std::string& course,
            const std::string& startDate, const std::string& endDate)
        : name(name), description(description), url(url), skillsUsed(skillsUsed),
          course(course), startDate(startDate), endDate(endDate) {}
    // Getters
    std::string getName() const { return name; }
    std::string getDescription() const { return description; }
    std::string getUrl() const { return url; }
    std::vector<Skill> getSkillsUsed() const { return skillsUsed; }
    std::string getCourse() const { return course; }
    std::string getStartDate() const { return startDate; }
    std::string getEndDate() const { return endDate; }
    // Setters
    void setName(const std::string& name) { this->name = name; }
    void setDescription(const std::string& description) { this->description = description; }
    void setUrl(const std::string& url) { this->url = url; }
    void setSkillsUsed(const std::vector<Skill>& skillsUsed) { this->skillsUsed = skillsUsed; }
    void setCourse(const std::string& course) { this->course = course; }
    void setStartDate(const std::string& startDate) { this->startDate = startDate; }
    void setEndDate(const std::string& endDate) { this->endDate = endDate; }
    // Equality operator
    bool operator==(const Project& other) const {
        return name == other.name && description == other.description &&
               url == other.url && skillsUsed == other.skillsUsed &&
               course == other.course && startDate == other.startDate &&
               endDate == other.endDate;
    }
};

class Projects {
private:
    std::vector<Project> projects;
public:
    void addProject(const Project& project) {
        projects.push_back(project);
    }
    std::vector<Project> getProjects() const {
        return projects;
    }
    void clearProjects() {
        projects.clear();
    }
    void removeProject(const std::string& projectName) {
        projects.erase(std::remove_if(projects.begin(), projects.end(),
                                      [&projectName](const Project& p) { return p.getName() == projectName; }),
                       projects.end());
    }
};