#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include <algorithm>
#include "../../../src/ds/resume/educations.h"

void testDefaultConstructor() {
    Education edu("MIT", "Bachelor of Science", "Computer Science", "2020-09", "2024-05");
    
    assert(edu.getInstitutionName() == "MIT");
    assert(edu.getDegree() == "Bachelor of Science");
    assert(edu.getFieldOfStudy() == "Computer Science");
    assert(edu.getStartDate() == "2020-09");
    assert(edu.getEndDate() == "2024-05");
    assert(edu.getGPA() == -1.0f);
    assert(edu.getCourses().empty());
    
    std::cout << "Education Default Constructor: PASSED" << std::endl;
}

void testParameterizedConstructor() {
    std::vector<std::string> courses = {"Data Structures", "Algorithms", "Operating Systems"};
    Education edu("Stanford", "Master of Science", "Computer Science", "2024-09", "2026-06", 3.8f, courses);
    
    assert(edu.getInstitutionName() == "Stanford");
    assert(edu.getDegree() == "Master of Science");
    assert(edu.getFieldOfStudy() == "Computer Science");
    assert(edu.getStartDate() == "2024-09");
    assert(edu.getEndDate() == "2026-06");
    assert(edu.getGPA() == 3.8f);
    assert(edu.getCourses().size() == 3);
    assert(edu.getCourses()[0] == "Data Structures");
    assert(edu.getCourses()[1] == "Algorithms");
    assert(edu.getCourses()[2] == "Operating Systems");
    
    std::cout << "Education Parameterized Constructor: PASSED" << std::endl;
}

void testSetters() {
    Education edu("Old University", "Old Degree", "Old Field", "2020-01", "2024-01");
    
    edu.setInstitutionName("New University");
    edu.setDegree("New Degree");
    edu.setFieldOfStudy("New Field");
    edu.setStartDate("2021-09");
    edu.setEndDate("2025-05");
    edu.setGPA(3.9f);
    
    assert(edu.getInstitutionName() == "New University");
    assert(edu.getDegree() == "New Degree");
    assert(edu.getFieldOfStudy() == "New Field");
    assert(edu.getStartDate() == "2021-09");
    assert(edu.getEndDate() == "2025-05");
    assert(edu.getGPA() == 3.9f);
    
    std::cout << "Education Setters: PASSED" << std::endl;
}

void testAddCourse() {
    Education edu("Harvard", "PhD", "Computer Science", "2025-09", "Present");
    
    assert(edu.getCourses().empty());
    
    edu.addCourse("Machine Learning");
    assert(edu.getCourses().size() == 1);
    assert(edu.getCourses()[0] == "Machine Learning");
    
    edu.addCourse("Deep Learning");
    edu.addCourse("Natural Language Processing");
    assert(edu.getCourses().size() == 3);
    assert(edu.getCourses()[1] == "Deep Learning");
    assert(edu.getCourses()[2] == "Natural Language Processing");
    
    std::cout << "Education Add Course: PASSED" << std::endl;
}

void testGPAHandling() {
    // Test default GPA (-1.0)
    Education edu1("University A", "Bachelor", "Engineering", "2020-09", "2024-05");
    assert(edu1.getGPA() == -1.0f);
    
    // Test valid GPA
    Education edu2("University B", "Master", "Science", "2024-09", "2026-05", 4.0f);
    assert(edu2.getGPA() == 4.0f);
    
    // Test GPA modification
    edu1.setGPA(3.5f);
    assert(edu1.getGPA() == 3.5f);
    
    // Test edge cases
    edu1.setGPA(0.0f);
    assert(edu1.getGPA() == 0.0f);
    
    edu1.setGPA(4.0f);
    assert(edu1.getGPA() == 4.0f);
    
    std::cout << "Education GPA Handling: PASSED" << std::endl;
}

void testDateFormats() {
    // Test various date formats
    Education edu1("University", "Degree", "Field", "2020-01", "2024-12");
    assert(edu1.getStartDate() == "2020-01");
    assert(edu1.getEndDate() == "2024-12");
    
    // Test with "Present"
    Education edu2("University", "Degree", "Field", "2024-09", "Present");
    assert(edu2.getStartDate() == "2024-09");
    assert(edu2.getEndDate() == "Present");
    
    std::cout << "Education Date Formats: PASSED" << std::endl;
}

void testCourseOperations() {
    std::vector<std::string> initialCourses = {"Course 1", "Course 2"};
    Education edu("University", "Degree", "Field", "2020-09", "2024-05", 3.0f, initialCourses);
    
    // Test initial courses
    assert(edu.getCourses().size() == 2);
    
    // Test adding more courses
    edu.addCourse("Course 3");
    edu.addCourse("Course 4");
    assert(edu.getCourses().size() == 4);
    
    // Test course order is maintained
    std::vector<std::string> courses = edu.getCourses();
    assert(courses[0] == "Course 1");
    assert(courses[1] == "Course 2");
    assert(courses[2] == "Course 3");
    assert(courses[3] == "Course 4");
    
    std::cout << "Education Course Operations: PASSED" << std::endl;
}

void testEmptyStrings() {
    // Test with empty strings
    Education edu("", "", "", "", "");
    assert(edu.getInstitutionName() == "");
    assert(edu.getDegree() == "");
    assert(edu.getFieldOfStudy() == "");
    assert(edu.getStartDate() == "");
    assert(edu.getEndDate() == "");
    
    // Test adding empty course
    edu.addCourse("");
    assert(edu.getCourses().size() == 1);
    assert(edu.getCourses()[0] == "");
    
    std::cout << "Education Empty Strings: PASSED" << std::endl;
}

void testCopyBehavior() {
    std::vector<std::string> courses = {"AI", "ML", "DL"};
    Education original("MIT", "MS", "CS", "2023-09", "2025-05", 3.9f, courses);
    
    // Test that getCourses returns a copy (modifications don't affect original)
    std::vector<std::string> coursesCopy = original.getCourses();
    coursesCopy.push_back("New Course");
    
    assert(original.getCourses().size() == 3);  // Original should be unchanged
    assert(coursesCopy.size() == 4);
    
    std::cout << "Education Copy Behavior: PASSED" << std::endl;
}

// Educations class tests
void testEducationsDefaultConstructor() {
    Educations educations;
    
    assert(educations.getEducations().empty());
    assert(educations.getEducations().size() == 0);
    
    std::cout << "Educations Default Constructor: PASSED" << std::endl;
}

void testEducationsAddSingle() {
    Educations educations;
    
    Education edu("MIT", "Bachelor of Science", "Computer Science", "2020-09", "2024-05", 3.8f);
    educations.addEducation(edu);
    
    std::vector<Education> eduList = educations.getEducations();
    assert(eduList.size() == 1);
    assert(eduList[0].getInstitutionName() == "MIT");
    assert(eduList[0].getDegree() == "Bachelor of Science");
    assert(eduList[0].getFieldOfStudy() == "Computer Science");
    assert(eduList[0].getGPA() == 3.8f);
    
    std::cout << "Educations Add Single: PASSED" << std::endl;
}

void testEducationsAddMultiple() {
    Educations educations;
    
    Education undergrad("Stanford University", "Bachelor of Science", "Computer Science", "2018-09", "2022-06", 3.7f);
    Education grad("MIT", "Master of Science", "Artificial Intelligence", "2022-09", "2024-06", 3.9f);
    Education phd("Harvard", "PhD", "Machine Learning", "2024-09", "Present", -1.0f);
    
    educations.addEducation(undergrad);
    educations.addEducation(grad);
    educations.addEducation(phd);
    
    std::vector<Education> eduList = educations.getEducations();
    assert(eduList.size() == 3);
    assert(eduList[0].getInstitutionName() == "Stanford University");
    assert(eduList[1].getInstitutionName() == "MIT");
    assert(eduList[2].getInstitutionName() == "Harvard");
    
    std::cout << "Educations Add Multiple: PASSED" << std::endl;
}

void testEducationsClearEducations() {
    Educations educations;
    
    // Add several educations
    educations.addEducation(Education("UC Berkeley", "Bachelor", "Engineering", "2019-09", "2023-05", 3.5f));
    educations.addEducation(Education("Caltech", "Master", "Physics", "2023-09", "2025-06", 3.8f));
    educations.addEducation(Education("Yale", "PhD", "Chemistry", "2025-09", "Present", -1.0f));
    
    assert(educations.getEducations().size() == 3);
    
    // Clear all educations
    educations.clearEducations();
    assert(educations.getEducations().empty());
    assert(educations.getEducations().size() == 0);
    
    // Test clearing empty collection (should not crash)
    educations.clearEducations();
    assert(educations.getEducations().empty());
    
    std::cout << "Educations Clear Educations: PASSED" << std::endl;
}

void testEducationsRemoveEducation() {
    Educations educations;
    
    Education edu1("Princeton", "Bachelor", "Mathematics", "2020-09", "2024-05", 3.9f);
    Education edu2("Columbia", "Master", "Statistics", "2024-09", "2026-05", 3.7f);
    Education edu3("NYU", "Certificate", "Data Science", "2026-06", "2026-12", -1.0f);
    
    educations.addEducation(edu1);
    educations.addEducation(edu2);
    educations.addEducation(edu3);
    
    assert(educations.getEducations().size() == 3);
    
    // Remove middle education
    educations.removeEducation("Columbia");
    std::vector<Education> eduList = educations.getEducations();
    assert(eduList.size() == 2);
    assert(eduList[0].getInstitutionName() == "Princeton");
    assert(eduList[1].getInstitutionName() == "NYU");
    
    // Remove first education
    educations.removeEducation("Princeton");
    eduList = educations.getEducations();
    assert(eduList.size() == 1);
    assert(eduList[0].getInstitutionName() == "NYU");
    
    // Remove last education
    educations.removeEducation("NYU");
    assert(educations.getEducations().empty());
    
    std::cout << "Educations Remove Education: PASSED" << std::endl;
}

void testEducationsRemoveNonexistent() {
    Educations educations;
    
    Education edu("Brown University", "Bachelor", "Liberal Arts", "2021-09", "2025-05", 3.6f);
    educations.addEducation(edu);
    
    assert(educations.getEducations().size() == 1);
    
    // Try to remove non-existent education
    educations.removeEducation("Nonexistent University");
    assert(educations.getEducations().size() == 1);  // Should remain unchanged
    assert(educations.getEducations()[0].getInstitutionName() == "Brown University");
    
    // Try to remove from empty collection
    educations.clearEducations();
    educations.removeEducation("Any University");
    assert(educations.getEducations().empty());
    
    std::cout << "Educations Remove Nonexistent: PASSED" << std::endl;
}

void testEducationsRemoveDuplicateInstitutions() {
    Educations educations;
    
    // Add multiple educations from same institution
    Education undergrad("University of Chicago", "Bachelor", "Economics", "2018-09", "2022-06", 3.8f);
    Education grad("University of Chicago", "Master", "Business", "2022-09", "2024-06", 3.9f);
    Education other("Cornell", "Bachelor", "Engineering", "2020-09", "2024-05", 3.7f);
    
    educations.addEducation(undergrad);
    educations.addEducation(grad);
    educations.addEducation(other);
    
    assert(educations.getEducations().size() == 3);
    
    // Remove all educations from "University of Chicago" - should remove both
    educations.removeEducation("University of Chicago");
    std::vector<Education> eduList = educations.getEducations();
    assert(eduList.size() == 1);
    assert(eduList[0].getInstitutionName() == "Cornell");
    
    std::cout << "Educations Remove Duplicate Institutions: PASSED" << std::endl;
}

void testEducationsOrder() {
    Educations educations;
    
    Education first("First University", "Bachelor", "First Major", "2020-09", "2024-05", 3.5f);
    Education second("Second University", "Master", "Second Major", "2024-09", "2026-05", 3.7f);
    Education third("Third University", "PhD", "Third Major", "2026-09", "Present", -1.0f);
    
    educations.addEducation(first);
    educations.addEducation(second);
    educations.addEducation(third);
    
    std::vector<Education> eduList = educations.getEducations();
    
    // Verify order is maintained
    assert(eduList[0].getInstitutionName() == "First University");
    assert(eduList[1].getInstitutionName() == "Second University");
    assert(eduList[2].getInstitutionName() == "Third University");
    
    std::cout << "Educations Order: PASSED" << std::endl;
}

void testEducationsCopyBehavior() {
    Educations educations;
    
    Education original("Original University", "Bachelor", "Original Major", "2020-09", "2024-05", 3.6f);
    educations.addEducation(original);
    
    // Get copy of educations
    std::vector<Education> eduList = educations.getEducations();
    
    // Modify the copy (add new education)
    eduList.push_back(Education("Added University", "Master", "Added Major", "2024-09", "2026-05", 3.8f));
    
    // Original should be unchanged
    std::vector<Education> originalList = educations.getEducations();
    assert(originalList.size() == 1);  // Original unchanged
    assert(eduList.size() == 2);       // Copy was modified
    
    std::cout << "Educations Copy Behavior: PASSED" << std::endl;
}

void testEducationsModifyAfterOperations() {
    Educations educations;
    
    // Add initial educations
    educations.addEducation(Education("Harvard", "Bachelor", "History", "2019-09", "2023-05", 3.8f));
    educations.addEducation(Education("Yale", "Master", "Literature", "2023-09", "2025-05", 3.9f));
    educations.addEducation(Education("Princeton", "Certificate", "Writing", "2025-06", "2025-12", -1.0f));
    
    // Remove one
    educations.removeEducation("Yale");
    assert(educations.getEducations().size() == 2);
    
    // Add more after removal
    educations.addEducation(Education("Columbia", "Master", "Journalism", "2025-09", "2027-05", 3.7f));
    educations.addEducation(Education("NYU", "Certificate", "Digital Media", "2027-01", "2027-06", -1.0f));
    
    std::vector<Education> eduList = educations.getEducations();
    assert(eduList.size() == 4);
    assert(eduList[0].getInstitutionName() == "Harvard");
    assert(eduList[1].getInstitutionName() == "Princeton");
    assert(eduList[2].getInstitutionName() == "Columbia");
    assert(eduList[3].getInstitutionName() == "NYU");
    
    std::cout << "Educations Modify After Operations: PASSED" << std::endl;
}

void testEducationsComplexOperations() {
    Educations educations;
    
    // Add diverse educations
    educations.addEducation(Education("MIT", "Bachelor", "Computer Science", "2018-09", "2022-06", 3.9f));
    educations.addEducation(Education("Stanford", "Master", "AI", "2022-09", "2024-06", 4.0f));
    educations.addEducation(Education("UC Berkeley", "Certificate", "Machine Learning", "2024-07", "2024-12", -1.0f));
    educations.addEducation(Education("Caltech", "PhD", "Robotics", "2025-01", "Present", -1.0f));
    educations.addEducation(Education("Harvard", "Executive Program", "Leadership", "2024-06", "2024-08", -1.0f));
    
    assert(educations.getEducations().size() == 5);
    
    // Remove some educations
    educations.removeEducation("UC Berkeley");
    educations.removeEducation("Harvard");
    
    std::vector<Education> remaining = educations.getEducations();
    assert(remaining.size() == 3);
    assert(remaining[0].getInstitutionName() == "MIT");
    assert(remaining[1].getInstitutionName() == "Stanford");
    assert(remaining[2].getInstitutionName() == "Caltech");
    
    // Add more
    educations.addEducation(Education("Carnegie Mellon", "Master", "Software Engineering", "2025-09", "2027-05", 3.8f));
    assert(educations.getEducations().size() == 4);
    
    // Clear all
    educations.clearEducations();
    assert(educations.getEducations().empty());
    
    std::cout << "Educations Complex Operations: PASSED" << std::endl;
}

void testEducationsWithCourses() {
    Educations educations;
    
    // Create educations with courses
    std::vector<std::string> csCourses = {"Data Structures", "Algorithms", "Operating Systems", "Database Systems"};
    std::vector<std::string> mathCourses = {"Calculus", "Linear Algebra", "Statistics", "Discrete Mathematics"};
    
    Education csEducation("MIT", "Bachelor", "Computer Science", "2020-09", "2024-05", 3.9f, csCourses);
    Education mathEducation("Harvard", "Master", "Mathematics", "2024-09", "2026-05", 3.8f, mathCourses);
    
    educations.addEducation(csEducation);
    educations.addEducation(mathEducation);
    
    std::vector<Education> eduList = educations.getEducations();
    assert(eduList.size() == 2);
    
    // Verify courses are preserved
    assert(eduList[0].getCourses().size() == 4);
    assert(eduList[0].getCourses()[0] == "Data Structures");
    assert(eduList[1].getCourses().size() == 4);
    assert(eduList[1].getCourses()[0] == "Calculus");
    
    std::cout << "Educations With Courses: PASSED" << std::endl;
}

void testEducationsWithVariousGPAs() {
    Educations educations;
    
    // Add educations with different GPA scenarios
    educations.addEducation(Education("Perfect GPA University", "Bachelor", "Math", "2020-09", "2024-05", 4.0f));
    educations.addEducation(Education("Good GPA University", "Master", "Physics", "2024-09", "2026-05", 3.7f));
    educations.addEducation(Education("Average GPA University", "Certificate", "Chemistry", "2026-06", "2026-12", 3.0f));
    educations.addEducation(Education("No GPA University", "PhD", "Biology", "2027-01", "Present", -1.0f));
    
    std::vector<Education> eduList = educations.getEducations();
    assert(eduList.size() == 4);
    
    // Verify GPAs
    assert(eduList[0].getGPA() == 4.0f);
    assert(eduList[1].getGPA() == 3.7f);
    assert(eduList[2].getGPA() == 3.0f);
    assert(eduList[3].getGPA() == -1.0f);
    
    // Remove education with perfect GPA
    educations.removeEducation("Perfect GPA University");
    eduList = educations.getEducations();
    assert(eduList.size() == 3);
    
    std::cout << "Educations With Various GPAs: PASSED" << std::endl;
}

void testEducationsLargeScaleOperations() {
    Educations educations;
    
    // Add many educations
    for (int i = 0; i < 20; ++i) {
        std::string institutionName = "University " + std::to_string(i);
        std::string degree = (i % 2 == 0) ? "Bachelor" : "Master";
        std::string field = "Field " + std::to_string(i);
        float gpa = 2.5f + (i % 4) * 0.5f;  // GPAs from 2.5 to 4.0
        
        educations.addEducation(Education(institutionName, degree, field, "2020-09", "2024-05", gpa));
    }
    
    assert(educations.getEducations().size() == 20);
    
    // Remove every other education
    for (int i = 0; i < 20; i += 2) {
        std::string institutionName = "University " + std::to_string(i);
        educations.removeEducation(institutionName);
    }
    
    assert(educations.getEducations().size() == 10);
    
    // Verify remaining educations are odd-numbered
    std::vector<Education> remaining = educations.getEducations();
    for (size_t i = 0; i < remaining.size(); ++i) {
        std::string expectedName = "University " + std::to_string((i * 2) + 1);
        assert(remaining[i].getInstitutionName() == expectedName);
    }
    
    std::cout << "Educations Large Scale Operations: PASSED" << std::endl;
}

void testEducationsEmptyStringHandling() {
    Educations educations;
    
    // Add education with empty institution name
    educations.addEducation(Education("", "Bachelor", "Unknown", "2020-09", "2024-05", 3.0f));
    educations.addEducation(Education("Valid University", "Master", "Known Field", "2024-09", "2026-05", 3.5f));
    
    assert(educations.getEducations().size() == 2);
    
    // Remove education with empty institution name
    educations.removeEducation("");
    std::vector<Education> eduList = educations.getEducations();
    assert(eduList.size() == 1);
    assert(eduList[0].getInstitutionName() == "Valid University");
    
    std::cout << "Educations Empty String Handling: PASSED" << std::endl;
}

int main() {
    std::cout << "Running Education and Educations Class Tests..." << std::endl;
    std::cout << "===============================================" << std::endl;
    
    // Education class tests
    testDefaultConstructor();
    testParameterizedConstructor();
    testSetters();
    testAddCourse();
    testGPAHandling();
    testDateFormats();
    testCourseOperations();
    testEmptyStrings();
    testCopyBehavior();
    
    // Educations class tests
    testEducationsDefaultConstructor();
    testEducationsAddSingle();
    testEducationsAddMultiple();
    testEducationsClearEducations();
    testEducationsRemoveEducation();
    testEducationsRemoveNonexistent();
    testEducationsRemoveDuplicateInstitutions();
    testEducationsOrder();
    testEducationsCopyBehavior();
    testEducationsModifyAfterOperations();
    testEducationsComplexOperations();
    testEducationsWithCourses();
    testEducationsWithVariousGPAs();
    testEducationsLargeScaleOperations();
    testEducationsEmptyStringHandling();
    
    std::cout << "===============================================" << std::endl;
    std::cout << "Education and Educations tests completed." << std::endl;
    
    return 0;
}