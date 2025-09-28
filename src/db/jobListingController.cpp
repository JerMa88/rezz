#include "jobListingController.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <memory>

JobListingController::JobListingController() : BaseController() {}

bool JobListingController::createJobListing(const JobListing& jobListing) {
    if (!initializeDatabase()) {
        return false;
    }

    if (jobListingExists(jobListing.getJobId())) {
        std::cerr << "Job listing with ID " << jobListing.getJobId() << " already exists" << std::endl;
        return false;
    }

    if (!db->beginTransaction()) {
        return false;
    }

    std::string query = R"(
        INSERT INTO job_listings (
            job_id, title, company, description, location, remote_type, job_type,
            experience_level, salary_min, salary_max, salary_currency,
            minimum_years_experience, application_deadline, posted_date,
            application_url, contact_email, company_size, industry,
            company_website, is_active, department, reporting_to
        ) VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15, $16, $17, $18, $19, $20, $21, $22)
        RETURNING id
    )";

    std::vector<std::string> params = {
        jobListing.getJobId(),
        jobListing.getTitle(),
        jobListing.getCompany(),
        jobListing.getDescription(),
        jobListing.getLocation(),
        std::to_string(static_cast<int>(jobListing.getRemoteType())),
        std::to_string(static_cast<int>(jobListing.getJobType())),
        std::to_string(static_cast<int>(jobListing.getExperienceLevel())),
        std::to_string(jobListing.getSalaryMin()),
        std::to_string(jobListing.getSalaryMax()),
        jobListing.getSalaryCurrency(),
        std::to_string(jobListing.getMinimumYearsExperience()),
        jobListing.getApplicationDeadline(),
        jobListing.getPostedDate(),
        jobListing.getApplicationUrl(),
        jobListing.getContactEmail(),
        jobListing.getCompanySize(),
        jobListing.getIndustry(),
        jobListing.getCompanyWebsite(),
        jobListing.getIsActive() ? "true" : "false",
        jobListing.getDepartment(),
        jobListing.getReportingTo()
    };

    PGresult* result = db->executeParameterizedQuery(query, params);
    if (!result) {
        db->rollbackTransaction();
        return false;
    }

    DatabaseResult dbResult(result);
    if (!dbResult.next()) {
        db->rollbackTransaction();
        return false;
    }

    int jobListingId = dbResult.getInt("id");

    bool success = insertJobRequiredSkills(jobListingId, jobListing.getRequiredSkills()) &&
                  insertJobPreferredSkills(jobListingId, jobListing.getPreferredSkills());

    if (success) {
        db->commitTransaction();
    } else {
        db->rollbackTransaction();
    }

    return success;
}

std::unique_ptr<JobListing> JobListingController::getJobListingById(const std::string& jobId) {
    if (!initializeDatabase()) {
        return nullptr;
    }

    std::string query = "SELECT * FROM job_listings WHERE job_id = $1";
    std::vector<std::string> params = {jobId};

    PGresult* result = db->executeParameterizedQuery(query, params);
    if (!result) {
        return nullptr;
    }

    DatabaseResult dbResult(result);
    if (dbResult.getRowCount() == 0) {
        return nullptr;
    }

    dbResult.next();
    JobListing* jobListing = mapResultToJobListing(dbResult);

    if (jobListing) {
        int jobListingId = dbResult.getInt("id");
        jobListing->setRequiredSkills(getJobRequiredSkills(jobListingId));
        jobListing->setPreferredSkills(getJobPreferredSkills(jobListingId));
    }

    return std::unique_ptr<JobListing>(jobListing);
}

std::unique_ptr<JobListing> JobListingController::getJobListingByIdInt(int id) {
    if (!initializeDatabase()) {
        return nullptr;
    }

    std::string query = "SELECT * FROM job_listings WHERE id = $1";
    std::vector<std::string> params = {std::to_string(id)};

    PGresult* result = db->executeParameterizedQuery(query, params);
    if (!result) {
        return nullptr;
    }

    DatabaseResult dbResult(result);
    if (dbResult.getRowCount() == 0) {
        return nullptr;
    }

    dbResult.next();
    JobListing* jobListing = mapResultToJobListing(dbResult);

    if (jobListing) {
        jobListing->setRequiredSkills(getJobRequiredSkills(id));
        jobListing->setPreferredSkills(getJobPreferredSkills(id));
    }

    return std::unique_ptr<JobListing>(jobListing);
}

std::vector<std::unique_ptr<JobListing>> JobListingController::getAllJobListings() {
    std::vector<std::unique_ptr<JobListing>> jobListings;

    if (!initializeDatabase()) {
        return jobListings;
    }

    std::string query = "SELECT * FROM job_listings ORDER BY posted_date DESC";
    PGresult* result = db->executeQuery(query);

    if (!result) {
        return jobListings;
    }

    DatabaseResult dbResult(result);

    while (dbResult.next()) {
        JobListing* jobListing = mapResultToJobListing(dbResult);
        if (jobListing) {
            int jobListingId = dbResult.getInt("id");
            jobListing->setRequiredSkills(getJobRequiredSkills(jobListingId));
            jobListing->setPreferredSkills(getJobPreferredSkills(jobListingId));
            jobListings.push_back(std::unique_ptr<JobListing>(jobListing));
        }
    }

    return jobListings;
}

std::vector<std::unique_ptr<JobListing>> JobListingController::getJobListingsByCompany(const std::string& company) {
    std::vector<std::unique_ptr<JobListing>> jobListings;

    if (!initializeDatabase()) {
        return jobListings;
    }

    std::string query = "SELECT * FROM job_listings WHERE LOWER(company) LIKE LOWER($1) ORDER BY posted_date DESC";
    std::vector<std::string> params = {"%" + company + "%"};

    PGresult* result = db->executeParameterizedQuery(query, params);
    if (!result) {
        return jobListings;
    }

    DatabaseResult dbResult(result);

    while (dbResult.next()) {
        JobListing* jobListing = mapResultToJobListing(dbResult);
        if (jobListing) {
            int jobListingId = dbResult.getInt("id");
            jobListing->setRequiredSkills(getJobRequiredSkills(jobListingId));
            jobListing->setPreferredSkills(getJobPreferredSkills(jobListingId));
            jobListings.push_back(std::unique_ptr<JobListing>(jobListing));
        }
    }

    return jobListings;
}

std::vector<std::unique_ptr<JobListing>> JobListingController::getJobListingsByLocation(const std::string& location) {
    std::vector<std::unique_ptr<JobListing>> jobListings;

    if (!initializeDatabase()) {
        return jobListings;
    }

    std::string query = "SELECT * FROM job_listings WHERE LOWER(location) LIKE LOWER($1) ORDER BY posted_date DESC";
    std::vector<std::string> params = {"%" + location + "%"};

    PGresult* result = db->executeParameterizedQuery(query, params);
    if (!result) {
        return jobListings;
    }

    DatabaseResult dbResult(result);

    while (dbResult.next()) {
        JobListing* jobListing = mapResultToJobListing(dbResult);
        if (jobListing) {
            int jobListingId = dbResult.getInt("id");
            jobListing->setRequiredSkills(getJobRequiredSkills(jobListingId));
            jobListing->setPreferredSkills(getJobPreferredSkills(jobListingId));
            jobListings.push_back(std::unique_ptr<JobListing>(jobListing));
        }
    }

    return jobListings;
}

std::vector<std::unique_ptr<JobListing>> JobListingController::getJobListingsByType(JobType jobType) {
    std::vector<std::unique_ptr<JobListing>> jobListings;

    if (!initializeDatabase()) {
        return jobListings;
    }

    std::string query = "SELECT * FROM job_listings WHERE job_type = $1 ORDER BY posted_date DESC";
    std::vector<std::string> params = {std::to_string(static_cast<int>(jobType))};

    PGresult* result = db->executeParameterizedQuery(query, params);
    if (!result) {
        return jobListings;
    }

    DatabaseResult dbResult(result);

    while (dbResult.next()) {
        JobListing* jobListing = mapResultToJobListing(dbResult);
        if (jobListing) {
            int jobListingId = dbResult.getInt("id");
            jobListing->setRequiredSkills(getJobRequiredSkills(jobListingId));
            jobListing->setPreferredSkills(getJobPreferredSkills(jobListingId));
            jobListings.push_back(std::unique_ptr<JobListing>(jobListing));
        }
    }

    return jobListings;
}

std::vector<std::unique_ptr<JobListing>> JobListingController::getJobListingsByExperienceLevel(JobExperienceLevel level) {
    std::vector<std::unique_ptr<JobListing>> jobListings;

    if (!initializeDatabase()) {
        return jobListings;
    }

    std::string query = "SELECT * FROM job_listings WHERE experience_level = $1 ORDER BY posted_date DESC";
    std::vector<std::string> params = {std::to_string(static_cast<int>(level))};

    PGresult* result = db->executeParameterizedQuery(query, params);
    if (!result) {
        return jobListings;
    }

    DatabaseResult dbResult(result);

    while (dbResult.next()) {
        JobListing* jobListing = mapResultToJobListing(dbResult);
        if (jobListing) {
            int jobListingId = dbResult.getInt("id");
            jobListing->setRequiredSkills(getJobRequiredSkills(jobListingId));
            jobListing->setPreferredSkills(getJobPreferredSkills(jobListingId));
            jobListings.push_back(std::unique_ptr<JobListing>(jobListing));
        }
    }

    return jobListings;
}

std::vector<std::unique_ptr<JobListing>> JobListingController::getJobListingsBySalaryRange(float minSalary, float maxSalary) {
    std::vector<std::unique_ptr<JobListing>> jobListings;

    if (!initializeDatabase()) {
        return jobListings;
    }

    std::string query = "SELECT * FROM job_listings WHERE salary_min >= $1 AND salary_max <= $2 ORDER BY salary_max DESC";
    std::vector<std::string> params = {std::to_string(minSalary), std::to_string(maxSalary)};

    PGresult* result = db->executeParameterizedQuery(query, params);
    if (!result) {
        return jobListings;
    }

    DatabaseResult dbResult(result);

    while (dbResult.next()) {
        JobListing* jobListing = mapResultToJobListing(dbResult);
        if (jobListing) {
            int jobListingId = dbResult.getInt("id");
            jobListing->setRequiredSkills(getJobRequiredSkills(jobListingId));
            jobListing->setPreferredSkills(getJobPreferredSkills(jobListingId));
            jobListings.push_back(std::unique_ptr<JobListing>(jobListing));
        }
    }

    return jobListings;
}

std::vector<std::unique_ptr<JobListing>> JobListingController::getActiveJobListings() {
    std::vector<std::unique_ptr<JobListing>> jobListings;

    if (!initializeDatabase()) {
        return jobListings;
    }

    std::string query = "SELECT * FROM job_listings WHERE is_active = true ORDER BY posted_date DESC";
    PGresult* result = db->executeQuery(query);

    if (!result) {
        return jobListings;
    }

    DatabaseResult dbResult(result);

    while (dbResult.next()) {
        JobListing* jobListing = mapResultToJobListing(dbResult);
        if (jobListing) {
            int jobListingId = dbResult.getInt("id");
            jobListing->setRequiredSkills(getJobRequiredSkills(jobListingId));
            jobListing->setPreferredSkills(getJobPreferredSkills(jobListingId));
            jobListings.push_back(std::unique_ptr<JobListing>(jobListing));
        }
    }

    return jobListings;
}

bool JobListingController::updateJobListing(const JobListing& jobListing) {
    if (!initializeDatabase()) {
        return false;
    }

    if (!jobListingExists(jobListing.getJobId())) {
        std::cerr << "Job listing with ID " << jobListing.getJobId() << " does not exist" << std::endl;
        return false;
    }

    if (!db->beginTransaction()) {
        return false;
    }

    std::string query = R"(
        UPDATE job_listings SET
            title = $2, company = $3, description = $4, location = $5,
            remote_type = $6, job_type = $7, experience_level = $8,
            salary_min = $9, salary_max = $10, salary_currency = $11,
            minimum_years_experience = $12, application_deadline = $13,
            application_url = $14, contact_email = $15, company_size = $16,
            industry = $17, company_website = $18, is_active = $19,
            department = $20, reporting_to = $21, updated_at = CURRENT_TIMESTAMP
        WHERE job_id = $1
    )";

    std::vector<std::string> params = {
        jobListing.getJobId(),
        jobListing.getTitle(),
        jobListing.getCompany(),
        jobListing.getDescription(),
        jobListing.getLocation(),
        std::to_string(static_cast<int>(jobListing.getRemoteType())),
        std::to_string(static_cast<int>(jobListing.getJobType())),
        std::to_string(static_cast<int>(jobListing.getExperienceLevel())),
        std::to_string(jobListing.getSalaryMin()),
        std::to_string(jobListing.getSalaryMax()),
        jobListing.getSalaryCurrency(),
        std::to_string(jobListing.getMinimumYearsExperience()),
        jobListing.getApplicationDeadline(),
        jobListing.getApplicationUrl(),
        jobListing.getContactEmail(),
        jobListing.getCompanySize(),
        jobListing.getIndustry(),
        jobListing.getCompanyWebsite(),
        jobListing.getIsActive() ? "true" : "false",
        jobListing.getDepartment(),
        jobListing.getReportingTo()
    };

    bool success = db->executeParameterizedNonQuery(query, params);

    if (success) {
        auto existingListing = getJobListingById(jobListing.getJobId());
        if (existingListing) {
            int jobListingId = existingListing->getId();
            success = deleteJobRequiredSkills(jobListingId) &&
                     deleteJobPreferredSkills(jobListingId) &&
                     insertJobRequiredSkills(jobListingId, jobListing.getRequiredSkills()) &&
                     insertJobPreferredSkills(jobListingId, jobListing.getPreferredSkills());
        }
    }

    if (success) {
        db->commitTransaction();
    } else {
        db->rollbackTransaction();
    }

    return success;
}

bool JobListingController::updateJobListingStatus(const std::string& jobId, bool isActive) {
    if (!initializeDatabase()) {
        return false;
    }

    std::string query = "UPDATE job_listings SET is_active = $2, updated_at = CURRENT_TIMESTAMP WHERE job_id = $1";
    std::vector<std::string> params = {jobId, isActive ? "true" : "false"};

    return db->executeParameterizedNonQuery(query, params);
}

bool JobListingController::updateJobListingSalary(const std::string& jobId, float minSalary, float maxSalary, const std::string& currency) {
    if (!initializeDatabase()) {
        return false;
    }

    std::string query = "UPDATE job_listings SET salary_min = $2, salary_max = $3, salary_currency = $4, updated_at = CURRENT_TIMESTAMP WHERE job_id = $1";
    std::vector<std::string> params = {jobId, std::to_string(minSalary), std::to_string(maxSalary), currency};

    return db->executeParameterizedNonQuery(query, params);
}

bool JobListingController::deleteJobListing(const std::string& jobId) {
    if (!initializeDatabase()) {
        return false;
    }

    auto jobListing = getJobListingById(jobId);
    if (!jobListing) {
        return false;
    }

    if (!db->beginTransaction()) {
        return false;
    }

    int jobListingId = jobListing->getId();
    bool success = deleteJobRequiredSkills(jobListingId) &&
                  deleteJobPreferredSkills(jobListingId);

    if (success) {
        std::string query = "DELETE FROM job_listings WHERE job_id = $1";
        std::vector<std::string> params = {jobId};
        success = db->executeParameterizedNonQuery(query, params);
    }

    if (success) {
        db->commitTransaction();
    } else {
        db->rollbackTransaction();
    }

    return success;
}

bool JobListingController::deleteJobListingsByCompany(const std::string& company) {
    if (!initializeDatabase()) {
        return false;
    }

    std::string query = "DELETE FROM job_listings WHERE LOWER(company) = LOWER($1)";
    std::vector<std::string> params = {company};

    return db->executeParameterizedNonQuery(query, params);
}

int JobListingController::getJobListingCount() const {
    if (!const_cast<JobListingController*>(this)->initializeDatabase()) {
        return 0;
    }

    std::string query = "SELECT COUNT(*) FROM job_listings";
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

int JobListingController::getActiveJobListingCount() const {
    if (!const_cast<JobListingController*>(this)->initializeDatabase()) {
        return 0;
    }

    std::string query = "SELECT COUNT(*) FROM job_listings WHERE is_active = true";
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

int JobListingController::getJobListingCountByCompany(const std::string& company) const {
    if (!const_cast<JobListingController*>(this)->initializeDatabase()) {
        return 0;
    }

    std::string query = "SELECT COUNT(*) FROM job_listings WHERE LOWER(company) = LOWER($1)";
    std::vector<std::string> params = {company};

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

std::string JobListingController::exportToJson() {
    auto jobListings = getAllJobListings();

    std::ostringstream json;
    json << "{\n  \"jobListings\": [\n";

    for (size_t i = 0; i < jobListings.size(); ++i) {
        json << jobListingToJsonString(*jobListings[i], i < jobListings.size() - 1);
    }

    json << "  ]\n}";
    return json.str();
}

std::string JobListingController::exportToCsv() {
    auto jobListings = getAllJobListings();

    std::ostringstream csv;
    csv << "Job ID,Title,Company,Location,Remote Type,Job Type,Experience Level,";
    csv << "Salary Min,Salary Max,Currency,Min Years Experience,Posted Date,";
    csv << "Application Deadline,Application URL,Contact Email,Is Active\n";

    for (const auto& job : jobListings) {
        csv << escapeCsvField(job->getJobId()) << ",";
        csv << escapeCsvField(job->getTitle()) << ",";
        csv << escapeCsvField(job->getCompany()) << ",";
        csv << escapeCsvField(job->getLocation()) << ",";
        csv << escapeCsvField(JobListing::remoteTypeToString(job->getRemoteType())) << ",";
        csv << escapeCsvField(JobListing::jobTypeToString(job->getJobType())) << ",";
        csv << escapeCsvField(JobListing::experienceLevelToString(job->getExperienceLevel())) << ",";
        csv << job->getSalaryMin() << ",";
        csv << job->getSalaryMax() << ",";
        csv << escapeCsvField(job->getSalaryCurrency()) << ",";
        csv << job->getMinimumYearsExperience() << ",";
        csv << escapeCsvField(job->getPostedDate()) << ",";
        csv << escapeCsvField(job->getApplicationDeadline()) << ",";
        csv << escapeCsvField(job->getApplicationUrl()) << ",";
        csv << escapeCsvField(job->getContactEmail()) << ",";
        csv << (job->getIsActive() ? "Yes" : "No") << "\n";
    }

    return csv.str();
}

bool JobListingController::importFromJson(const std::string& jsonData) {
    return false;
}

bool JobListingController::importFromCsv(const std::string& csvData) {
    return false;
}

bool JobListingController::jobListingExists(const std::string& jobId) const {
    if (!const_cast<JobListingController*>(this)->initializeDatabase()) {
        return false;
    }

    std::string query = "SELECT COUNT(*) FROM job_listings WHERE job_id = $1";
    std::vector<std::string> params = {jobId};

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

JobListing* JobListingController::mapResultToJobListing(DatabaseResult& result) {
    JobListing* jobListing = new JobListing();

    jobListing->setJobId(result.getString("job_id"));
    jobListing->setTitle(result.getString("title"));
    jobListing->setCompany(result.getString("company"));
    jobListing->setDescription(result.getString("description"));
    jobListing->setLocation(result.getString("location"));
    jobListing->setRemoteType(static_cast<RemoteType>(result.getInt("remote_type")));
    jobListing->setJobType(static_cast<JobType>(result.getInt("job_type")));
    jobListing->setExperienceLevel(static_cast<JobExperienceLevel>(result.getInt("experience_level")));
    jobListing->setSalaryRange(result.getDouble("salary_min"), result.getDouble("salary_max"), result.getString("salary_currency"));
    jobListing->setMinimumYearsExperience(result.getInt("minimum_years_experience"));
    jobListing->setApplicationDeadline(result.getString("application_deadline"));
    jobListing->setPostedDate(result.getString("posted_date"));
    jobListing->setApplicationUrl(result.getString("application_url"));
    jobListing->setContactEmail(result.getString("contact_email"));
    jobListing->setCompanySize(result.getString("company_size"));
    jobListing->setIndustry(result.getString("industry"));
    jobListing->setCompanyWebsite(result.getString("company_website"));
    jobListing->setIsActive(result.getBool("is_active"));
    jobListing->setDepartment(result.getString("department"));
    jobListing->setReportingTo(result.getString("reporting_to"));

    return jobListing;
}

bool JobListingController::insertJobRequiredSkills(int jobListingId, const Skills& skills) {
    return true;
}

bool JobListingController::insertJobPreferredSkills(int jobListingId, const Skills& skills) {
    return true;
}

bool JobListingController::deleteJobRequiredSkills(int jobListingId) {
    std::string query = "DELETE FROM job_required_skills WHERE job_listing_id = $1";
    std::vector<std::string> params = {std::to_string(jobListingId)};
    return db->executeParameterizedNonQuery(query, params);
}

bool JobListingController::deleteJobPreferredSkills(int jobListingId) {
    std::string query = "DELETE FROM job_preferred_skills WHERE job_listing_id = $1";
    std::vector<std::string> params = {std::to_string(jobListingId)};
    return db->executeParameterizedNonQuery(query, params);
}

Skills JobListingController::getJobRequiredSkills(int jobListingId) {
    Skills skills;
    return skills;
}

Skills JobListingController::getJobPreferredSkills(int jobListingId) {
    Skills skills;
    return skills;
}

std::string JobListingController::jobListingToJsonString(const JobListing& jobListing, bool includeComma) {
    std::ostringstream json;

    json << "    {\n";
    json << "      \"jobId\": \"" << escapeJsonString(jobListing.getJobId()) << "\",\n";
    json << "      \"title\": \"" << escapeJsonString(jobListing.getTitle()) << "\",\n";
    json << "      \"company\": \"" << escapeJsonString(jobListing.getCompany()) << "\",\n";
    json << "      \"description\": \"" << escapeJsonString(jobListing.getDescription()) << "\",\n";
    json << "      \"location\": \"" << escapeJsonString(jobListing.getLocation()) << "\",\n";
    json << "      \"remoteType\": \"" << escapeJsonString(JobListing::remoteTypeToString(jobListing.getRemoteType())) << "\",\n";
    json << "      \"jobType\": \"" << escapeJsonString(JobListing::jobTypeToString(jobListing.getJobType())) << "\",\n";
    json << "      \"experienceLevel\": \"" << escapeJsonString(JobListing::experienceLevelToString(jobListing.getExperienceLevel())) << "\",\n";
    json << "      \"salaryMin\": " << jobListing.getSalaryMin() << ",\n";
    json << "      \"salaryMax\": " << jobListing.getSalaryMax() << ",\n";
    json << "      \"salaryCurrency\": \"" << escapeJsonString(jobListing.getSalaryCurrency()) << "\",\n";
    json << "      \"minimumYearsExperience\": " << jobListing.getMinimumYearsExperience() << ",\n";
    json << "      \"applicationDeadline\": \"" << escapeJsonString(jobListing.getApplicationDeadline()) << "\",\n";
    json << "      \"postedDate\": \"" << escapeJsonString(jobListing.getPostedDate()) << "\",\n";
    json << "      \"applicationUrl\": \"" << escapeJsonString(jobListing.getApplicationUrl()) << "\",\n";
    json << "      \"contactEmail\": \"" << escapeJsonString(jobListing.getContactEmail()) << "\",\n";
    json << "      \"companySize\": \"" << escapeJsonString(jobListing.getCompanySize()) << "\",\n";
    json << "      \"industry\": \"" << escapeJsonString(jobListing.getIndustry()) << "\",\n";
    json << "      \"companyWebsite\": \"" << escapeJsonString(jobListing.getCompanyWebsite()) << "\",\n";
    json << "      \"isActive\": " << (jobListing.getIsActive() ? "true" : "false") << ",\n";
    json << "      \"department\": \"" << escapeJsonString(jobListing.getDepartment()) << "\",\n";
    json << "      \"reportingTo\": \"" << escapeJsonString(jobListing.getReportingTo()) << "\"\n";
    json << "    }";
    if (includeComma) json << ",";
    json << "\n";

    return json.str();
}