#include <string>
#include <vector>

class Award {
private:
    std::string name;
    std::string date; // Format: "YYYY-MM"
    std::string issuer;
    std::string description; // Optional description of the award
public:
    Award(const std::string& name, const std::string& date, const std::string& issuer,
          const std::string& description = "")
        : name(name), date(date), issuer(issuer), description(description) {}
    std::string getName() const { return name; }
    std::string getDate() const { return date; }
    std::string getIssuer() const { return issuer; }
    std::string getDescription() const { return description; }
    bool operator==(const Award& other) const {
        return name == other.name && date == other.date &&
               issuer == other.issuer && description == other.description;
    }
};
class Awards {
private:
    std::vector<Award> awards;
public:
    void addAward(const Award& award) {
        awards.push_back(award);
    }
    std::vector<Award> getAwards() const {
        return awards;
    }
    void clearAwards() {
        awards.clear();
    }
    void removeAward(const std::string& name) {
        awards.erase(std::remove_if(awards.begin(), awards.end(),
                                    [&name](const Award& award) { return award.getName() == name; }),
                     awards.end());
    }
};