#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include <algorithm>
#include "../../src/ds/languages.h"

// Language class tests
void testLanguageConstructor() {
    Language lang("English", LanguageProficiency::NATIVE);
    
    assert(lang.getName() == "English");
    assert(lang.getProficiency() == LanguageProficiency::NATIVE);
    
    std::cout << "Language Constructor: PASSED" << std::endl;
}

void testLanguageProficiencyLevels() {
    Language native("Spanish", LanguageProficiency::NATIVE);
    Language fluent("French", LanguageProficiency::FLUENT);
    Language intermediate("German", LanguageProficiency::INTERMEDIATE);
    Language basic("Japanese", LanguageProficiency::BASIC);
    
    assert(native.getProficiency() == LanguageProficiency::NATIVE);
    assert(fluent.getProficiency() == LanguageProficiency::FLUENT);
    assert(intermediate.getProficiency() == LanguageProficiency::INTERMEDIATE);
    assert(basic.getProficiency() == LanguageProficiency::BASIC);
    
    std::cout << "Language Proficiency Levels: PASSED" << std::endl;
}

void testLanguageSetProficiency() {
    Language lang("Italian", LanguageProficiency::BASIC);
    
    assert(lang.getProficiency() == LanguageProficiency::BASIC);
    
    // Improve proficiency
    lang.setProficiency(LanguageProficiency::INTERMEDIATE);
    assert(lang.getProficiency() == LanguageProficiency::INTERMEDIATE);
    
    lang.setProficiency(LanguageProficiency::FLUENT);
    assert(lang.getProficiency() == LanguageProficiency::FLUENT);
    
    lang.setProficiency(LanguageProficiency::NATIVE);
    assert(lang.getProficiency() == LanguageProficiency::NATIVE);
    
    // Can also go backwards (for testing purposes)
    lang.setProficiency(LanguageProficiency::BASIC);
    assert(lang.getProficiency() == LanguageProficiency::BASIC);
    
    std::cout << "Language Set Proficiency: PASSED" << std::endl;
}

void testLanguageEquality() {
    Language lang1("Mandarin", LanguageProficiency::FLUENT);
    Language lang2("Mandarin", LanguageProficiency::FLUENT);
    Language lang3("Mandarin", LanguageProficiency::INTERMEDIATE);  // Different proficiency
    Language lang4("Cantonese", LanguageProficiency::FLUENT);       // Different name
    
    // Test equality
    assert(lang1 == lang2);  // Same name and proficiency
    
    // Test inequality
    assert(!(lang1 == lang3));  // Same name, different proficiency
    assert(!(lang1 == lang4));  // Different name, same proficiency
    
    std::cout << "Language Equality: PASSED" << std::endl;
}

void testLanguageEdgeCases() {
    // Test with empty string
    Language empty("", LanguageProficiency::BASIC);
    assert(empty.getName() == "");
    assert(empty.getProficiency() == LanguageProficiency::BASIC);
    
    // Test with long language name
    std::string longName = "Proto-Indo-European-Hypothetical-Ancient-Language";
    Language longLang(longName, LanguageProficiency::INTERMEDIATE);
    assert(longLang.getName() == longName);
    
    // Test with special characters and spaces
    Language special("Sign Language (ASL)", LanguageProficiency::FLUENT);
    assert(special.getName() == "Sign Language (ASL)");
    
    Language regional("Swiss German", LanguageProficiency::INTERMEDIATE);
    assert(regional.getName() == "Swiss German");
    
    std::cout << "Language Edge Cases: PASSED" << std::endl;
}

void testLanguageProficiencyProgression() {
    Language lang("Portuguese", LanguageProficiency::BASIC);
    
    // Simulate learning progression
    assert(lang.getProficiency() == LanguageProficiency::BASIC);
    
    lang.setProficiency(LanguageProficiency::INTERMEDIATE);
    assert(lang.getProficiency() == LanguageProficiency::INTERMEDIATE);
    
    lang.setProficiency(LanguageProficiency::FLUENT);
    assert(lang.getProficiency() == LanguageProficiency::FLUENT);
    
    lang.setProficiency(LanguageProficiency::NATIVE);
    assert(lang.getProficiency() == LanguageProficiency::NATIVE);
    
    std::cout << "Language Proficiency Progression: PASSED" << std::endl;
}

// Languages class tests
void testLanguagesDefaultConstructor() {
    Languages languages;
    
    assert(languages.getLanguages().empty());
    assert(languages.getLanguages().size() == 0);
    
    std::cout << "Languages Default Constructor: PASSED" << std::endl;
}

void testLanguagesAddSingle() {
    Languages languages;
    
    Language lang("Korean", LanguageProficiency::INTERMEDIATE);
    languages.addLanguage(lang);
    
    std::vector<Language> langList = languages.getLanguages();
    assert(langList.size() == 1);
    assert(langList[0] == lang);
    assert(langList[0].getName() == "Korean");
    assert(langList[0].getProficiency() == LanguageProficiency::INTERMEDIATE);
    
    std::cout << "Languages Add Single: PASSED" << std::endl;
}

void testLanguagesAddMultiple() {
    Languages languages;
    
    Language english("English", LanguageProficiency::NATIVE);
    Language spanish("Spanish", LanguageProficiency::FLUENT);
    Language french("French", LanguageProficiency::INTERMEDIATE);
    Language german("German", LanguageProficiency::BASIC);
    
    languages.addLanguage(english);
    languages.addLanguage(spanish);
    languages.addLanguage(french);
    languages.addLanguage(german);
    
    std::vector<Language> langList = languages.getLanguages();
    assert(langList.size() == 4);
    assert(langList[0] == english);
    assert(langList[1] == spanish);
    assert(langList[2] == french);
    assert(langList[3] == german);
    
    std::cout << "Languages Add Multiple: PASSED" << std::endl;
}

void testLanguagesClearLanguages() {
    Languages languages;
    
    // Add several languages
    languages.addLanguage(Language("Russian", LanguageProficiency::INTERMEDIATE));
    languages.addLanguage(Language("Arabic", LanguageProficiency::BASIC));
    languages.addLanguage(Language("Hebrew", LanguageProficiency::FLUENT));
    
    assert(languages.getLanguages().size() == 3);
    
    // Clear all languages
    languages.clearLanguages();
    assert(languages.getLanguages().empty());
    assert(languages.getLanguages().size() == 0);
    
    // Test clearing empty collection (should not crash)
    languages.clearLanguages();
    assert(languages.getLanguages().empty());
    
    std::cout << "Languages Clear Languages: PASSED" << std::endl;
}

void testLanguagesRemoveLanguage() {
    Languages languages;
    
    Language lang1("Dutch", LanguageProficiency::INTERMEDIATE);
    Language lang2("Swedish", LanguageProficiency::BASIC);
    Language lang3("Norwegian", LanguageProficiency::FLUENT);
    
    languages.addLanguage(lang1);
    languages.addLanguage(lang2);
    languages.addLanguage(lang3);
    
    assert(languages.getLanguages().size() == 3);
    
    // Remove middle language
    languages.removeLanguage("Swedish");
    std::vector<Language> langList = languages.getLanguages();
    assert(langList.size() == 2);
    assert(langList[0].getName() == "Dutch");
    assert(langList[1].getName() == "Norwegian");
    
    // Remove first language
    languages.removeLanguage("Dutch");
    langList = languages.getLanguages();
    assert(langList.size() == 1);
    assert(langList[0].getName() == "Norwegian");
    
    // Remove last language
    languages.removeLanguage("Norwegian");
    assert(languages.getLanguages().empty());
    
    std::cout << "Languages Remove Language: PASSED" << std::endl;
}

void testLanguagesRemoveNonexistent() {
    Languages languages;
    
    Language lang("Polish", LanguageProficiency::BASIC);
    languages.addLanguage(lang);
    
    assert(languages.getLanguages().size() == 1);
    
    // Try to remove non-existent language
    languages.removeLanguage("Nonexistent Language");
    assert(languages.getLanguages().size() == 1);  // Should remain unchanged
    assert(languages.getLanguages()[0].getName() == "Polish");
    
    // Try to remove from empty collection
    languages.clearLanguages();
    languages.removeLanguage("Any Language");
    assert(languages.getLanguages().empty());
    
    std::cout << "Languages Remove Nonexistent: PASSED" << std::endl;
}

void testLanguagesRemoveDuplicateNames() {
    Languages languages;
    
    // Add languages with same name but different proficiency levels
    Language chinese1("Chinese", LanguageProficiency::BASIC);
    Language chinese2("Chinese", LanguageProficiency::INTERMEDIATE);  // Same name, different proficiency
    Language hindi("Hindi", LanguageProficiency::FLUENT);
    
    languages.addLanguage(chinese1);
    languages.addLanguage(chinese2);
    languages.addLanguage(hindi);
    
    assert(languages.getLanguages().size() == 3);
    
    // Remove languages with "Chinese" name - should remove both
    languages.removeLanguage("Chinese");
    std::vector<Language> langList = languages.getLanguages();
    assert(langList.size() == 1);
    assert(langList[0].getName() == "Hindi");
    
    std::cout << "Languages Remove Duplicate Names: PASSED" << std::endl;
}

void testLanguagesOrder() {
    Languages languages;
    
    Language first("First Language", LanguageProficiency::NATIVE);
    Language second("Second Language", LanguageProficiency::FLUENT);
    Language third("Third Language", LanguageProficiency::INTERMEDIATE);
    Language fourth("Fourth Language", LanguageProficiency::BASIC);
    
    languages.addLanguage(first);
    languages.addLanguage(second);
    languages.addLanguage(third);
    languages.addLanguage(fourth);
    
    std::vector<Language> langList = languages.getLanguages();
    
    // Verify order is maintained
    assert(langList[0].getName() == "First Language");
    assert(langList[1].getName() == "Second Language");
    assert(langList[2].getName() == "Third Language");
    assert(langList[3].getName() == "Fourth Language");
    
    std::cout << "Languages Order: PASSED" << std::endl;
}

void testLanguagesCopyBehavior() {
    Languages languages;
    
    Language original("Turkish", LanguageProficiency::INTERMEDIATE);
    languages.addLanguage(original);
    
    // Get copy of languages
    std::vector<Language> langList = languages.getLanguages();
    
    // Modify the copy
    langList.push_back(Language("Finnish", LanguageProficiency::BASIC));
    
    // Original should be unchanged
    std::vector<Language> originalList = languages.getLanguages();
    assert(originalList.size() == 1);  // Original unchanged
    assert(langList.size() == 2);      // Copy was modified
    
    std::cout << "Languages Copy Behavior: PASSED" << std::endl;
}

void testLanguagesAllProficiencyLevels() {
    Languages languages;
    
    // Add one language for each proficiency level
    languages.addLanguage(Language("Native Language", LanguageProficiency::NATIVE));
    languages.addLanguage(Language("Fluent Language", LanguageProficiency::FLUENT));
    languages.addLanguage(Language("Intermediate Language", LanguageProficiency::INTERMEDIATE));
    languages.addLanguage(Language("Basic Language", LanguageProficiency::BASIC));
    
    std::vector<Language> langList = languages.getLanguages();
    assert(langList.size() == 4);
    
    // Verify proficiency levels
    assert(langList[0].getProficiency() == LanguageProficiency::NATIVE);
    assert(langList[1].getProficiency() == LanguageProficiency::FLUENT);
    assert(langList[2].getProficiency() == LanguageProficiency::INTERMEDIATE);
    assert(langList[3].getProficiency() == LanguageProficiency::BASIC);
    
    std::cout << "Languages All Proficiency Levels: PASSED" << std::endl;
}

void testLanguagesModifyAfterOperations() {
    Languages languages;
    
    // Add initial languages
    languages.addLanguage(Language("Bulgarian", LanguageProficiency::BASIC));
    languages.addLanguage(Language("Romanian", LanguageProficiency::INTERMEDIATE));
    languages.addLanguage(Language("Hungarian", LanguageProficiency::FLUENT));
    
    // Remove one
    languages.removeLanguage("Romanian");
    assert(languages.getLanguages().size() == 2);
    
    // Add more after removal
    languages.addLanguage(Language("Czech", LanguageProficiency::INTERMEDIATE));
    languages.addLanguage(Language("Slovak", LanguageProficiency::BASIC));
    
    std::vector<Language> langList = languages.getLanguages();
    assert(langList.size() == 4);
    assert(langList[0].getName() == "Bulgarian");
    assert(langList[1].getName() == "Hungarian");
    assert(langList[2].getName() == "Czech");
    assert(langList[3].getName() == "Slovak");
    
    std::cout << "Languages Modify After Operations: PASSED" << std::endl;
}

void testLanguagesComplexOperations() {
    Languages languages;
    
    // Add diverse languages from different families
    languages.addLanguage(Language("English", LanguageProficiency::NATIVE));      // Germanic
    languages.addLanguage(Language("Spanish", LanguageProficiency::FLUENT));      // Romance
    languages.addLanguage(Language("Mandarin", LanguageProficiency::INTERMEDIATE)); // Sino-Tibetan
    languages.addLanguage(Language("Arabic", LanguageProficiency::BASIC));        // Semitic
    languages.addLanguage(Language("Japanese", LanguageProficiency::INTERMEDIATE)); // Japonic
    languages.addLanguage(Language("Swahili", LanguageProficiency::BASIC));       // Niger-Congo
    
    assert(languages.getLanguages().size() == 6);
    
    // Remove some languages
    languages.removeLanguage("Arabic");
    languages.removeLanguage("Swahili");
    
    std::vector<Language> remaining = languages.getLanguages();
    assert(remaining.size() == 4);
    assert(remaining[0].getName() == "English");
    assert(remaining[1].getName() == "Spanish");
    assert(remaining[2].getName() == "Mandarin");
    assert(remaining[3].getName() == "Japanese");
    
    // Add more
    languages.addLanguage(Language("Portuguese", LanguageProficiency::FLUENT));
    languages.addLanguage(Language("Korean", LanguageProficiency::BASIC));
    assert(languages.getLanguages().size() == 6);
    
    // Clear all
    languages.clearLanguages();
    assert(languages.getLanguages().empty());
    
    std::cout << "Languages Complex Operations: PASSED" << std::endl;
}

void testLanguagesLargeScaleOperations() {
    Languages languages;
    
    // Add many languages (representing a polyglot's language skills)
    std::vector<std::string> worldLanguages = {
        "English", "Spanish", "French", "German", "Italian", "Portuguese", "Dutch",
        "Russian", "Polish", "Czech", "Hungarian", "Romanian", "Bulgarian",
        "Arabic", "Hebrew", "Persian", "Turkish", "Kurdish",
        "Mandarin", "Cantonese", "Japanese", "Korean", "Vietnamese", "Thai",
        "Hindi", "Bengali", "Tamil", "Telugu", "Marathi", "Gujarati",
        "Swahili", "Yoruba", "Amharic", "Hausa"
    };
    
    for (size_t i = 0; i < worldLanguages.size(); ++i) {
        LanguageProficiency level = static_cast<LanguageProficiency>(i % 4);
        languages.addLanguage(Language(worldLanguages[i], level));
    }
    
    assert(languages.getLanguages().size() == worldLanguages.size());
    
    // Remove every other language
    for (size_t i = 0; i < worldLanguages.size(); i += 2) {
        languages.removeLanguage(worldLanguages[i]);
    }
    
    size_t expectedRemaining = worldLanguages.size() - (worldLanguages.size() / 2);
    assert(languages.getLanguages().size() == expectedRemaining);
    
    // Verify remaining languages are odd-indexed from original list
    std::vector<Language> remaining = languages.getLanguages();
    for (size_t i = 0; i < remaining.size(); ++i) {
        size_t originalIndex = (i * 2) + 1;
        if (originalIndex < worldLanguages.size()) {
            assert(remaining[i].getName() == worldLanguages[originalIndex]);
        }
    }
    
    std::cout << "Languages Large Scale Operations: PASSED" << std::endl;
}

void testLanguagesEmptyStringHandling() {
    Languages languages;
    
    // Add language with empty name
    languages.addLanguage(Language("", LanguageProficiency::BASIC));
    languages.addLanguage(Language("Valid Language", LanguageProficiency::FLUENT));
    
    assert(languages.getLanguages().size() == 2);
    
    // Remove language with empty name
    languages.removeLanguage("");
    std::vector<Language> langList = languages.getLanguages();
    assert(langList.size() == 1);
    assert(langList[0].getName() == "Valid Language");
    
    std::cout << "Languages Empty String Handling: PASSED" << std::endl;
}

void testLanguagesProficiencyDistribution() {
    Languages languages;
    
    // Add languages with different proficiency distributions
    languages.addLanguage(Language("Native 1", LanguageProficiency::NATIVE));
    languages.addLanguage(Language("Native 2", LanguageProficiency::NATIVE));
    languages.addLanguage(Language("Fluent 1", LanguageProficiency::FLUENT));
    languages.addLanguage(Language("Fluent 2", LanguageProficiency::FLUENT));
    languages.addLanguage(Language("Fluent 3", LanguageProficiency::FLUENT));
    languages.addLanguage(Language("Intermediate 1", LanguageProficiency::INTERMEDIATE));
    languages.addLanguage(Language("Intermediate 2", LanguageProficiency::INTERMEDIATE));
    languages.addLanguage(Language("Basic 1", LanguageProficiency::BASIC));
    
    std::vector<Language> langList = languages.getLanguages();
    assert(langList.size() == 8);
    
    // Count proficiency levels
    int nativeCount = 0, fluentCount = 0, intermediateCount = 0, basicCount = 0;
    for (const auto& lang : langList) {
        switch (lang.getProficiency()) {
            case LanguageProficiency::NATIVE: nativeCount++; break;
            case LanguageProficiency::FLUENT: fluentCount++; break;
            case LanguageProficiency::INTERMEDIATE: intermediateCount++; break;
            case LanguageProficiency::BASIC: basicCount++; break;
        }
    }
    
    assert(nativeCount == 2);
    assert(fluentCount == 3);
    assert(intermediateCount == 2);
    assert(basicCount == 1);
    
    std::cout << "Languages Proficiency Distribution: PASSED" << std::endl;
}

void testLanguagesRealWorldScenario() {
    Languages languages;
    
    // Simulate a multilingual professional's language portfolio
    languages.addLanguage(Language("English", LanguageProficiency::NATIVE));
    languages.addLanguage(Language("Spanish", LanguageProficiency::FLUENT));
    languages.addLanguage(Language("French", LanguageProficiency::INTERMEDIATE));
    languages.addLanguage(Language("German", LanguageProficiency::BASIC));
    languages.addLanguage(Language("Mandarin", LanguageProficiency::INTERMEDIATE));
    
    // Verify initial state
    assert(languages.getLanguages().size() == 5);
    
    // Simulate improving proficiency (would need to remove and re-add with new proficiency)
    languages.removeLanguage("German");
    languages.addLanguage(Language("German", LanguageProficiency::INTERMEDIATE));  // Improved from BASIC
    
    // Verify improvement
    std::vector<Language> langList = languages.getLanguages();
    bool foundGerman = false;
    for (const auto& lang : langList) {
        if (lang.getName() == "German") {
            assert(lang.getProficiency() == LanguageProficiency::INTERMEDIATE);
            foundGerman = true;
        }
    }
    assert(foundGerman);
    
    std::cout << "Languages Real World Scenario: PASSED" << std::endl;
}

int main() {
    std::cout << "Running Language and Languages Class Tests..." << std::endl;
    std::cout << "=============================================" << std::endl;
    
    // Language class tests
    testLanguageConstructor();
    testLanguageProficiencyLevels();
    testLanguageSetProficiency();
    testLanguageEquality();
    testLanguageEdgeCases();
    testLanguageProficiencyProgression();
    
    // Languages class tests
    testLanguagesDefaultConstructor();
    testLanguagesAddSingle();
    testLanguagesAddMultiple();
    testLanguagesClearLanguages();
    testLanguagesRemoveLanguage();
    testLanguagesRemoveNonexistent();
    testLanguagesRemoveDuplicateNames();
    testLanguagesOrder();
    testLanguagesCopyBehavior();
    testLanguagesAllProficiencyLevels();
    testLanguagesModifyAfterOperations();
    testLanguagesComplexOperations();
    testLanguagesLargeScaleOperations();
    testLanguagesEmptyStringHandling();
    testLanguagesProficiencyDistribution();
    testLanguagesRealWorldScenario();
    
    std::cout << "=============================================" << std::endl;
    std::cout << "Language and Languages tests completed." << std::endl;
    
    return 0;
}