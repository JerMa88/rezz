#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include "../../../src/ds/resume/jobListing.h"

// Test helper function to display test results
void testResult(const std::string& testName, bool result) {
    std::cout << testName << ": " << (result ? "PASSED" : "FAILED") << std::endl;
}

// Test JobListing default constructor
void testJobListingDefaultConstructor() {
    JobListing job;

    bool emptyJobId = job.getJobId().empty();
    bool emptyTitle = job.getTitle().empty();
    bool emptyCompany = job.getCompany().empty();
    bool defaultRemoteType = (job.getRemoteType() == RemoteType::ON_SITE);
    bool defaultJobType = (job.getJobType() == JobType::FULL_TIME);
    bool defaultExpLevel = (job.getExperienceLevel() == JobExperienceLevel::ENTRY_LEVEL);
    bool zeroSalary = (job.getSalaryMin() == 0.0f && job.getSalaryMax() == 0.0f);
    bool defaultCurrency = (job.getSalaryCurrency() == "USD");
    bool isActive = job.getIsActive();

    testResult("JobListing Default Constructor",
               emptyJobId && emptyTitle && emptyCompany && defaultRemoteType &&
               defaultJobType && defaultExpLevel && zeroSalary && defaultCurrency && isActive);
}

// Test JobListing parameterized constructor
void testJobListingParameterizedConstructor() {
    JobListing job("JOB001", "Senior Software Engineer", "Tech Corp",
                   "Develop amazing software", "San Francisco, CA",
                   RemoteType::HYBRID, JobType::FULL_TIME, JobExperienceLevel::SENIOR);

    bool correctJobId = (job.getJobId() == "JOB001");
    bool correctTitle = (job.getTitle() == "Senior Software Engineer");
    bool correctCompany = (job.getCompany() == "Tech Corp");
    bool correctDescription = (job.getDescription() == "Develop amazing software");
    bool correctLocation = (job.getLocation() == "San Francisco, CA");
    bool correctRemoteType = (job.getRemoteType() == RemoteType::HYBRID);
    bool correctJobType = (job.getJobType() == JobType::FULL_TIME);
    bool correctExpLevel = (job.getExperienceLevel() == JobExperienceLevel::SENIOR);

    testResult("JobListing Parameterized Constructor",
               correctJobId && correctTitle && correctCompany && correctDescription &&
               correctLocation && correctRemoteType && correctJobType && correctExpLevel);
}

// Test basic setters and getters
void testBasicSettersGetters() {
    JobListing job;

    job.setJobId("TEST123");
    job.setTitle("Data Scientist");
    job.setCompany("AI Startup");
    job.setDescription("Work with machine learning models");
    job.setLocation("New York, NY");
    job.setRemoteType(RemoteType::REMOTE);
    job.setJobType(JobType::CONTRACT);
    job.setExperienceLevel(JobExperienceLevel::MID_LEVEL);

    bool correctJobId = (job.getJobId() == "TEST123");
    bool correctTitle = (job.getTitle() == "Data Scientist");
    bool correctCompany = (job.getCompany() == "AI Startup");
    bool correctDescription = (job.getDescription() == "Work with machine learning models");
    bool correctLocation = (job.getLocation() == "New York, NY");
    bool correctRemoteType = (job.getRemoteType() == RemoteType::REMOTE);
    bool correctJobType = (job.getJobType() == JobType::CONTRACT);
    bool correctExpLevel = (job.getExperienceLevel() == JobExperienceLevel::MID_LEVEL);

    testResult("Basic Setters and Getters",
               correctJobId && correctTitle && correctCompany && correctDescription &&
               correctLocation && correctRemoteType && correctJobType && correctExpLevel);
}

// Test salary functionality
void testSalaryFunctionality() {
    JobListing job;

    // Test individual setters
    job.setSalaryMin(80000.0f);
    job.setSalaryMax(120000.0f);
    job.setSalaryCurrency("USD");

    bool correctMin = (job.getSalaryMin() == 80000.0f);
    bool correctMax = (job.getSalaryMax() == 120000.0f);
    bool correctCurrency = (job.getSalaryCurrency() == "USD");

    // Test range setter
    job.setSalaryRange(90000.0f, 140000.0f, "CAD");
    bool correctRangeMin = (job.getSalaryMin() == 90000.0f);
    bool correctRangeMax = (job.getSalaryMax() == 140000.0f);
    bool correctRangeCurrency = (job.getSalaryCurrency() == "CAD");

    testResult("Salary Functionality",
               correctMin && correctMax && correctCurrency &&
               correctRangeMin && correctRangeMax && correctRangeCurrency);
}

// Test required skills functionality
void testRequiredSkillsFunctionality() {
    JobListing job;

    Skills requiredSkills("Programming Languages");
    Skill python("Python", 3, ADVANCED);
    Skill java("Java", 2, INTERMEDIATE);

    requiredSkills.addSkill(python);
    requiredSkills.addSkill(java);
    job.setRequiredSkills(requiredSkills);

    bool hasRequiredSkills = (job.getRequiredSkills().getSkillCount() == 2);
    bool hasPython = job.hasRequiredSkill(python);
    bool hasJava = job.hasRequiredSkill(java);

    Skill cpp("C++", 1, BEGINNER);
    bool doesNotHaveCpp = !job.hasRequiredSkill(cpp);

    testResult("Required Skills Functionality",
               hasRequiredSkills && hasPython && hasJava && doesNotHaveCpp);
}

// Test preferred skills functionality
void testPreferredSkillsFunctionality() {
    JobListing job;

    Skills preferredSkills("Frameworks");
    Skill react("React", 2, INTERMEDIATE);
    Skill angular("Angular", 1, BEGINNER);

    preferredSkills.addSkill(react);
    preferredSkills.addSkill(angular);
    job.setPreferredSkills(preferredSkills);

    bool hasPreferredSkills = (job.getPreferredSkills().getSkillCount() == 2);
    bool hasReact = job.hasPreferredSkill(react);
    bool hasAngular = job.hasPreferredSkill(angular);

    Skill vue("Vue.js", 1, BEGINNER);
    bool doesNotHaveVue = !job.hasPreferredSkill(vue);

    testResult("Preferred Skills Functionality",
               hasPreferredSkills && hasReact && hasAngular && doesNotHaveVue);
}

// Test education requirements
void testEducationRequirements() {
    JobListing job;

    job.addRequiredEducation("Bachelor's in Computer Science");
    job.addRequiredEducation("Master's preferred");
    job.addPreferredEducation("PhD in AI/ML");
    job.addPreferredEducation("Coursework in Statistics");

    std::vector<std::string> requiredEdu = job.getRequiredEducation();
    std::vector<std::string> preferredEdu = job.getPreferredEducation();

    bool correctRequiredCount = (requiredEdu.size() == 2);
    bool correctPreferredCount = (preferredEdu.size() == 2);
    bool correctRequiredContent = (requiredEdu[0] == "Bachelor's in Computer Science");
    bool correctPreferredContent = (preferredEdu[0] == "PhD in AI/ML");

    testResult("Education Requirements",
               correctRequiredCount && correctPreferredCount &&
               correctRequiredContent && correctPreferredContent);
}

// Test experience requirements
void testExperienceRequirements() {
    JobListing job;

    Experience python("Python Programming", ExperienceLevel::ADVANCED);
    Experience docker("Docker", ExperienceLevel::INTERMEDIATE);
    Experience kubernetes("Kubernetes", ExperienceLevel::BEGINNER);

    job.addRequiredExperience(python);
    job.addRequiredExperience(docker);
    job.addPreferredExperience(kubernetes);

    job.setMinimumYearsExperience(5);

    std::vector<Experience> requiredExp = job.getRequiredExperiences();
    std::vector<Experience> preferredExp = job.getPreferredExperiences();

    bool correctRequiredCount = (requiredExp.size() == 2);
    bool correctPreferredCount = (preferredExp.size() == 1);
    bool correctMinYears = (job.getMinimumYearsExperience() == 5);
    bool meetsExperience = job.meetsMinimumExperience(6);
    bool doesNotMeetExperience = !job.meetsMinimumExperience(3);

    testResult("Experience Requirements",
               correctRequiredCount && correctPreferredCount && correctMinYears &&
               meetsExperience && doesNotMeetExperience);
}

// Test benefits and responsibilities
void testBenefitsAndResponsibilities() {
    JobListing job;

    job.addBenefit("Health Insurance");
    job.addBenefit("401k Matching");
    job.addBenefit("Flexible PTO");

    job.addResponsibility("Design scalable systems");
    job.addResponsibility("Mentor junior developers");
    job.addResponsibility("Code reviews");

    std::vector<std::string> benefits = job.getBenefits();
    std::vector<std::string> responsibilities = job.getResponsibilities();

    bool correctBenefitsCount = (benefits.size() == 3);
    bool correctResponsibilitiesCount = (responsibilities.size() == 3);
    bool correctBenefit = (benefits[0] == "Health Insurance");
    bool correctResponsibility = (responsibilities[0] == "Design scalable systems");

    testResult("Benefits and Responsibilities",
               correctBenefitsCount && correctResponsibilitiesCount &&
               correctBenefit && correctResponsibility);
}

// Test company information
void testCompanyInformation() {
    JobListing job;

    job.setCompanySize("50-200 employees");
    job.setIndustry("Financial Technology");
    job.setCompanyWebsite("https://techcorp.com");
    job.setDepartment("Engineering");
    job.setReportingTo("VP of Engineering");

    bool correctSize = (job.getCompanySize() == "50-200 employees");
    bool correctIndustry = (job.getIndustry() == "Financial Technology");
    bool correctWebsite = (job.getCompanyWebsite() == "https://techcorp.com");
    bool correctDepartment = (job.getDepartment() == "Engineering");
    bool correctReporting = (job.getReportingTo() == "VP of Engineering");

    testResult("Company Information",
               correctSize && correctIndustry && correctWebsite &&
               correctDepartment && correctReporting);
}

// Test application details
void testApplicationDetails() {
    JobListing job;

    job.setApplicationDeadline("2024-12-31");
    job.setPostedDate("2024-01-15");
    job.setApplicationUrl("https://techcorp.com/jobs/123");
    job.setContactEmail("hr@techcorp.com");
    job.setIsActive(true);

    bool correctDeadline = (job.getApplicationDeadline() == "2024-12-31");
    bool correctPostedDate = (job.getPostedDate() == "2024-01-15");
    bool correctUrl = (job.getApplicationUrl() == "https://techcorp.com/jobs/123");
    bool correctEmail = (job.getContactEmail() == "hr@techcorp.com");
    bool isActive = job.getIsActive();

    // Test deactivation
    job.setIsActive(false);
    bool isInactive = !job.getIsActive();

    testResult("Application Details",
               correctDeadline && correctPostedDate && correctUrl &&
               correctEmail && isActive && isInactive);
}

// Test tags functionality
void testTagsFunctionality() {
    JobListing job;

    job.addTag("startup");
    job.addTag("fintech");
    job.addTag("AI");
    job.addTag("remote-friendly");

    std::vector<std::string> tags = job.getTags();

    bool correctCount = (tags.size() == 4);
    bool hasStartup = (tags[0] == "startup");
    bool hasFintech = (tags[1] == "fintech");
    bool hasAI = (tags[2] == "AI");
    bool hasRemoteFriendly = (tags[3] == "remote-friendly");

    testResult("Tags Functionality",
               correctCount && hasStartup && hasFintech && hasAI && hasRemoteFriendly);
}

// Test preferred certifications
void testPreferredCertifications() {
    JobListing job;

    job.addPreferredCertification("AWS Certified Solutions Architect");
    job.addPreferredCertification("Certified Kubernetes Administrator");
    job.addPreferredCertification("PMP Certification");

    std::vector<std::string> certifications = job.getPreferredCertifications();

    bool correctCount = (certifications.size() == 3);
    bool hasAWS = (certifications[0] == "AWS Certified Solutions Architect");
    bool hasK8s = (certifications[1] == "Certified Kubernetes Administrator");
    bool hasPMP = (certifications[2] == "PMP Certification");

    testResult("Preferred Certifications",
               correctCount && hasAWS && hasK8s && hasPMP);
}

// Test clear methods
void testClearMethods() {
    JobListing job;

    // Add some data
    job.addRequiredEducation("Bachelor's");
    job.addPreferredEducation("Master's");
    job.addBenefit("Health Insurance");
    job.addResponsibility("Code review");
    job.addTag("startup");
    job.addPreferredCertification("AWS");

    // Verify data exists
    bool hasData = !job.getRequiredEducation().empty() &&
                   !job.getPreferredEducation().empty() &&
                   !job.getBenefits().empty() &&
                   !job.getResponsibilities().empty() &&
                   !job.getTags().empty() &&
                   !job.getPreferredCertifications().empty();

    // Clear all data
    job.clearRequiredEducation();
    job.clearPreferredEducation();
    job.clearBenefits();
    job.clearResponsibilities();
    job.clearTags();
    job.clearPreferredCertifications();

    // Verify data is cleared
    bool dataCleared = job.getRequiredEducation().empty() &&
                       job.getPreferredEducation().empty() &&
                       job.getBenefits().empty() &&
                       job.getResponsibilities().empty() &&
                       job.getTags().empty() &&
                       job.getPreferredCertifications().empty();

    testResult("Clear Methods", hasData && dataCleared);
}

// Test enum conversion methods
void testEnumConversions() {
    // Test JobType conversions
    bool jobTypeToString = (JobListing::jobTypeToString(JobType::FULL_TIME) == "Full-time");
    bool jobTypeFromString = (JobListing::stringToJobType("Full-time") == JobType::FULL_TIME);

    // Test ExperienceLevel conversions
    bool expLevelToString = (JobListing::experienceLevelToString(JobExperienceLevel::SENIOR) == "Senior");
    bool expLevelFromString = (JobListing::stringToExperienceLevel("Senior") == JobExperienceLevel::SENIOR);

    // Test RemoteType conversions
    bool remoteTypeToString = (JobListing::remoteTypeToString(RemoteType::HYBRID) == "Hybrid");
    bool remoteTypeFromString = (JobListing::stringToRemoteType("Hybrid") == RemoteType::HYBRID);

    testResult("Enum Conversions",
               jobTypeToString && jobTypeFromString && expLevelToString &&
               expLevelFromString && remoteTypeToString && remoteTypeFromString);
}

// Test equality operators
void testEqualityOperators() {
    JobListing job1("JOB001", "Engineer", "Tech Corp", "Description", "Location",
                    RemoteType::REMOTE, JobType::FULL_TIME, JobExperienceLevel::SENIOR);
    JobListing job2("JOB001", "Engineer", "Tech Corp", "Different Description", "Different Location",
                    RemoteType::ON_SITE, JobType::PART_TIME, JobExperienceLevel::JUNIOR);
    JobListing job3("JOB002", "Engineer", "Tech Corp", "Description", "Location",
                    RemoteType::REMOTE, JobType::FULL_TIME, JobExperienceLevel::SENIOR);

    // Same ID, title, and company should be equal
    bool areEqual = (job1 == job2);
    // Different ID should not be equal
    bool areNotEqual = (job1 != job3);

    testResult("Equality Operators", areEqual && areNotEqual);
}

// Test complex job listing scenario
void testComplexJobListingScenario() {
    JobListing job("SENIOR_DEV_001", "Senior Full Stack Developer", "TechCorp Inc.",
                   "We are looking for an experienced full stack developer to join our team.",
                   "San Francisco, CA", RemoteType::HYBRID, JobType::FULL_TIME,
                   JobExperienceLevel::SENIOR);

    // Set salary
    job.setSalaryRange(120000.0f, 180000.0f, "USD");

    // Set required skills
    Skills requiredSkills("Programming");
    requiredSkills.addSkill(Skill("JavaScript", 5, EXPERT));
    requiredSkills.addSkill(Skill("React", 3, ADVANCED));
    requiredSkills.addSkill(Skill("Node.js", 3, ADVANCED));
    job.setRequiredSkills(requiredSkills);

    // Set preferred skills
    Skills preferredSkills("Cloud");
    preferredSkills.addSkill(Skill("AWS", 2, INTERMEDIATE));
    preferredSkills.addSkill(Skill("Docker", 2, INTERMEDIATE));
    job.setPreferredSkills(preferredSkills);

    // Add requirements
    job.addRequiredEducation("Bachelor's in Computer Science or equivalent");
    job.setMinimumYearsExperience(5);

    // Add preferences
    job.addPreferredEducation("Master's degree preferred");
    job.addPreferredCertification("AWS Certified Developer");

    // Add benefits and responsibilities
    job.addBenefit("Comprehensive health insurance");
    job.addBenefit("401k with company matching");
    job.addBenefit("Flexible PTO");
    job.addResponsibility("Develop and maintain web applications");
    job.addResponsibility("Collaborate with cross-functional teams");
    job.addResponsibility("Mentor junior developers");

    // Set company info
    job.setCompanySize("200-500 employees");
    job.setIndustry("Software");
    job.setDepartment("Engineering");

    // Add tags
    job.addTag("fintech");
    job.addTag("fast-growing");
    job.addTag("equity");

    // Set dates and application info
    job.setPostedDate("2024-01-15");
    job.setApplicationDeadline("2024-03-15");
    job.setApplicationUrl("https://techcorp.com/careers/senior-dev-001");
    job.setContactEmail("hiring@techcorp.com");

    // Validate the complex job listing
    bool hasBasicInfo = !job.getTitle().empty() && !job.getCompany().empty();
    bool hasSalaryRange = (job.getSalaryMin() > 0 && job.getSalaryMax() > job.getSalaryMin());
    bool hasRequiredSkills = (job.getRequiredSkills().getSkillCount() == 3);
    bool hasPreferredSkills = (job.getPreferredSkills().getSkillCount() == 2);
    bool hasEducationReqs = !job.getRequiredEducation().empty();
    bool hasBenefits = (job.getBenefits().size() == 3);
    bool hasResponsibilities = (job.getResponsibilities().size() == 3);
    bool hasTags = (job.getTags().size() == 3);
    bool hasApplicationInfo = !job.getApplicationUrl().empty();

    testResult("Complex Job Listing Scenario",
               hasBasicInfo && hasSalaryRange && hasRequiredSkills && hasPreferredSkills &&
               hasEducationReqs && hasBenefits && hasResponsibilities && hasTags && hasApplicationInfo);
}

int main() {
    std::cout << "Running JobListing Class Tests..." << std::endl;
    std::cout << "=================================" << std::endl;

    testJobListingDefaultConstructor();
    testJobListingParameterizedConstructor();
    testBasicSettersGetters();
    testSalaryFunctionality();
    testRequiredSkillsFunctionality();
    testPreferredSkillsFunctionality();
    testEducationRequirements();
    testExperienceRequirements();
    testBenefitsAndResponsibilities();
    testCompanyInformation();
    testApplicationDetails();
    testTagsFunctionality();
    testPreferredCertifications();
    testClearMethods();
    testEnumConversions();
    testEqualityOperators();
    testComplexJobListingScenario();

    std::cout << "=================================" << std::endl;
    std::cout << "JobListing tests completed." << std::endl;

    return 0;
}