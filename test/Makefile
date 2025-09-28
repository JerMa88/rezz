# Directories
SRC_DIR = ../src
TEST_DIR = .
BUILD_DIR = build

# Run all tests
test_all: test_ds_skills test_ds_education test_ds_experiences test_ds_projects test_ds_certifications test_ds_awards test_ds_languages test_ds_resume test_ds_jobListing
	@echo "Running all tests..."
	@echo "===================="
	@echo "Running Skills Tests:"
	./build/test_skills
	@echo ""
	@echo "Running Education Tests:"
	./build/test_education
	@echo ""
	@echo "Running Experiences Tests:"
	./build/test_experiences
	@echo ""
	@echo "Running Projects Tests:"
	./build/test_projects
	@echo ""
	@echo "Running Certifications Tests:"
	./build/test_certifications
	@echo ""
	@echo "Running Awards Tests:"
	./build/test_awards
	@echo ""
	@echo "Running Languages Tests:"
	./build/test_languages
	@echo ""
	@echo "Running Resume Tests:"
	./build/test_resume
	@echo ""
	@echo "Running JobListing Tests:"
	./build/test_jobListing
	@echo ""
	@echo "All tests completed!"

test_ds_skills:
	g++ -I../src -I. ./ds/resume/test_skills.cpp ../src/ds/resume/skills.cpp ../src/ds/resume/skill.cpp -o build/test_skills -lm -std=c++11

test_ds_education:
	g++ -I../src -I. ./ds/resume/test_education.cpp ../src/ds/resume/education.cpp -o build/test_education -lm -std=c++11

test_ds_experiences:
	g++ -I../src -I. ./ds/resume/test_experiences.cpp -o build/test_experiences -lm -std=c++11

test_ds_projects:
	g++ -I../src -I. ./ds/resume/test_projects.cpp ../src/ds/resume/skill.cpp -o build/test_projects -lm -std=c++11

test_ds_certifications:
	g++ -I../src -I. ./ds/resume/test_certifications.cpp -o build/test_certifications -lm -std=c++11

test_ds_awards:
	g++ -I../src -I. ./ds/resume/test_awards.cpp -o build/test_awards -lm -std=c++11

test_ds_languages:
	g++ -I../src -I. ./ds/resume/test_languages.cpp -o build/test_languages -lm -std=c++11

test_ds_resume:
	g++ -I../src -I. ./ds/resume/test_resume.cpp ../src/ds/resume/resume.cpp ../src/ds/resume/skills.cpp ../src/ds/resume/skill.cpp ../src/ds/resume/education.cpp -o build/test_resume -lm -std=c++11

test_ds_jobListing:
	g++ -I../src -I. ./ds/resume/test_jobListing.cpp ../src/ds/resume/jobListing.cpp ../src/ds/resume/skills.cpp ../src/ds/resume/skill.cpp -o build/test_jobListing -lm -std=c++11