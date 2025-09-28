#include <string>
#include <vector>

enum class ExperienceLevel {
    BEGINNER = 1,
    INTERMEDIATE,
    ADVANCED,
    EXPERT,
    MASTER
};

class Experience {
public:
    Experience(const std::string& name, ExperienceLevel level)
        : name(name), level(level) {}

    std::string getName() const { return name; }
    ExperienceLevel getLevel() const { return level; }

    void setLevel(ExperienceLevel newLevel) { level = newLevel; }
    bool operator==(const Experience& other) const {
        return name == other.name && level == other.level;
    }
private:
    std::string name;
    ExperienceLevel level;
};

class Experiences {
public:
    void addExperience(const Experience& experience) {
        experiences.push_back(experience);
    }

    std::vector<Experience> getExperiences() const {
        return experiences;
    }
    void clearExperiences() {
        experiences.clear();
    }
    void removeExperience(const std::string& name) {
        experiences.erase(std::remove_if(experiences.begin(), experiences.end(),
                                         [&name](const Experience& exp) { return exp.getName() == name; }),
                          experiences.end());
    }

private:
    std::vector<Experience> experiences;
};