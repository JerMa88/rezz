#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include "../../../src/ds/resume/skills.h"

// Test helper function to display test results
void testResult(const std::string& testName, bool result) {
    std::cout << testName << ": " << (result ? "PASSED" : "FAILED") << std::endl;
}

// Test Skills default constructor
void testSkillsDefaultConstructor() {
    Skills skills;
    
    bool emptyCount = (skills.getSkillCount() == 0);
    bool emptyType = (skills.getSkillType() == "");
    std::vector<Skill> allSkills = skills.getAllSkills();
    bool emptyList = allSkills.empty();
    
    testResult("Skills Default Constructor", emptyCount && emptyType && emptyList);
}

// Test Skills parameterized constructor
void testSkillsParameterizedConstructor() {
    Skills skills("Programming Languages");
    
    bool correctType = (skills.getSkillType() == "Programming Languages");
    bool emptyCount = (skills.getSkillCount() == 0);
    
    testResult("Skills Parameterized Constructor", correctType && emptyCount);
}

// Test adding skills
void testAddSkill() {
    Skills skills("DevOps");
    Skill docker("Docker", 3, INTERMEDIATE);
    Skill kubernetes("Kubernetes", 2, BEGINNER);
    
    skills.addSkill(docker);
    skills.addSkill(kubernetes);
    
    bool correctCount = (skills.getSkillCount() == 2);
    bool hasDocker = skills.hasSkill(docker);
    bool hasKubernetes = skills.hasSkill(kubernetes);
    
    testResult("Add Skills", correctCount && hasDocker && hasKubernetes);
}

// Test hasSkill functionality
void testHasSkill() {
    Skills skills("Frameworks");
    Skill react("React", 4, ADVANCED);
    Skill angular("Angular", 2, INTERMEDIATE);
    Skill vue("Vue.js", 1, BEGINNER);
    
    skills.addSkill(react);
    skills.addSkill(angular);
    
    bool hasReact = skills.hasSkill(react);
    bool hasAngular = skills.hasSkill(angular);
    bool doesNotHaveVue = !skills.hasSkill(vue);
    
    testResult("Has Skill", hasReact && hasAngular && doesNotHaveVue);
}

// Test removing skills
void testRemoveSkill() {
    Skills skills("Tools");
    Skill git("Git", 5, ADVANCED);
    Skill jenkins("Jenkins", 3, INTERMEDIATE);
    Skill jira("Jira", 2, INTERMEDIATE);
    
    skills.addSkill(git);
    skills.addSkill(jenkins);
    skills.addSkill(jira);
    
    // Verify all skills are added
    bool allAdded = (skills.getSkillCount() == 3);
    
    // Remove Jenkins
    skills.removeSkill(jenkins);
    
    bool jenkinsRemoved = !skills.hasSkill(jenkins);
    bool stillHasGit = skills.hasSkill(git);
    bool stillHasJira = skills.hasSkill(jira);
    bool correctCount = (skills.getSkillCount() == 2);
    
    testResult("Remove Skill", allAdded && jenkinsRemoved && stillHasGit && stillHasJira && correctCount);
}

// Test getAllSkills functionality
void testGetAllSkills() {
    Skills skills("Languages");
    Skill cpp("C++", 4, ADVANCED);
    Skill python("Python", 3, INTERMEDIATE);
    Skill java("Java", 5, EXPERT);
    
    skills.addSkill(cpp);
    skills.addSkill(python);
    skills.addSkill(java);
    
    std::vector<Skill> allSkills = skills.getAllSkills();
    
    bool correctSize = (allSkills.size() == 3);
    bool containsCpp = false;
    bool containsPython = false;
    bool containsJava = false;
    
    for (const auto& skill : allSkills) {
        if (skill == cpp) containsCpp = true;
        if (skill == python) containsPython = true;
        if (skill == java) containsJava = true;
    }
    
    testResult("Get All Skills", correctSize && containsCpp && containsPython && containsJava);
}

// Test skill type setter and getter
void testSkillTypeSetterGetter() {
    Skills skills("Original Type");
    
    bool originalType = (skills.getSkillType() == "Original Type");
    
    skills.setSkillType("New Type");
    bool newType = (skills.getSkillType() == "New Type");
    
    testResult("Skill Type Setter/Getter", originalType && newType);
}

// Test duplicate skill handling
void testDuplicateSkills() {
    Skills skills("Test");
    Skill skill1("Test Skill", 2, INTERMEDIATE);
    Skill skill2("Test Skill", 2, INTERMEDIATE); // Same skill
    
    skills.addSkill(skill1);
    skills.addSkill(skill2); // Adding duplicate
    
    // Should have 2 identical skills
    bool correctCount = (skills.getSkillCount() == 2);
    bool hasSkill = skills.hasSkill(skill1);
    
    testResult("Duplicate Skills", correctCount && hasSkill);
}

int main() {
    std::cout << "Running Skills Class Tests..." << std::endl;
    std::cout << "==============================" << std::endl;
    
    testSkillsDefaultConstructor();
    testSkillsParameterizedConstructor();
    testAddSkill();
    testHasSkill();
    testRemoveSkill();
    testGetAllSkills();
    testSkillTypeSetterGetter();
    testDuplicateSkills();
    
    std::cout << "==============================" << std::endl;
    std::cout << "Skills tests completed." << std::endl;
    
    return 0;
}