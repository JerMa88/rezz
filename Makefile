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
		-o $(BUILD_DIR)/resume_builder

# Run the application
run: resume_builder
	./$(BUILD_DIR)/resume_builder

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)

# Run tests (delegate to test directory)
test:
	$(MAKE) -C test test_all

# Build everything
all: resume_builder

.PHONY: clean run test all