#ifndef JOBLISTING_H
#define JOBLISTING_H

#include <string>
#include <vector>
#include "skills.h"
#include "experiences.h"

enum class JobType {
    FULL_TIME = 1,
    PART_TIME = 2,
    CONTRACT = 3,
    INTERNSHIP = 4,
    FREELANCE = 5
};

enum class JobExperienceLevel {
    ENTRY_LEVEL = 1,
    JUNIOR = 2,
    MID_LEVEL = 3,
    SENIOR = 4,
    LEAD = 5,
    EXECUTIVE = 6
};

enum class RemoteType {
    ON_SITE = 1,
    REMOTE = 2,
    HYBRID = 3
};

class JobListing {
private:
    std::string jobId;
    std::string title;
    std::string company;
    std::string description;
    std::string location;
    RemoteType remoteType;
    JobType jobType;
    JobExperienceLevel experienceLevel;

    // Salary information
    float salaryMin;
    float salaryMax;
    std::string salaryCurrency;

    // Required qualifications
    Skills requiredSkills;
    std::vector<std::string> requiredEducation; // e.g., "Bachelor's in Computer Science"
    std::vector<Experience> requiredExperiences;
    int minimumYearsExperience;

    // Preferred qualifications
    Skills preferredSkills;
    std::vector<std::string> preferredEducation;
    std::vector<Experience> preferredExperiences;
    std::vector<std::string> preferredCertifications;

    // Benefits and other details
    std::vector<std::string> benefits;
    std::vector<std::string> responsibilities;
    std::string applicationDeadline; // Format: "YYYY-MM-DD"
    std::string postedDate; // Format: "YYYY-MM-DD"
    std::string applicationUrl;
    std::string contactEmail;

    // Company information
    std::string companySize; // e.g., "50-200 employees"
    std::string industry;
    std::string companyWebsite;

    // Additional metadata
    bool isActive;
    std::string department;
    std::string reportingTo;
    std::vector<std::string> tags; // e.g., "startup", "fintech", "AI"

public:
    // Constructors
    JobListing();
    JobListing(const std::string& jobId, const std::string& title, const std::string& company,
               const std::string& description, const std::string& location, RemoteType remoteType,
               JobType jobType, JobExperienceLevel experienceLevel);
    ~JobListing();

    // Basic getters
    std::string getJobId() const;
    std::string getTitle() const;
    std::string getCompany() const;
    std::string getDescription() const;
    std::string getLocation() const;
    RemoteType getRemoteType() const;
    JobType getJobType() const;
    JobExperienceLevel getExperienceLevel() const;

    // Salary getters
    float getSalaryMin() const;
    float getSalaryMax() const;
    std::string getSalaryCurrency() const;

    // Required qualifications getters
    Skills& getRequiredSkills();
    const Skills& getRequiredSkills() const;
    std::vector<std::string> getRequiredEducation() const;
    std::vector<Experience> getRequiredExperiences() const;
    int getMinimumYearsExperience() const;

    // Preferred qualifications getters
    Skills& getPreferredSkills();
    const Skills& getPreferredSkills() const;
    std::vector<std::string> getPreferredEducation() const;
    std::vector<Experience> getPreferredExperiences() const;
    std::vector<std::string> getPreferredCertifications() const;

    // Benefits and details getters
    std::vector<std::string> getBenefits() const;
    std::vector<std::string> getResponsibilities() const;
    std::string getApplicationDeadline() const;
    std::string getPostedDate() const;
    std::string getApplicationUrl() const;
    std::string getContactEmail() const;

    // Company information getters
    std::string getCompanySize() const;
    std::string getIndustry() const;
    std::string getCompanyWebsite() const;

    // Additional metadata getters
    bool getIsActive() const;
    std::string getDepartment() const;
    std::string getReportingTo() const;
    std::vector<std::string> getTags() const;

    // Basic setters
    void setJobId(const std::string& jobId);
    void setTitle(const std::string& title);
    void setCompany(const std::string& company);
    void setDescription(const std::string& description);
    void setLocation(const std::string& location);
    void setRemoteType(RemoteType remoteType);
    void setJobType(JobType jobType);
    void setExperienceLevel(JobExperienceLevel experienceLevel);

    // Salary setters
    void setSalaryRange(float min, float max, const std::string& currency = "USD");
    void setSalaryMin(float salaryMin);
    void setSalaryMax(float salaryMax);
    void setSalaryCurrency(const std::string& currency);

    // Required qualifications setters
    void setRequiredSkills(const Skills& skills);
    void addRequiredEducation(const std::string& education);
    void addRequiredExperience(const Experience& experience);
    void setMinimumYearsExperience(int years);

    // Preferred qualifications setters
    void setPreferredSkills(const Skills& skills);
    void addPreferredEducation(const std::string& education);
    void addPreferredExperience(const Experience& experience);
    void addPreferredCertification(const std::string& certification);

    // Benefits and details setters
    void addBenefit(const std::string& benefit);
    void addResponsibility(const std::string& responsibility);
    void setApplicationDeadline(const std::string& deadline);
    void setPostedDate(const std::string& date);
    void setApplicationUrl(const std::string& url);
    void setContactEmail(const std::string& email);

    // Company information setters
    void setCompanySize(const std::string& size);
    void setIndustry(const std::string& industry);
    void setCompanyWebsite(const std::string& website);

    // Additional metadata setters
    void setIsActive(bool active);
    void setDepartment(const std::string& department);
    void setReportingTo(const std::string& reportingTo);
    void addTag(const std::string& tag);

    // Utility methods
    void clearRequiredEducation();
    void clearRequiredExperiences();
    void clearPreferredEducation();
    void clearPreferredExperiences();
    void clearPreferredCertifications();
    void clearBenefits();
    void clearResponsibilities();
    void clearTags();

    // Validation methods
    bool hasRequiredSkill(const Skill& skill) const;
    bool hasPreferredSkill(const Skill& skill) const;
    bool meetsMinimumExperience(int candidateYears) const;

    // Comparison operators
    bool operator==(const JobListing& other) const;
    bool operator!=(const JobListing& other) const;

    // Helper methods for enum conversion
    static std::string jobTypeToString(JobType type);
    static std::string experienceLevelToString(JobExperienceLevel level);
    static std::string remoteTypeToString(RemoteType type);
    static JobType stringToJobType(const std::string& str);
    static JobExperienceLevel stringToExperienceLevel(const std::string& str);
    static RemoteType stringToRemoteType(const std::string& str);
};

#endif // JOBLISTING_H