#include <iostream>
#include <cassert>
#include <string>
#include "../src/ds/skill.h"

// Test helper function to display test results
void testResult(const std::string& testName, bool result) {
    std::cout << testName << ": " << (result ? "PASSED" : "FAILED") << std::endl;
}

// Test Skill constructor and getters
void testSkillConstructorAndGetters() {
    Skill skill("C++", 5, ADVANCED);
    
    bool nameTest = (skill.getName() == "C++");
    bool yearsTest = (skill.getYearsOfExperience() == 5);
    bool levelTest = (skill.getLevel() == ADVANCED);
    
    testResult("Skill Constructor and Getters", nameTest && yearsTest && levelTest);
}

// Test Skill setLevel functionality
void testSkillSetLevel() {
    Skill skill("Python", 2, BEGINNER);
    
    skill.setLevel(INTERMEDIATE);
    bool levelChanged = (skill.getLevel() == INTERMEDIATE);
    
    // Original name and years should remain unchanged
    bool nameUnchanged = (skill.getName() == "Python");
    bool yearsUnchanged = (skill.getYearsOfExperience() == 2);
    
    testResult("Skill setLevel", levelChanged && nameUnchanged && yearsUnchanged);
}

// Test Skill equality operator
void testSkillEquality() {
    Skill skill1("Java", 3, INTERMEDIATE);
    Skill skill2("Java", 3, INTERMEDIATE);
    Skill skill3("Java", 4, INTERMEDIATE);  // Different years
    Skill skill4("JavaScript", 3, INTERMEDIATE);  // Different name
    Skill skill5("Java", 3, ADVANCED);  // Different level
    
    bool equalSkills = (skill1 == skill2);
    bool differentYears = !(skill1 == skill3);
    bool differentNames = !(skill1 == skill4);
    bool differentLevels = !(skill1 == skill5);
    
    testResult("Skill Equality Operator", equalSkills && differentYears && differentNames && differentLevels);
}

// Test all skill levels
void testAllSkillLevels() {
    Skill beginner("HTML", 1, BEGINNER);
    Skill intermediate("CSS", 2, INTERMEDIATE);
    Skill advanced("JavaScript", 4, ADVANCED);
    Skill expert("Docker", 6, EXPERT);
    
    bool allLevelsWork = (beginner.getLevel() == BEGINNER) &&
                        (intermediate.getLevel() == INTERMEDIATE) &&
                        (advanced.getLevel() == ADVANCED) &&
                        (expert.getLevel() == EXPERT);
    
    testResult("All Skill Levels", allLevelsWork);
}

int main() {
    std::cout << "Running Skill Class Tests..." << std::endl;
    std::cout << "============================" << std::endl;
    
    testSkillConstructorAndGetters();
    testSkillSetLevel();
    testSkillEquality();
    testAllSkillLevels();
    
    std::cout << "============================" << std::endl;
    std::cout << "Skill tests completed." << std::endl;
    
    return 0;
}