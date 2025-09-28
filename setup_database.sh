#!/bin/bash

# REZZ Database Setup Script
# This script sets up the PostgreSQL database for the REZZ application

echo "=== REZZ Database Setup ==="
echo ""

# Check if PostgreSQL is installed
if ! command -v psql &> /dev/null; then
    echo "Error: PostgreSQL is not installed or not in PATH"
    echo "Please install PostgreSQL first:"
    echo "  - macOS: brew install postgresql"
    echo "  - Ubuntu: sudo apt-get install postgresql postgresql-contrib"
    echo "  - CentOS/RHEL: sudo yum install postgresql-server postgresql-contrib"
    exit 1
fi

# Check if PostgreSQL is running
if ! pg_isready &> /dev/null; then
    echo "Error: PostgreSQL is not running"
    echo "Please start PostgreSQL first:"
    echo "  - macOS: brew services start postgresql"
    echo "  - Ubuntu: sudo systemctl start postgresql"
    echo "  - CentOS/RHEL: sudo systemctl start postgresql"
    exit 1
fi

echo "PostgreSQL is installed and running ✓"

# Database configuration
DB_NAME="rezz_db"
TEST_DB_NAME="rezz_test_db"
DB_USER="postgres"

echo ""
echo "Creating databases..."

# Create main database
echo "Creating database: $DB_NAME"
createdb $DB_NAME -U $DB_USER 2>/dev/null || echo "Database $DB_NAME already exists"

# Create test database
echo "Creating test database: $TEST_DB_NAME"
createdb $TEST_DB_NAME -U $DB_USER 2>/dev/null || echo "Database $TEST_DB_NAME already exists"

# Run schema creation on main database
echo ""
echo "Setting up schema for main database..."
if psql -U $DB_USER -d $DB_NAME -f src/db/schema.sql; then
    echo "Main database schema created successfully ✓"
else
    echo "Error: Failed to create main database schema"
    exit 1
fi

# Run schema creation on test database
echo ""
echo "Setting up schema for test database..."
if psql -U $DB_USER -d $TEST_DB_NAME -f src/db/schema.sql; then
    echo "Test database schema created successfully ✓"
else
    echo "Error: Failed to create test database schema"
    exit 1
fi

echo ""
echo "=== Database Setup Complete ==="
echo ""
echo "Main database: $DB_NAME"
echo "Test database: $TEST_DB_NAME"
echo "User: $DB_USER"
echo "Host: localhost"
echo "Port: 5432"
echo ""
echo "You can now build and run the REZZ applications:"
echo "  make job_tracker_db    # Build job tracker with database support"
echo "  make run_tracker_db    # Run job tracker with database"
echo "  make test_db           # Run database tests"
echo ""
echo "To connect to the database manually:"
echo "  psql -U $DB_USER -d $DB_NAME"