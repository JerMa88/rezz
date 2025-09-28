-- Database Schema for REZZ Job Application Tracking System
-- PostgreSQL Database

-- Create database (run this manually)
-- CREATE DATABASE rezz_db;
-- \c rezz_db;

-- Drop tables if they exist (for testing)
DROP TABLE IF EXISTS job_applications CASCADE;
DROP TABLE IF EXISTS job_listings CASCADE;
DROP TABLE IF EXISTS resumes CASCADE;

-- Resume table
CREATE TABLE resumes (
    id SERIAL PRIMARY KEY,
    name VARCHAR(255) NOT NULL,
    email VARCHAR(255) UNIQUE NOT NULL,
    city VARCHAR(255),
    phone VARCHAR(50),
    linkedin VARCHAR(500),
    website VARCHAR(500),
    interests TEXT,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Job Listings table
CREATE TABLE job_listings (
    id SERIAL PRIMARY KEY,
    job_id VARCHAR(100) UNIQUE NOT NULL,
    title VARCHAR(255) NOT NULL,
    company VARCHAR(255) NOT NULL,
    description TEXT,
    location VARCHAR(255),
    remote_type INTEGER DEFAULT 1, -- 1=ON_SITE, 2=REMOTE, 3=HYBRID
    job_type INTEGER DEFAULT 1, -- 1=FULL_TIME, 2=PART_TIME, 3=CONTRACT, 4=INTERNSHIP, 5=FREELANCE
    experience_level INTEGER DEFAULT 1, -- 1=ENTRY_LEVEL, 2=JUNIOR, 3=MID_LEVEL, 4=SENIOR, 5=LEAD, 6=EXECUTIVE
    salary_min DECIMAL(10,2),
    salary_max DECIMAL(10,2),
    salary_currency VARCHAR(10) DEFAULT 'USD',
    minimum_years_experience INTEGER DEFAULT 0,
    application_deadline DATE,
    posted_date DATE DEFAULT CURRENT_DATE,
    application_url VARCHAR(500),
    contact_email VARCHAR(255),
    company_size VARCHAR(100),
    industry VARCHAR(100),
    company_website VARCHAR(500),
    is_active BOOLEAN DEFAULT TRUE,
    department VARCHAR(100),
    reporting_to VARCHAR(255),
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Job Applications table
CREATE TABLE job_applications (
    id SERIAL PRIMARY KEY,
    application_id VARCHAR(100) UNIQUE NOT NULL,
    job_id VARCHAR(100),
    job_title VARCHAR(255) NOT NULL,
    company VARCHAR(255) NOT NULL,
    date_applied DATE NOT NULL,
    status INTEGER DEFAULT 1, -- 1=APPLIED, 2=REVIEWING, 3=INTERVIEW_SCHEDULED, etc.
    contact_name VARCHAR(255),
    contact_email VARCHAR(255),
    contact_phone VARCHAR(50),
    comments TEXT,
    application_url VARCHAR(500),
    salary_offered VARCHAR(100),
    expected_salary VARCHAR(100),
    response_deadline DATE,
    referral_source VARCHAR(255),
    application_method VARCHAR(255),
    notes TEXT,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (job_id) REFERENCES job_listings(job_id) ON DELETE SET NULL
);

-- Interview dates table (separate table for multiple interview dates)
CREATE TABLE interview_dates (
    id SERIAL PRIMARY KEY,
    application_id VARCHAR(100) NOT NULL,
    interview_date DATE NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (application_id) REFERENCES job_applications(application_id) ON DELETE CASCADE
);

-- Follow-up dates table (separate table for multiple follow-up dates)
CREATE TABLE followup_dates (
    id SERIAL PRIMARY KEY,
    application_id VARCHAR(100) NOT NULL,
    followup_date DATE NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (application_id) REFERENCES job_applications(application_id) ON DELETE CASCADE
);

-- Skills table for resumes
CREATE TABLE resume_skills (
    id SERIAL PRIMARY KEY,
    resume_id INTEGER NOT NULL,
    skill_name VARCHAR(255) NOT NULL,
    proficiency_level INTEGER DEFAULT 1, -- 1=BEGINNER, 2=INTERMEDIATE, 3=ADVANCED, 4=EXPERT
    years_experience INTEGER DEFAULT 0,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (resume_id) REFERENCES resumes(id) ON DELETE CASCADE
);

-- Education table for resumes
CREATE TABLE resume_education (
    id SERIAL PRIMARY KEY,
    resume_id INTEGER NOT NULL,
    institution VARCHAR(255) NOT NULL,
    degree VARCHAR(255),
    field_of_study VARCHAR(255),
    start_date DATE,
    end_date DATE,
    gpa DECIMAL(4,2),
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (resume_id) REFERENCES resumes(id) ON DELETE CASCADE
);

-- Experience table for resumes
CREATE TABLE resume_experiences (
    id SERIAL PRIMARY KEY,
    resume_id INTEGER NOT NULL,
    company VARCHAR(255) NOT NULL,
    position VARCHAR(255) NOT NULL,
    start_date DATE,
    end_date DATE,
    description TEXT,
    location VARCHAR(255),
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (resume_id) REFERENCES resumes(id) ON DELETE CASCADE
);

-- Required skills for job listings
CREATE TABLE job_required_skills (
    id SERIAL PRIMARY KEY,
    job_listing_id INTEGER NOT NULL,
    skill_name VARCHAR(255) NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (job_listing_id) REFERENCES job_listings(id) ON DELETE CASCADE
);

-- Preferred skills for job listings
CREATE TABLE job_preferred_skills (
    id SERIAL PRIMARY KEY,
    job_listing_id INTEGER NOT NULL,
    skill_name VARCHAR(255) NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (job_listing_id) REFERENCES job_listings(id) ON DELETE CASCADE
);

-- Create indexes for better performance
CREATE INDEX idx_resumes_email ON resumes(email);
CREATE INDEX idx_job_listings_job_id ON job_listings(job_id);
CREATE INDEX idx_job_listings_company ON job_listings(company);
CREATE INDEX idx_job_applications_application_id ON job_applications(application_id);
CREATE INDEX idx_job_applications_job_id ON job_applications(job_id);
CREATE INDEX idx_job_applications_company ON job_applications(company);
CREATE INDEX idx_job_applications_status ON job_applications(status);
CREATE INDEX idx_job_applications_date_applied ON job_applications(date_applied);
CREATE INDEX idx_interview_dates_application_id ON interview_dates(application_id);
CREATE INDEX idx_followup_dates_application_id ON followup_dates(application_id);

-- Create update triggers for updated_at timestamps
CREATE OR REPLACE FUNCTION update_updated_at_column()
RETURNS TRIGGER AS $$
BEGIN
    NEW.updated_at = CURRENT_TIMESTAMP;
    RETURN NEW;
END;
$$ language 'plpgsql';

CREATE TRIGGER update_resumes_updated_at BEFORE UPDATE ON resumes
    FOR EACH ROW EXECUTE FUNCTION update_updated_at_column();

CREATE TRIGGER update_job_listings_updated_at BEFORE UPDATE ON job_listings
    FOR EACH ROW EXECUTE FUNCTION update_updated_at_column();

CREATE TRIGGER update_job_applications_updated_at BEFORE UPDATE ON job_applications
    FOR EACH ROW EXECUTE FUNCTION update_updated_at_column();