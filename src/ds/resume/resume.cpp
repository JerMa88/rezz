#include "resume.h"

Resume::Resume() : id(0), education("", "", "", "", "") {}

Resume::Resume(int id, const std::string& name, const std::string& email,
               const std::string& city, const std::string& phone,
               const std::string& linkedin, const std::string& website)
    : id(id), name(name), email(email), city(city), phone(phone),
      linkedin(linkedin), website(website), education("", "", "", "", "") {}

// Getters
int Resume::getId() const {
    return id;
}

std::string Resume::getName() const {
    return name;
}

std::string Resume::getEmail() const {
    return email;
}

std::string Resume::getCity() const {
    return city;
}

std::string Resume::getPhone() const {
    return phone;
}

std::string Resume::getLinkedin() const {
    return linkedin;
}

std::string Resume::getWebsite() const {
    return website;
}

std::string Resume::getInterests() const {
    return interests;
}

Skills& Resume::getSkills() {
    return skills;
}

Education& Resume::getEducation() {
    return education;
}

Experiences& Resume::getExperiences() {
    return experiences;
}

Projects& Resume::getProjects() {
    return projects;
}

Publications& Resume::getPublications() {
    return publications;
}

Certifications& Resume::getCertifications() {
    return certifications;
}

Awards& Resume::getAwards() {
    return awards;
}

Languages& Resume::getLanguages() {
    return languages;
}

// Setters
void Resume::setId(int id) {
    this->id = id;
}

void Resume::setName(const std::string& name) {
    this->name = name;
}

void Resume::setEmail(const std::string& email) {
    this->email = email;
}

void Resume::setCity(const std::string& city) {
    this->city = city;
}

void Resume::setPhone(const std::string& phone) {
    this->phone = phone;
}

void Resume::setLinkedin(const std::string& linkedin) {
    this->linkedin = linkedin;
}

void Resume::setWebsite(const std::string& website) {
    this->website = website;
}

void Resume::setInterests(const std::string& interests) {
    this->interests = interests;
}

void Resume::setSkills(const Skills& skills) {
    this->skills = skills;
}

void Resume::setEducation(const Education& education) {
    this->education = education;
}

void Resume::setExperiences(const Experiences& experiences) {
    this->experiences = experiences;
}

void Resume::setProjects(const Projects& projects) {
    this->projects = projects;
}

void Resume::setPublications(const Publications& publications) {
    this->publications = publications;
}

void Resume::setCertifications(const Certifications& certifications) {
    this->certifications = certifications;
}

void Resume::setAwards(const Awards& awards) {
    this->awards = awards;
}

void Resume::setLanguages(const Languages& languages) {
    this->languages = languages;
}