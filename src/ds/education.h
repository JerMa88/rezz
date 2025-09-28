#include <string>
#include <vector>
class Education {
private:
    std::string institutionName;
    std::string degree;
    std::string fieldOfStudy;
    std::string startDate; // Format: "YYYY-MM"
    std::string endDate;   // Format: "YYYY-MM" or "Present"
    float gpa;           // Optional, can be -1 if not provided
    std::vector<std::string> courses; // List of relevant courses
public:
    Education(const std::string& institutionName, const std::string& degree,
              const std::string& fieldOfStudy, const std::string& startDate,
              const std::string& endDate, float gpa = -1.0f,
              const std::vector<std::string>& courses = {});
    ~Education();

    // Getters
    std::string getInstitutionName() const;
    std::string getDegree() const;
    std::string getFieldOfStudy() const;
    std::string getStartDate() const;
    std::string getEndDate() const;
    float getGPA() const;
    std::vector<std::string> getCourses() const;

    // Setters
    void setInstitutionName(const std::string& name);
    void setDegree(const std::string& degree);
    void setFieldOfStudy(const std::string& field);
    void setStartDate(const std::string& date);
    void setEndDate(const std::string& date);
    void setGPA(float gpa);
    void addCourse(const std::string& course);
};