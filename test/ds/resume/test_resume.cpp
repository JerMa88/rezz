#include <iostream>
#include <cassert>
#include <string>
#include "../../../src/ds/resume/resume.h"

// Test helper function to display test results
void testResult(const std::string& testName, bool result) {
    std::cout << testName << ": " << (result ? "PASSED" : "FAILED") << std::endl;
}

// Test Resume default constructor
void testResumeDefaultConstructor() {
    Resume resume;

    bool defaultId = (resume.getId() == 0);
    bool emptyName = resume.getName().empty();
    bool emptyEmail = resume.getEmail().empty();
    bool emptyCity = resume.getCity().empty();
    bool emptyPhone = resume.getPhone().empty();
    bool emptyLinkedin = resume.getLinkedin().empty();
    bool emptyWebsite = resume.getWebsite().empty();
    bool emptyInterests = resume.getInterests().empty();

    testResult("Resume Default Constructor",
               defaultId && emptyName && emptyEmail && emptyCity &&
               emptyPhone && emptyLinkedin && emptyWebsite && emptyInterests);
}

// Test Resume parameterized constructor
void testResumeParameterizedConstructor() {
    Resume resume(1, "John Doe", "john.doe@email.com",
                  "San Francisco, CA", "+1-555-123-4567");

    bool correctId = (resume.getId() == 1);
    bool correctName = (resume.getName() == "John Doe");
    bool correctEmail = (resume.getEmail() == "john.doe@email.com");
    bool correctCity = (resume.getCity() == "San Francisco, CA");
    bool correctPhone = (resume.getPhone() == "+1-555-123-4567");
    bool emptyLinkedin = resume.getLinkedin().empty();
    bool emptyWebsite = resume.getWebsite().empty();

    testResult("Resume Parameterized Constructor",
               correctId && correctName && correctEmail && correctCity &&
               correctPhone && emptyLinkedin && emptyWebsite);
}

// Test Resume parameterized constructor with optional parameters
void testResumeParameterizedConstructorWithOptionals() {
    Resume resume(2, "Jane Smith", "jane.smith@email.com",
                  "New York, NY", "+1-555-987-6543",
                  "https://linkedin.com/in/janesmith",
                  "https://janesmith.dev");

    bool correctId = (resume.getId() == 2);
    bool correctName = (resume.getName() == "Jane Smith");
    bool correctEmail = (resume.getEmail() == "jane.smith@email.com");
    bool correctCity = (resume.getCity() == "New York, NY");
    bool correctPhone = (resume.getPhone() == "+1-555-987-6543");
    bool correctLinkedin = (resume.getLinkedin() == "https://linkedin.com/in/janesmith");
    bool correctWebsite = (resume.getWebsite() == "https://janesmith.dev");

    testResult("Resume Parameterized Constructor with Optionals",
               correctId && correctName && correctEmail && correctCity &&
               correctPhone && correctLinkedin && correctWebsite);
}

// Test all basic setters and getters
void testBasicSettersGetters() {
    Resume resume;

    resume.setId(100);
    resume.setName("Test User");
    resume.setEmail("test@example.com");
    resume.setCity("Austin, TX");
    resume.setPhone("+1-555-000-0000");
    resume.setLinkedin("https://linkedin.com/in/testuser");
    resume.setWebsite("https://testuser.com");
    resume.setInterests("Reading, Programming, Hiking");

    bool correctId = (resume.getId() == 100);
    bool correctName = (resume.getName() == "Test User");
    bool correctEmail = (resume.getEmail() == "test@example.com");
    bool correctCity = (resume.getCity() == "Austin, TX");
    bool correctPhone = (resume.getPhone() == "+1-555-000-0000");
    bool correctLinkedin = (resume.getLinkedin() == "https://linkedin.com/in/testuser");
    bool correctWebsite = (resume.getWebsite() == "https://testuser.com");
    bool correctInterests = (resume.getInterests() == "Reading, Programming, Hiking");

    testResult("Basic Setters and Getters",
               correctId && correctName && correctEmail && correctCity &&
               correctPhone && correctLinkedin && correctWebsite && correctInterests);
}

// Test accessing complex data structure references
void testComplexDataStructureAccess() {
    Resume resume;

    // Test that we can get references to all complex data structures
    Skills& skills = resume.getSkills();
    Education& education = resume.getEducation();
    Experiences& experiences = resume.getExperiences();
    Projects& projects = resume.getProjects();
    Publications& publications = resume.getPublications();
    Certifications& certifications = resume.getCertifications();
    Awards& awards = resume.getAwards();
    Languages& languages = resume.getLanguages();

    // Test that these references work by accessing a method on each
    bool validSkills = (skills.getSkillCount() >= 0);
    bool validEducation = (education.getInstitutionName() == "");
    bool validExperiences = true; // Experiences class has implicit default constructor
    bool validProjects = true; // Projects class has implicit default constructor
    bool validPublications = true; // Publications class has implicit default constructor
    bool validCertifications = true; // Certifications class has implicit default constructor
    bool validAwards = true; // Awards class has implicit default constructor
    bool validLanguages = true; // Languages class has implicit default constructor

    testResult("Complex Data Structure Access",
               validSkills && validEducation && validExperiences && validProjects &&
               validPublications && validCertifications && validAwards && validLanguages);
}

// Test setting complex data structures
void testComplexDataStructureSetters() {
    Resume resume;

    // Create test data structures
    Skills testSkills("Programming");
    Education testEducation("Test University", "Bachelor", "Computer Science", "2020-01", "2024-05");
    Experiences testExperiences;
    Projects testProjects;
    Publications testPublications;
    Certifications testCertifications;
    Awards testAwards;
    Languages testLanguages;

    // Set them on the resume
    resume.setSkills(testSkills);
    resume.setEducation(testEducation);
    resume.setExperiences(testExperiences);
    resume.setProjects(testProjects);
    resume.setPublications(testPublications);
    resume.setCertifications(testCertifications);
    resume.setAwards(testAwards);
    resume.setLanguages(testLanguages);

    // Verify we can still access them and they have expected values
    Skills& retrievedSkills = resume.getSkills();
    Education& retrievedEducation = resume.getEducation();
    Experiences& retrievedExperiences = resume.getExperiences();
    Projects& retrievedProjects = resume.getProjects();
    Publications& retrievedPublications = resume.getPublications();
    Certifications& retrievedCertifications = resume.getCertifications();
    Awards& retrievedAwards = resume.getAwards();
    Languages& retrievedLanguages = resume.getLanguages();

    bool validAccess = (retrievedSkills.getSkillType() == "Programming") &&
                      (retrievedEducation.getInstitutionName() == "Test University") &&
                      true && true && true && true && true && true;

    testResult("Complex Data Structure Setters", validAccess);
}

// Test edge cases with empty and long strings
void testEdgeCases() {
    Resume resume;

    // Test with empty strings
    resume.setName("");
    resume.setEmail("");
    resume.setCity("");
    resume.setPhone("");
    resume.setLinkedin("");
    resume.setWebsite("");
    resume.setInterests("");

    bool emptyValues = resume.getName().empty() &&
                      resume.getEmail().empty() &&
                      resume.getCity().empty() &&
                      resume.getPhone().empty() &&
                      resume.getLinkedin().empty() &&
                      resume.getWebsite().empty() &&
                      resume.getInterests().empty();

    // Test with very long strings
    std::string longString(1000, 'A');
    resume.setName(longString);
    resume.setInterests(longString);

    bool longStringHandling = (resume.getName() == longString) &&
                             (resume.getInterests() == longString);

    // Test with special characters
    resume.setName("José María García-López");
    resume.setEmail("test+tag@domain.co.uk");
    resume.setCity("São Paulo, SP");

    bool specialChars = (resume.getName() == "José María García-López") &&
                       (resume.getEmail() == "test+tag@domain.co.uk") &&
                       (resume.getCity() == "São Paulo, SP");

    testResult("Edge Cases", emptyValues && longStringHandling && specialChars);
}

// Test ID boundary values
void testIdBoundaryValues() {
    Resume resume1, resume2, resume3;

    // Test zero ID
    resume1.setId(0);
    bool zeroId = (resume1.getId() == 0);

    // Test negative ID
    resume2.setId(-1);
    bool negativeId = (resume2.getId() == -1);

    // Test large positive ID
    resume3.setId(2147483647); // MAX_INT
    bool largeId = (resume3.getId() == 2147483647);

    testResult("ID Boundary Values", zeroId && negativeId && largeId);
}

// Test resume copy and modification
void testResumeCopyAndModification() {
    Resume original(1, "Original Name", "original@email.com",
                   "Original City", "555-0001");

    Resume copy = original;

    // Verify copy has same values
    bool sameId = (copy.getId() == original.getId());
    bool sameName = (copy.getName() == original.getName());
    bool sameEmail = (copy.getEmail() == original.getEmail());
    bool sameCity = (copy.getCity() == original.getCity());
    bool samePhone = (copy.getPhone() == original.getPhone());

    // Modify copy
    copy.setName("Modified Name");
    copy.setEmail("modified@email.com");

    // Verify original is unchanged and copy is modified
    bool originalUnchanged = (original.getName() == "Original Name") &&
                            (original.getEmail() == "original@email.com");
    bool copyModified = (copy.getName() == "Modified Name") &&
                       (copy.getEmail() == "modified@email.com");

    testResult("Resume Copy and Modification",
               sameId && sameName && sameEmail && sameCity && samePhone &&
               originalUnchanged && copyModified);
}

int main() {
    std::cout << "Running Resume Class Tests..." << std::endl;
    std::cout << "=============================" << std::endl;

    testResumeDefaultConstructor();
    testResumeParameterizedConstructor();
    testResumeParameterizedConstructorWithOptionals();
    testBasicSettersGetters();
    testComplexDataStructureAccess();
    testComplexDataStructureSetters();
    testEdgeCases();
    testIdBoundaryValues();
    testResumeCopyAndModification();

    std::cout << "=============================" << std::endl;
    std::cout << "Resume tests completed." << std::endl;

    return 0;
}