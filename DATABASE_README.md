# REZZ Database System

This document describes the PostgreSQL database integration for the REZZ job application tracking system.

## Overview

The REZZ system now includes a PostgreSQL database backend that provides:
- Relational data storage for Resume, JobListing, and JobApplication entities
- CRUD operations for all data types
- CSV and JSON export functionality
- Data integrity and consistency through foreign key constraints
- Comprehensive test coverage

## Database Schema

### Tables

#### `resumes`
Stores resume information for job applicants.
- `id` (SERIAL PRIMARY KEY)
- `name`, `email`, `city`, `phone`, `linkedin`, `website`
- `interests` (TEXT)
- `created_at`, `updated_at` (TIMESTAMP)

#### `job_listings`
Stores job listing information.
- `id` (SERIAL PRIMARY KEY)
- `job_id` (VARCHAR UNIQUE) - External job identifier
- `title`, `company`, `description`, `location`
- `remote_type`, `job_type`, `experience_level` (INTEGER enums)
- `salary_min`, `salary_max`, `salary_currency`
- Various other job-related fields
- `created_at`, `updated_at` (TIMESTAMP)

#### `job_applications`
Stores job application tracking information.
- `id` (SERIAL PRIMARY KEY)
- `application_id` (VARCHAR UNIQUE)
- `job_id` (VARCHAR) - Foreign key to job_listings
- `job_title`, `company`, `date_applied`
- `status` (INTEGER enum)
- Contact information, salary details, notes
- `created_at`, `updated_at` (TIMESTAMP)

#### Supporting Tables
- `interview_dates` - Multiple interview dates per application
- `followup_dates` - Multiple follow-up dates per application
- `resume_skills`, `resume_education`, `resume_experiences` - Resume components
- `job_required_skills`, `job_preferred_skills` - Job listing skills

## Setup Instructions

### Prerequisites
1. PostgreSQL 12+ installed and running
2. C++ compiler with C++11 support
3. PostgreSQL development headers (`libpq-dev` on Ubuntu, included with PostgreSQL on macOS)

### Database Setup
1. Run the setup script:
   ```bash
   ./setup_database.sh
   ```

2. Or manually create databases:
   ```bash
   createdb rezz_db -U postgres
   createdb rezz_test_db -U postgres
   psql -U postgres -d rezz_db -f src/db/schema.sql
   psql -U postgres -d rezz_test_db -f src/db/schema.sql
   ```

### Building Applications
```bash
# Build job tracker with database support
make job_tracker_db

# Build database tests
make test_database
make test_job_application_controller

# Build everything
make all
```

### Running Applications
```bash
# Run job tracker with database
make run_tracker_db

# Run database tests
make test_db
```

## Database Connection Configuration

The system uses the following default connection parameters:
- Host: localhost
- Port: 5432
- Database: rezz_db (production), rezz_test_db (testing)
- User: postgres
- Password: postgres (if required)

To modify connection parameters, update the connection string in the DatabaseConnection class or set environment variables.

## Controllers and CRUD Operations

### JobApplicationController

#### Create Operations
```cpp
JobApplicationController controller;
JobApplication app("APP_001", "JOB_001", "Software Engineer", "TechCorp", "2024-01-15");
bool success = controller.createJobApplication(app);
```

#### Read Operations
```cpp
// Get by ID
auto app = controller.getJobApplicationById("APP_001");

// Get all applications
auto allApps = controller.getAllJobApplications();

// Search by company
auto companyApps = controller.getJobApplicationsByCompany("TechCorp");

// Search by status
auto statusApps = controller.getJobApplicationsByStatus(ApplicationStatus::INTERVIEW_SCHEDULED);

// Search by date range
auto dateApps = controller.getJobApplicationsByDateRange("2024-01-01", "2024-01-31");
```

#### Update Operations
```cpp
// Update entire application
app->setStatus(ApplicationStatus::OFFER_RECEIVED);
bool success = controller.updateJobApplication(*app);

// Update just status
bool success = controller.updateJobApplicationStatus("APP_001", ApplicationStatus::REJECTED);

// Add interview date
bool success = controller.addInterviewDate("APP_001", "2024-02-15");
```

#### Delete Operations
```cpp
// Delete single application
bool success = controller.deleteJobApplication("APP_001");

// Delete all applications from a company
bool success = controller.deleteJobApplicationsByCompany("TechCorp");
```

#### Export Operations
```cpp
// Export to JSON
std::string jsonData = controller.exportToJson();

// Export to CSV
std::string csvData = controller.exportToCsv();
```

#### Statistics
```cpp
// Get total count
int total = controller.getJobApplicationCount();

// Get count by status
int applied = controller.getJobApplicationCountByStatus(ApplicationStatus::APPLIED);
```

## Testing

### Unit Tests
The system includes comprehensive unit tests for:
- Database connection and basic operations
- JobApplicationController CRUD operations
- Data integrity and constraint validation
- Export functionality
- Edge cases and error handling

### Running Tests
```bash
# Run all database tests
make test_db

# Run individual test components
make run_test_database
make run_test_job_application_controller
```

### Test Database
Tests use a separate `rezz_test_db` database to avoid interfering with production data. The test database is automatically cleaned up after test runs.

## Error Handling

The database system includes robust error handling:
- Connection failure detection and reporting
- SQL error reporting with detailed messages
- Transaction rollback on failure
- Graceful handling of constraint violations
- Memory management for PostgreSQL result sets

## Performance Considerations

### Indexes
The schema includes indexes on commonly queried fields:
- `job_applications.application_id`
- `job_applications.company`
- `job_applications.status`
- `job_applications.date_applied`

### Transactions
All multi-table operations use transactions to ensure data consistency:
- Creating applications with interview dates
- Updating applications with related data
- Deleting applications and dependencies

### Connection Pooling
The current implementation uses a singleton connection pattern. For high-load applications, consider implementing connection pooling.

## Data Migration

### From JSON Files
The system maintains backward compatibility with JSON file storage:
- JSON export maintains the same format as the original system
- Applications can be exported to JSON for backup or migration
- The original file-based tracker remains available

### Import/Export Formats

#### JSON Format
```json
{
  "applications": [
    {
      "applicationId": "APP_001",
      "jobTitle": "Software Engineer",
      "company": "TechCorp",
      "dateApplied": "2024-01-15",
      "status": "APPLIED",
      "interviewDates": ["2024-02-01"],
      "followUpDates": ["2024-02-10"]
    }
  ]
}
```

#### CSV Format
Comma-separated values with headers, suitable for Excel import:
```csv
Application ID,Job Title,Company,Date Applied,Status,Interview Dates,Follow Up Dates
APP_001,Software Engineer,TechCorp,2024-01-15,APPLIED,2024-02-01,2024-02-10
```

## Security Considerations

- SQL injection prevention through parameterized queries
- Input validation and sanitization
- No plaintext password storage (use PostgreSQL authentication)
- Database connection string protection
- Proper error message handling (no sensitive data exposure)

## Troubleshooting

### Common Issues

1. **Connection Refused**
   - Ensure PostgreSQL is running: `pg_isready`
   - Check connection parameters
   - Verify user permissions

2. **Database Does Not Exist**
   - Run setup script: `./setup_database.sh`
   - Manually create: `createdb rezz_db -U postgres`

3. **Compilation Errors**
   - Install PostgreSQL development headers
   - Check pg_config is in PATH
   - Verify libpq is installed

4. **Permission Denied**
   - Check PostgreSQL user permissions
   - Ensure database owner privileges
   - Verify authentication method in pg_hba.conf

### Debug Mode
Set environment variable for verbose output:
```bash
export REZZ_DEBUG=1
make run_tracker_db
```

## Future Enhancements

- Resume and JobListing controller implementations
- Connection pooling for better performance
- Database migration scripts
- Backup and restore utilities
- REST API for remote access
- Advanced search and filtering capabilities
- Data analytics and reporting features