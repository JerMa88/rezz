#ifndef SKILL_H
#define SKILL_H

#include <string>

enum SkillLevel {
    BEGINNER = 1,
    INTERMEDIATE = 2,
    ADVANCED = 3,
    EXPERT = 4
};

class Skill {
public:
    Skill(const std::string& name, int yearsOfExperience, SkillLevel level);
    ~Skill();

    std::string getName() const;
    int getYearsOfExperience() const;
    SkillLevel getLevel() const;
    void setLevel(SkillLevel level);
    
    // Equality operator for comparison
    bool operator==(const Skill& other) const;

private:
    std::string name;
    int yearsOfExperience;
    SkillLevel level;
};

#endif // SKILL_H