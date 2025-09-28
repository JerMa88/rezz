#include <algorithm>
#include "skills.h"

Skills::Skills() {}
Skills::Skills(const std::string& type) : skillType(type) {}
Skills::~Skills() {}

void Skills::addSkill(const Skill& skill) {
    skillList.push_back(skill);
}

bool Skills::hasSkill(const Skill& skill) const {
    return std::find(skillList.begin(), skillList.end(), skill) != skillList.end();
}

void Skills::removeSkill(const Skill& skill) {
    skillList.erase(std::remove(skillList.begin(), skillList.end(), skill), skillList.end());
}

std::vector<Skill> Skills::getAllSkills() const {
    return skillList;
}

std::string Skills::getSkillType() const {
    return skillType;
}

void Skills::setSkillType(const std::string& type) {
    skillType = type;
}

size_t Skills::getSkillCount() const {
    return skillList.size();
}
