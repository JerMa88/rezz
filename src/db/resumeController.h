#ifndef RESUMECONTROLLER_H
#define RESUMECONTROLLER_H

#include "baseController.h"
#include "../ds/resume/resume.h"
#include <vector>
#include <memory>

class ResumeController : public BaseController {
public:
    ResumeController();
    ~ResumeController() = default;

    bool createResume(const Resume& resume);
    std::unique_ptr<Resume> getResumeById(int resumeId);
    std::unique_ptr<Resume> getResumeByEmail(const std::string& email);
    std::vector<std::unique_ptr<Resume>> getAllResumes();
    std::vector<std::unique_ptr<Resume>> getResumesByName(const std::string& name);

    bool updateResume(const Resume& resume);
    bool updateResumeBasicInfo(int resumeId, const std::string& name, const std::string& email,
                              const std::string& city, const std::string& phone);

    bool deleteResume(int resumeId);
    bool deleteResumeByEmail(const std::string& email);

    int getResumeCount() const;

    std::string exportToJson() override;
    std::string exportToCsv() override;
    bool importFromJson(const std::string& jsonData) override;
    bool importFromCsv(const std::string& csvData);

    bool resumeExists(int resumeId) const;
    bool resumeExistsByEmail(const std::string& email) const;

private:
    Resume* mapResultToResume(DatabaseResult& result);
    bool insertResumeSkills(int resumeId, const Skills& skills);
    bool insertResumeEducation(int resumeId, const Education& education);
    bool insertResumeExperiences(int resumeId, const Experiences& experiences);
    bool deleteResumeSkills(int resumeId);
    bool deleteResumeEducation(int resumeId);
    bool deleteResumeExperiences(int resumeId);
    Skills getResumeSkills(int resumeId);
    Education getResumeEducation(int resumeId);
    Experiences getResumeExperiences(int resumeId);
    std::string resumeToJsonString(const Resume& resume, bool includeComma = true);
};

#endif // RESUMECONTROLLER_H