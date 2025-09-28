#ifndef JOBLISTINGCONTROLLER_H
#define JOBLISTINGCONTROLLER_H

#include "baseController.h"
#include "../ds/resume/jobListing.h"
#include <vector>
#include <memory>

class JobListingController : public BaseController {
public:
    JobListingController();
    ~JobListingController() = default;

    bool createJobListing(const JobListing& jobListing);
    std::unique_ptr<JobListing> getJobListingById(const std::string& jobId);
    std::unique_ptr<JobListing> getJobListingByIdInt(int id);
    std::vector<std::unique_ptr<JobListing>> getAllJobListings();
    std::vector<std::unique_ptr<JobListing>> getJobListingsByCompany(const std::string& company);
    std::vector<std::unique_ptr<JobListing>> getJobListingsByLocation(const std::string& location);
    std::vector<std::unique_ptr<JobListing>> getJobListingsByType(JobType jobType);
    std::vector<std::unique_ptr<JobListing>> getJobListingsByExperienceLevel(JobExperienceLevel level);
    std::vector<std::unique_ptr<JobListing>> getJobListingsBySalaryRange(float minSalary, float maxSalary);
    std::vector<std::unique_ptr<JobListing>> getActiveJobListings();

    bool updateJobListing(const JobListing& jobListing);
    bool updateJobListingStatus(const std::string& jobId, bool isActive);
    bool updateJobListingSalary(const std::string& jobId, float minSalary, float maxSalary, const std::string& currency);

    bool deleteJobListing(const std::string& jobId);
    bool deleteJobListingsByCompany(const std::string& company);

    int getJobListingCount() const;
    int getActiveJobListingCount() const;
    int getJobListingCountByCompany(const std::string& company) const;

    std::string exportToJson() override;
    std::string exportToCsv() override;
    bool importFromJson(const std::string& jsonData) override;
    bool importFromCsv(const std::string& csvData);

    bool jobListingExists(const std::string& jobId) const;

private:
    JobListing* mapResultToJobListing(DatabaseResult& result);
    bool insertJobRequiredSkills(int jobListingId, const Skills& skills);
    bool insertJobPreferredSkills(int jobListingId, const Skills& skills);
    bool deleteJobRequiredSkills(int jobListingId);
    bool deleteJobPreferredSkills(int jobListingId);
    Skills getJobRequiredSkills(int jobListingId);
    Skills getJobPreferredSkills(int jobListingId);
    std::string jobListingToJsonString(const JobListing& jobListing, bool includeComma = true);
};

#endif // JOBLISTINGCONTROLLER_H