#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include <algorithm>
#include "../../../src/ds/resume/certifications.h"

// Certification class tests
void testCertificationConstructor() {
    Certification cert("AWS Certified Solutions Architect", "Amazon Web Services", 
                      "AWS-12345", "https://aws.amazon.com/verify", 
                      "2024-01", "2027-01", false);
    
    assert(cert.getName() == "AWS Certified Solutions Architect");
    assert(cert.getAuthority() == "Amazon Web Services");
    assert(cert.getLicenseNumber() == "AWS-12345");
    assert(cert.getUrl() == "https://aws.amazon.com/verify");
    assert(cert.getIssueDate() == "2024-01");
    assert(cert.getExpirationDate() == "2027-01");
    assert(cert.isDoesNotExpire() == false);
    
    std::cout << "Certification Constructor: PASSED" << std::endl;
}

void testCertificationNonExpiring() {
    Certification cert("Professional Engineer License", "State Engineering Board", 
                      "PE-67890", "https://engineering.state.gov/verify", 
                      "2020-06", "2025-06", true);  // doesNotExpire = true
    
    assert(cert.getName() == "Professional Engineer License");
    assert(cert.getAuthority() == "State Engineering Board");
    assert(cert.getLicenseNumber() == "PE-67890");
    assert(cert.getUrl() == "https://engineering.state.gov/verify");
    assert(cert.getIssueDate() == "2020-06");
    assert(cert.getExpirationDate() == "");  // Should be empty for non-expiring
    assert(cert.isDoesNotExpire() == true);
    
    std::cout << "Certification Non-Expiring: PASSED" << std::endl;
}

void testCertificationExpiring() {
    Certification cert("Microsoft Azure Fundamentals", "Microsoft", 
                      "AZ-900-12345", "https://docs.microsoft.com/verify", 
                      "2023-05", "2025-05", false);  // doesNotExpire = false
    
    assert(cert.getName() == "Microsoft Azure Fundamentals");
    assert(cert.getAuthority() == "Microsoft");
    assert(cert.getLicenseNumber() == "AZ-900-12345");
    assert(cert.getUrl() == "https://docs.microsoft.com/verify");
    assert(cert.getIssueDate() == "2023-05");
    assert(cert.getExpirationDate() == "2025-05");  // Should have expiration date
    assert(cert.isDoesNotExpire() == false);
    
    std::cout << "Certification Expiring: PASSED" << std::endl;
}

void testCertificationEquality() {
    Certification cert1("Google Cloud Professional", "Google", "GCP-12345", 
                       "https://cloud.google.com/verify", "2024-03", "2026-03", false);
    Certification cert2("Google Cloud Professional", "Google", "GCP-12345", 
                       "https://cloud.google.com/verify", "2024-03", "2026-03", false);
    Certification cert3("Google Cloud Professional", "Google", "GCP-67890",  // Different license number
                       "https://cloud.google.com/verify", "2024-03", "2026-03", false);
    Certification cert4("Google Cloud Professional", "Google", "GCP-12345", 
                       "https://cloud.google.com/verify", "2024-03", "", true);  // Non-expiring version
    
    // Test equality
    assert(cert1 == cert2);  // Identical certifications
    
    // Test inequality
    assert(!(cert1 == cert3));  // Different license number
    assert(!(cert1 == cert4));  // Different expiration settings
    
    std::cout << "Certification Equality: PASSED" << std::endl;
}

void testCertificationEdgeCases() {
    // Test with empty strings
    Certification empty("", "", "", "", "", "", false);
    assert(empty.getName() == "");
    assert(empty.getAuthority() == "");
    assert(empty.getLicenseNumber() == "");
    assert(empty.getUrl() == "");
    assert(empty.getIssueDate() == "");
    assert(empty.getExpirationDate() == "");
    assert(empty.isDoesNotExpire() == false);
    
    // Test with very long strings
    std::string longName = "Very Long Certification Name That Could Be Used In Real World Professional Certifications";
    Certification longCert(longName, "Long Authority Name", "LONG-LICENSE-123456789", 
                          "https://very-long-url-for-verification.example.com/verify/cert/123456789",
                          "2024-12", "2027-12", false);
    assert(longCert.getName() == longName);
    
    std::cout << "Certification Edge Cases: PASSED" << std::endl;
}

// Certifications class tests
void testCertificationsDefaultConstructor() {
    Certifications certifications;
    
    assert(certifications.getCertifications().empty());
    assert(certifications.getCertifications().size() == 0);
    
    std::cout << "Certifications Default Constructor: PASSED" << std::endl;
}

void testCertificationsAddSingle() {
    Certifications certifications;
    
    Certification cert("CISSP", "ISC2", "CISSP-123456", "https://isc2.org/verify", 
                      "2023-01", "2026-01", false);
    certifications.addCertification(cert);
    
    std::vector<Certification> certList = certifications.getCertifications();
    assert(certList.size() == 1);
    assert(certList[0] == cert);
    assert(certList[0].getName() == "CISSP");
    assert(certList[0].getAuthority() == "ISC2");
    
    std::cout << "Certifications Add Single: PASSED" << std::endl;
}

void testCertificationsAddMultiple() {
    Certifications certifications;
    
    Certification aws("AWS Solutions Architect", "Amazon", "AWS-SA-123", "https://aws.com", 
                     "2024-01", "2027-01", false);
    Certification azure("Azure DevOps Engineer", "Microsoft", "AZ-400-456", "https://microsoft.com", 
                       "2024-02", "2026-02", false);
    Certification gcp("Google Cloud Architect", "Google", "GCP-ARCH-789", "https://google.com", 
                     "2024-03", "", true);  // Non-expiring
    
    certifications.addCertification(aws);
    certifications.addCertification(azure);
    certifications.addCertification(gcp);
    
    std::vector<Certification> certList = certifications.getCertifications();
    assert(certList.size() == 3);
    assert(certList[0] == aws);
    assert(certList[1] == azure);
    assert(certList[2] == gcp);
    
    // Verify non-expiring certification
    assert(certList[2].isDoesNotExpire() == true);
    assert(certList[2].getExpirationDate() == "");
    
    std::cout << "Certifications Add Multiple: PASSED" << std::endl;
}

void testCertificationsClearCertifications() {
    Certifications certifications;
    
    // Add several certifications
    certifications.addCertification(Certification("PMP", "PMI", "PMP-123", "", "2023-01", "2026-01", false));
    certifications.addCertification(Certification("SCRUM Master", "Scrum Alliance", "SM-456", "", "2023-06", "", true));
    certifications.addCertification(Certification("Six Sigma", "ASQ", "BB-789", "", "2022-12", "2025-12", false));
    
    assert(certifications.getCertifications().size() == 3);
    
    // Clear all certifications
    certifications.clearCertifications();
    assert(certifications.getCertifications().empty());
    assert(certifications.getCertifications().size() == 0);
    
    // Test clearing empty collection (should not crash)
    certifications.clearCertifications();
    assert(certifications.getCertifications().empty());
    
    std::cout << "Certifications Clear Certifications: PASSED" << std::endl;
}

void testCertificationsRemoveCertification() {
    Certifications certifications;
    
    Certification cert1("CompTIA Security+", "CompTIA", "SEC-001", "", "2023-01", "2026-01", false);
    Certification cert2("CompTIA Network+", "CompTIA", "NET-002", "", "2023-03", "2026-03", false);
    Certification cert3("CompTIA A+", "CompTIA", "A-003", "", "2023-05", "2026-05", false);
    
    certifications.addCertification(cert1);
    certifications.addCertification(cert2);
    certifications.addCertification(cert3);
    
    assert(certifications.getCertifications().size() == 3);
    
    // Remove middle certification
    certifications.removeCertification("CompTIA Network+");
    std::vector<Certification> certList = certifications.getCertifications();
    assert(certList.size() == 2);
    assert(certList[0].getName() == "CompTIA Security+");
    assert(certList[1].getName() == "CompTIA A+");
    
    // Remove first certification
    certifications.removeCertification("CompTIA Security+");
    certList = certifications.getCertifications();
    assert(certList.size() == 1);
    assert(certList[0].getName() == "CompTIA A+");
    
    // Remove last certification
    certifications.removeCertification("CompTIA A+");
    assert(certifications.getCertifications().empty());
    
    std::cout << "Certifications Remove Certification: PASSED" << std::endl;
}

void testCertificationsRemoveNonexistent() {
    Certifications certifications;
    
    Certification cert("Oracle Certified Professional", "Oracle", "OCP-123", "", "2024-01", "2027-01", false);
    certifications.addCertification(cert);
    
    assert(certifications.getCertifications().size() == 1);
    
    // Try to remove non-existent certification
    certifications.removeCertification("Non-existent Certification");
    assert(certifications.getCertifications().size() == 1);  // Should remain unchanged
    assert(certifications.getCertifications()[0].getName() == "Oracle Certified Professional");
    
    // Try to remove from empty collection
    certifications.clearCertifications();
    certifications.removeCertification("Any Certification");
    assert(certifications.getCertifications().empty());
    
    std::cout << "Certifications Remove Nonexistent: PASSED" << std::endl;
}

void testCertificationsRemoveDuplicateNames() {
    Certifications certifications;
    
    // Add certifications with same name but different details (different license numbers)
    Certification cert1("Java Certification", "Oracle", "JAVA-001", "", "2023-01", "2026-01", false);
    Certification cert2("Java Certification", "Oracle", "JAVA-002", "", "2024-01", "2027-01", false);  // Same name, different license
    Certification cert3("Python Certification", "Python Institute", "PYTHON-001", "", "2024-01", "", true);
    
    certifications.addCertification(cert1);
    certifications.addCertification(cert2);
    certifications.addCertification(cert3);
    
    assert(certifications.getCertifications().size() == 3);
    
    // Remove certifications with "Java Certification" name - should remove both
    certifications.removeCertification("Java Certification");
    std::vector<Certification> certList = certifications.getCertifications();
    assert(certList.size() == 1);
    assert(certList[0].getName() == "Python Certification");
    
    std::cout << "Certifications Remove Duplicate Names: PASSED" << std::endl;
}

void testCertificationsOrder() {
    Certifications certifications;
    
    Certification first("First Certification", "Authority 1", "CERT-001", "", "2023-01", "2026-01", false);
    Certification second("Second Certification", "Authority 2", "CERT-002", "", "2023-02", "", true);
    Certification third("Third Certification", "Authority 3", "CERT-003", "", "2023-03", "2026-03", false);
    
    certifications.addCertification(first);
    certifications.addCertification(second);
    certifications.addCertification(third);
    
    std::vector<Certification> certList = certifications.getCertifications();
    
    // Verify order is maintained
    assert(certList[0].getName() == "First Certification");
    assert(certList[1].getName() == "Second Certification");
    assert(certList[2].getName() == "Third Certification");
    
    std::cout << "Certifications Order: PASSED" << std::endl;
}

void testCertificationsCopyBehavior() {
    Certifications certifications;
    
    Certification original("Original Certification", "Original Authority", "ORIG-001", 
                          "", "2024-01", "2027-01", false);
    certifications.addCertification(original);
    
    // Get copy of certifications
    std::vector<Certification> certList = certifications.getCertifications();
    
    // Modify the copy
    certList.push_back(Certification("Added Certification", "Added Authority", "ADD-001", 
                                    "", "2024-06", "2027-06", false));
    
    // Original should be unchanged
    std::vector<Certification> originalList = certifications.getCertifications();
    assert(originalList.size() == 1);  // Original unchanged
    assert(certList.size() == 2);      // Copy was modified
    
    std::cout << "Certifications Copy Behavior: PASSED" << std::endl;
}

void testCertificationsExpirationHandling() {
    Certifications certifications;
    
    // Add mix of expiring and non-expiring certifications
    Certification expiring("Expiring Cert", "Authority A", "EXP-001", "", "2024-01", "2025-01", false);
    Certification nonExpiring("Non-Expiring Cert", "Authority B", "NOEXP-001", "", "2024-01", "2025-01", true);  // doesNotExpire = true
    Certification noExpDate("No Exp Date Cert", "Authority C", "NODATE-001", "", "2024-01", "", false);
    
    certifications.addCertification(expiring);
    certifications.addCertification(nonExpiring);
    certifications.addCertification(noExpDate);
    
    std::vector<Certification> certList = certifications.getCertifications();
    assert(certList.size() == 3);
    
    // Verify expiration handling
    assert(certList[0].getExpirationDate() == "2025-01");  // Has expiration date
    assert(certList[0].isDoesNotExpire() == false);
    
    assert(certList[1].getExpirationDate() == "");  // Non-expiring, so empty expiration date
    assert(certList[1].isDoesNotExpire() == true);
    
    assert(certList[2].getExpirationDate() == "");  // Empty expiration date
    assert(certList[2].isDoesNotExpire() == false);
    
    std::cout << "Certifications Expiration Handling: PASSED" << std::endl;
}

void testCertificationsComplexOperations() {
    Certifications certifications;
    
    // Add diverse certifications
    certifications.addCertification(Certification("AWS Solutions Architect", "AWS", "AWS-SA-123", 
                                                  "https://aws.com", "2024-01", "2027-01", false));
    certifications.addCertification(Certification("Google Cloud Architect", "Google", "GCP-ARCH-456", 
                                                  "https://google.com", "2024-02", "", true));
    certifications.addCertification(Certification("Microsoft Azure Expert", "Microsoft", "AZ-305-789", 
                                                  "https://microsoft.com", "2024-03", "2026-03", false));
    certifications.addCertification(Certification("Kubernetes Administrator", "CNCF", "CKA-101", 
                                                  "https://cncf.io", "2024-04", "2027-04", false));
    certifications.addCertification(Certification("Professional Scrum Master", "Scrum.org", "PSM-202", 
                                                  "https://scrum.org", "2024-05", "", true));
    
    assert(certifications.getCertifications().size() == 5);
    
    // Remove some certifications
    certifications.removeCertification("Microsoft Azure Expert");
    certifications.removeCertification("Professional Scrum Master");
    
    std::vector<Certification> remaining = certifications.getCertifications();
    assert(remaining.size() == 3);
    assert(remaining[0].getName() == "AWS Solutions Architect");
    assert(remaining[1].getName() == "Google Cloud Architect");
    assert(remaining[2].getName() == "Kubernetes Administrator");
    
    // Add more
    certifications.addCertification(Certification("Docker Certified Associate", "Docker", "DCA-303", 
                                                  "https://docker.com", "2024-06", "2027-06", false));
    assert(certifications.getCertifications().size() == 4);
    
    // Clear all
    certifications.clearCertifications();
    assert(certifications.getCertifications().empty());
    
    std::cout << "Certifications Complex Operations: PASSED" << std::endl;
}

void testCertificationsLargeScaleOperations() {
    Certifications certifications;
    
    // Add many certifications
    for (int i = 0; i < 25; ++i) {
        std::string name = "Certification " + std::to_string(i);
        std::string authority = "Authority " + std::to_string(i % 5);  // 5 different authorities
        std::string license = "LIC-" + std::to_string(i);
        std::string url = "https://example" + std::to_string(i % 3) + ".com";  // 3 different URLs
        bool doesNotExpire = (i % 3 == 0);  // Every 3rd certification doesn't expire
        std::string expDate = doesNotExpire ? "" : ("2027-" + std::to_string((i % 12) + 1));
        
        certifications.addCertification(Certification(name, authority, license, url, 
                                                     "2024-01", expDate, doesNotExpire));
    }
    
    assert(certifications.getCertifications().size() == 25);
    
    // Remove every other certification
    for (int i = 0; i < 25; i += 2) {
        std::string name = "Certification " + std::to_string(i);
        certifications.removeCertification(name);
    }
    
    assert(certifications.getCertifications().size() == 12);  // 25 - 13 removed = 12
    
    // Verify remaining certifications are odd-numbered
    std::vector<Certification> remaining = certifications.getCertifications();
    for (size_t i = 0; i < remaining.size(); ++i) {
        std::string expectedName = "Certification " + std::to_string((i * 2) + 1);
        assert(remaining[i].getName() == expectedName);
    }
    
    std::cout << "Certifications Large Scale Operations: PASSED" << std::endl;
}

void testCertificationsEmptyStringHandling() {
    Certifications certifications;
    
    // Add certification with empty name
    certifications.addCertification(Certification("", "Valid Authority", "VALID-123", 
                                                  "", "2024-01", "2027-01", false));
    certifications.addCertification(Certification("Valid Certification", "Valid Authority", "VALID-456", 
                                                  "", "2024-02", "", true));
    
    assert(certifications.getCertifications().size() == 2);
    
    // Remove certification with empty name
    certifications.removeCertification("");
    std::vector<Certification> certList = certifications.getCertifications();
    assert(certList.size() == 1);
    assert(certList[0].getName() == "Valid Certification");
    
    std::cout << "Certifications Empty String Handling: PASSED" << std::endl;
}

int main() {
    std::cout << "Running Certification and Certifications Class Tests..." << std::endl;
    std::cout << "======================================================" << std::endl;
    
    // Certification class tests
    testCertificationConstructor();
    testCertificationNonExpiring();
    testCertificationExpiring();
    testCertificationEquality();
    testCertificationEdgeCases();
    
    // Certifications class tests
    testCertificationsDefaultConstructor();
    testCertificationsAddSingle();
    testCertificationsAddMultiple();
    testCertificationsClearCertifications();
    testCertificationsRemoveCertification();
    testCertificationsRemoveNonexistent();
    testCertificationsRemoveDuplicateNames();
    testCertificationsOrder();
    testCertificationsCopyBehavior();
    testCertificationsExpirationHandling();
    testCertificationsComplexOperations();
    testCertificationsLargeScaleOperations();
    testCertificationsEmptyStringHandling();
    
    std::cout << "======================================================" << std::endl;
    std::cout << "Certification and Certifications tests completed." << std::endl;
    
    return 0;
}