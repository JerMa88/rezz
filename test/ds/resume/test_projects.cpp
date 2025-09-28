#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include <algorithm>
#include "../../../src/ds/resume/projects.h"
#include "../../../src/ds/resume/skill.h"

void testProjectDefaultConstructor() {
    Project project;
    
    assert(project.getName() == "");
    assert(project.getDescription() == "");
    assert(project.getUrl() == "");
    assert(project.getSkillsUsed().empty());
    assert(project.getCourse() == "");
    assert(project.getStartDate() == "");
    assert(project.getEndDate() == "");
    
    std::cout << "Project Default Constructor: PASSED" << std::endl;
}

void testProjectParameterizedConstructor() {
    std::vector<Skill> skills;
    skills.push_back(Skill("C++", 5, SkillLevel::ADVANCED));
    skills.push_back(Skill("Python", 4, SkillLevel::INTERMEDIATE));
    
    Project project("Web Application", "A full-stack web application", 
                   "https://github.com/user/webapp", skills, 
                   "CS 101", "2023-01", "2023-05");
    
    assert(project.getName() == "Web Application");
    assert(project.getDescription() == "A full-stack web application");
    assert(project.getUrl() == "https://github.com/user/webapp");
    assert(project.getSkillsUsed().size() == 2);
    assert(project.getCourse() == "CS 101");
    assert(project.getStartDate() == "2023-01");
    assert(project.getEndDate() == "2023-05");
    
    // Verify skills
    std::vector<Skill> retrievedSkills = project.getSkillsUsed();
    assert(retrievedSkills[0].getName() == "C++");
    assert(retrievedSkills[0].getLevel() == SkillLevel::ADVANCED);
    assert(retrievedSkills[1].getName() == "Python");
    assert(retrievedSkills[1].getLevel() == SkillLevel::INTERMEDIATE);
    
    std::cout << "Project Parameterized Constructor: PASSED" << std::endl;
}

void testProjectSetters() {
    Project project;
    
    // Test setters
    project.setName("Mobile App");
    project.setDescription("iOS application for task management");
    project.setUrl("https://apps.apple.com/myapp");
    project.setCourse("Mobile Development");
    project.setStartDate("2024-01");
    project.setEndDate("2024-06");
    
    // Verify setters worked
    assert(project.getName() == "Mobile App");
    assert(project.getDescription() == "iOS application for task management");
    assert(project.getUrl() == "https://apps.apple.com/myapp");
    assert(project.getCourse() == "Mobile Development");
    assert(project.getStartDate() == "2024-01");
    assert(project.getEndDate() == "2024-06");
    
    std::cout << "Project Setters: PASSED" << std::endl;
}

void testProjectSkillsManagement() {
    Project project;
    
    // Create skills vector
    std::vector<Skill> skills;
    skills.push_back(Skill("JavaScript", 4, SkillLevel::INTERMEDIATE));
    skills.push_back(Skill("React", 3, SkillLevel::BEGINNER));
    skills.push_back(Skill("Node.js", 5, SkillLevel::ADVANCED));
    
    // Set skills
    project.setSkillsUsed(skills);
    
    // Verify skills were set
    std::vector<Skill> retrievedSkills = project.getSkillsUsed();
    assert(retrievedSkills.size() == 3);
    assert(retrievedSkills[0].getName() == "JavaScript");
    assert(retrievedSkills[1].getName() == "React");
    assert(retrievedSkills[2].getName() == "Node.js");
    
    // Test empty skills
    std::vector<Skill> emptySkills;
    project.setSkillsUsed(emptySkills);
    assert(project.getSkillsUsed().empty());
    
    std::cout << "Project Skills Management: PASSED" << std::endl;
}

void testProjectEquality() {
    std::vector<Skill> skills1;
    skills1.push_back(Skill("Java", 4, SkillLevel::INTERMEDIATE));
    
    std::vector<Skill> skills2;
    skills2.push_back(Skill("Java", 4, SkillLevel::INTERMEDIATE));
    
    std::vector<Skill> skills3;
    skills3.push_back(Skill("Python", 4, SkillLevel::INTERMEDIATE));
    
    Project project1("Database System", "MySQL database design", "http://db.com", 
                     skills1, "Database Course", "2023-09", "2023-12");
    
    Project project2("Database System", "MySQL database design", "http://db.com", 
                     skills2, "Database Course", "2023-09", "2023-12");
    
    Project project3("Database System", "MySQL database design", "http://db.com", 
                     skills3, "Database Course", "2023-09", "2023-12");
    
    Project project4("Web System", "MySQL database design", "http://db.com", 
                     skills1, "Database Course", "2023-09", "2023-12");
    
    // Test equality
    assert(project1 == project2);  // Identical projects
    
    // Test inequality
    assert(!(project1 == project3));  // Different skills
    assert(!(project1 == project4));  // Different name
    
    std::cout << "Project Equality: PASSED" << std::endl;
}

void testProjectEdgeCases() {
    // Test with empty strings and empty skills
    std::vector<Skill> emptySkills;
    Project emptyProject("", "", "", emptySkills, "", "", "");
    
    assert(emptyProject.getName() == "");
    assert(emptyProject.getDescription() == "");
    assert(emptyProject.getUrl() == "");
    assert(emptyProject.getSkillsUsed().empty());
    assert(emptyProject.getCourse() == "");
    assert(emptyProject.getStartDate() == "");
    assert(emptyProject.getEndDate() == "");
    
    // Test with very long strings
    std::string longName = "Very Long Project Name That Could Be Used In Real World Applications With Many Characters";
    std::string longDescription = "This is a very long description that describes the project in great detail and explains all the features and functionality that was implemented during the development process";
    
    Project longProject(longName, longDescription, "", emptySkills, "", "", "");
    assert(longProject.getName() == longName);
    assert(longProject.getDescription() == longDescription);
    
    std::cout << "Project Edge Cases: PASSED" << std::endl;
}

void testProjectUrlFormats() {
    Project project;
    
    // Test different URL formats
    project.setUrl("https://github.com/user/project");
    assert(project.getUrl() == "https://github.com/user/project");
    
    project.setUrl("http://localhost:3000");
    assert(project.getUrl() == "http://localhost:3000");
    
    project.setUrl("www.example.com");
    assert(project.getUrl() == "www.example.com");
    
    project.setUrl("");  // Empty URL (optional)
    assert(project.getUrl() == "");
    
    std::cout << "Project URL Formats: PASSED" << std::endl;
}

void testProjectDateFormats() {
    Project project;
    
    // Test various date formats
    project.setStartDate("2023-01-15");
    project.setEndDate("2023-06-30");
    assert(project.getStartDate() == "2023-01-15");
    assert(project.getEndDate() == "2023-06-30");
    
    // Test with "Present" for ongoing projects
    project.setEndDate("Present");
    assert(project.getEndDate() == "Present");
    
    // Test with year-month format
    project.setStartDate("2023-09");
    project.setEndDate("2024-05");
    assert(project.getStartDate() == "2023-09");
    assert(project.getEndDate() == "2024-05");
    
    std::cout << "Project Date Formats: PASSED" << std::endl;
}

void testProjectSkillsIntegration() {
    // Create various skill levels
    std::vector<Skill> mixedSkills;
    mixedSkills.push_back(Skill("HTML", 5, SkillLevel::EXPERT));
    mixedSkills.push_back(Skill("CSS", 4, SkillLevel::INTERMEDIATE));
    mixedSkills.push_back(Skill("JavaScript", 3, SkillLevel::BEGINNER));
    mixedSkills.push_back(Skill("TypeScript", 2, SkillLevel::BEGINNER));
    mixedSkills.push_back(Skill("Angular", 1, SkillLevel::BEGINNER));
    
    Project project("Frontend Project", "Modern web frontend", "", mixedSkills, "", "", "");
    
    std::vector<Skill> retrievedSkills = project.getSkillsUsed();
    assert(retrievedSkills.size() == 5);
    
    // Verify each skill
    assert(retrievedSkills[0].getName() == "HTML");
    assert(retrievedSkills[0].getLevel() == SkillLevel::EXPERT);
    assert(retrievedSkills[1].getName() == "CSS");
    assert(retrievedSkills[1].getLevel() == SkillLevel::INTERMEDIATE);
    assert(retrievedSkills[2].getName() == "JavaScript");
    assert(retrievedSkills[2].getLevel() == SkillLevel::BEGINNER);
    
    std::cout << "Project Skills Integration: PASSED" << std::endl;
}

void testProjectCopyBehavior() {
    std::vector<Skill> originalSkills;
    originalSkills.push_back(Skill("Docker", 3, SkillLevel::INTERMEDIATE));
    
    Project project("DevOps Project", "Container deployment", "", originalSkills, "", "", "");
    
    // Get skills (should be a copy)
    std::vector<Skill> skillsCopy = project.getSkillsUsed();
    skillsCopy.push_back(Skill("Kubernetes", 2, SkillLevel::BEGINNER));
    
    // Original project should be unchanged
    assert(project.getSkillsUsed().size() == 1);  // Original unchanged
    assert(skillsCopy.size() == 2);               // Copy was modified
    
    std::cout << "Project Copy Behavior: PASSED" << std::endl;
}

void testProjectComplexScenario() {
    // Test a realistic project scenario
    std::vector<Skill> fullStackSkills;
    fullStackSkills.push_back(Skill("React", 4, SkillLevel::INTERMEDIATE));
    fullStackSkills.push_back(Skill("Node.js", 4, SkillLevel::INTERMEDIATE));
    fullStackSkills.push_back(Skill("PostgreSQL", 3, SkillLevel::BEGINNER));
    fullStackSkills.push_back(Skill("Docker", 2, SkillLevel::BEGINNER));
    fullStackSkills.push_back(Skill("AWS", 3, SkillLevel::BEGINNER));
    
    Project project("E-commerce Platform", 
                   "Full-stack e-commerce web application with user authentication, product catalog, shopping cart, and payment integration",
                   "https://github.com/myuser/ecommerce-platform",
                   fullStackSkills,
                   "Software Engineering Capstone",
                   "2024-01-15",
                   "2024-05-30");
    
    // Verify all components
    assert(project.getName() == "E-commerce Platform");
    assert(project.getDescription().find("Full-stack") != std::string::npos);
    assert(project.getUrl().find("github.com") != std::string::npos);
    assert(project.getSkillsUsed().size() == 5);
    assert(project.getCourse() == "Software Engineering Capstone");
    assert(project.getStartDate() == "2024-01-15");
    assert(project.getEndDate() == "2024-05-30");
    
    std::cout << "Project Complex Scenario: PASSED" << std::endl;
}

void testProjectModification() {
    Project project("Initial Project", "Initial description", "", std::vector<Skill>(), "", "", "");
    
    // Modify all fields
    std::vector<Skill> newSkills;
    newSkills.push_back(Skill("Go", 3, SkillLevel::INTERMEDIATE));
    
    project.setName("Updated Project");
    project.setDescription("Updated description with new features");
    project.setUrl("https://updated-url.com");
    project.setSkillsUsed(newSkills);
    project.setCourse("Advanced Programming");
    project.setStartDate("2024-09");
    project.setEndDate("2024-12");
    
    // Verify all changes
    assert(project.getName() == "Updated Project");
    assert(project.getDescription() == "Updated description with new features");
    assert(project.getUrl() == "https://updated-url.com");
    assert(project.getSkillsUsed().size() == 1);
    assert(project.getSkillsUsed()[0].getName() == "Go");
    assert(project.getCourse() == "Advanced Programming");
    assert(project.getStartDate() == "2024-09");
    assert(project.getEndDate() == "2024-12");
    
    std::cout << "Project Modification: PASSED" << std::endl;
}

// Projects class tests
void testProjectsDefaultConstructor() {
    Projects projects;
    
    assert(projects.getProjects().empty());
    assert(projects.getProjects().size() == 0);
    
    std::cout << "Projects Default Constructor: PASSED" << std::endl;
}

void testProjectsAddSingle() {
    Projects projects;
    std::vector<Skill> skills;
    skills.push_back(Skill("Python", 4, SkillLevel::INTERMEDIATE));
    
    Project project("Data Analysis", "Statistical analysis project", "", skills, "", "", "");
    projects.addProject(project);
    
    std::vector<Project> projectList = projects.getProjects();
    assert(projectList.size() == 1);
    assert(projectList[0] == project);
    assert(projectList[0].getName() == "Data Analysis");
    
    std::cout << "Projects Add Single: PASSED" << std::endl;
}

void testProjectsAddMultiple() {
    Projects projects;
    std::vector<Skill> skills1, skills2, skills3;
    skills1.push_back(Skill("Java", 4, SkillLevel::INTERMEDIATE));
    skills2.push_back(Skill("React", 3, SkillLevel::BEGINNER));
    skills3.push_back(Skill("Docker", 2, SkillLevel::BEGINNER));
    
    Project project1("Backend API", "REST API service", "", skills1, "", "", "");
    Project project2("Frontend App", "React web application", "", skills2, "", "", "");
    Project project3("DevOps Setup", "Container deployment", "", skills3, "", "", "");
    
    projects.addProject(project1);
    projects.addProject(project2);
    projects.addProject(project3);
    
    std::vector<Project> projectList = projects.getProjects();
    assert(projectList.size() == 3);
    assert(projectList[0] == project1);
    assert(projectList[1] == project2);
    assert(projectList[2] == project3);
    
    std::cout << "Projects Add Multiple: PASSED" << std::endl;
}

void testProjectsClearProjects() {
    Projects projects;
    std::vector<Skill> skills;
    skills.push_back(Skill("C++", 5, SkillLevel::ADVANCED));
    
    // Add some projects
    projects.addProject(Project("Project 1", "Description 1", "", skills, "", "", ""));
    projects.addProject(Project("Project 2", "Description 2", "", skills, "", "", ""));
    projects.addProject(Project("Project 3", "Description 3", "", skills, "", "", ""));
    
    assert(projects.getProjects().size() == 3);
    
    // Clear all projects
    projects.clearProjects();
    assert(projects.getProjects().empty());
    assert(projects.getProjects().size() == 0);
    
    std::cout << "Projects Clear Projects: PASSED" << std::endl;
}

void testProjectsRemoveProject() {
    Projects projects;
    std::vector<Skill> skills;
    skills.push_back(Skill("JavaScript", 3, SkillLevel::BEGINNER));
    
    Project project1("Web App", "Frontend application", "", skills, "", "", "");
    Project project2("Mobile App", "iOS application", "", skills, "", "", "");
    Project project3("Desktop App", "Cross-platform app", "", skills, "", "", "");
    
    projects.addProject(project1);
    projects.addProject(project2);
    projects.addProject(project3);
    
    assert(projects.getProjects().size() == 3);
    
    // Remove middle project
    projects.removeProject("Mobile App");
    std::vector<Project> projectList = projects.getProjects();
    assert(projectList.size() == 2);
    assert(projectList[0].getName() == "Web App");
    assert(projectList[1].getName() == "Desktop App");
    
    // Remove first project
    projects.removeProject("Web App");
    projectList = projects.getProjects();
    assert(projectList.size() == 1);
    assert(projectList[0].getName() == "Desktop App");
    
    // Remove last project
    projects.removeProject("Desktop App");
    assert(projects.getProjects().empty());
    
    std::cout << "Projects Remove Project: PASSED" << std::endl;
}

void testProjectsRemoveNonexistent() {
    Projects projects;
    std::vector<Skill> skills;
    
    Project project("Existing Project", "Description", "", skills, "", "", "");
    projects.addProject(project);
    
    assert(projects.getProjects().size() == 1);
    
    // Try to remove non-existent project
    projects.removeProject("Non-existent Project");
    assert(projects.getProjects().size() == 1);  // Should remain unchanged
    assert(projects.getProjects()[0].getName() == "Existing Project");
    
    std::cout << "Projects Remove Nonexistent: PASSED" << std::endl;
}

void testProjectsRemoveDuplicateNames() {
    Projects projects;
    std::vector<Skill> skills1, skills2;
    skills1.push_back(Skill("Java", 4, SkillLevel::INTERMEDIATE));
    skills2.push_back(Skill("Python", 3, SkillLevel::BEGINNER));
    
    // Add projects with same name but different details
    Project project1("Same Name", "Description 1", "url1", skills1, "Course 1", "", "");
    Project project2("Same Name", "Description 2", "url2", skills2, "Course 2", "", "");
    Project project3("Different Name", "Description 3", "", skills1, "", "", "");
    
    projects.addProject(project1);
    projects.addProject(project2);
    projects.addProject(project3);
    
    assert(projects.getProjects().size() == 3);
    
    // Remove projects with "Same Name" - should remove both
    projects.removeProject("Same Name");
    std::vector<Project> projectList = projects.getProjects();
    assert(projectList.size() == 1);
    assert(projectList[0].getName() == "Different Name");
    
    std::cout << "Projects Remove Duplicate Names: PASSED" << std::endl;
}

void testProjectsOrder() {
    Projects projects;
    std::vector<Skill> skills;
    
    Project first("First Project", "First description", "", skills, "", "", "");
    Project second("Second Project", "Second description", "", skills, "", "", "");
    Project third("Third Project", "Third description", "", skills, "", "", "");
    
    projects.addProject(first);
    projects.addProject(second);
    projects.addProject(third);
    
    std::vector<Project> projectList = projects.getProjects();
    
    // Verify order is maintained
    assert(projectList[0].getName() == "First Project");
    assert(projectList[1].getName() == "Second Project");
    assert(projectList[2].getName() == "Third Project");
    
    std::cout << "Projects Order: PASSED" << std::endl;
}

void testProjectsCopyBehavior() {
    Projects projects;
    std::vector<Skill> skills;
    skills.push_back(Skill("Go", 3, SkillLevel::INTERMEDIATE));
    
    Project project1("Original Project", "Original description", "", skills, "", "", "");
    projects.addProject(project1);
    
    // Get copy of projects
    std::vector<Project> projectList = projects.getProjects();
    
    // Modify the copy
    projectList.push_back(Project("Added Project", "Added to copy", "", skills, "", "", ""));
    
    // Original should be unchanged
    std::vector<Project> originalList = projects.getProjects();
    assert(originalList.size() == 1);  // Original unchanged
    assert(projectList.size() == 2);   // Copy was modified
    
    std::cout << "Projects Copy Behavior: PASSED" << std::endl;
}

void testProjectsComplexOperations() {
    Projects projects;
    std::vector<Skill> webSkills, mobileSkills, aiSkills;
    
    webSkills.push_back(Skill("HTML", 5, SkillLevel::EXPERT));
    webSkills.push_back(Skill("CSS", 4, SkillLevel::INTERMEDIATE));
    
    mobileSkills.push_back(Skill("Swift", 3, SkillLevel::BEGINNER));
    mobileSkills.push_back(Skill("Kotlin", 3, SkillLevel::BEGINNER));
    
    aiSkills.push_back(Skill("Python", 5, SkillLevel::EXPERT));
    aiSkills.push_back(Skill("TensorFlow", 4, SkillLevel::INTERMEDIATE));
    
    // Add diverse projects
    projects.addProject(Project("E-commerce Website", "Full-stack web app", "https://github.com/web", webSkills, "Web Dev", "2023-01", "2023-06"));
    projects.addProject(Project("iOS Game", "Mobile puzzle game", "https://apps.apple.com/game", mobileSkills, "Mobile Dev", "2023-07", "2023-12"));
    projects.addProject(Project("ML Classifier", "Image classification model", "https://github.com/ml", aiSkills, "AI Course", "2024-01", "Present"));
    
    assert(projects.getProjects().size() == 3);
    
    // Remove middle project
    projects.removeProject("iOS Game");
    assert(projects.getProjects().size() == 2);
    
    // Verify remaining projects
    std::vector<Project> remaining = projects.getProjects();
    assert(remaining[0].getName() == "E-commerce Website");
    assert(remaining[1].getName() == "ML Classifier");
    
    // Clear all
    projects.clearProjects();
    assert(projects.getProjects().empty());
    
    std::cout << "Projects Complex Operations: PASSED" << std::endl;
}

int main() {
    std::cout << "Running Project and Projects Class Tests..." << std::endl;
    std::cout << "===========================================" << std::endl;
    
    // Project class tests
    testProjectDefaultConstructor();
    testProjectParameterizedConstructor();
    testProjectSetters();
    testProjectSkillsManagement();
    testProjectEquality();
    testProjectEdgeCases();
    testProjectUrlFormats();
    testProjectDateFormats();
    testProjectSkillsIntegration();
    testProjectCopyBehavior();
    testProjectComplexScenario();
    testProjectModification();
    
    // Projects class tests
    testProjectsDefaultConstructor();
    testProjectsAddSingle();
    testProjectsAddMultiple();
    testProjectsClearProjects();
    testProjectsRemoveProject();
    testProjectsRemoveNonexistent();
    testProjectsRemoveDuplicateNames();
    testProjectsOrder();
    testProjectsCopyBehavior();
    testProjectsComplexOperations();
    
    std::cout << "===========================================" << std::endl;
    std::cout << "Project and Projects tests completed." << std::endl;
    
    return 0;
}