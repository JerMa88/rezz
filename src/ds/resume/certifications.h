#include <string>
#include <vector>

class Certification {
private:
    std::string name;
    std::string authority;
    std::string licenseNumber;
    std::string url;
    std::string issueDate; // Format: "YYYY-MM"
    std::string expirationDate; // Format: "YYYY-MM", empty if no expiration
    bool doesNotExpire;
public:
    Certification(const std::string& name, const std::string& authority, const std::string& licenseNumber,
                  const std::string& url, const std::string& issueDate, const std::string& expirationDate, bool doesNotExpire)
        : name(name), authority(authority), licenseNumber(licenseNumber), url(url),
          issueDate(issueDate), expirationDate
            (doesNotExpire ? "" : expirationDate), doesNotExpire(doesNotExpire) {}
    std::string getName() const { return name; }
    std::string getAuthority() const { return authority; }
    std::string getLicenseNumber() const { return licenseNumber; }
    std::string getUrl() const { return url; }
    std::string getIssueDate() const { return issueDate; }
    std::string getExpirationDate() const { return expirationDate; }
    bool isDoesNotExpire() const { return doesNotExpire; }
    bool operator==(const Certification& other) const {
        return name == other.name && authority == other.authority &&
               licenseNumber == other.licenseNumber && url == other.url &&
               issueDate == other.issueDate && expirationDate == other.expirationDate &&
               doesNotExpire == other.doesNotExpire;
    }
};
class Certifications {
private:
    std::vector<Certification> certifications;
public:
    void addCertification(const Certification& certification) {
        certifications.push_back(certification);
    }
    std::vector<Certification> getCertifications() const {
        return certifications;
    }
    void clearCertifications() {
        certifications.clear();
    }
    void removeCertification(const std::string& name) {
        certifications.erase(std::remove_if(certifications.begin(), certifications.end(),
                                            [&name](const Certification& cert) { return cert.getName() == name; }),
                             certifications.end());
    }
};