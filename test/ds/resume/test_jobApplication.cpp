#include <iostream>
#include <cassert>
#include "../../../src/ds/resume/jobApplication.h"

void testJobApplicationConstructors() {
    std::cout << "Testing JobApplication constructors..." << std::endl;

    JobApplication app1;
    assert(app1.getApplicationId().empty());
    assert(app1.getJobId().empty());
    assert(app1.getJobTitle().empty());
    assert(app1.getCompany().empty());
    assert(app1.getStatus() == ApplicationStatus::APPLIED);

    JobApplication app2("APP_123", "JOB_456", "Software Engineer", "TechCorp", "2024-01-15");
    assert(app2.getApplicationId() == "APP_123");
    assert(app2.getJobId() == "JOB_456");
    assert(app2.getJobTitle() == "Software Engineer");
    assert(app2.getCompany() == "TechCorp");
    assert(app2.getDateApplied() == "2024-01-15");
    assert(app2.getStatus() == ApplicationStatus::APPLIED);

    std::cout << "✓ Constructor tests passed" << std::endl;
}

void testJobApplicationSettersAndGetters() {
    std::cout << "Testing JobApplication setters and getters..." << std::endl;

    JobApplication app;

    app.setApplicationId("APP_789");
    assert(app.getApplicationId() == "APP_789");

    app.setJobId("JOB_101");
    assert(app.getJobId() == "JOB_101");

    app.setJobTitle("Data Scientist");
    assert(app.getJobTitle() == "Data Scientist");

    app.setCompany("DataCorp");
    assert(app.getCompany() == "DataCorp");

    app.setDateApplied("2024-02-20");
    assert(app.getDateApplied() == "2024-02-20");

    app.setStatus(ApplicationStatus::INTERVIEW_SCHEDULED);
    assert(app.getStatus() == ApplicationStatus::INTERVIEW_SCHEDULED);

    app.setContactName("John Smith");
    assert(app.getContactName() == "John Smith");

    app.setContactEmail("john.smith@datacorp.com");
    assert(app.getContactEmail() == "john.smith@datacorp.com");

    app.setContactPhone("+1-555-0123");
    assert(app.getContactPhone() == "+1-555-0123");

    app.setComments("Applied through LinkedIn");
    assert(app.getComments() == "Applied through LinkedIn");

    app.setApplicationUrl("https://datacorp.com/careers/123");
    assert(app.getApplicationUrl() == "https://datacorp.com/careers/123");

    app.setSalaryOffered("120000");
    assert(app.getSalaryOffered() == "120000");

    app.setExpectedSalary("110000");
    assert(app.getExpectedSalary() == "110000");

    app.setResponseDeadline("2024-03-15");
    assert(app.getResponseDeadline() == "2024-03-15");

    app.setReferralSource("LinkedIn");
    assert(app.getReferralSource() == "LinkedIn");

    app.setApplicationMethod("Online Portal");
    assert(app.getApplicationMethod() == "Online Portal");

    app.setNotes("Promising opportunity");
    assert(app.getNotes() == "Promising opportunity");

    std::cout << "✓ Setter and getter tests passed" << std::endl;
}

void testInterviewAndFollowUpDates() {
    std::cout << "Testing interview and follow-up date management..." << std::endl;

    JobApplication app;

    app.addInterviewDate("2024-03-01");
    app.addInterviewDate("2024-03-05");

    auto interviewDates = app.getInterviewDates();
    assert(interviewDates.size() == 2);
    assert(interviewDates[0] == "2024-03-01");
    assert(interviewDates[1] == "2024-03-05");

    app.addFollowUpDate("2024-03-10");
    app.addFollowUpDate("2024-03-20");

    auto followUpDates = app.getFollowUpDates();
    assert(followUpDates.size() == 2);
    assert(followUpDates[0] == "2024-03-10");
    assert(followUpDates[1] == "2024-03-20");

    app.clearInterviewDates();
    assert(app.getInterviewDates().empty());

    app.clearFollowUpDates();
    assert(app.getFollowUpDates().empty());

    std::cout << "✓ Interview and follow-up date tests passed" << std::endl;
}

void testStatusConversion() {
    std::cout << "Testing status string conversion..." << std::endl;

    assert(JobApplication::statusToString(ApplicationStatus::APPLIED) == "APPLIED");
    assert(JobApplication::statusToString(ApplicationStatus::REVIEWING) == "REVIEWING");
    assert(JobApplication::statusToString(ApplicationStatus::INTERVIEW_SCHEDULED) == "INTERVIEW_SCHEDULED");
    assert(JobApplication::statusToString(ApplicationStatus::INTERVIEWING) == "INTERVIEWING");
    assert(JobApplication::statusToString(ApplicationStatus::WAITING_RESPONSE) == "WAITING_RESPONSE");
    assert(JobApplication::statusToString(ApplicationStatus::OFFER_RECEIVED) == "OFFER_RECEIVED");
    assert(JobApplication::statusToString(ApplicationStatus::REJECTED) == "REJECTED");
    assert(JobApplication::statusToString(ApplicationStatus::WITHDRAWN) == "WITHDRAWN");
    assert(JobApplication::statusToString(ApplicationStatus::ACCEPTED) == "ACCEPTED");

    assert(JobApplication::stringToStatus("APPLIED") == ApplicationStatus::APPLIED);
    assert(JobApplication::stringToStatus("REVIEWING") == ApplicationStatus::REVIEWING);
    assert(JobApplication::stringToStatus("INTERVIEW_SCHEDULED") == ApplicationStatus::INTERVIEW_SCHEDULED);
    assert(JobApplication::stringToStatus("INTERVIEWING") == ApplicationStatus::INTERVIEWING);
    assert(JobApplication::stringToStatus("WAITING_RESPONSE") == ApplicationStatus::WAITING_RESPONSE);
    assert(JobApplication::stringToStatus("OFFER_RECEIVED") == ApplicationStatus::OFFER_RECEIVED);
    assert(JobApplication::stringToStatus("REJECTED") == ApplicationStatus::REJECTED);
    assert(JobApplication::stringToStatus("WITHDRAWN") == ApplicationStatus::WITHDRAWN);
    assert(JobApplication::stringToStatus("ACCEPTED") == ApplicationStatus::ACCEPTED);

    assert(JobApplication::stringToStatus("INVALID") == ApplicationStatus::APPLIED);

    std::cout << "✓ Status conversion tests passed" << std::endl;
}

void testEqualityOperators() {
    std::cout << "Testing equality operators..." << std::endl;

    JobApplication app1("APP_123", "JOB_456", "Software Engineer", "TechCorp", "2024-01-15");
    JobApplication app2("APP_123", "JOB_789", "Data Scientist", "DataCorp", "2024-02-20");
    JobApplication app3("APP_456", "JOB_456", "Software Engineer", "TechCorp", "2024-01-15");

    assert(app1 == app2);
    assert(app1 != app3);
    assert(app2 != app3);

    std::cout << "✓ Equality operator tests passed" << std::endl;
}

int main() {
    std::cout << "=== JobApplication Unit Tests ===" << std::endl;

    testJobApplicationConstructors();
    testJobApplicationSettersAndGetters();
    testInterviewAndFollowUpDates();
    testStatusConversion();
    testEqualityOperators();

    std::cout << "\n✓ All JobApplication tests passed successfully!" << std::endl;
    return 0;
}