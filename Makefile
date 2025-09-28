# Directories
SRC_DIR = src
BUILD_DIR = build

# PostgreSQL configuration
PG_CONFIG = pg_config
PG_INCLUDE = $(shell $(PG_CONFIG) --includedir 2>/dev/null || echo "/usr/include/postgresql")
PG_LIBDIR = $(shell $(PG_CONFIG) --libdir 2>/dev/null || echo "/usr/lib")
PG_LIBS = -lpq

# Compiler flags
CXXFLAGS = -std=c++11 -Wall -I$(SRC_DIR) -I$(PG_INCLUDE)
LDFLAGS = -L$(PG_LIBDIR) $(PG_LIBS)

# Create build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Build the resume builder CLI application
resume_builder: $(BUILD_DIR)
	g++ $(CXXFLAGS) \
		$(SRC_DIR)/resume_builder.cpp \
		$(SRC_DIR)/ds/resume/resume.cpp \
		$(SRC_DIR)/ds/resume/skills.cpp \
		$(SRC_DIR)/ds/resume/skill.cpp \
		$(SRC_DIR)/ds/resume/education.cpp \
		$(SRC_DIR)/ds/resume/jobListing.cpp \
		-o $(BUILD_DIR)/resume_builder

# Build the job application tracker (original version)
job_tracker: $(BUILD_DIR)
	g++ $(CXXFLAGS) \
		$(SRC_DIR)/jobApplicationTracker.cpp \
		$(SRC_DIR)/ds/resume/jobApplication.cpp \
		-o $(BUILD_DIR)/job_tracker

# Build the job application tracker with database support
job_tracker_db: $(BUILD_DIR)
	g++ $(CXXFLAGS) \
		$(SRC_DIR)/jobApplicationTrackerDB.cpp \
		$(SRC_DIR)/ds/resume/jobApplication.cpp \
		$(SRC_DIR)/db/database.cpp \
		$(SRC_DIR)/db/baseController.cpp \
		$(SRC_DIR)/db/jobApplicationController.cpp \
		$(LDFLAGS) \
		-o $(BUILD_DIR)/job_tracker_db

# Build the resume builder with database support
resume_builder_db: $(BUILD_DIR)
	g++ $(CXXFLAGS) \
		$(SRC_DIR)/resumeBuilderDB.cpp \
		$(SRC_DIR)/ds/resume/resume.cpp \
		$(SRC_DIR)/ds/resume/skills.cpp \
		$(SRC_DIR)/ds/resume/skill.cpp \
		$(SRC_DIR)/ds/resume/education.cpp \
		$(SRC_DIR)/db/database.cpp \
		$(SRC_DIR)/db/baseController.cpp \
		$(SRC_DIR)/db/resumeController.cpp \
		$(LDFLAGS) \
		-o $(BUILD_DIR)/resume_builder_db

# Build database tests
test_database: $(BUILD_DIR)
	g++ $(CXXFLAGS) \
		test/db/test_database.cpp \
		$(SRC_DIR)/db/database.cpp \
		$(LDFLAGS) \
		-o $(BUILD_DIR)/test_database

test_job_application_controller: $(BUILD_DIR)
	g++ $(CXXFLAGS) \
		test/db/test_jobApplicationController.cpp \
		$(SRC_DIR)/ds/resume/jobApplication.cpp \
		$(SRC_DIR)/db/database.cpp \
		$(SRC_DIR)/db/baseController.cpp \
		$(SRC_DIR)/db/jobApplicationController.cpp \
		$(LDFLAGS) \
		-o $(BUILD_DIR)/test_job_application_controller

# Run the resume builder application
run_resume: resume_builder
	./$(BUILD_DIR)/resume_builder

# Run the job tracker application (original)
run_tracker: job_tracker
	./$(BUILD_DIR)/job_tracker

# Run the job tracker application with database
run_tracker_db: job_tracker_db
	./$(BUILD_DIR)/job_tracker_db

# Run the resume builder application with database
run_resume_db: resume_builder_db
	./$(BUILD_DIR)/resume_builder_db

# Run database tests
run_test_database: test_database
	./$(BUILD_DIR)/test_database

run_test_job_application_controller: test_job_application_controller
	./$(BUILD_DIR)/test_job_application_controller

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)

# Run tests (delegate to test directory)
test:
	$(MAKE) -C test test_all

# Run database tests
test_db: test_database test_job_application_controller
	@echo "Running database tests..."
	@echo "========================="
	@echo "Running Database Connection Tests:"
	./$(BUILD_DIR)/test_database
	@echo ""
	@echo "Running JobApplication Controller Tests:"
	./$(BUILD_DIR)/test_job_application_controller
	@echo ""
	@echo "All database tests completed!"

# Build everything
all: resume_builder job_tracker resume_builder_db job_tracker_db

# Build all tests
all_tests: test_database test_job_application_controller
	$(MAKE) -C test test_all

.PHONY: clean run_resume run_tracker run_resume_db run_tracker_db test test_db all all_tests run_test_database run_test_job_application_controller