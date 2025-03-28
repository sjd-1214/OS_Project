 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <ctype.h>
 
 #define MAX_STUDENTS 20
 #define MAX_NAME_LENGTH 50
 #define MAX_PASSWORD_LENGTH 20
 #define DATA_FILE "student_data.txt"
 #define TEACHER_ID "teacher"
 #define TEACHER_PASSWORD "admin123"
 
 // Constants for grade calculation according to FAST's grading criteria
 #define A_GRADE_THRESHOLD 90
 #define A_MINUS_GRADE_THRESHOLD 85
 #define B_PLUS_GRADE_THRESHOLD 80
 #define B_GRADE_THRESHOLD 75
 #define B_MINUS_GRADE_THRESHOLD 70
 #define C_PLUS_GRADE_THRESHOLD 65
 #define C_GRADE_THRESHOLD 60
 #define C_MINUS_GRADE_THRESHOLD 55
 #define D_GRADE_THRESHOLD 50
 #define PASS_CGPA 2.0 // Assuming CGPA threshold for passing
 
 // Structure to hold course information
 typedef struct {
     char courseName[MAX_NAME_LENGTH];
     float marks;
     char grade[3]; // A, A-, B+, etc.
     float gradePoints;
 } Course;
 
 // Structure to hold student information
 typedef struct {
     int rollNo;
     char name[MAX_NAME_LENGTH];
     char password[MAX_PASSWORD_LENGTH];
     int totalCourses;
     Course courses[5]; // Assuming maximum 5 courses per student
     float cgpa;
     int isActive; // 1 if student record is active, 0 if deleted
 } Student;
 
 // Global array to store students
 Student students[MAX_STUDENTS];
 int studentCount = 0;
 
 // Function declarations
 void displayMainMenu();
 void displayTeacherMenu();
 void displayStudentMenu();
 int authenticateUser(char *userId, char *password);
 void addStudent();
 void viewStudentDetails();
 void updateStudentInfo();
 void deleteStudent();
 void calculateGrade(Course *course);
 float calculateCGPA(Student *student);
 void generateReport(int sortOrder);
 void listPassedStudents();
 void listFailedStudents();
 void saveData();
 void loadData();
 void assignMarks();
 void viewStudentGrades(int studentIndex);
 void clearScreen();
 
 /**
  * Main function - Entry point of the program
  */
 int main() {
     int choice;
     char userId[MAX_NAME_LENGTH], password[MAX_PASSWORD_LENGTH];
     int userType = 0; // 1 for teacher, 2 for student, 0 for not authenticated
     int studentIndex = -1;
     
     // Load existing data from file
     loadData();
     
     while (1) {
         if (userType == 0) {
             clearScreen();
             displayMainMenu();
             printf("Enter your choice: ");
             scanf("%d", &choice);
             getchar(); // Consume the newline character
             
             switch (choice) {
                 case 1: // Login
                     clearScreen();
                     printf("===== LOGIN =====\n");
                     printf("Enter User ID: ");
                     scanf("%s", userId);
                     printf("Enter Password: ");
                     scanf("%s", password);
                     
                     userType = authenticateUser(userId, password);
                     
                     if (userType == 0) {
                         printf("Invalid credentials! Press Enter to continue...");
                         getchar();
                         getchar();
                     } else if (userType == 2) {
                         // Find student index
                         int rollNo = atoi(userId);
                         for (int i = 0; i < studentCount; i++) {
                             if (students[i].rollNo == rollNo && students[i].isActive) {
                                 studentIndex = i;
                                 break;
                             }
                         }
                     }
                     break;
                     
                 case 2: // Exit
                     printf("Thank you for using Student Management System!\n");
                     return 0;
                     
                 default:
                     printf("Invalid choice! Press Enter to continue...");
                     getchar();
                     getchar();
             }
         } else if (userType == 1) { // Teacher menu
             clearScreen();
             displayTeacherMenu();
             printf("Enter your choice: ");
             scanf("%d", &choice);
             getchar(); // Consume the newline character
             
             switch (choice) {
                 case 1: // Add student
                     addStudent();
                     break;
                     
                 case 2: // View student details
                     viewStudentDetails();
                     break;
                     
                 case 3: // Update student information
                     updateStudentInfo();
                     break;
                     
                 case 4: // Delete student
                     deleteStudent();
                     break;
                     
                 case 5: // Assign marks
                     assignMarks();
                     break;
                     
                 case 6: // Generate report (ascending)
                     generateReport(1);
                     break;
                     
                 case 7: // Generate report (descending)
                     generateReport(2);
                     break;
                     
                 case 8: // List passed students
                     listPassedStudents();
                     break;
                     
                 case 9: // List failed students
                     listFailedStudents();
                     break;
                     
                 case 10: // Save data
                     saveData();
                     printf("Data saved successfully! Press Enter to continue...");
                     getchar();
                     break;
                     
                 case 11: // Logout
                     userType = 0;
                     printf("Logged out successfully! Press Enter to continue...");
                     getchar();
                     break;
                     
                 default:
                     printf("Invalid choice! Press Enter to continue...");
                     getchar();
             }
         } else if (userType == 2 && studentIndex != -1) { // Student menu
             clearScreen();
             displayStudentMenu();
             printf("Enter your choice: ");
             scanf("%d", &choice);
             getchar(); // Consume the newline character
             
             switch (choice) {
                 case 1: // View grades and CGPA
                     viewStudentGrades(studentIndex);
                     break;
                     
                 case 2: // Logout
                     userType = 0;
                     studentIndex = -1;
                     printf("Logged out successfully! Press Enter to continue...");
                     getchar();
                     break;
                     
                 default:
                     printf("Invalid choice! Press Enter to continue...");
                     getchar();
             }
         }
     }
     
     return 0;
 }
 
 /**
  * Displays the main menu of the Student Management System
  */
 void displayMainMenu() {
     printf("====================================\n");
     printf("    STUDENT MANAGEMENT SYSTEM       \n");
     printf("====================================\n");
     printf("1. Login\n");
     printf("2. Exit\n");
     printf("====================================\n");
 }
 
 /**
  * Displays the teacher menu with all available operations
  */
 void displayTeacherMenu() {
     printf("====================================\n");
     printf("         TEACHER MENU               \n");
     printf("====================================\n");
     printf("1. Add Student\n");
     printf("2. View Student Details\n");
     printf("3. Update Student Information\n");
     printf("4. Delete Student\n");
     printf("5. Assign Marks\n");
     printf("6. Generate Report (Ascending order by CGPA)\n");
     printf("7. Generate Report (Descending order by CGPA)\n");
     printf("8. List Passed Students\n");
     printf("9. List Failed Students\n");
     printf("10. Save Data\n");
     printf("11. Logout\n");
     printf("====================================\n");
 }
 
 /**
  * Displays the student menu with available operations for students
  */
 void displayStudentMenu() {
     printf("====================================\n");
     printf("         STUDENT MENU               \n");
     printf("====================================\n");
     printf("1. View Grades and CGPA\n");
     printf("2. Logout\n");
     printf("====================================\n");
 }
 
 /**
  * Authenticates a user based on ID and password
  * 
  * @param userId The user ID entered
  * @param password The password entered
  * @return 1 if teacher, 2 if student, 0 if invalid credentials
  */
 int authenticateUser(char *userId, char *password) {
     // Check if teacher credentials
     if (strcmp(userId, TEACHER_ID) == 0 && strcmp(password, TEACHER_PASSWORD) == 0) {
         return 1; // Teacher
     }
     
     // Check if student credentials
     int rollNo = atoi(userId);
     for (int i = 0; i < studentCount; i++) {
         if (students[i].rollNo == rollNo && strcmp(students[i].password, password) == 0 && students[i].isActive) {
             return 2; // Student
         }
     }
     
     return 0; // Invalid credentials
 }
 
 /**
  * Adds a new student to the system
  */
 void addStudent() {
     if (studentCount >= MAX_STUDENTS) {
         printf("Maximum student limit reached! Press Enter to continue...");
         getchar();
         return;
     }
     
     Student newStudent;
     newStudent.isActive = 1;
     
     clearScreen();
     printf("====================================\n");
     printf("          ADD NEW STUDENT           \n");
     printf("====================================\n");
     
     printf("Enter Roll Number: ");
     scanf("%d", &newStudent.rollNo);
     getchar(); // Consume the newline character
     
     // Check if roll number already exists
     for (int i = 0; i < studentCount; i++) {
         if (students[i].rollNo == newStudent.rollNo && students[i].isActive) {
             printf("Student with this Roll Number already exists! Press Enter to continue...");
             getchar();
             return;
         }
     }
     
     printf("Enter Name: ");
     fgets(newStudent.name, MAX_NAME_LENGTH, stdin);
     newStudent.name[strcspn(newStudent.name, "\n")] = 0; // Remove the newline character
     
     printf("Enter Password: ");
     scanf("%s", newStudent.password);
     
     printf("Enter Number of Courses (max 5): ");
     scanf("%d", &newStudent.totalCourses);
     getchar(); // Consume the newline character
     
     if (newStudent.totalCourses > 5) {
         newStudent.totalCourses = 5;
         printf("Maximum 5 courses allowed. Setting to 5.\n");
     }
     
     // Initialize courses with zero marks
     for (int i = 0; i < newStudent.totalCourses; i++) {
         printf("Enter Course %d Name: ", i + 1);
         fgets(newStudent.courses[i].courseName, MAX_NAME_LENGTH, stdin);
         newStudent.courses[i].courseName[strcspn(newStudent.courses[i].courseName, "\n")] = 0; // Remove the newline character
         
         newStudent.courses[i].marks = 0.0;
         strcpy(newStudent.courses[i].grade, "NA");
         newStudent.courses[i].gradePoints = 0.0;
     }
     
     newStudent.cgpa = 0.0;
     
     // Add the new student to the array
     students[studentCount] = newStudent;
     studentCount++;
     
     printf("Student added successfully! Press Enter to continue...");
     getchar();
     
     // Save data to file
     saveData();
 }
 
 /**
  * View details of a specific student by roll number
  */
 void viewStudentDetails() {
     int rollNo;
     int found = 0;
     
     clearScreen();
     printf("====================================\n");
     printf("        VIEW STUDENT DETAILS        \n");
     printf("====================================\n");
     
     printf("Enter Student Roll Number: ");
     scanf("%d", &rollNo);
     getchar(); // Consume the newline character
     
     for (int i = 0; i < studentCount; i++) {
         if (students[i].rollNo == rollNo && students[i].isActive) {
             found = 1;
             
             printf("\nRoll Number: %d\n", students[i].rollNo);
             printf("Name: %s\n", students[i].name);
             printf("CGPA: %.2f\n", students[i].cgpa);
             printf("Courses: %d\n", students[i].totalCourses);
             
             printf("\n%-20s %-10s %-5s %-10s\n", "Course Name", "Marks", "Grade", "Grade Points");
             printf("----------------------------------------------------------\n");
             
             for (int j = 0; j < students[i].totalCourses; j++) {
                 printf("%-20s %-10.2f %-5s %-10.2f\n",
                        students[i].courses[j].courseName,
                        students[i].courses[j].marks,
                        students[i].courses[j].grade,
                        students[i].courses[j].gradePoints);
             }
             
             break;
         }
     }
     
     if (!found) {
         printf("Student not found! Press Enter to continue...");
     } else {
         printf("\nPress Enter to continue...");
     }
     
     getchar();
 }
 
 /**
  * Updates student information such as marks
  */
 void updateStudentInfo() {
     int rollNo;
     int found = 0;
     
     clearScreen();
     printf("====================================\n");
     printf("    UPDATE STUDENT INFORMATION      \n");
     printf("====================================\n");
     
     printf("Enter Student Roll Number: ");
     scanf("%d", &rollNo);
     getchar(); // Consume the newline character
     
     for (int i = 0; i < studentCount; i++) {
         if (students[i].rollNo == rollNo && students[i].isActive) {
             found = 1;
             
             printf("\nStudent Found:\n");
             printf("Roll Number: %d\n", students[i].rollNo);
             printf("Name: %s\n", students[i].name);
             printf("\nWhat would you like to update?\n");
             printf("1. Name\n");
             printf("2. Password\n");
             printf("3. Course Marks\n");
             printf("Enter choice: ");
             
             int updateChoice;
             scanf("%d", &updateChoice);
             getchar(); // Consume the newline character
             
             switch (updateChoice) {
                 case 1: // Update name
                     printf("Enter new name: ");
                     fgets(students[i].name, MAX_NAME_LENGTH, stdin);
                     students[i].name[strcspn(students[i].name, "\n")] = 0; // Remove the newline character
                     printf("Name updated successfully!\n");
                     break;
                     
                 case 2: // Update password
                     printf("Enter new password: ");
                     scanf("%s", students[i].password);
                     printf("Password updated successfully!\n");
                     break;
                     
                 case 3: // Update course marks
                     if (students[i].totalCourses > 0) {
                         printf("\nSelect course to update marks:\n");
                         
                         for (int j = 0; j < students[i].totalCourses; j++) {
                             printf("%d. %s\n", j + 1, students[i].courses[j].courseName);
                         }
                         
                         printf("Enter choice: ");
                         int courseChoice;
                         scanf("%d", &courseChoice);
                         
                         if (courseChoice >= 1 && courseChoice <= students[i].totalCourses) {
                             printf("Enter new marks for %s: ", students[i].courses[courseChoice - 1].courseName);
                             float newMarks;
                             scanf("%f", &newMarks);
                             
                             if (newMarks >= 0 && newMarks <= 100) {
                                 students[i].courses[courseChoice - 1].marks = newMarks;
                                 
                                 // Recalculate grade and grade points
                                 calculateGrade(&students[i].courses[courseChoice - 1]);
                                 
                                 // Recalculate CGPA
                                 students[i].cgpa = calculateCGPA(&students[i]);
                                 
                                 printf("Marks updated successfully!\n");
                             } else {
                                 printf("Invalid marks! Marks should be between 0 and 100.\n");
                             }
                         } else {
                             printf("Invalid course selection!\n");
                         }
                     } else {
                         printf("No courses available to update!\n");
                     }
                     break;
                     
                 default:
                     printf("Invalid choice!\n");
             }
             
             break;
         }
     }
     
     if (!found) {
         printf("Student not found! Press Enter to continue...");
     } else {
         printf("\nPress Enter to continue...");
         
         // Save data to file
         saveData();
     }
     
     getchar();
 }
 
 /**
  * Deletes a student record from the system
  */
 void deleteStudent() {
     int rollNo;
     int found = 0;
     
     clearScreen();
     printf("====================================\n");
     printf("          DELETE STUDENT            \n");
     printf("====================================\n");
     
     printf("Enter Student Roll Number to delete: ");
     scanf("%d", &rollNo);
     getchar(); // Consume the newline character
     
     for (int i = 0; i < studentCount; i++) {
         if (students[i].rollNo == rollNo && students[i].isActive) {
             found = 1;
             
             printf("\nStudent Found:\n");
             printf("Roll Number: %d\n", students[i].rollNo);
             printf("Name: %s\n", students[i].name);
             printf("\nAre you sure you want to delete this student? (Y/N): ");
             
             char confirmation;
             scanf("%c", &confirmation);
             getchar(); // Consume the newline character
             
             if (toupper(confirmation) == 'Y') {
                 students[i].isActive = 0; // Mark as deleted
                 printf("Student deleted successfully!\n");
                 
                 // Save data to file
                 saveData();
             } else {
                 printf("Deletion cancelled.\n");
             }
             
             break;
         }
     }
     
     if (!found) {
         printf("Student not found! Press Enter to continue...");
     } else {
         printf("\nPress Enter to continue...");
     }
     
     getchar();
 }
 
 /**
  * Calculates grade and grade points based on marks
  * 
  * @param course Pointer to the course structure
  */
 void calculateGrade(Course *course) {
     float marks = course->marks;
     
     if (marks >= A_GRADE_THRESHOLD) {
         strcpy(course->grade, "A");
         course->gradePoints = 4.0;
     } else if (marks >= A_MINUS_GRADE_THRESHOLD) {
         strcpy(course->grade, "A-");
         course->gradePoints = 3.7;
     } else if (marks >= B_PLUS_GRADE_THRESHOLD) {
         strcpy(course->grade, "B+");
         course->gradePoints = 3.3;
     } else if (marks >= B_GRADE_THRESHOLD) {
         strcpy(course->grade, "B");
         course->gradePoints = 3.0;
     } else if (marks >= B_MINUS_GRADE_THRESHOLD) {
         strcpy(course->grade, "B-");
         course->gradePoints = 2.7;
     } else if (marks >= C_PLUS_GRADE_THRESHOLD) {
         strcpy(course->grade, "C+");
         course->gradePoints = 2.3;
     } else if (marks >= C_GRADE_THRESHOLD) {
         strcpy(course->grade, "C");
         course->gradePoints = 2.0;
     } else if (marks >= C_MINUS_GRADE_THRESHOLD) {
         strcpy(course->grade, "C-");
         course->gradePoints = 1.7;
     } else if (marks >= D_GRADE_THRESHOLD) {
         strcpy(course->grade, "D");
         course->gradePoints = 1.0;
     } else {
         strcpy(course->grade, "F");
         course->gradePoints = 0.0;
     }
 }
 
 /**
  * Calculates CGPA based on all course grade points
  * 
  * @param student Pointer to the student structure
  * @return The calculated CGPA
  */
 float calculateCGPA(Student *student) {
     if (student->totalCourses == 0) {
         return 0.0;
     }
     
     float totalGradePoints = 0.0;
     
     for (int i = 0; i < student->totalCourses; i++) {
         totalGradePoints += student->courses[i].gradePoints;
     }
     
     return totalGradePoints / student->totalCourses;
 }
 
 /**
  * Assigns marks to students for their courses
  */
 void assignMarks() {
     int rollNo;
     int found = 0;
     
     clearScreen();
     printf("====================================\n");
     printf("            ASSIGN MARKS            \n");
     printf("====================================\n");
     
     printf("Enter Student Roll Number: ");
     scanf("%d", &rollNo);
     getchar(); // Consume the newline character
     
     for (int i = 0; i < studentCount; i++) {
         if (students[i].rollNo == rollNo && students[i].isActive) {
             found = 1;
             
             printf("\nAssigning marks for %s (Roll No: %d)\n", students[i].name, students[i].rollNo);
             
             for (int j = 0; j < students[i].totalCourses; j++) {
                 printf("Enter marks for %s (0-100): ", students[i].courses[j].courseName);
                 float marks;
                 scanf("%f", &marks);
                 
                 if (marks >= 0 && marks <= 100) {
                     students[i].courses[j].marks = marks;
                     calculateGrade(&students[i].courses[j]);
                 } else {
                     printf("Invalid marks! Marks should be between 0 and 100. Setting to 0.\n");
                     students[i].courses[j].marks = 0;
                     strcpy(students[i].courses[j].grade, "F");
                     students[i].courses[j].gradePoints = 0.0;
                 }
             }
             
             // Calculate CGPA
             students[i].cgpa = calculateCGPA(&students[i]);
             
             printf("\nMarks assigned successfully! CGPA: %.2f\n", students[i].cgpa);
             
             // Save data to file
             saveData();
             
             break;
         }
     }
     
     if (!found) {
         printf("Student not found! Press Enter to continue...");
     } else {
         printf("\nPress Enter to continue...");
     }
     
     getchar();
     getchar(); // Consume an additional newline character
 }
 
 /**
  * Generates a report of all students in tabular format
  * 
  * @param sortOrder 1 for ascending order by CGPA, 2 for descending
  */
 void generateReport(int sortOrder) {
     clearScreen();
     printf("====================================\n");
     printf("          STUDENT REPORT            \n");
     printf("====================================\n");
     
     // Count active students
     int activeCount = 0;
     for (int i = 0; i < studentCount; i++) {
         if (students[i].isActive) {
             activeCount++;
         }
     }
     
     if (activeCount == 0) {
         printf("No students found! Press Enter to continue...");
         getchar();
         return;
     }
     
     // Create a temporary array for sorting
     Student tempStudents[MAX_STUDENTS];
     int index = 0;
     
     // Copy active students to temp array
     for (int i = 0; i < studentCount; i++) {
         if (students[i].isActive) {
             tempStudents[index++] = students[i];
         }
     }
     
     // Sort by CGPA
     for (int i = 0; i < activeCount - 1; i++) {
         for (int j = 0; j < activeCount - i - 1; j++) {
             if ((sortOrder == 1 && tempStudents[j].cgpa > tempStudents[j + 1].cgpa) || 
                 (sortOrder == 2 && tempStudents[j].cgpa < tempStudents[j + 1].cgpa)) {
                 Student temp = tempStudents[j];
                 tempStudents[j] = tempStudents[j + 1];
                 tempStudents[j + 1] = temp;
             }
         }
     }
     
     // Print report header
     printf("%-10s %-20s %-10s %-15s\n", "Roll No", "Name", "CGPA", "Status");
     printf("----------------------------------------------------\n");
     
     // Print sorted student data
     for (int i = 0; i < activeCount; i++) {
         char status[10] = "PASS";
         if (tempStudents[i].cgpa < PASS_CGPA) {
             strcpy(status, "FAIL");
         }
         
         printf("%-10d %-20s %-10.2f %-15s\n",
                tempStudents[i].rollNo,
                tempStudents[i].name,
                tempStudents[i].cgpa,
                status);
     }
     
     printf("\nPress Enter to continue...");
     getchar();
 }
 
 /**
  * Lists all students who have passed (CGPA >= threshold)
  */
 void listPassedStudents() {
     clearScreen();
     printf("====================================\n");
     printf("         PASSED STUDENTS            \n");
     printf("====================================\n");
     
     // Count passed students
     int passedCount = 0;
     for (int i = 0; i < studentCount; i++) {
         if (students[i].isActive && students[i].cgpa >= PASS_CGPA) {
             passedCount++;
         }
     }
     
     if (passedCount == 0) {
         printf("No passed students found! Press Enter to continue...");
         getchar();
         return;
     }
     
     // Print report header
     printf("%-10s %-20s %-10s\n", "Roll No", "Name", "CGPA");
     printf("----------------------------------------\n");
     
     // Print passed student data
     for (int i = 0; i < studentCount; i++) {
         if (students[i].isActive && students[i].cgpa >= PASS_CGPA) {
             printf("%-10d %-20s %-10.2f\n",
                    students[i].rollNo,
                    students[i].name,
                    students[i].cgpa);
         }
     }
     
     printf("\nPress Enter to continue...");
     getchar();
 }
 
 /**
  * Lists all students who have failed (CGPA < threshold)
  */
 void listFailedStudents() {
     clearScreen();
     printf("====================================\n");
     printf("         FAILED STUDENTS            \n");
     printf("====================================\n");
     
     // Count failed students
     int failedCount = 0;
     for (int i = 0; i < studentCount; i++) {
         if (students[i].isActive && students[i].cgpa < PASS_CGPA) {
             failedCount++;
         }
     }
     
     if (failedCount == 0) {
         printf("No failed students found! Press Enter to continue...");
         getchar();
         return;
     }
     
     // Print report header
     printf("%-10s %-20s %-10s\n", "Roll No", "Name", "CGPA");
     printf("----------------------------------------\n");
     
     // Print failed student data
     for (int i = 0; i < studentCount; i++) {
         if (students[i].isActive && students[i].cgpa < PASS_CGPA) {
             printf("%-10d %-20s %-10.2f\n",
                    students[i].rollNo,
                    students[i].name,
                    students[i].cgpa);
         }
     }
     
     printf("\nPress Enter to continue...");
     getchar();
 }
 
 /**
  * Saves student data to a file
  */
 void saveData() {
     FILE *file = fopen(DATA_FILE, "w");
     
     if (file == NULL) {
         printf("Error opening file for saving data!\n");
         return;
     }
     
     // Write the number of students first
     fprintf(file, "%d\n", studentCount);
     
     // Write each student's data
     for (int i = 0; i < studentCount; i++) {
         fprintf(file, "%d\n", students[i].rollNo);
         fprintf(file, "%s\n", students[i].name);
         fprintf(file, "%s\n", students[i].password);
         fprintf(file, "%d\n", students[i].totalCourses);
         fprintf(file, "%.2f\n", students[i].cgpa);
         fprintf(file, "%d\n", students[i].isActive);
         
         // Write course data
         for (int j = 0; j < students[i].totalCourses; j++) {
             fprintf(file, "%s\n", students[i].courses[j].courseName);
             fprintf(file, "%.2f\n", students[i].courses[j].marks);
             fprintf(file, "%s\n", students[i].courses[j].grade);
             fprintf(file, "%.2f\n", students[i].courses[j].gradePoints);
         }
     }
     
     fclose(file);
 }
 
 /**
  * Loads student data from a file
  */
 void loadData() {
     FILE *file = fopen(DATA_FILE, "r");
     
     if (file == NULL) {
         // File doesn't exist yet, which is fine for the first run
         return;
     }
     
     // Read the number of students
     if (fscanf(file, "%d", &studentCount) != 1) {
         fclose(file);
         return;
     }
     
     char buffer[100]; // Buffer to consume newline character
     fgets(buffer, sizeof(buffer), file); // Consume newline
     
     // Read each student's data
     for (int i = 0; i < studentCount; i++) {
         fscanf(file, "%d", &students[i].rollNo);
         fgets(buffer, sizeof(buffer), file); // Consume newline
         
         fgets(students[i].name, MAX_NAME_LENGTH, file);
         students[i].name[strcspn(students[i].name, "\n")] = 0; // Remove newline
         
         fgets(students[i].password, MAX_PASSWORD_LENGTH, file);
         students[i].password[strcspn(students[i].password, "\n")] = 0; // Remove newline
         
         fscanf(file, "%d", &students[i].totalCourses);
         fgets(buffer, sizeof(buffer), file); // Consume newline
         
         fscanf(file, "%f", &students[i].cgpa);
         fgets(buffer, sizeof(buffer), file); // Consume newline
         
         fscanf(file, "%d", &students[i].isActive);
         fgets(buffer, sizeof(buffer), file); // Consume newline
         
         // Read course data
         for (int j = 0; j < students[i].totalCourses; j++) {
             fgets(students[i].courses[j].courseName, MAX_NAME_LENGTH, file);
             students[i].courses[j].courseName[strcspn(students[i].courses[j].courseName, "\n")] = 0; // Remove newline
             
             fscanf(file, "%f", &students[i].courses[j].marks);
             fgets(buffer, sizeof(buffer), file); // Consume newline
             
             fgets(students[i].courses[j].grade, 3, file);
             students[i].courses[j].grade[strcspn(students[i].courses[j].grade, "\n")] = 0; // Remove newline
             
             fscanf(file, "%f", &students[i].courses[j].gradePoints);
             fgets(buffer, sizeof(buffer), file); // Consume newline
         }
     }
     
     fclose(file);
 }
 
 /**
  * Student viewing their own grades and CGPA
  * 
  * @param studentIndex The index of the student in the students array
  */
 void viewStudentGrades(int studentIndex) {
     clearScreen();
     printf("====================================\n");
     printf("        YOUR GRADES AND CGPA        \n");
     printf("====================================\n");
     
     printf("Name: %s\n", students[studentIndex].name);
     printf("Roll Number: %d\n", students[studentIndex].rollNo);
     printf("CGPA: %.2f\n", students[studentIndex].cgpa);
     
     printf("\n%-20s %-10s %-5s\n", "Course Name", "Marks", "Grade");
     printf("---------------------------------------\n");
     
     for (int i = 0; i < students[studentIndex].totalCourses; i++) {
         printf("%-20s %-10.2f %-5s\n",
                students[studentIndex].courses[i].courseName,
                students[studentIndex].courses[i].marks,
                students[studentIndex].courses[i].grade);
     }
     
     printf("\nPress Enter to continue...");
     getchar();
 }
 
 /**
  * Clears the console screen
  */
 void clearScreen() {
     #ifdef _WIN32
         system("cls");
     #else
         system("clear");
     #endif
 }