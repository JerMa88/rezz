#ifndef JOBAPPLICATIONCONTROLLER_H
#define JOBAPPLICATIONCONTROLLER_H

#include "baseController.h"
#include "../ds/resume/jobApplication.h"
#include <vector>
#include <memory>

class JobApplicationController : public BaseController {
public:
    JobApplicationController();
    ~JobApplicationController() = default;

    bool createJobApplication(const JobApplication& application);
    std::unique_ptr<JobApplication> getJobApplicationById(const std::string& applicationId);
    std::vector<std::unique_ptr<JobApplication>> getAllJobApplications();
    std::vector<std::unique_ptr<JobApplication>> getJobApplicationsByCompany(const std::string& company);
    std::vector<std::unique_ptr<JobApplication>> getJobApplicationsByStatus(ApplicationStatus status);
    std::vector<std::unique_ptr<JobApplication>> getJobApplicationsByDateRange(const std::string& startDate, const std::string& endDate);

    bool updateJobApplication(const JobApplication& application);
    bool updateJobApplicationStatus(const std::string& applicationId, ApplicationStatus status);
    bool addInterviewDate(const std::string& applicationId, const std::string& interviewDate);
    bool addFollowUpDate(const std::string& applicationId, const std::string& followUpDate);

    bool deleteJobApplication(const std::string& applicationId);
    bool deleteJobApplicationsByCompany(const std::string& company);

    int getJobApplicationCount() const;
    int getJobApplicationCountByStatus(ApplicationStatus status) const;

    std::string exportToJson() override;
    std::string exportToCsv() override;
    bool importFromJson(const std::string& jsonData) override;
    bool importFromCsv(const std::string& csvData);


    bool jobApplicationExists(const std::string& applicationId) const;

private:
    JobApplication* mapResultToJobApplication(DatabaseResult& result);
    std::vector<std::string> getInterviewDates(const std::string& applicationId);
    std::vector<std::string> getFollowUpDates(const std::string& applicationId);
    bool deleteInterviewDates(const std::string& applicationId);
    bool deleteFollowUpDates(const std::string& applicationId);
    bool insertInterviewDates(const std::string& applicationId, const std::vector<std::string>& dates);
    bool insertFollowUpDates(const std::string& applicationId, const std::vector<std::string>& dates);
    std::string applicationToJsonString(const JobApplication& application, bool includeComma = true);
};

#endif // JOBAPPLICATIONCONTROLLER_H