#include <string>
#include <vector>

class Publication {
private:
    std::string title;
    std::string publisher;
    std::string url; // Optional URL to the publication
    std::string date; // Publication date in "YYYY-MM" format
    std::string description; // Optional description or summary
public:
    // Default constructor
    Publication() : title(""), publisher(""), url(""), date(""), description("") {}
    // Parameterized constructor
    Publication(const std::string& title, const std::string& publisher, const std::string& url,
                const std::string& date, const std::string& description)
        : title(title), publisher(publisher), url(url), date(date), description(description) {}
    // Getters
    std::string getTitle() const { return title; }
    std::string getPublisher() const { return publisher; }
    std::string getUrl() const { return url; }
    std::string getDate() const { return date; }
    std::string getDescription() const { return description; }
    // Setters
    void setTitle(const std::string& title) { this->title = title; }
    void setPublisher(const std::string& publisher) { this->publisher = publisher; }
    void setUrl(const std::string& url) { this->url = url; }
    void setDate(const std::string& date) { this->date = date; }
    void setDescription(const std::string& description) { this->description = description; }
    // Equality operator
    bool operator==(const Publication& other) const {
        return title == other.title && publisher == other.publisher &&
               url == other.url && date == other.date && description == other.description;
    }
};

class Publications {
private:
    std::vector<Publication> publications;
public:
    void addPublication(const Publication& publication) {
        publications.push_back(publication);
    }
    std::vector<Publication> getPublications() const {
        return publications;
    }
    void clearPublications() {
        publications.clear();
    }
    void removePublication(const std::string& publicationTitle) {
        publications.erase(std::remove_if(publications.begin(), publications.end(),
            [&publicationTitle](const Publication& pub) {
                return pub.getTitle() == publicationTitle;
            }), publications.end());
    }
};