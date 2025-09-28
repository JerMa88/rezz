#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include <algorithm>
#include "../../../src/ds/resume/experiences.h"

void testExperienceConstructor() {
    Experience exp("Python Programming", ExperienceLevel::INTERMEDIATE);
    
    assert(exp.getName() == "Python Programming");
    assert(exp.getLevel() == ExperienceLevel::INTERMEDIATE);
    
    std::cout << "Experience Constructor: PASSED" << std::endl;
}

void testExperienceEnumLevels() {
    Experience beginner("HTML", ExperienceLevel::BEGINNER);
    Experience intermediate("CSS", ExperienceLevel::INTERMEDIATE);
    Experience advanced("JavaScript", ExperienceLevel::ADVANCED);
    Experience expert("React", ExperienceLevel::EXPERT);
    Experience master("System Design", ExperienceLevel::MASTER);
    
    assert(beginner.getLevel() == ExperienceLevel::BEGINNER);
    assert(intermediate.getLevel() == ExperienceLevel::INTERMEDIATE);
    assert(advanced.getLevel() == ExperienceLevel::ADVANCED);
    assert(expert.getLevel() == ExperienceLevel::EXPERT);
    assert(master.getLevel() == ExperienceLevel::MASTER);
    
    // Test enum values
    assert(static_cast<int>(ExperienceLevel::BEGINNER) == 1);
    assert(static_cast<int>(ExperienceLevel::INTERMEDIATE) == 2);
    assert(static_cast<int>(ExperienceLevel::ADVANCED) == 3);
    assert(static_cast<int>(ExperienceLevel::EXPERT) == 4);
    assert(static_cast<int>(ExperienceLevel::MASTER) == 5);
    
    std::cout << "Experience Enum Levels: PASSED" << std::endl;
}

void testExperienceSetLevel() {
    Experience exp("Java", ExperienceLevel::BEGINNER);
    
    assert(exp.getLevel() == ExperienceLevel::BEGINNER);
    
    exp.setLevel(ExperienceLevel::ADVANCED);
    assert(exp.getLevel() == ExperienceLevel::ADVANCED);
    
    exp.setLevel(ExperienceLevel::MASTER);
    assert(exp.getLevel() == ExperienceLevel::MASTER);
    
    std::cout << "Experience Set Level: PASSED" << std::endl;
}

void testExperienceEquality() {
    Experience exp1("C++", ExperienceLevel::EXPERT);
    Experience exp2("C++", ExperienceLevel::EXPERT);
    Experience exp3("C++", ExperienceLevel::INTERMEDIATE);
    Experience exp4("Python", ExperienceLevel::EXPERT);
    
    // Test equality
    assert(exp1 == exp2);  // Same name and level
    
    // Test inequality
    assert(!(exp1 == exp3));  // Same name, different level
    assert(!(exp1 == exp4));  // Different name, same level
    
    std::cout << "Experience Equality: PASSED" << std::endl;
}

void testExperienceEdgeCases() {
    // Test with empty string
    Experience empty("", ExperienceLevel::BEGINNER);
    assert(empty.getName() == "");
    assert(empty.getLevel() == ExperienceLevel::BEGINNER);
    
    // Test with long string
    std::string longName = "Very Long Experience Name That Could Be Used In Real World Applications";
    Experience longExp(longName, ExperienceLevel::MASTER);
    assert(longExp.getName() == longName);
    
    // Test special characters
    Experience special("C# .NET Framework", ExperienceLevel::ADVANCED);
    assert(special.getName() == "C# .NET Framework");
    
    std::cout << "Experience Edge Cases: PASSED" << std::endl;
}

void testExperiencesDefaultConstructor() {
    Experiences experiences;
    
    assert(experiences.getExperiences().empty());
    assert(experiences.getExperiences().size() == 0);
    
    std::cout << "Experiences Default Constructor: PASSED" << std::endl;
}

void testExperiencesAddSingle() {
    Experiences experiences;
    Experience exp("Docker", ExperienceLevel::INTERMEDIATE);
    
    experiences.addExperience(exp);
    
    std::vector<Experience> expList = experiences.getExperiences();
    assert(expList.size() == 1);
    assert(expList[0] == exp);
    assert(expList[0].getName() == "Docker");
    assert(expList[0].getLevel() == ExperienceLevel::INTERMEDIATE);
    
    std::cout << "Experiences Add Single: PASSED" << std::endl;
}

void testExperiencesAddMultiple() {
    Experiences experiences;
    
    Experience exp1("Kubernetes", ExperienceLevel::BEGINNER);
    Experience exp2("AWS", ExperienceLevel::INTERMEDIATE);
    Experience exp3("Linux", ExperienceLevel::EXPERT);
    
    experiences.addExperience(exp1);
    experiences.addExperience(exp2);
    experiences.addExperience(exp3);
    
    std::vector<Experience> expList = experiences.getExperiences();
    assert(expList.size() == 3);
    assert(expList[0] == exp1);
    assert(expList[1] == exp2);
    assert(expList[2] == exp3);
    
    std::cout << "Experiences Add Multiple: PASSED" << std::endl;
}

void testExperiencesOrder() {
    Experiences experiences;
    
    // Add experiences in specific order
    Experience first("First", ExperienceLevel::BEGINNER);
    Experience second("Second", ExperienceLevel::INTERMEDIATE);
    Experience third("Third", ExperienceLevel::ADVANCED);
    
    experiences.addExperience(first);
    experiences.addExperience(second);
    experiences.addExperience(third);
    
    std::vector<Experience> expList = experiences.getExperiences();
    
    // Verify order is maintained
    assert(expList[0].getName() == "First");
    assert(expList[1].getName() == "Second");
    assert(expList[2].getName() == "Third");
    
    std::cout << "Experiences Order: PASSED" << std::endl;
}

void testExperiencesDuplicates() {
    Experiences experiences;
    
    Experience exp1("Git", ExperienceLevel::ADVANCED);
    Experience exp2("Git", ExperienceLevel::ADVANCED);  // Exact duplicate
    Experience exp3("Git", ExperienceLevel::EXPERT);    // Same name, different level
    
    experiences.addExperience(exp1);
    experiences.addExperience(exp2);
    experiences.addExperience(exp3);
    
    std::vector<Experience> expList = experiences.getExperiences();
    assert(expList.size() == 3);  // All are added (no duplicate prevention)
    
    // Verify all are present
    assert(expList[0] == exp1);
    assert(expList[1] == exp2);
    assert(expList[2] == exp3);
    
    std::cout << "Experiences Duplicates: PASSED" << std::endl;
}

void testExperiencesCopyBehavior() {
    Experiences experiences;
    
    Experience exp1("MongoDB", ExperienceLevel::INTERMEDIATE);
    Experience exp2("Redis", ExperienceLevel::BEGINNER);
    
    experiences.addExperience(exp1);
    experiences.addExperience(exp2);
    
    // Get copy of experiences
    std::vector<Experience> expList = experiences.getExperiences();
    
    // Modify the copy
    expList.push_back(Experience("PostgreSQL", ExperienceLevel::ADVANCED));
    
    // Original should be unchanged
    std::vector<Experience> originalList = experiences.getExperiences();
    assert(originalList.size() == 2);  // Original unchanged
    assert(expList.size() == 3);       // Copy was modified
    
    std::cout << "Experiences Copy Behavior: PASSED" << std::endl;
}

void testExperiencesWithAllLevels() {
    Experiences experiences;
    
    experiences.addExperience(Experience("Beginner Skill", ExperienceLevel::BEGINNER));
    experiences.addExperience(Experience("Intermediate Skill", ExperienceLevel::INTERMEDIATE));
    experiences.addExperience(Experience("Advanced Skill", ExperienceLevel::ADVANCED));
    experiences.addExperience(Experience("Expert Skill", ExperienceLevel::EXPERT));
    experiences.addExperience(Experience("Master Skill", ExperienceLevel::MASTER));
    
    std::vector<Experience> expList = experiences.getExperiences();
    assert(expList.size() == 5);
    
    assert(expList[0].getLevel() == ExperienceLevel::BEGINNER);
    assert(expList[1].getLevel() == ExperienceLevel::INTERMEDIATE);
    assert(expList[2].getLevel() == ExperienceLevel::ADVANCED);
    assert(expList[3].getLevel() == ExperienceLevel::EXPERT);
    assert(expList[4].getLevel() == ExperienceLevel::MASTER);
    
    std::cout << "Experiences With All Levels: PASSED" << std::endl;
}

void testLargeNumberOfExperiences() {
    Experiences experiences;
    
    // Add many experiences
    for (int i = 0; i < 100; ++i) {
        std::string name = "Skill " + std::to_string(i);
        ExperienceLevel level = static_cast<ExperienceLevel>((i % 5) + 1);
        experiences.addExperience(Experience(name, level));
    }
    
    std::vector<Experience> expList = experiences.getExperiences();
    assert(expList.size() == 100);
    
    // Verify some random entries
    assert(expList[0].getName() == "Skill 0");
    assert(expList[50].getName() == "Skill 50");
    assert(expList[99].getName() == "Skill 99");
    
    std::cout << "Large Number of Experiences: PASSED" << std::endl;
}

// Additional Experiences class tests for new methods
void testExperiencesClearExperiences() {
    Experiences experiences;
    
    // Add several experiences
    experiences.addExperience(Experience("Java", ExperienceLevel::INTERMEDIATE));
    experiences.addExperience(Experience("Python", ExperienceLevel::ADVANCED));
    experiences.addExperience(Experience("JavaScript", ExperienceLevel::BEGINNER));
    
    assert(experiences.getExperiences().size() == 3);
    
    // Clear all experiences
    experiences.clearExperiences();
    assert(experiences.getExperiences().empty());
    assert(experiences.getExperiences().size() == 0);
    
    // Test clearing empty collection (should not crash)
    experiences.clearExperiences();
    assert(experiences.getExperiences().empty());
    
    std::cout << "Experiences Clear Experiences: PASSED" << std::endl;
}

void testExperiencesRemoveExperience() {
    Experiences experiences;
    
    Experience exp1("C++", ExperienceLevel::EXPERT);
    Experience exp2("Rust", ExperienceLevel::BEGINNER);
    Experience exp3("Go", ExperienceLevel::INTERMEDIATE);
    
    experiences.addExperience(exp1);
    experiences.addExperience(exp2);
    experiences.addExperience(exp3);
    
    assert(experiences.getExperiences().size() == 3);
    
    // Remove middle experience
    experiences.removeExperience("Rust");
    std::vector<Experience> expList = experiences.getExperiences();
    assert(expList.size() == 2);
    assert(expList[0].getName() == "C++");
    assert(expList[1].getName() == "Go");
    
    // Remove first experience
    experiences.removeExperience("C++");
    expList = experiences.getExperiences();
    assert(expList.size() == 1);
    assert(expList[0].getName() == "Go");
    
    // Remove last experience
    experiences.removeExperience("Go");
    assert(experiences.getExperiences().empty());
    
    std::cout << "Experiences Remove Experience: PASSED" << std::endl;
}

void testExperiencesRemoveNonexistent() {
    Experiences experiences;
    
    Experience exp("PHP", ExperienceLevel::INTERMEDIATE);
    experiences.addExperience(exp);
    
    assert(experiences.getExperiences().size() == 1);
    
    // Try to remove non-existent experience
    experiences.removeExperience("Ruby");
    assert(experiences.getExperiences().size() == 1);  // Should remain unchanged
    assert(experiences.getExperiences()[0].getName() == "PHP");
    
    // Try to remove from empty collection
    experiences.clearExperiences();
    experiences.removeExperience("Anything");
    assert(experiences.getExperiences().empty());
    
    std::cout << "Experiences Remove Nonexistent: PASSED" << std::endl;
}

void testExperiencesRemoveDuplicateNames() {
    Experiences experiences;
    
    // Add experiences with same name but different levels
    Experience exp1("Docker", ExperienceLevel::BEGINNER);
    Experience exp2("Docker", ExperienceLevel::INTERMEDIATE);  // Same name, different level
    Experience exp3("Kubernetes", ExperienceLevel::ADVANCED);
    
    experiences.addExperience(exp1);
    experiences.addExperience(exp2);
    experiences.addExperience(exp3);
    
    assert(experiences.getExperiences().size() == 3);
    
    // Remove experiences with "Docker" name - should remove both
    experiences.removeExperience("Docker");
    std::vector<Experience> expList = experiences.getExperiences();
    assert(expList.size() == 1);
    assert(expList[0].getName() == "Kubernetes");
    
    std::cout << "Experiences Remove Duplicate Names: PASSED" << std::endl;
}

void testExperiencesModifyAfterOperations() {
    Experiences experiences;
    
    // Add initial experiences
    experiences.addExperience(Experience("React", ExperienceLevel::INTERMEDIATE));
    experiences.addExperience(Experience("Vue", ExperienceLevel::BEGINNER));
    experiences.addExperience(Experience("Angular", ExperienceLevel::ADVANCED));
    
    // Remove one
    experiences.removeExperience("Vue");
    assert(experiences.getExperiences().size() == 2);
    
    // Add more after removal
    experiences.addExperience(Experience("Svelte", ExperienceLevel::BEGINNER));
    experiences.addExperience(Experience("Next.js", ExperienceLevel::INTERMEDIATE));
    
    std::vector<Experience> expList = experiences.getExperiences();
    assert(expList.size() == 4);
    assert(expList[0].getName() == "React");
    assert(expList[1].getName() == "Angular");
    assert(expList[2].getName() == "Svelte");
    assert(expList[3].getName() == "Next.js");
    
    std::cout << "Experiences Modify After Operations: PASSED" << std::endl;
}

void testExperiencesComplexOperations() {
    Experiences experiences;
    
    // Add experiences with all levels
    experiences.addExperience(Experience("HTML", ExperienceLevel::MASTER));
    experiences.addExperience(Experience("CSS", ExperienceLevel::EXPERT));
    experiences.addExperience(Experience("JavaScript", ExperienceLevel::ADVANCED));
    experiences.addExperience(Experience("TypeScript", ExperienceLevel::INTERMEDIATE));
    experiences.addExperience(Experience("WebAssembly", ExperienceLevel::BEGINNER));
    
    assert(experiences.getExperiences().size() == 5);
    
    // Remove some experiences
    experiences.removeExperience("CSS");
    experiences.removeExperience("WebAssembly");
    
    std::vector<Experience> remaining = experiences.getExperiences();
    assert(remaining.size() == 3);
    assert(remaining[0].getName() == "HTML");
    assert(remaining[1].getName() == "JavaScript");
    assert(remaining[2].getName() == "TypeScript");
    
    // Add more
    experiences.addExperience(Experience("Node.js", ExperienceLevel::EXPERT));
    experiences.addExperience(Experience("Express", ExperienceLevel::INTERMEDIATE));
    
    assert(experiences.getExperiences().size() == 5);
    
    // Clear all
    experiences.clearExperiences();
    assert(experiences.getExperiences().empty());
    
    std::cout << "Experiences Complex Operations: PASSED" << std::endl;
}

void testExperiencesRemoveWithSpecialCharacters() {
    Experiences experiences;
    
    // Add experiences with special characters in names
    experiences.addExperience(Experience("C#", ExperienceLevel::INTERMEDIATE));
    experiences.addExperience(Experience("C++", ExperienceLevel::ADVANCED));
    experiences.addExperience(Experience(".NET Core", ExperienceLevel::EXPERT));
    experiences.addExperience(Experience("ASP.NET", ExperienceLevel::INTERMEDIATE));
    
    assert(experiences.getExperiences().size() == 4);
    
    // Remove experience with special characters
    experiences.removeExperience("C#");
    std::vector<Experience> expList = experiences.getExperiences();
    assert(expList.size() == 3);
    
    // Verify C# is removed but C++ remains
    bool foundCPlusPlus = false;
    bool foundCSharp = false;
    for (const auto& exp : expList) {
        if (exp.getName() == "C++") foundCPlusPlus = true;
        if (exp.getName() == "C#") foundCSharp = true;
    }
    assert(foundCPlusPlus);
    assert(!foundCSharp);
    
    std::cout << "Experiences Remove Special Characters: PASSED" << std::endl;
}

void testExperiencesLargeScaleOperations() {
    Experiences experiences;
    
    // Add many experiences
    for (int i = 0; i < 50; ++i) {
        std::string name = "Technology " + std::to_string(i);
        ExperienceLevel level = static_cast<ExperienceLevel>((i % 5) + 1);
        experiences.addExperience(Experience(name, level));
    }
    
    assert(experiences.getExperiences().size() == 50);
    
    // Remove every other experience
    for (int i = 0; i < 50; i += 2) {
        std::string name = "Technology " + std::to_string(i);
        experiences.removeExperience(name);
    }
    
    assert(experiences.getExperiences().size() == 25);
    
    // Verify remaining experiences are odd-numbered
    std::vector<Experience> remaining = experiences.getExperiences();
    for (size_t i = 0; i < remaining.size(); ++i) {
        std::string expectedName = "Technology " + std::to_string((i * 2) + 1);
        assert(remaining[i].getName() == expectedName);
    }
    
    std::cout << "Experiences Large Scale Operations: PASSED" << std::endl;
}

void testExperiencesEmptyStringHandling() {
    Experiences experiences;
    
    // Add experience with empty name
    experiences.addExperience(Experience("", ExperienceLevel::BEGINNER));
    experiences.addExperience(Experience("Valid Name", ExperienceLevel::INTERMEDIATE));
    
    assert(experiences.getExperiences().size() == 2);
    
    // Remove experience with empty name
    experiences.removeExperience("");
    std::vector<Experience> expList = experiences.getExperiences();
    assert(expList.size() == 1);
    assert(expList[0].getName() == "Valid Name");
    
    std::cout << "Experiences Empty String Handling: PASSED" << std::endl;
}

void testExperiencesAllLevelsOperations() {
    Experiences experiences;
    
    // Add one experience for each level
    experiences.addExperience(Experience("Beginner Tech", ExperienceLevel::BEGINNER));
    experiences.addExperience(Experience("Intermediate Tech", ExperienceLevel::INTERMEDIATE));
    experiences.addExperience(Experience("Advanced Tech", ExperienceLevel::ADVANCED));
    experiences.addExperience(Experience("Expert Tech", ExperienceLevel::EXPERT));
    experiences.addExperience(Experience("Master Tech", ExperienceLevel::MASTER));
    
    assert(experiences.getExperiences().size() == 5);
    
    // Remove intermediate level experiences
    experiences.removeExperience("Intermediate Tech");
    experiences.removeExperience("Advanced Tech");
    
    std::vector<Experience> remaining = experiences.getExperiences();
    assert(remaining.size() == 3);
    
    // Verify correct experiences remain
    assert(remaining[0].getName() == "Beginner Tech");
    assert(remaining[0].getLevel() == ExperienceLevel::BEGINNER);
    assert(remaining[1].getName() == "Expert Tech");
    assert(remaining[1].getLevel() == ExperienceLevel::EXPERT);
    assert(remaining[2].getName() == "Master Tech");
    assert(remaining[2].getLevel() == ExperienceLevel::MASTER);
    
    std::cout << "Experiences All Levels Operations: PASSED" << std::endl;
}

int main() {
    std::cout << "Running Experience and Experiences Class Tests..." << std::endl;
    std::cout << "=================================================" << std::endl;
    
    // Experience class tests
    testExperienceConstructor();
    testExperienceEnumLevels();
    testExperienceSetLevel();
    testExperienceEquality();
    testExperienceEdgeCases();
    
    // Original Experiences class tests
    testExperiencesDefaultConstructor();
    testExperiencesAddSingle();
    testExperiencesAddMultiple();
    testExperiencesOrder();
    testExperiencesDuplicates();
    testExperiencesCopyBehavior();
    testExperiencesWithAllLevels();
    testLargeNumberOfExperiences();
    
    // Additional Experiences class tests for new methods
    testExperiencesClearExperiences();
    testExperiencesRemoveExperience();
    testExperiencesRemoveNonexistent();
    testExperiencesRemoveDuplicateNames();
    testExperiencesModifyAfterOperations();
    testExperiencesComplexOperations();
    testExperiencesRemoveWithSpecialCharacters();
    testExperiencesLargeScaleOperations();
    testExperiencesEmptyStringHandling();
    testExperiencesAllLevelsOperations();
    
    std::cout << "=================================================" << std::endl;
    std::cout << "Experience and Experiences tests completed." << std::endl;
    
    return 0;
}