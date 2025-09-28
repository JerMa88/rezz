#include "jobApplication.h"
#include <algorithm>

JobApplication::JobApplication()
    : applicationId(""), jobId(""), jobTitle(""), company(""),
      dateApplied(""), status(ApplicationStatus::APPLIED),
      contactName(""), contactEmail(""), contactPhone(""),
      comments(""), applicationUrl(""), salaryOffered(""),
      expectedSalary(""), responseDeadline(""), referralSource(""),
      applicationMethod(""), notes("") {}

JobApplication::JobApplication(const std::string& applicationId, const std::string& jobId,
                               const std::string& jobTitle, const std::string& company,
                               const std::string& dateApplied)
    : applicationId(applicationId), jobId(jobId), jobTitle(jobTitle),
      company(company), dateApplied(dateApplied), status(ApplicationStatus::APPLIED),
      contactName(""), contactEmail(""), contactPhone(""),
      comments(""), applicationUrl(""), salaryOffered(""),
      expectedSalary(""), responseDeadline(""), referralSource(""),
      applicationMethod(""), notes("") {}

JobApplication::~JobApplication() {}

std::string JobApplication::getApplicationId() const { return applicationId; }
std::string JobApplication::getJobId() const { return jobId; }
std::string JobApplication::getJobTitle() const { return jobTitle; }
std::string JobApplication::getCompany() const { return company; }
std::string JobApplication::getDateApplied() const { return dateApplied; }
ApplicationStatus JobApplication::getStatus() const { return status; }
std::string JobApplication::getContactName() const { return contactName; }
std::string JobApplication::getContactEmail() const { return contactEmail; }
std::string JobApplication::getContactPhone() const { return contactPhone; }
std::string JobApplication::getComments() const { return comments; }
std::string JobApplication::getApplicationUrl() const { return applicationUrl; }
std::string JobApplication::getSalaryOffered() const { return salaryOffered; }
std::string JobApplication::getExpectedSalary() const { return expectedSalary; }
std::vector<std::string> JobApplication::getInterviewDates() const { return interviewDates; }
std::vector<std::string> JobApplication::getFollowUpDates() const { return followUpDates; }
std::string JobApplication::getResponseDeadline() const { return responseDeadline; }
std::string JobApplication::getReferralSource() const { return referralSource; }
std::string JobApplication::getApplicationMethod() const { return applicationMethod; }
std::string JobApplication::getNotes() const { return notes; }

void JobApplication::setApplicationId(const std::string& applicationId) { this->applicationId = applicationId; }
void JobApplication::setJobId(const std::string& jobId) { this->jobId = jobId; }
void JobApplication::setJobTitle(const std::string& jobTitle) { this->jobTitle = jobTitle; }
void JobApplication::setCompany(const std::string& company) { this->company = company; }
void JobApplication::setDateApplied(const std::string& dateApplied) { this->dateApplied = dateApplied; }
void JobApplication::setStatus(ApplicationStatus status) { this->status = status; }
void JobApplication::setContactName(const std::string& contactName) { this->contactName = contactName; }
void JobApplication::setContactEmail(const std::string& contactEmail) { this->contactEmail = contactEmail; }
void JobApplication::setContactPhone(const std::string& contactPhone) { this->contactPhone = contactPhone; }
void JobApplication::setComments(const std::string& comments) { this->comments = comments; }
void JobApplication::setApplicationUrl(const std::string& applicationUrl) { this->applicationUrl = applicationUrl; }
void JobApplication::setSalaryOffered(const std::string& salaryOffered) { this->salaryOffered = salaryOffered; }
void JobApplication::setExpectedSalary(const std::string& expectedSalary) { this->expectedSalary = expectedSalary; }
void JobApplication::setResponseDeadline(const std::string& responseDeadline) { this->responseDeadline = responseDeadline; }
void JobApplication::setReferralSource(const std::string& referralSource) { this->referralSource = referralSource; }
void JobApplication::setApplicationMethod(const std::string& applicationMethod) { this->applicationMethod = applicationMethod; }
void JobApplication::setNotes(const std::string& notes) { this->notes = notes; }

void JobApplication::addInterviewDate(const std::string& interviewDate) {
    interviewDates.push_back(interviewDate);
}

void JobApplication::addFollowUpDate(const std::string& followUpDate) {
    followUpDates.push_back(followUpDate);
}

void JobApplication::clearInterviewDates() {
    interviewDates.clear();
}

void JobApplication::clearFollowUpDates() {
    followUpDates.clear();
}

bool JobApplication::operator==(const JobApplication& other) const {
    return applicationId == other.applicationId;
}

bool JobApplication::operator!=(const JobApplication& other) const {
    return !(*this == other);
}

std::string JobApplication::statusToString(ApplicationStatus status) {
    switch (status) {
        case ApplicationStatus::APPLIED: return "APPLIED";
        case ApplicationStatus::REVIEWING: return "REVIEWING";
        case ApplicationStatus::INTERVIEW_SCHEDULED: return "INTERVIEW_SCHEDULED";
        case ApplicationStatus::INTERVIEWING: return "INTERVIEWING";
        case ApplicationStatus::WAITING_RESPONSE: return "WAITING_RESPONSE";
        case ApplicationStatus::OFFER_RECEIVED: return "OFFER_RECEIVED";
        case ApplicationStatus::REJECTED: return "REJECTED";
        case ApplicationStatus::WITHDRAWN: return "WITHDRAWN";
        case ApplicationStatus::ACCEPTED: return "ACCEPTED";
        default: return "UNKNOWN";
    }
}

ApplicationStatus JobApplication::stringToStatus(const std::string& str) {
    if (str == "APPLIED") return ApplicationStatus::APPLIED;
    if (str == "REVIEWING") return ApplicationStatus::REVIEWING;
    if (str == "INTERVIEW_SCHEDULED") return ApplicationStatus::INTERVIEW_SCHEDULED;
    if (str == "INTERVIEWING") return ApplicationStatus::INTERVIEWING;
    if (str == "WAITING_RESPONSE") return ApplicationStatus::WAITING_RESPONSE;
    if (str == "OFFER_RECEIVED") return ApplicationStatus::OFFER_RECEIVED;
    if (str == "REJECTED") return ApplicationStatus::REJECTED;
    if (str == "WITHDRAWN") return ApplicationStatus::WITHDRAWN;
    if (str == "ACCEPTED") return ApplicationStatus::ACCEPTED;
    return ApplicationStatus::APPLIED;
}