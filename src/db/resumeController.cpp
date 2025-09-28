#include "resumeController.h"
#include <iostream>
#include <sstream>
#include <memory>

ResumeController::ResumeController() : BaseController() {}

bool ResumeController::createResume(const Resume& resume) {
    if (!initializeDatabase()) {
        return false;
    }

    if (resumeExistsByEmail(resume.getEmail())) {
        std::cerr << "Resume with email " << resume.getEmail() << " already exists" << std::endl;
        return false;
    }

    if (!db->beginTransaction()) {
        return false;
    }

    std::string query = R"(
        INSERT INTO resumes (name, email, city, phone, linkedin, website, interests)
        VALUES ($1, $2, $3, $4, $5, $6, $7)
        RETURNING id
    )";

    std::vector<std::string> params = {
        resume.getName(),
        resume.getEmail(),
        resume.getCity(),
        resume.getPhone(),
        resume.getLinkedin(),
        resume.getWebsite(),
        resume.getInterests()
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

    int resumeId = dbResult.getInt("id");

    bool success = insertResumeSkills(resumeId, resume.getSkills()) &&
                  insertResumeEducation(resumeId, resume.getEducation()) &&
                  insertResumeExperiences(resumeId, resume.getExperiences());

    if (success) {
        db->commitTransaction();
    } else {
        db->rollbackTransaction();
    }

    return success;
}

std::unique_ptr<Resume> ResumeController::getResumeById(int resumeId) {
    if (!initializeDatabase()) {
        return nullptr;
    }

    std::string query = "SELECT * FROM resumes WHERE id = $1";
    std::vector<std::string> params = {std::to_string(resumeId)};

    PGresult* result = db->executeParameterizedQuery(query, params);
    if (!result) {
        return nullptr;
    }

    DatabaseResult dbResult(result);
    if (dbResult.getRowCount() == 0) {
        return nullptr;
    }

    dbResult.next();
    Resume* resume = mapResultToResume(dbResult);

    if (resume) {
        // Note: Setting complex objects would require proper implementation
        // For now, we'll create a basic resume structure
        resume->setSkills(getResumeSkills(resumeId));
        resume->setEducation(getResumeEducation(resumeId));
        resume->setExperiences(getResumeExperiences(resumeId));
    }

    return std::unique_ptr<Resume>(resume);
}

std::unique_ptr<Resume> ResumeController::getResumeByEmail(const std::string& email) {
    if (!initializeDatabase()) {
        return nullptr;
    }

    std::string query = "SELECT * FROM resumes WHERE email = $1";
    std::vector<std::string> params = {email};

    PGresult* result = db->executeParameterizedQuery(query, params);
    if (!result) {
        return nullptr;
    }

    DatabaseResult dbResult(result);
    if (dbResult.getRowCount() == 0) {
        return nullptr;
    }

    dbResult.next();
    Resume* resume = mapResultToResume(dbResult);

    if (resume) {
        int resumeId = dbResult.getInt("id");
        resume->setSkills(getResumeSkills(resumeId));
        resume->setEducation(getResumeEducation(resumeId));
        resume->setExperiences(getResumeExperiences(resumeId));
    }

    return std::unique_ptr<Resume>(resume);
}

std::vector<std::unique_ptr<Resume>> ResumeController::getAllResumes() {
    std::vector<std::unique_ptr<Resume>> resumes;

    if (!initializeDatabase()) {
        return resumes;
    }

    std::string query = "SELECT * FROM resumes ORDER BY created_at DESC";
    PGresult* result = db->executeQuery(query);

    if (!result) {
        return resumes;
    }

    DatabaseResult dbResult(result);

    while (dbResult.next()) {
        Resume* resume = mapResultToResume(dbResult);
        if (resume) {
            int resumeId = dbResult.getInt("id");
            resume->setSkills(getResumeSkills(resumeId));
            resume->setEducation(getResumeEducation(resumeId));
            resume->setExperiences(getResumeExperiences(resumeId));
            resumes.push_back(std::unique_ptr<Resume>(resume));
        }
    }

    return resumes;
}

std::vector<std::unique_ptr<Resume>> ResumeController::getResumesByName(const std::string& name) {
    std::vector<std::unique_ptr<Resume>> resumes;

    if (!initializeDatabase()) {
        return resumes;
    }

    std::string query = "SELECT * FROM resumes WHERE LOWER(name) LIKE LOWER($1) ORDER BY created_at DESC";
    std::vector<std::string> params = {"%" + name + "%"};

    PGresult* result = db->executeParameterizedQuery(query, params);
    if (!result) {
        return resumes;
    }

    DatabaseResult dbResult(result);

    while (dbResult.next()) {
        Resume* resume = mapResultToResume(dbResult);
        if (resume) {
            int resumeId = dbResult.getInt("id");
            resume->setSkills(getResumeSkills(resumeId));
            resume->setEducation(getResumeEducation(resumeId));
            resume->setExperiences(getResumeExperiences(resumeId));
            resumes.push_back(std::unique_ptr<Resume>(resume));
        }
    }

    return resumes;
}

bool ResumeController::updateResume(const Resume& resume) {
    if (!initializeDatabase()) {
        return false;
    }

    if (!resumeExists(resume.getId())) {
        std::cerr << "Resume with ID " << resume.getId() << " does not exist" << std::endl;
        return false;
    }

    if (!db->beginTransaction()) {
        return false;
    }

    std::string query = R"(
        UPDATE resumes SET
            name = $2, email = $3, city = $4, phone = $5,
            linkedin = $6, website = $7, interests = $8,
            updated_at = CURRENT_TIMESTAMP
        WHERE id = $1
    )";

    std::vector<std::string> params = {
        std::to_string(resume.getId()),
        resume.getName(),
        resume.getEmail(),
        resume.getCity(),
        resume.getPhone(),
        resume.getLinkedin(),
        resume.getWebsite(),
        resume.getInterests()
    };

    bool success = db->executeParameterizedNonQuery(query, params);

    if (success) {
        success = deleteResumeSkills(resume.getId()) &&
                 deleteResumeEducation(resume.getId()) &&
                 deleteResumeExperiences(resume.getId()) &&
                 insertResumeSkills(resume.getId(), resume.getSkills()) &&
                 insertResumeEducation(resume.getId(), resume.getEducation()) &&
                 insertResumeExperiences(resume.getId(), resume.getExperiences());
    }

    if (success) {
        db->commitTransaction();
    } else {
        db->rollbackTransaction();
    }

    return success;
}

bool ResumeController::updateResumeBasicInfo(int resumeId, const std::string& name,
                                           const std::string& email, const std::string& city,
                                           const std::string& phone) {
    if (!initializeDatabase()) {
        return false;
    }

    std::string query = R"(
        UPDATE resumes SET
            name = $2, email = $3, city = $4, phone = $5,
            updated_at = CURRENT_TIMESTAMP
        WHERE id = $1
    )";

    std::vector<std::string> params = {
        std::to_string(resumeId), name, email, city, phone
    };

    return db->executeParameterizedNonQuery(query, params);
}

bool ResumeController::deleteResume(int resumeId) {
    if (!initializeDatabase()) {
        return false;
    }

    if (!db->beginTransaction()) {
        return false;
    }

    bool success = deleteResumeSkills(resumeId) &&
                  deleteResumeEducation(resumeId) &&
                  deleteResumeExperiences(resumeId);

    if (success) {
        std::string query = "DELETE FROM resumes WHERE id = $1";
        std::vector<std::string> params = {std::to_string(resumeId)};
        success = db->executeParameterizedNonQuery(query, params);
    }

    if (success) {
        db->commitTransaction();
    } else {
        db->rollbackTransaction();
    }

    return success;
}

bool ResumeController::deleteResumeByEmail(const std::string& email) {
    if (!initializeDatabase()) {
        return false;
    }

    auto resume = getResumeByEmail(email);
    if (!resume) {
        return false;
    }

    return deleteResume(resume->getId());
}

int ResumeController::getResumeCount() const {
    if (!const_cast<ResumeController*>(this)->initializeDatabase()) {
        return 0;
    }

    std::string query = "SELECT COUNT(*) FROM resumes";
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

std::string ResumeController::exportToJson() {
    auto resumes = getAllResumes();

    std::ostringstream json;
    json << "{\n  \"resumes\": [\n";

    for (size_t i = 0; i < resumes.size(); ++i) {
        json << resumeToJsonString(*resumes[i], i < resumes.size() - 1);
    }

    json << "  ]\n}";
    return json.str();
}

std::string ResumeController::exportToCsv() {
    auto resumes = getAllResumes();

    std::ostringstream csv;
    csv << "ID,Name,Email,City,Phone,LinkedIn,Website,Interests\n";

    for (const auto& resume : resumes) {
        csv << resume->getId() << ",";
        csv << escapeCsvField(resume->getName()) << ",";
        csv << escapeCsvField(resume->getEmail()) << ",";
        csv << escapeCsvField(resume->getCity()) << ",";
        csv << escapeCsvField(resume->getPhone()) << ",";
        csv << escapeCsvField(resume->getLinkedin()) << ",";
        csv << escapeCsvField(resume->getWebsite()) << ",";
        csv << escapeCsvField(resume->getInterests()) << "\n";
    }

    return csv.str();
}

bool ResumeController::importFromJson(const std::string& jsonData) {
    return false;
}

bool ResumeController::importFromCsv(const std::string& csvData) {
    return false;
}

bool ResumeController::resumeExists(int resumeId) const {
    if (!const_cast<ResumeController*>(this)->initializeDatabase()) {
        return false;
    }

    std::string query = "SELECT COUNT(*) FROM resumes WHERE id = $1";
    std::vector<std::string> params = {std::to_string(resumeId)};

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

bool ResumeController::resumeExistsByEmail(const std::string& email) const {
    if (!const_cast<ResumeController*>(this)->initializeDatabase()) {
        return false;
    }

    std::string query = "SELECT COUNT(*) FROM resumes WHERE email = $1";
    std::vector<std::string> params = {email};

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

Resume* ResumeController::mapResultToResume(DatabaseResult& result) {
    Resume* resume = new Resume();

    resume->setId(result.getInt("id"));
    resume->setName(result.getString("name"));
    resume->setEmail(result.getString("email"));
    resume->setCity(result.getString("city"));
    resume->setPhone(result.getString("phone"));
    resume->setLinkedin(result.getString("linkedin"));
    resume->setWebsite(result.getString("website"));
    resume->setInterests(result.getString("interests"));

    return resume;
}

bool ResumeController::insertResumeSkills(int resumeId, const Skills& skills) {
    return true;
}

bool ResumeController::insertResumeEducation(int resumeId, const Education& education) {
    return true;
}

bool ResumeController::insertResumeExperiences(int resumeId, const Experiences& experiences) {
    return true;
}

bool ResumeController::deleteResumeSkills(int resumeId) {
    std::string query = "DELETE FROM resume_skills WHERE resume_id = $1";
    std::vector<std::string> params = {std::to_string(resumeId)};
    return db->executeParameterizedNonQuery(query, params);
}

bool ResumeController::deleteResumeEducation(int resumeId) {
    std::string query = "DELETE FROM resume_education WHERE resume_id = $1";
    std::vector<std::string> params = {std::to_string(resumeId)};
    return db->executeParameterizedNonQuery(query, params);
}

bool ResumeController::deleteResumeExperiences(int resumeId) {
    std::string query = "DELETE FROM resume_experiences WHERE resume_id = $1";
    std::vector<std::string> params = {std::to_string(resumeId)};
    return db->executeParameterizedNonQuery(query, params);
}

Skills ResumeController::getResumeSkills(int resumeId) {
    Skills skills;
    return skills;
}

Education ResumeController::getResumeEducation(int resumeId) {
    Education education;
    return education;
}

Experiences ResumeController::getResumeExperiences(int resumeId) {
    Experiences experiences;
    return experiences;
}

std::string ResumeController::resumeToJsonString(const Resume& resume, bool includeComma) {
    std::ostringstream json;

    json << "    {\n";
    json << "      \"id\": " << resume.getId() << ",\n";
    json << "      \"name\": \"" << escapeJsonString(resume.getName()) << "\",\n";
    json << "      \"email\": \"" << escapeJsonString(resume.getEmail()) << "\",\n";
    json << "      \"city\": \"" << escapeJsonString(resume.getCity()) << "\",\n";
    json << "      \"phone\": \"" << escapeJsonString(resume.getPhone()) << "\",\n";
    json << "      \"linkedin\": \"" << escapeJsonString(resume.getLinkedin()) << "\",\n";
    json << "      \"website\": \"" << escapeJsonString(resume.getWebsite()) << "\",\n";
    json << "      \"interests\": \"" << escapeJsonString(resume.getInterests()) << "\"\n";
    json << "    }";
    if (includeComma) json << ",";
    json << "\n";

    return json.str();
}