#include "skill.h"

Skill::Skill(const std::string& name, int yearsOfExperience, SkillLevel level)
    : name(name), yearsOfExperience(yearsOfExperience), level(level) {}

Skill::~Skill() {}
std::string Skill::getName() const { return name; }
int Skill::getYearsOfExperience() const { return yearsOfExperience; }
SkillLevel Skill::getLevel() const { return level; }
void Skill::setLevel(SkillLevel level) { this->level = level; }

bool Skill::operator==(const Skill& other) const {
    return name == other.name && 
           yearsOfExperience == other.yearsOfExperience && 
           level == other.level;
}
