#include <string>
#include <vector>
#include "skills.h"
#include "educations.h"
#include "experiences.h"
#include "projects.h"
#include "publications.h"
#include "certifications.h"
#include "awards.h"
#include "languages.h"

class Resume {
    private:
        int id;
        std::string name;
        std::string email;
        std::string city; //city, state. eg: "San Francisco, CA"
        std::string phone;
        std::string linkedin; //linkedin profile url
        std::string website; //personal website or portfolio

        Skills skills;
        Education education;
        Experiences experiences;
        Projects projects;
        Publications publications;
        Certifications certifications;
        Awards awards;
        Languages languages;
        std::string interests;

    public:
        Resume();
        Resume(int id, const std::string& name, const std::string& email,
               const std::string& city, const std::string& phone,
               const std::string& linkedin = "", const std::string& website = "");

        // Getters
        int getId() const;
        std::string getName() const;
        std::string getEmail() const;
        std::string getCity() const;
        std::string getPhone() const;
        std::string getLinkedin() const;
        std::string getWebsite() const;
        std::string getInterests() const;

        Skills& getSkills();
        Education& getEducation();
        Experiences& getExperiences();
        Projects& getProjects();
        Publications& getPublications();
        Certifications& getCertifications();
        Awards& getAwards();
        Languages& getLanguages();

        // Setters
        void setId(int id);
        void setName(const std::string& name);
        void setEmail(const std::string& email);
        void setCity(const std::string& city);
        void setPhone(const std::string& phone);
        void setLinkedin(const std::string& linkedin);
        void setWebsite(const std::string& website);
        void setInterests(const std::string& interests);

        void setSkills(const Skills& skills);
        void setEducation(const Education& education);
        void setExperiences(const Experiences& experiences);
        void setProjects(const Projects& projects);
        void setPublications(const Publications& publications);
        void setCertifications(const Certifications& certifications);
        void setAwards(const Awards& awards);
        void setLanguages(const Languages& languages);
};
