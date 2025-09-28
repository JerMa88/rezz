#include <string>
#include <vector>
#include "education.h"

class Educations {
private:
    std::vector<Education> educations;
public:
    void addEducation(const Education& education) {
        educations.push_back(education);
    }
    std::vector<Education> getEducations() const {
        return educations;
    }
    void clearEducations() {
        educations.clear();
    }
    void removeEducation(const std::string& institutionName) {
        educations.erase(std::remove_if(educations.begin(), educations.end(),
        [&institutionName](const Education& edu) { return edu.getInstitutionName() == institutionName; }),
        educations.end());
    }
};