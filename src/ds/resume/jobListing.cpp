#include "jobListing.h"
#include <algorithm>

// Constructors
JobListing::JobListing()
    : jobId(""), title(""), company(""), description(""), location(""),
      remoteType(RemoteType::ON_SITE), jobType(JobType::FULL_TIME),
      experienceLevel(JobExperienceLevel::ENTRY_LEVEL), salaryMin(0.0f),
      salaryMax(0.0f), salaryCurrency("USD"), minimumYearsExperience(0),
      applicationDeadline(""), postedDate(""), applicationUrl(""),
      contactEmail(""), companySize(""), industry(""), companyWebsite(""),
      isActive(true), department(""), reportingTo("") {}

JobListing::JobListing(const std::string& jobId, const std::string& title,
                       const std::string& company, const std::string& description,
                       const std::string& location, RemoteType remoteType,
                       JobType jobType, JobExperienceLevel experienceLevel)
    : jobId(jobId), title(title), company(company), description(description),
      location(location), remoteType(remoteType), jobType(jobType),
      experienceLevel(experienceLevel), salaryMin(0.0f), salaryMax(0.0f),
      salaryCurrency("USD"), minimumYearsExperience(0), applicationDeadline(""),
      postedDate(""), applicationUrl(""), contactEmail(""), companySize(""),
      industry(""), companyWebsite(""), isActive(true), department(""),
      reportingTo("") {}

JobListing::~JobListing() {}

// Basic getters
std::string JobListing::getJobId() const { return jobId; }
std::string JobListing::getTitle() const { return title; }
std::string JobListing::getCompany() const { return company; }
std::string JobListing::getDescription() const { return description; }
std::string JobListing::getLocation() const { return location; }
RemoteType JobListing::getRemoteType() const { return remoteType; }
JobType JobListing::getJobType() const { return jobType; }
JobExperienceLevel JobListing::getExperienceLevel() const { return experienceLevel; }

// Salary getters
float JobListing::getSalaryMin() const { return salaryMin; }
float JobListing::getSalaryMax() const { return salaryMax; }
std::string JobListing::getSalaryCurrency() const { return salaryCurrency; }

// Required qualifications getters
Skills& JobListing::getRequiredSkills() { return requiredSkills; }
const Skills& JobListing::getRequiredSkills() const { return requiredSkills; }
std::vector<std::string> JobListing::getRequiredEducation() const { return requiredEducation; }
std::vector<Experience> JobListing::getRequiredExperiences() const { return requiredExperiences; }
int JobListing::getMinimumYearsExperience() const { return minimumYearsExperience; }

// Preferred qualifications getters
Skills& JobListing::getPreferredSkills() { return preferredSkills; }
const Skills& JobListing::getPreferredSkills() const { return preferredSkills; }
std::vector<std::string> JobListing::getPreferredEducation() const { return preferredEducation; }
std::vector<Experience> JobListing::getPreferredExperiences() const { return preferredExperiences; }
std::vector<std::string> JobListing::getPreferredCertifications() const { return preferredCertifications; }

// Benefits and details getters
std::vector<std::string> JobListing::getBenefits() const { return benefits; }
std::vector<std::string> JobListing::getResponsibilities() const { return responsibilities; }
std::string JobListing::getApplicationDeadline() const { return applicationDeadline; }
std::string JobListing::getPostedDate() const { return postedDate; }
std::string JobListing::getApplicationUrl() const { return applicationUrl; }
std::string JobListing::getContactEmail() const { return contactEmail; }

// Company information getters
std::string JobListing::getCompanySize() const { return companySize; }
std::string JobListing::getIndustry() const { return industry; }
std::string JobListing::getCompanyWebsite() const { return companyWebsite; }

// Additional metadata getters
bool JobListing::getIsActive() const { return isActive; }
std::string JobListing::getDepartment() const { return department; }
std::string JobListing::getReportingTo() const { return reportingTo; }
std::vector<std::string> JobListing::getTags() const { return tags; }

// Basic setters
void JobListing::setJobId(const std::string& jobId) { this->jobId = jobId; }
void JobListing::setTitle(const std::string& title) { this->title = title; }
void JobListing::setCompany(const std::string& company) { this->company = company; }
void JobListing::setDescription(const std::string& description) { this->description = description; }
void JobListing::setLocation(const std::string& location) { this->location = location; }
void JobListing::setRemoteType(RemoteType remoteType) { this->remoteType = remoteType; }
void JobListing::setJobType(JobType jobType) { this->jobType = jobType; }
void JobListing::setExperienceLevel(JobExperienceLevel experienceLevel) { this->experienceLevel = experienceLevel; }

// Salary setters
void JobListing::setSalaryRange(float min, float max, const std::string& currency) {
    this->salaryMin = min;
    this->salaryMax = max;
    this->salaryCurrency = currency;
}

void JobListing::setSalaryMin(float salaryMin) { this->salaryMin = salaryMin; }
void JobListing::setSalaryMax(float salaryMax) { this->salaryMax = salaryMax; }
void JobListing::setSalaryCurrency(const std::string& currency) { this->salaryCurrency = currency; }

// Required qualifications setters
void JobListing::setRequiredSkills(const Skills& skills) { this->requiredSkills = skills; }

void JobListing::addRequiredEducation(const std::string& education) {
    requiredEducation.push_back(education);
}

void JobListing::addRequiredExperience(const Experience& experience) {
    requiredExperiences.push_back(experience);
}

void JobListing::setMinimumYearsExperience(int years) { this->minimumYearsExperience = years; }

// Preferred qualifications setters
void JobListing::setPreferredSkills(const Skills& skills) { this->preferredSkills = skills; }

void JobListing::addPreferredEducation(const std::string& education) {
    preferredEducation.push_back(education);
}

void JobListing::addPreferredExperience(const Experience& experience) {
    preferredExperiences.push_back(experience);
}

void JobListing::addPreferredCertification(const std::string& certification) {
    preferredCertifications.push_back(certification);
}

// Benefits and details setters
void JobListing::addBenefit(const std::string& benefit) {
    benefits.push_back(benefit);
}

void JobListing::addResponsibility(const std::string& responsibility) {
    responsibilities.push_back(responsibility);
}

void JobListing::setApplicationDeadline(const std::string& deadline) { this->applicationDeadline = deadline; }
void JobListing::setPostedDate(const std::string& date) { this->postedDate = date; }
void JobListing::setApplicationUrl(const std::string& url) { this->applicationUrl = url; }
void JobListing::setContactEmail(const std::string& email) { this->contactEmail = email; }

// Company information setters
void JobListing::setCompanySize(const std::string& size) { this->companySize = size; }
void JobListing::setIndustry(const std::string& industry) { this->industry = industry; }
void JobListing::setCompanyWebsite(const std::string& website) { this->companyWebsite = website; }

// Additional metadata setters
void JobListing::setIsActive(bool active) { this->isActive = active; }
void JobListing::setDepartment(const std::string& department) { this->department = department; }
void JobListing::setReportingTo(const std::string& reportingTo) { this->reportingTo = reportingTo; }

void JobListing::addTag(const std::string& tag) {
    tags.push_back(tag);
}

// Utility methods
void JobListing::clearRequiredEducation() { requiredEducation.clear(); }
void JobListing::clearRequiredExperiences() { requiredExperiences.clear(); }
void JobListing::clearPreferredEducation() { preferredEducation.clear(); }
void JobListing::clearPreferredExperiences() { preferredExperiences.clear(); }
void JobListing::clearPreferredCertifications() { preferredCertifications.clear(); }
void JobListing::clearBenefits() { benefits.clear(); }
void JobListing::clearResponsibilities() { responsibilities.clear(); }
void JobListing::clearTags() { tags.clear(); }

// Validation methods
bool JobListing::hasRequiredSkill(const Skill& skill) const {
    return requiredSkills.hasSkill(skill);
}

bool JobListing::hasPreferredSkill(const Skill& skill) const {
    return preferredSkills.hasSkill(skill);
}

bool JobListing::meetsMinimumExperience(int candidateYears) const {
    return candidateYears >= minimumYearsExperience;
}

// Comparison operators
bool JobListing::operator==(const JobListing& other) const {
    return jobId == other.jobId && title == other.title && company == other.company;
}

bool JobListing::operator!=(const JobListing& other) const {
    return !(*this == other);
}

// Helper methods for enum conversion
std::string JobListing::jobTypeToString(JobType type) {
    switch (type) {
        case JobType::FULL_TIME: return "Full-time";
        case JobType::PART_TIME: return "Part-time";
        case JobType::CONTRACT: return "Contract";
        case JobType::INTERNSHIP: return "Internship";
        case JobType::FREELANCE: return "Freelance";
        default: return "Unknown";
    }
}

std::string JobListing::experienceLevelToString(JobExperienceLevel level) {
    switch (level) {
        case JobExperienceLevel::ENTRY_LEVEL: return "Entry Level";
        case JobExperienceLevel::JUNIOR: return "Junior";
        case JobExperienceLevel::MID_LEVEL: return "Mid Level";
        case JobExperienceLevel::SENIOR: return "Senior";
        case JobExperienceLevel::LEAD: return "Lead";
        case JobExperienceLevel::EXECUTIVE: return "Executive";
        default: return "Unknown";
    }
}

std::string JobListing::remoteTypeToString(RemoteType type) {
    switch (type) {
        case RemoteType::ON_SITE: return "On-site";
        case RemoteType::REMOTE: return "Remote";
        case RemoteType::HYBRID: return "Hybrid";
        default: return "Unknown";
    }
}

JobType JobListing::stringToJobType(const std::string& str) {
    if (str == "Full-time" || str == "FULL_TIME") return JobType::FULL_TIME;
    if (str == "Part-time" || str == "PART_TIME") return JobType::PART_TIME;
    if (str == "Contract" || str == "CONTRACT") return JobType::CONTRACT;
    if (str == "Internship" || str == "INTERNSHIP") return JobType::INTERNSHIP;
    if (str == "Freelance" || str == "FREELANCE") return JobType::FREELANCE;
    return JobType::FULL_TIME; // Default
}

JobExperienceLevel JobListing::stringToExperienceLevel(const std::string& str) {
    if (str == "Entry Level" || str == "ENTRY_LEVEL") return JobExperienceLevel::ENTRY_LEVEL;
    if (str == "Junior" || str == "JUNIOR") return JobExperienceLevel::JUNIOR;
    if (str == "Mid Level" || str == "MID_LEVEL") return JobExperienceLevel::MID_LEVEL;
    if (str == "Senior" || str == "SENIOR") return JobExperienceLevel::SENIOR;
    if (str == "Lead" || str == "LEAD") return JobExperienceLevel::LEAD;
    if (str == "Executive" || str == "EXECUTIVE") return JobExperienceLevel::EXECUTIVE;
    return JobExperienceLevel::ENTRY_LEVEL; // Default
}

RemoteType JobListing::stringToRemoteType(const std::string& str) {
    if (str == "On-site" || str == "ON_SITE") return RemoteType::ON_SITE;
    if (str == "Remote" || str == "REMOTE") return RemoteType::REMOTE;
    if (str == "Hybrid" || str == "HYBRID") return RemoteType::HYBRID;
    return RemoteType::ON_SITE; // Default
}