# Directories
SRC_DIR = src
BUILD_DIR = build

# Create build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Build the resume builder CLI application
resume_builder: $(BUILD_DIR)
	g++ -I$(SRC_DIR) -std=c++11 \
		$(SRC_DIR)/resume_builder.cpp \
		$(SRC_DIR)/ds/resume/resume.cpp \
		$(SRC_DIR)/ds/resume/skills.cpp \
		$(SRC_DIR)/ds/resume/skill.cpp \
		$(SRC_DIR)/ds/resume/education.cpp \
		$(SRC_DIR)/ds/resume/jobListing.cpp \
		-o $(BUILD_DIR)/resume_builder

# Build the job application tracker
job_tracker: $(BUILD_DIR)
	g++ -I$(SRC_DIR) -std=c++11 \
		$(SRC_DIR)/jobApplicationTracker.cpp \
		$(SRC_DIR)/ds/resume/jobApplication.cpp \
		-o $(BUILD_DIR)/job_tracker

# Run the resume builder application
run_resume: resume_builder
	./$(BUILD_DIR)/resume_builder

# Run the job tracker application
run_tracker: job_tracker
	./$(BUILD_DIR)/job_tracker

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)

# Run tests (delegate to test directory)
test:
	$(MAKE) -C test test_all

# Build everything
all: resume_builder job_tracker

.PHONY: clean run_resume run_tracker test all