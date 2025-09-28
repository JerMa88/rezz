#include <string>
#include <vector>

enum class LanguageProficiency {
    NATIVE,
    FLUENT,
    INTERMEDIATE,
    BASIC
};
class Language {
private:
    std::string name;
    LanguageProficiency proficiency;
public:
    Language(const std::string& name, LanguageProficiency proficiency)
        : name(name), proficiency(proficiency) {}
    std::string getName() const { return name; }
    LanguageProficiency getProficiency() const { return proficiency; }
    void setProficiency(LanguageProficiency newProficiency) { proficiency = newProficiency; }
    bool operator==(const Language& other) const {
        return name == other.name && proficiency == other.proficiency;
    }
};
class Languages {
private:
    std::vector<Language> languages;
public:
    void addLanguage(const Language& language) {
        languages.push_back(language);
    }
    std::vector<Language> getLanguages() const {
        return languages;
    }
    void clearLanguages() {
        languages.clear();
    }
    void removeLanguage(const std::string& name) {
        languages.erase(std::remove_if(languages.begin(), languages.end(),
                                       [&name](const Language& lang) { return lang.getName() == name; }),
                        languages.end());
    }
};