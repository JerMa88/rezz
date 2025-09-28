#include "jobApplicationController.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <memory>

JobApplicationController::JobApplicationController() : BaseController() {}

bool JobApplicationController::createJobApplication(const JobApplication& application) {
    if (!initializeDatabase()) {
        return false;
    }

    if (jobApplicationExists(application.getApplicationId())) {
        std::cerr << "Job application with ID " << application.getApplicationId() << " already exists" << std::endl;
        return false;
    }

    if (!db->beginTransaction()) {
        return false;
    }

    std::string query = R"(
        INSERT INTO job_applications (
            application_id, job_id, job_title, company, date_applied, status,
            contact_name, contact_email, contact_phone, comments, application_url,
            salary_offered, expected_salary, response_deadline, referral_source,
            application_method, notes
        ) VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15, $16, $17)
    )";

    std::vector<std::string> params = {
        application.getApplicationId(),
        application.getJobId(),
        application.getJobTitle(),
        application.getCompany(),
        application.getDateApplied(),
        std::to_string(static_cast<int>(application.getStatus())),
        application.getContactName(),
        application.getContactEmail(),
        application.getContactPhone(),
        application.getComments(),
        application.getApplicationUrl(),
        application.getSalaryOffered(),
        application.getExpectedSalary(),
        application.getResponseDeadline(),
        application.getReferralSource(),
        application.getApplicationMethod(),
        application.getNotes()
    };

    bool success = db->executeParameterizedNonQuery(query, params);

    if (success) {
        success = insertInterviewDates(application.getApplicationId(), application.getInterviewDates()) &&
                 insertFollowUpDates(application.getApplicationId(), application.getFollowUpDates());
    }

    if (success) {
        db->commitTransaction();
    } else {
        db->rollbackTransaction();
    }

    return success;
}

std::unique_ptr<JobApplication> JobApplicationController::getJobApplicationById(const std::string& applicationId) {
    if (!initializeDatabase()) {
        return nullptr;
    }

    std::string query = "SELECT * FROM job_applications WHERE application_id = $1";
    std::vector<std::string> params = {applicationId};

    PGresult* result = db->executeParameterizedQuery(query, params);
    if (!result) {
        return nullptr;
    }

    DatabaseResult dbResult(result);
    if (dbResult.getRowCount() == 0) {
        return nullptr;
    }

    dbResult.next();
    JobApplication* app = mapResultToJobApplication(dbResult);

    if (app) {
        std::vector<std::string> interviewDates = getInterviewDates(applicationId);
        std::vector<std::string> followUpDates = getFollowUpDates(applicationId);

        for (const auto& date : interviewDates) {
            app->addInterviewDate(date);
        }

        for (const auto& date : followUpDates) {
            app->addFollowUpDate(date);
        }
    }

    return std::unique_ptr<JobApplication>(app);
}

std::vector<std::unique_ptr<JobApplication>> JobApplicationController::getAllJobApplications() {
    std::vector<std::unique_ptr<JobApplication>> applications;

    if (!initializeDatabase()) {
        return applications;
    }

    std::string query = "SELECT * FROM job_applications ORDER BY date_applied DESC";
    PGresult* result = db->executeQuery(query);

    if (!result) {
        return applications;
    }

    DatabaseResult dbResult(result);

    while (dbResult.next()) {
        JobApplication* app = mapResultToJobApplication(dbResult);
        if (app) {
            std::string applicationId = app->getApplicationId();
            std::vector<std::string> interviewDates = getInterviewDates(applicationId);
            std::vector<std::string> followUpDates = getFollowUpDates(applicationId);

            for (const auto& date : interviewDates) {
                app->addInterviewDate(date);
            }

            for (const auto& date : followUpDates) {
                app->addFollowUpDate(date);
            }

            applications.push_back(std::unique_ptr<JobApplication>(app));
        }
    }

    return applications;
}

std::vector<std::unique_ptr<JobApplication>> JobApplicationController::getJobApplicationsByCompany(const std::string& company) {
    std::vector<std::unique_ptr<JobApplication>> applications;

    if (!initializeDatabase()) {
        return applications;
    }

    std::string query = "SELECT * FROM job_applications WHERE LOWER(company) LIKE LOWER($1) ORDER BY date_applied DESC";
    std::vector<std::string> params = {"%" + company + "%"};

    PGresult* result = db->executeParameterizedQuery(query, params);
    if (!result) {
        return applications;
    }

    DatabaseResult dbResult(result);

    while (dbResult.next()) {
        JobApplication* app = mapResultToJobApplication(dbResult);
        if (app) {
            std::string applicationId = app->getApplicationId();
            std::vector<std::string> interviewDates = getInterviewDates(applicationId);
            std::vector<std::string> followUpDates = getFollowUpDates(applicationId);

            for (const auto& date : interviewDates) {
                app->addInterviewDate(date);
            }

            for (const auto& date : followUpDates) {
                app->addFollowUpDate(date);
            }

            applications.push_back(std::unique_ptr<JobApplication>(app));
        }
    }

    return applications;
}

std::vector<std::unique_ptr<JobApplication>> JobApplicationController::getJobApplicationsByStatus(ApplicationStatus status) {
    std::vector<std::unique_ptr<JobApplication>> applications;

    if (!initializeDatabase()) {
        return applications;
    }

    std::string query = "SELECT * FROM job_applications WHERE status = $1 ORDER BY date_applied DESC";
    std::vector<std::string> params = {std::to_string(static_cast<int>(status))};

    PGresult* result = db->executeParameterizedQuery(query, params);
    if (!result) {
        return applications;
    }

    DatabaseResult dbResult(result);

    while (dbResult.next()) {
        JobApplication* app = mapResultToJobApplication(dbResult);
        if (app) {
            std::string applicationId = app->getApplicationId();
            std::vector<std::string> interviewDates = getInterviewDates(applicationId);
            std::vector<std::string> followUpDates = getFollowUpDates(applicationId);

            for (const auto& date : interviewDates) {
                app->addInterviewDate(date);
            }

            for (const auto& date : followUpDates) {
                app->addFollowUpDate(date);
            }

            applications.push_back(std::unique_ptr<JobApplication>(app));
        }
    }

    return applications;
}

std::vector<std::unique_ptr<JobApplication>> JobApplicationController::getJobApplicationsByDateRange(const std::string& startDate, const std::string& endDate) {
    std::vector<std::unique_ptr<JobApplication>> applications;

    if (!initializeDatabase()) {
        return applications;
    }

    std::string query = "SELECT * FROM job_applications WHERE date_applied BETWEEN $1 AND $2 ORDER BY date_applied DESC";
    std::vector<std::string> params = {startDate, endDate};

    PGresult* result = db->executeParameterizedQuery(query, params);
    if (!result) {
        return applications;
    }

    DatabaseResult dbResult(result);

    while (dbResult.next()) {
        JobApplication* app = mapResultToJobApplication(dbResult);
        if (app) {
            std::string applicationId = app->getApplicationId();
            std::vector<std::string> interviewDates = getInterviewDates(applicationId);
            std::vector<std::string> followUpDates = getFollowUpDates(applicationId);

            for (const auto& date : interviewDates) {
                app->addInterviewDate(date);
            }

            for (const auto& date : followUpDates) {
                app->addFollowUpDate(date);
            }

            applications.push_back(std::unique_ptr<JobApplication>(app));
        }
    }

    return applications;
}

bool JobApplicationController::updateJobApplication(const JobApplication& application) {
    if (!initializeDatabase()) {
        return false;
    }

    if (!jobApplicationExists(application.getApplicationId())) {
        std::cerr << "Job application with ID " << application.getApplicationId() << " does not exist" << std::endl;
        return false;
    }

    if (!db->beginTransaction()) {
        return false;
    }

    std::string query = R"(
        UPDATE job_applications SET
            job_id = $2, job_title = $3, company = $4, date_applied = $5, status = $6,
            contact_name = $7, contact_email = $8, contact_phone = $9, comments = $10,
            application_url = $11, salary_offered = $12, expected_salary = $13,
            response_deadline = $14, referral_source = $15, application_method = $16,
            notes = $17, updated_at = CURRENT_TIMESTAMP
        WHERE application_id = $1
    )";

    std::vector<std::string> params = {
        application.getApplicationId(),
        application.getJobId(),
        application.getJobTitle(),
        application.getCompany(),
        application.getDateApplied(),
        std::to_string(static_cast<int>(application.getStatus())),
        application.getContactName(),
        application.getContactEmail(),
        application.getContactPhone(),
        application.getComments(),
        application.getApplicationUrl(),
        application.getSalaryOffered(),
        application.getExpectedSalary(),
        application.getResponseDeadline(),
        application.getReferralSource(),
        application.getApplicationMethod(),
        application.getNotes()
    };

    bool success = db->executeParameterizedNonQuery(query, params);

    if (success) {
        success = deleteInterviewDates(application.getApplicationId()) &&
                 deleteFollowUpDates(application.getApplicationId()) &&
                 insertInterviewDates(application.getApplicationId(), application.getInterviewDates()) &&
                 insertFollowUpDates(application.getApplicationId(), application.getFollowUpDates());
    }

    if (success) {
        db->commitTransaction();
    } else {
        db->rollbackTransaction();
    }

    return success;
}

bool JobApplicationController::updateJobApplicationStatus(const std::string& applicationId, ApplicationStatus status) {
    if (!initializeDatabase()) {
        return false;
    }

    std::string query = "UPDATE job_applications SET status = $2, updated_at = CURRENT_TIMESTAMP WHERE application_id = $1";
    std::vector<std::string> params = {applicationId, std::to_string(static_cast<int>(status))};

    return db->executeParameterizedNonQuery(query, params);
}

bool JobApplicationController::addInterviewDate(const std::string& applicationId, const std::string& interviewDate) {
    if (!initializeDatabase()) {
        return false;
    }

    std::string query = "INSERT INTO interview_dates (application_id, interview_date) VALUES ($1, $2)";
    std::vector<std::string> params = {applicationId, interviewDate};

    return db->executeParameterizedNonQuery(query, params);
}

bool JobApplicationController::addFollowUpDate(const std::string& applicationId, const std::string& followUpDate) {
    if (!initializeDatabase()) {
        return false;
    }

    std::string query = "INSERT INTO followup_dates (application_id, followup_date) VALUES ($1, $2)";
    std::vector<std::string> params = {applicationId, followUpDate};

    return db->executeParameterizedNonQuery(query, params);
}

bool JobApplicationController::deleteJobApplication(const std::string& applicationId) {
    if (!initializeDatabase()) {
        return false;
    }

    if (!db->beginTransaction()) {
        return false;
    }

    bool success = deleteInterviewDates(applicationId) &&
                  deleteFollowUpDates(applicationId);

    if (success) {
        std::string query = "DELETE FROM job_applications WHERE application_id = $1";
        std::vector<std::string> params = {applicationId};
        success = db->executeParameterizedNonQuery(query, params);
    }

    if (success) {
        db->commitTransaction();
    } else {
        db->rollbackTransaction();
    }

    return success;
}

bool JobApplicationController::deleteJobApplicationsByCompany(const std::string& company) {
    if (!initializeDatabase()) {
        return false;
    }

    std::string query = "DELETE FROM job_applications WHERE LOWER(company) = LOWER($1)";
    std::vector<std::string> params = {company};

    return db->executeParameterizedNonQuery(query, params);
}

int JobApplicationController::getJobApplicationCount() const {
    if (!const_cast<JobApplicationController*>(this)->initializeDatabase()) {
        return 0;
    }

    std::string query = "SELECT COUNT(*) FROM job_applications";
    PGresult* result = db->executeQuery(query);

    if (!result) {
        return 0;
    }

    DatabaseResult dbResult(result);
    if (dbResult.next()) {
        return dbResult.getInt(0);
    }

    return 0;
}

int JobApplicationController::getJobApplicationCountByStatus(ApplicationStatus status) const {
    if (!const_cast<JobApplicationController*>(this)->initializeDatabase()) {
        return 0;
    }

    std::string query = "SELECT COUNT(*) FROM job_applications WHERE status = $1";
    std::vector<std::string> params = {std::to_string(static_cast<int>(status))};

    PGresult* result = db->executeParameterizedQuery(query, params);
    if (!result) {
        return 0;
    }

    DatabaseResult dbResult(result);
    if (dbResult.next()) {
        return dbResult.getInt(0);
    }

    return 0;
}

std::string JobApplicationController::exportToJson() {
    auto applications = getAllJobApplications();

    std::ostringstream json;
    json << "{\n  \"applications\": [\n";

    for (size_t i = 0; i < applications.size(); ++i) {
        json << applicationToJsonString(*applications[i], i < applications.size() - 1);
    }

    json << "  ]\n}";
    return json.str();
}

std::string JobApplicationController::exportToCsv() {
    auto applications = getAllJobApplications();

    std::ostringstream csv;
    csv << "Application ID,Job ID,Job Title,Company,Date Applied,Status,Contact Name,Contact Email,Contact Phone,";
    csv << "Comments,Application URL,Salary Offered,Expected Salary,Response Deadline,Referral Source,";
    csv << "Application Method,Notes,Interview Dates,Follow Up Dates\n";

    for (const auto& app : applications) {
        csv << escapeCsvField(app->getApplicationId()) << ",";
        csv << escapeCsvField(app->getJobId()) << ",";
        csv << escapeCsvField(app->getJobTitle()) << ",";
        csv << escapeCsvField(app->getCompany()) << ",";
        csv << escapeCsvField(app->getDateApplied()) << ",";
        csv << escapeCsvField(JobApplication::statusToString(app->getStatus())) << ",";
        csv << escapeCsvField(app->getContactName()) << ",";
        csv << escapeCsvField(app->getContactEmail()) << ",";
        csv << escapeCsvField(app->getContactPhone()) << ",";
        csv << escapeCsvField(app->getComments()) << ",";
        csv << escapeCsvField(app->getApplicationUrl()) << ",";
        csv << escapeCsvField(app->getSalaryOffered()) << ",";
        csv << escapeCsvField(app->getExpectedSalary()) << ",";
        csv << escapeCsvField(app->getResponseDeadline()) << ",";
        csv << escapeCsvField(app->getReferralSource()) << ",";
        csv << escapeCsvField(app->getApplicationMethod()) << ",";
        csv << escapeCsvField(app->getNotes()) << ",";

        const auto& interviewDates = app->getInterviewDates();
        std::string interviewStr;
        for (size_t i = 0; i < interviewDates.size(); ++i) {
            if (i > 0) interviewStr += ";";
            interviewStr += interviewDates[i];
        }
        csv << escapeCsvField(interviewStr) << ",";

        const auto& followUpDates = app->getFollowUpDates();
        std::string followUpStr;
        for (size_t i = 0; i < followUpDates.size(); ++i) {
            if (i > 0) followUpStr += ";";
            followUpStr += followUpDates[i];
        }
        csv << escapeCsvField(followUpStr) << "\n";
    }

    return csv.str();
}

bool JobApplicationController::importFromJson(const std::string& jsonData) {
    return false;
}

bool JobApplicationController::importFromCsv(const std::string& csvData) {
    return false;
}

bool JobApplicationController::jobApplicationExists(const std::string& applicationId) const {
    if (!const_cast<JobApplicationController*>(this)->initializeDatabase()) {
        return false;
    }

    std::string query = "SELECT COUNT(*) FROM job_applications WHERE application_id = $1";
    std::vector<std::string> params = {applicationId};

    PGresult* result = db->executeParameterizedQuery(query, params);
    if (!result) {
        return false;
    }

    DatabaseResult dbResult(result);
    if (dbResult.next()) {
        return dbResult.getInt(0) > 0;
    }

    return false;
}

JobApplication* JobApplicationController::mapResultToJobApplication(DatabaseResult& result) {
    JobApplication* app = new JobApplication();

    app->setApplicationId(result.getString("application_id"));
    app->setJobId(result.getString("job_id"));
    app->setJobTitle(result.getString("job_title"));
    app->setCompany(result.getString("company"));
    app->setDateApplied(result.getString("date_applied"));
    app->setStatus(static_cast<ApplicationStatus>(result.getInt("status")));
    app->setContactName(result.getString("contact_name"));
    app->setContactEmail(result.getString("contact_email"));
    app->setContactPhone(result.getString("contact_phone"));
    app->setComments(result.getString("comments"));
    app->setApplicationUrl(result.getString("application_url"));
    app->setSalaryOffered(result.getString("salary_offered"));
    app->setExpectedSalary(result.getString("expected_salary"));
    app->setResponseDeadline(result.getString("response_deadline"));
    app->setReferralSource(result.getString("referral_source"));
    app->setApplicationMethod(result.getString("application_method"));
    app->setNotes(result.getString("notes"));

    return app;
}

std::vector<std::string> JobApplicationController::getInterviewDates(const std::string& applicationId) {
    std::vector<std::string> dates;

    std::string query = "SELECT interview_date FROM interview_dates WHERE application_id = $1 ORDER BY interview_date";
    std::vector<std::string> params = {applicationId};

    PGresult* result = db->executeParameterizedQuery(query, params);
    if (!result) {
        return dates;
    }

    DatabaseResult dbResult(result);
    while (dbResult.next()) {
        dates.push_back(dbResult.getString("interview_date"));
    }

    return dates;
}

std::vector<std::string> JobApplicationController::getFollowUpDates(const std::string& applicationId) {
    std::vector<std::string> dates;

    std::string query = "SELECT followup_date FROM followup_dates WHERE application_id = $1 ORDER BY followup_date";
    std::vector<std::string> params = {applicationId};

    PGresult* result = db->executeParameterizedQuery(query, params);
    if (!result) {
        return dates;
    }

    DatabaseResult dbResult(result);
    while (dbResult.next()) {
        dates.push_back(dbResult.getString("followup_date"));
    }

    return dates;
}

bool JobApplicationController::deleteInterviewDates(const std::string& applicationId) {
    std::string query = "DELETE FROM interview_dates WHERE application_id = $1";
    std::vector<std::string> params = {applicationId};
    return db->executeParameterizedNonQuery(query, params);
}

bool JobApplicationController::deleteFollowUpDates(const std::string& applicationId) {
    std::string query = "DELETE FROM followup_dates WHERE application_id = $1";
    std::vector<std::string> params = {applicationId};
    return db->executeParameterizedNonQuery(query, params);
}

bool JobApplicationController::insertInterviewDates(const std::string& applicationId, const std::vector<std::string>& dates) {
    for (const auto& date : dates) {
        if (!addInterviewDate(applicationId, date)) {
            return false;
        }
    }
    return true;
}

bool JobApplicationController::insertFollowUpDates(const std::string& applicationId, const std::vector<std::string>& dates) {
    for (const auto& date : dates) {
        if (!addFollowUpDate(applicationId, date)) {
            return false;
        }
    }
    return true;
}

std::string JobApplicationController::applicationToJsonString(const JobApplication& application, bool includeComma) {
    std::ostringstream json;

    json << "    {\n";
    json << "      \"applicationId\": \"" << escapeJsonString(application.getApplicationId()) << "\",\n";
    json << "      \"jobId\": \"" << escapeJsonString(application.getJobId()) << "\",\n";
    json << "      \"jobTitle\": \"" << escapeJsonString(application.getJobTitle()) << "\",\n";
    json << "      \"company\": \"" << escapeJsonString(application.getCompany()) << "\",\n";
    json << "      \"dateApplied\": \"" << escapeJsonString(application.getDateApplied()) << "\",\n";
    json << "      \"status\": \"" << escapeJsonString(JobApplication::statusToString(application.getStatus())) << "\",\n";
    json << "      \"contactName\": \"" << escapeJsonString(application.getContactName()) << "\",\n";
    json << "      \"contactEmail\": \"" << escapeJsonString(application.getContactEmail()) << "\",\n";
    json << "      \"contactPhone\": \"" << escapeJsonString(application.getContactPhone()) << "\",\n";
    json << "      \"comments\": \"" << escapeJsonString(application.getComments()) << "\",\n";
    json << "      \"applicationUrl\": \"" << escapeJsonString(application.getApplicationUrl()) << "\",\n";
    json << "      \"salaryOffered\": \"" << escapeJsonString(application.getSalaryOffered()) << "\",\n";
    json << "      \"expectedSalary\": \"" << escapeJsonString(application.getExpectedSalary()) << "\",\n";
    json << "      \"responseDeadline\": \"" << escapeJsonString(application.getResponseDeadline()) << "\",\n";
    json << "      \"referralSource\": \"" << escapeJsonString(application.getReferralSource()) << "\",\n";
    json << "      \"applicationMethod\": \"" << escapeJsonString(application.getApplicationMethod()) << "\",\n";
    json << "      \"notes\": \"" << escapeJsonString(application.getNotes()) << "\",\n";

    json << "      \"interviewDates\": [";
    const auto& interviewDates = application.getInterviewDates();
    for (size_t i = 0; i < interviewDates.size(); ++i) {
        json << "\"" << escapeJsonString(interviewDates[i]) << "\"";
        if (i < interviewDates.size() - 1) json << ", ";
    }
    json << "],\n";

    json << "      \"followUpDates\": [";
    const auto& followUpDates = application.getFollowUpDates();
    for (size_t i = 0; i < followUpDates.size(); ++i) {
        json << "\"" << escapeJsonString(followUpDates[i]) << "\"";
        if (i < followUpDates.size() - 1) json << ", ";
    }
    json << "]\n";

    json << "    }";
    if (includeComma) json << ",";
    json << "\n";

    return json.str();
}