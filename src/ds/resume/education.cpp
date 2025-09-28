#include "education.h"

Education::Education(const std::string& institutionName, const std::string& degree,
                     const std::string& fieldOfStudy, const std::string& startDate,
                     const std::string& endDate, float gpa,
                     const std::vector<std::string>& courses)
    : institutionName(institutionName), degree(degree), fieldOfStudy(fieldOfStudy),
      startDate(startDate), endDate(endDate), gpa(gpa), courses(courses) {}
Education::~Education() {}
std::string Education::getInstitutionName() const { return institutionName; }
std::string Education::getDegree() const { return degree; }
std::string Education::getFieldOfStudy() const { return fieldOfStudy; }
std::string Education::getStartDate() const { return startDate; }
std::string Education::getEndDate() const { return endDate; }
float Education::getGPA() const { return gpa; }
std::vector<std::string> Education::getCourses() const { return courses; }
void Education::setInstitutionName(const std::string& name) { institutionName = name; }
void Education::setDegree(const std::string& degree) { this->degree = degree; }
void Education::setFieldOfStudy(const std::string& field) { fieldOfStudy = field; }
void Education::setStartDate(const std::string& date) { startDate = date; }
void Education::setEndDate(const std::string& date) { endDate = date; }
void Education::setGPA(float gpa) { this->gpa = gpa; }
void Education::addCourse(const std::string& course) { courses.push_back(course); }