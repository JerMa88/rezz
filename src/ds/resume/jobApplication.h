#ifndef JOBAPPLICATION_H
#define JOBAPPLICATION_H

#include <string>
#include <vector>

enum class ApplicationStatus {
    APPLIED = 1,
    REVIEWING = 2,
    INTERVIEW_SCHEDULED = 3,
    INTERVIEWING = 4,
    WAITING_RESPONSE = 5,
    OFFER_RECEIVED = 6,
    REJECTED = 7,
    WITHDRAWN = 8,
    ACCEPTED = 9
};

class JobApplication {
private:
    std::string applicationId;
    std::string jobId;
    std::string jobTitle;
    std::string company;
    std::string dateApplied;
    ApplicationStatus status;

    std::string contactName;
    std::string contactEmail;
    std::string contactPhone;

    std::string comments;
    std::string applicationUrl;
    std::string salaryOffered;
    std::string expectedSalary;

    std::vector<std::string> interviewDates;
    std::vector<std::string> followUpDates;
    std::string responseDeadline;

    std::string referralSource;
    std::string applicationMethod;
    std::string notes;

public:
    JobApplication();
    JobApplication(const std::string& applicationId, const std::string& jobId,
                   const std::string& jobTitle, const std::string& company,
                   const std::string& dateApplied);
    ~JobApplication();

    std::string getApplicationId() const;
    std::string getJobId() const;
    std::string getJobTitle() const;
    std::string getCompany() const;
    std::string getDateApplied() const;
    ApplicationStatus getStatus() const;
    std::string getContactName() const;
    std::string getContactEmail() const;
    std::string getContactPhone() const;
    std::string getComments() const;
    std::string getApplicationUrl() const;
    std::string getSalaryOffered() const;
    std::string getExpectedSalary() const;
    std::vector<std::string> getInterviewDates() const;
    std::vector<std::string> getFollowUpDates() const;
    std::string getResponseDeadline() const;
    std::string getReferralSource() const;
    std::string getApplicationMethod() const;
    std::string getNotes() const;

    void setApplicationId(const std::string& applicationId);
    void setJobId(const std::string& jobId);
    void setJobTitle(const std::string& jobTitle);
    void setCompany(const std::string& company);
    void setDateApplied(const std::string& dateApplied);
    void setStatus(ApplicationStatus status);
    void setContactName(const std::string& contactName);
    void setContactEmail(const std::string& contactEmail);
    void setContactPhone(const std::string& contactPhone);
    void setComments(const std::string& comments);
    void setApplicationUrl(const std::string& applicationUrl);
    void setSalaryOffered(const std::string& salaryOffered);
    void setExpectedSalary(const std::string& expectedSalary);
    void setResponseDeadline(const std::string& responseDeadline);
    void setReferralSource(const std::string& referralSource);
    void setApplicationMethod(const std::string& applicationMethod);
    void setNotes(const std::string& notes);

    void addInterviewDate(const std::string& interviewDate);
    void addFollowUpDate(const std::string& followUpDate);
    void clearInterviewDates();
    void clearFollowUpDates();

    bool operator==(const JobApplication& other) const;
    bool operator!=(const JobApplication& other) const;

    static std::string statusToString(ApplicationStatus status);
    static ApplicationStatus stringToStatus(const std::string& str);
};

#endif // JOBAPPLICATION_H