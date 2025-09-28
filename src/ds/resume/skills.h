#ifndef SKILLS_H
#define SKILLS_H

#include <string>
#include <vector>
#include "skill.h"

class Skills {
private:
    std::string skillType; //e.g., "DevOps", "Programming Languages", "Frameworks", "Tools"
    std::vector<Skill> skillList;

public:
    Skills();
    Skills(const std::string& type);
    ~Skills();

    void addSkill(const Skill& skill);
    bool hasSkill(const Skill& skill) const;
    void removeSkill(const Skill& skill);
    std::vector<Skill> getAllSkills() const;
    std::string getSkillType() const;
    void setSkillType(const std::string& type);
    size_t getSkillCount() const;
};

#endif // SKILLS_H