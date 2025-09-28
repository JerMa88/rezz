#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include <algorithm>
#include "../../../src/ds/resume/awards.h"

// Award class tests
void testAwardConstructorWithDescription() {
    Award award("Employee of the Month", "2024-03", "ABC Corporation", 
                "Outstanding performance in software development");
    
    assert(award.getName() == "Employee of the Month");
    assert(award.getDate() == "2024-03");
    assert(award.getIssuer() == "ABC Corporation");
    assert(award.getDescription() == "Outstanding performance in software development");
    
    std::cout << "Award Constructor With Description: PASSED" << std::endl;
}

void testAwardConstructorWithoutDescription() {
    Award award("Dean's List", "2023-12", "MIT");  // Using default empty description
    
    assert(award.getName() == "Dean's List");
    assert(award.getDate() == "2023-12");
    assert(award.getIssuer() == "MIT");
    assert(award.getDescription() == "");  // Should be empty string
    
    std::cout << "Award Constructor Without Description: PASSED" << std::endl;
}

void testAwardConstructorExplicitEmptyDescription() {
    Award award("Best Student Award", "2024-05", "Stanford University", "");  // Explicit empty description
    
    assert(award.getName() == "Best Student Award");
    assert(award.getDate() == "2024-05");
    assert(award.getIssuer() == "Stanford University");
    assert(award.getDescription() == "");
    
    std::cout << "Award Constructor Explicit Empty Description: PASSED" << std::endl;
}

void testAwardEquality() {
    Award award1("Excellence Award", "2024-01", "Tech Company", "For innovative solutions");
    Award award2("Excellence Award", "2024-01", "Tech Company", "For innovative solutions");
    Award award3("Excellence Award", "2024-01", "Tech Company", "For creative thinking");  // Different description
    Award award4("Excellence Award", "2024-02", "Tech Company", "For innovative solutions");  // Different date
    Award award5("Leadership Award", "2024-01", "Tech Company", "For innovative solutions");  // Different name
    Award award6("Excellence Award", "2024-01", "Other Company", "For innovative solutions");  // Different issuer
    
    // Test equality
    assert(award1 == award2);  // Identical awards
    
    // Test inequality
    assert(!(award1 == award3));  // Different description
    assert(!(award1 == award4));  // Different date
    assert(!(award1 == award5));  // Different name
    assert(!(award1 == award6));  // Different issuer
    
    std::cout << "Award Equality: PASSED" << std::endl;
}

void testAwardDateFormats() {
    // Test various date formats
    Award award1("Monthly Award", "2024-01", "Company A");
    Award award2("Quarterly Award", "2024-12", "Company B");
    Award award3("Annual Award", "2023-06", "Company C");
    
    assert(award1.getDate() == "2024-01");
    assert(award2.getDate() == "2024-12");
    assert(award3.getDate() == "2023-06");
    
    std::cout << "Award Date Formats: PASSED" << std::endl;
}

void testAwardEdgeCases() {
    // Test with empty strings
    Award emptyAward("", "", "", "");
    assert(emptyAward.getName() == "");
    assert(emptyAward.getDate() == "");
    assert(emptyAward.getIssuer() == "");
    assert(emptyAward.getDescription() == "");
    
    // Test with very long strings
    std::string longName = "Very Long Award Name That Could Be Used In Real World Professional Recognition Programs";
    std::string longDescription = "This is a very long description that explains in great detail the reasons for receiving this award and the criteria that were met to qualify for this recognition";
    
    Award longAward(longName, "2024-08", "International Organization", longDescription);
    assert(longAward.getName() == longName);
    assert(longAward.getDescription() == longDescription);
    
    // Test with special characters
    Award specialAward("C++ Programming Excellence", "2024-09", "IEEE Computer Society", 
                      "Awarded for outstanding contributions to C++ development");
    assert(specialAward.getName() == "C++ Programming Excellence");
    
    std::cout << "Award Edge Cases: PASSED" << std::endl;
}

// Awards class tests
void testAwardsDefaultConstructor() {
    Awards awards;
    
    assert(awards.getAwards().empty());
    assert(awards.getAwards().size() == 0);
    
    std::cout << "Awards Default Constructor: PASSED" << std::endl;
}

void testAwardsAddSingle() {
    Awards awards;
    
    Award award("Hackathon Winner", "2024-04", "Tech Conference", "First place in AI category");
    awards.addAward(award);
    
    std::vector<Award> awardList = awards.getAwards();
    assert(awardList.size() == 1);
    assert(awardList[0] == award);
    assert(awardList[0].getName() == "Hackathon Winner");
    assert(awardList[0].getIssuer() == "Tech Conference");
    
    std::cout << "Awards Add Single: PASSED" << std::endl;
}

void testAwardsAddMultiple() {
    Awards awards;
    
    Award academic("Summa Cum Laude", "2023-05", "University", "Graduated with highest honors");
    Award professional("Top Performer", "2024-02", "Current Employer", "Exceeded all performance metrics");
    Award community("Volunteer of the Year", "2024-06", "Local Charity", "Outstanding community service");
    
    awards.addAward(academic);
    awards.addAward(professional);
    awards.addAward(community);
    
    std::vector<Award> awardList = awards.getAwards();
    assert(awardList.size() == 3);
    assert(awardList[0] == academic);
    assert(awardList[1] == professional);
    assert(awardList[2] == community);
    
    std::cout << "Awards Add Multiple: PASSED" << std::endl;
}

void testAwardsClearAwards() {
    Awards awards;
    
    // Add several awards
    awards.addAward(Award("Award 1", "2024-01", "Issuer 1", "Description 1"));
    awards.addAward(Award("Award 2", "2024-02", "Issuer 2"));  // No description
    awards.addAward(Award("Award 3", "2024-03", "Issuer 3", "Description 3"));
    
    assert(awards.getAwards().size() == 3);
    
    // Clear all awards
    awards.clearAwards();
    assert(awards.getAwards().empty());
    assert(awards.getAwards().size() == 0);
    
    // Test clearing empty collection (should not crash)
    awards.clearAwards();
    assert(awards.getAwards().empty());
    
    std::cout << "Awards Clear Awards: PASSED" << std::endl;
}

void testAwardsRemoveAward() {
    Awards awards;
    
    Award award1("Innovation Award", "2024-01", "Company A", "For creative problem solving");
    Award award2("Team Player Award", "2024-03", "Company A", "For excellent collaboration");
    Award award3("Customer Service Award", "2024-05", "Company A", "For outstanding client support");
    
    awards.addAward(award1);
    awards.addAward(award2);
    awards.addAward(award3);
    
    assert(awards.getAwards().size() == 3);
    
    // Remove middle award
    awards.removeAward("Team Player Award");
    std::vector<Award> awardList = awards.getAwards();
    assert(awardList.size() == 2);
    assert(awardList[0].getName() == "Innovation Award");
    assert(awardList[1].getName() == "Customer Service Award");
    
    // Remove first award
    awards.removeAward("Innovation Award");
    awardList = awards.getAwards();
    assert(awardList.size() == 1);
    assert(awardList[0].getName() == "Customer Service Award");
    
    // Remove last award
    awards.removeAward("Customer Service Award");
    assert(awards.getAwards().empty());
    
    std::cout << "Awards Remove Award: PASSED" << std::endl;
}

void testAwardsRemoveNonexistent() {
    Awards awards;
    
    Award award("Scholarship Award", "2023-08", "Educational Foundation", "Merit-based scholarship");
    awards.addAward(award);
    
    assert(awards.getAwards().size() == 1);
    
    // Try to remove non-existent award
    awards.removeAward("Non-existent Award");
    assert(awards.getAwards().size() == 1);  // Should remain unchanged
    assert(awards.getAwards()[0].getName() == "Scholarship Award");
    
    // Try to remove from empty collection
    awards.clearAwards();
    awards.removeAward("Any Award");
    assert(awards.getAwards().empty());
    
    std::cout << "Awards Remove Nonexistent: PASSED" << std::endl;
}

void testAwardsRemoveDuplicateNames() {
    Awards awards;
    
    // Add awards with same name but different details
    Award award1("Best Employee", "2023-12", "Department A", "Outstanding annual performance");
    Award award2("Best Employee", "2024-06", "Department B", "Exceptional project leadership");  // Same name, different details
    Award award3("Safety Award", "2024-03", "HR Department", "Perfect safety record");
    
    awards.addAward(award1);
    awards.addAward(award2);
    awards.addAward(award3);
    
    assert(awards.getAwards().size() == 3);
    
    // Remove awards with "Best Employee" name - should remove both
    awards.removeAward("Best Employee");
    std::vector<Award> awardList = awards.getAwards();
    assert(awardList.size() == 1);
    assert(awardList[0].getName() == "Safety Award");
    
    std::cout << "Awards Remove Duplicate Names: PASSED" << std::endl;
}

void testAwardsOrder() {
    Awards awards;
    
    Award first("First Award", "2024-01", "First Issuer", "First description");
    Award second("Second Award", "2024-02", "Second Issuer");  // No description
    Award third("Third Award", "2024-03", "Third Issuer", "Third description");
    
    awards.addAward(first);
    awards.addAward(second);
    awards.addAward(third);
    
    std::vector<Award> awardList = awards.getAwards();
    
    // Verify order is maintained
    assert(awardList[0].getName() == "First Award");
    assert(awardList[1].getName() == "Second Award");
    assert(awardList[2].getName() == "Third Award");
    
    std::cout << "Awards Order: PASSED" << std::endl;
}

void testAwardsCopyBehavior() {
    Awards awards;
    
    Award original("Original Award", "2024-07", "Original Issuer", "Original description");
    awards.addAward(original);
    
    // Get copy of awards
    std::vector<Award> awardList = awards.getAwards();
    
    // Modify the copy
    awardList.push_back(Award("Added Award", "2024-08", "Added Issuer", "Added description"));
    
    // Original should be unchanged
    std::vector<Award> originalList = awards.getAwards();
    assert(originalList.size() == 1);  // Original unchanged
    assert(awardList.size() == 2);     // Copy was modified
    
    std::cout << "Awards Copy Behavior: PASSED" << std::endl;
}

void testAwardsModifyAfterOperations() {
    Awards awards;
    
    // Add initial awards
    awards.addAward(Award("Research Award", "2023-11", "University", "Outstanding research contribution"));
    awards.addAward(Award("Teaching Award", "2024-01", "University", "Excellence in education"));
    awards.addAward(Award("Service Award", "2024-04", "Community", "Dedicated volunteer work"));
    
    // Remove one
    awards.removeAward("Teaching Award");
    assert(awards.getAwards().size() == 2);
    
    // Add more after removal
    awards.addAward(Award("Publication Award", "2024-07", "Academic Journal", "Best paper of the year"));
    awards.addAward(Award("Conference Award", "2024-08", "International Conference", "Best presentation"));
    
    std::vector<Award> awardList = awards.getAwards();
    assert(awardList.size() == 4);
    assert(awardList[0].getName() == "Research Award");
    assert(awardList[1].getName() == "Service Award");
    assert(awardList[2].getName() == "Publication Award");
    assert(awardList[3].getName() == "Conference Award");
    
    std::cout << "Awards Modify After Operations: PASSED" << std::endl;
}

void testAwardsComplexOperations() {
    Awards awards;
    
    // Add diverse awards
    awards.addAward(Award("Academic Excellence", "2023-05", "Harvard University", "Graduated magna cum laude"));
    awards.addAward(Award("Innovation Prize", "2023-10", "Tech Startup", "Revolutionary app development"));
    awards.addAward(Award("Leadership Recognition", "2024-02", "Management Institute", "Outstanding team leadership"));
    awards.addAward(Award("Community Impact", "2024-04", "City Council", "Significant community contribution"));
    awards.addAward(Award("Professional Achievement", "2024-06", "Industry Association", "Career milestone recognition"));
    
    assert(awards.getAwards().size() == 5);
    
    // Remove some awards
    awards.removeAward("Innovation Prize");
    awards.removeAward("Community Impact");
    
    std::vector<Award> remaining = awards.getAwards();
    assert(remaining.size() == 3);
    assert(remaining[0].getName() == "Academic Excellence");
    assert(remaining[1].getName() == "Leadership Recognition");
    assert(remaining[2].getName() == "Professional Achievement");
    
    // Add more
    awards.addAward(Award("Mentorship Award", "2024-09", "Professional Society", "Exceptional mentoring of junior members"));
    assert(awards.getAwards().size() == 4);
    
    // Clear all
    awards.clearAwards();
    assert(awards.getAwards().empty());
    
    std::cout << "Awards Complex Operations: PASSED" << std::endl;
}

void testAwardsWithAndWithoutDescriptions() {
    Awards awards;
    
    // Mix of awards with and without descriptions
    awards.addAward(Award("Award With Description", "2024-01", "Issuer A", "This award has a description"));
    awards.addAward(Award("Award Without Description", "2024-02", "Issuer B"));  // Default empty description
    awards.addAward(Award("Award With Empty Description", "2024-03", "Issuer C", ""));  // Explicit empty description
    awards.addAward(Award("Another Award With Description", "2024-04", "Issuer D", "Another detailed description"));
    
    std::vector<Award> awardList = awards.getAwards();
    assert(awardList.size() == 4);
    
    // Verify descriptions
    assert(awardList[0].getDescription() == "This award has a description");
    assert(awardList[1].getDescription() == "");  // Should be empty
    assert(awardList[2].getDescription() == "");  // Should be empty
    assert(awardList[3].getDescription() == "Another detailed description");
    
    std::cout << "Awards With And Without Descriptions: PASSED" << std::endl;
}

void testAwardsLargeScaleOperations() {
    Awards awards;
    
    // Add many awards
    for (int i = 0; i < 30; ++i) {
        std::string name = "Award " + std::to_string(i);
        std::string date = "2024-" + std::to_string((i % 12) + 1);  // Months 1-12
        std::string issuer = "Issuer " + std::to_string(i % 5);     // 5 different issuers
        std::string description = (i % 2 == 0) ? ("Description for award " + std::to_string(i)) : "";  // Every other has description
        
        awards.addAward(Award(name, date, issuer, description));
    }
    
    assert(awards.getAwards().size() == 30);
    
    // Remove every third award
    for (int i = 0; i < 30; i += 3) {
        std::string name = "Award " + std::to_string(i);
        awards.removeAward(name);
    }
    
    assert(awards.getAwards().size() == 20);  // 30 - 10 removed = 20
    
    // Verify remaining awards are not multiples of 3
    std::vector<Award> remaining = awards.getAwards();
    for (size_t i = 0; i < remaining.size(); ++i) {
        std::string name = remaining[i].getName();
        // Extract number from name "Award X"
        int awardNum = std::stoi(name.substr(6));  // Skip "Award "
        assert(awardNum % 3 != 0);  // Should not be multiple of 3
    }
    
    std::cout << "Awards Large Scale Operations: PASSED" << std::endl;
}

void testAwardsEmptyStringHandling() {
    Awards awards;
    
    // Add award with empty name
    awards.addAward(Award("", "2024-01", "Valid Issuer", "Valid description"));
    awards.addAward(Award("Valid Award", "2024-02", "Valid Issuer", "Valid description"));
    
    assert(awards.getAwards().size() == 2);
    
    // Remove award with empty name
    awards.removeAward("");
    std::vector<Award> awardList = awards.getAwards();
    assert(awardList.size() == 1);
    assert(awardList[0].getName() == "Valid Award");
    
    std::cout << "Awards Empty String Handling: PASSED" << std::endl;
}

void testAwardsChronologicalData() {
    Awards awards;
    
    // Add awards from different time periods
    awards.addAward(Award("Student Award", "2020-05", "High School", "Valedictorian"));
    awards.addAward(Award("Undergraduate Award", "2024-05", "University", "Dean's List"));
    awards.addAward(Award("Industry Award", "2024-09", "Tech Company", "Employee of the Month"));
    awards.addAward(Award("Future Award", "2025-01", "Professional Organization", "Expected recognition"));
    
    std::vector<Award> awardList = awards.getAwards();
    assert(awardList.size() == 4);
    
    // Verify dates are stored correctly
    assert(awardList[0].getDate() == "2020-05");
    assert(awardList[1].getDate() == "2024-05");
    assert(awardList[2].getDate() == "2024-09");
    assert(awardList[3].getDate() == "2025-01");
    
    std::cout << "Awards Chronological Data: PASSED" << std::endl;
}

int main() {
    std::cout << "Running Award and Awards Class Tests..." << std::endl;
    std::cout << "=======================================" << std::endl;
    
    // Award class tests
    testAwardConstructorWithDescription();
    testAwardConstructorWithoutDescription();
    testAwardConstructorExplicitEmptyDescription();
    testAwardEquality();
    testAwardDateFormats();
    testAwardEdgeCases();
    
    // Awards class tests
    testAwardsDefaultConstructor();
    testAwardsAddSingle();
    testAwardsAddMultiple();
    testAwardsClearAwards();
    testAwardsRemoveAward();
    testAwardsRemoveNonexistent();
    testAwardsRemoveDuplicateNames();
    testAwardsOrder();
    testAwardsCopyBehavior();
    testAwardsModifyAfterOperations();
    testAwardsComplexOperations();
    testAwardsWithAndWithoutDescriptions();
    testAwardsLargeScaleOperations();
    testAwardsEmptyStringHandling();
    testAwardsChronologicalData();
    
    std::cout << "=======================================" << std::endl;
    std::cout << "Award and Awards tests completed." << std::endl;
    
    return 0;
}