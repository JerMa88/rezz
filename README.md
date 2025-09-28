# Resume Builder CLI

A comprehensive command-line interface for building resumes that outputs structured JSON data suitable for database ingestion.

## Features

- **Interactive CLI Interface**: Step-by-step prompts for all resume sections
- **Comprehensive Data Collection**:
  - Basic Information (name, email, contact details)
  - Skills with experience levels and years
  - Education with courses and GPA
  - Work Experiences with proficiency levels
  - Projects with associated skills
  - Languages with proficiency levels
  - Awards and certifications
- **JSON Output**: Clean, structured JSON format perfect for database import
- **Database-Ready**: Output format designed for easy ingestion into relational databases

## Building the Application

```bash
# Build the resume builder
make resume_builder

# Or build everything (including tests)
make all
```

## Running the Application

```bash
# Run the application interactively
make run

# Or run directly
./build/resume_builder
```

## Usage

The application will guide you through each section of your resume:

1. **Basic Information**: Name, email, phone, LinkedIn, website, interests
2. **Skills**: Technical skills with experience levels (1-4) and years of experience
3. **Education**: Institution, degree, field, dates, GPA, relevant courses
4. **Experiences**: Technologies/tools with proficiency levels (1-5)
5. **Projects**: Name, description, URLs, skills used, dates
6. **Languages**: Spoken languages with proficiency levels (1-5)
7. **Awards**: Name, date, issuer, description
8. **Certifications**: Name, authority, license numbers, expiration dates

## JSON Output Format

The application generates a structured JSON file with the following format:

```json
{
  "id": 1,
  "name": "John Doe",
  "email": "john.doe@email.com",
  "city": "San Francisco, CA",
  "phone": "+1-555-123-4567",
  "linkedin": "https://linkedin.com/in/johndoe",
  "website": "https://johndoe.dev",
  "interests": "Reading, hiking, coding",
  "skills": {
    "type": "Programming Languages",
    "count": 3,
    "list": [
      {
        "name": "Python",
        "years": 5,
        "level": 3
      }
    ]
  },
  "education": {
    "institution": "MIT",
    "degree": "Bachelor of Science",
    "field": "Computer Science",
    "start_date": "2020-09",
    "end_date": "2024-05",
    "gpa": 3.8,
    "courses": ["Data Structures", "Algorithms"]
  },
  "experiences": [...],
  "projects": [...],
  "languages": [...],
  "awards": [...],
  "certifications": [...]
}
```

## Database Integration

The JSON output is designed to be easily imported into relational databases:

### Option 1: Single JSON Column
Store the entire JSON as a single column in a `resumes` table:

```sql
CREATE TABLE resumes (
    id SERIAL PRIMARY KEY,
    resume_data JSONB,
    created_at TIMESTAMP DEFAULT NOW()
);

INSERT INTO resumes (resume_data) VALUES ('{ ... }');
```

### Option 2: Normalized Tables
Parse the JSON and insert into normalized tables:

```sql
-- Main resume table
CREATE TABLE resumes (
    id SERIAL PRIMARY KEY,
    name VARCHAR(255),
    email VARCHAR(255),
    city VARCHAR(255),
    phone VARCHAR(50),
    linkedin VARCHAR(255),
    website VARCHAR(255),
    interests TEXT
);

-- Skills table
CREATE TABLE skills (
    id SERIAL PRIMARY KEY,
    resume_id INTEGER REFERENCES resumes(id),
    name VARCHAR(255),
    years INTEGER,
    level INTEGER
);

-- Education table
CREATE TABLE education (
    id SERIAL PRIMARY KEY,
    resume_id INTEGER REFERENCES resumes(id),
    institution VARCHAR(255),
    degree VARCHAR(255),
    field VARCHAR(255),
    start_date VARCHAR(20),
    end_date VARCHAR(20),
    gpa DECIMAL(3,2)
);

-- Additional tables for projects, experiences, etc.
```

## Testing

```bash
# Run all tests
make test

# Run specific test
make -C test test_ds_resume
```

## Example Output

See `sample_resume.json` for a complete example of the JSON output format.

## Project Structure

```
├── src/
│   ├── resume_builder.cpp          # Main CLI application
│   └── ds/resume/                  # Data structure classes
│       ├── resume.h/.cpp           # Main Resume class
│       ├── skills.h/.cpp           # Skills management
│       ├── education.h/.cpp        # Education data
│       └── ...                     # Other data structures
├── test/
│   └── ds/resume/                  # Comprehensive test suite
├── build/                          # Build output
├── Makefile                        # Build configuration
└── README.md                       # This file
```

## Contributing

1. All data structures are thoroughly tested
2. Follow existing code style conventions
3. Update tests when adding new features
4. Ensure JSON output remains database-compatible
