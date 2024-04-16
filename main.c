#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**
 * @struct COURSE
 * @brief A structure to represent a course.
 * @var COURSE::course_Name
 * Member 'course_Name' represents the name of the course.
 * @var COURSE::course_Sched
 * Member 'course_Sched' represents the schedule of the course.
 * @var COURSE::course_Size
 * Member 'course_Size' represents the size of the course.
 * @var COURSE::course_Hours
 * Member 'course_Hours' represents the hours of the course.
 * @var COURSE::padding
 * Member 'padding' is used for padding.
 */
typedef struct {
    char course_Name[84];
    char course_Sched[4];
    unsigned course_Size;
    unsigned course_Hours;
    char padding[20];
} COURSE;

/**
 * @brief Function to display the menu to the user.
 */
void display_menu() {
    printf("Enter one of the following actions or press CTRL-D to exit.\n");
    printf("C - create a new course record\n");
    printf("U - update an existing course record\n");
    printf("R - read an existing course record\n");
    printf("D - delete an existing course record\n");
}

/**
 * @brief Function to create a new course.
 * @param fp The file pointer to the file where the course data is stored.
 */

void create_course(FILE *fp) {
    COURSE course;
    int courseNumber;

    // Prompt for all course details first
    printf("Course number: ");
    scanf("%d", &courseNumber);
    while (getchar() != '\n');  // Clear input buffer

    printf("Course name: ");
    fgets(course.course_Name, sizeof(course.course_Name), stdin);
    course.course_Name[strcspn(course.course_Name, "\n")] = 0; // Remove newline character

    printf("Course schedule (MWF or TR): ");
    scanf("%s", course.course_Sched);
    while (getchar() != '\n');  // Clear input buffer

    printf("Course credit hours: ");
    scanf("%u", &course.course_Hours);

    printf("Course enrollment: ");
    scanf("%u", &course.course_Size);

    memset(course.padding, 0, sizeof(course.padding)); // Clear padding

    // Check if course already exists
    fseek(fp, courseNumber * sizeof(COURSE), SEEK_SET);
    COURSE existingCourse;
    if (fread(&existingCourse, sizeof(COURSE), 1, fp) == 1 && existingCourse.course_Hours != 0) {
        printf("ERROR: course already exists \n");
        return;
    }

    // If course does not exist, write the new data
    fseek(fp, courseNumber * sizeof(COURSE), SEEK_SET);
    fwrite(&course, sizeof(COURSE), 1, fp);
    printf("Course created successfully.\n");
}

/**
 * @brief Function to update an existing course.
 * @param fp The file pointer to the file where the course data is stored.
 */

void update_course(FILE *fp) {
    COURSE course;
    int courseNumber;
    char inputBuffer[100]; // Buffer to handle input

    printf("Course number: ");
    scanf("%d", &courseNumber);
    while (getchar() != '\n'); // Clear the input buffer

    // Attempt to fetch existing course
    fseek(fp, courseNumber * sizeof(COURSE), SEEK_SET);
    if (fread(&course, sizeof(COURSE), 1, fp) != 1 || course.course_Hours == 0) {
        printf("ERROR: course not found\n");
        return;
    }

    // Update Course Name
    printf("Course name (press ENTER to keep current): ");
    fgets(inputBuffer, sizeof(inputBuffer), stdin);
    if (strcspn(inputBuffer, "\n") > 0) {
        inputBuffer[strcspn(inputBuffer, "\n")] = 0; // Remove newline character
        strncpy(course.course_Name, inputBuffer, sizeof(course.course_Name) - 1);
        course.course_Name[sizeof(course.course_Name) - 1] = '\0'; // Ensure null termination
    }

    // Update Course Schedule
    printf("Course schedule (MWF or TR, press ENTER to keep current): ");
    fgets(inputBuffer, sizeof(inputBuffer), stdin);
    if (strcspn(inputBuffer, "\n") > 0) {
        inputBuffer[strcspn(inputBuffer, "\n")] = 0; // Remove newline character
        strncpy(course.course_Sched, inputBuffer, sizeof(course.course_Sched) - 1);
        course.course_Sched[sizeof(course.course_Sched) - 1] = '\0'; // Ensure null termination
    }

    // Update Course Credit Hours
    printf("Course credit hours (press ENTER to keep current): ");
    fgets(inputBuffer, sizeof(inputBuffer), stdin);
    if (strcspn(inputBuffer, "\n") > 0) {
        course.course_Hours = (unsigned)atoi(inputBuffer);
    }

    // Update Course Enrollment
    printf("Course enrollment (press ENTER to keep current): ");
    fgets(inputBuffer, sizeof(inputBuffer), stdin);
    if (strcspn(inputBuffer, "\n") > 0) {
        course.course_Size = (unsigned)atoi(inputBuffer);
    }

    // Write the updated record back to the file
    fseek(fp, courseNumber * sizeof(COURSE), SEEK_SET);
    fwrite(&course, sizeof(COURSE), 1, fp);
    printf("Course updated successfully.\n");
}

/**
 * @brief Function to read an existing course.
 * @param fp The file pointer to the file where the course data is stored.
 */
void read_course(FILE *fp) {
    COURSE course;
    int courseNumber;

    // Prompt for course number
    printf("Enter a CS course number: ");
    scanf("%d", &courseNumber);
    while (getchar() != '\n'); // Clear the input buffer

    // Locate the course in the file
    fseek(fp, courseNumber * sizeof(COURSE), SEEK_SET);
    if (fread(&course, sizeof(COURSE), 1, fp) != 1 || course.course_Hours == 0) {
        printf("ERROR: course not found\n");
        return;
    }

    // Display the course information
    printf("Course number: %d\n", courseNumber);
    printf("Course name: %s\n", course.course_Name);
    printf("Scheduled days: %s\n", course.course_Sched);
    printf("Credit hours: %u\n", course.course_Hours);
    printf("Enrolled Students: %u\n", course.course_Size);
}

/**
 * @brief Function to delete an existing course.
 * @param fp The file pointer to the file where the course data is stored.
 */
void delete_course(FILE *fp) {
    COURSE course;
    int courseNumber;

    // Prompt for course number
    printf("Enter a course number: ");
    scanf("%d", &courseNumber);
    while (getchar() != '\n'); // Clear the input buffer

    // Check if course exists
    fseek(fp, courseNumber * sizeof(COURSE), SEEK_SET);
    if (fread(&course, sizeof(COURSE), 1, fp) != 1 || course.course_Hours == 0) {
        printf("ERROR: course not found\n");
        return;
    }

    // Zero out the course record (soft delete)
    memset(&course, 0, sizeof(course));
    fseek(fp, courseNumber * sizeof(COURSE), SEEK_SET);
    fwrite(&course, sizeof(COURSE), 1, fp);
    printf("Course number %d was successfully deleted.\n", courseNumber);
}




/**
 * @brief The main function of the program.
 * @return Returns 0 if the program runs successfully, 1 otherwise.
 */
int main() {
    FILE *fp = fopen("courses.dat", "r+b");
    if (!fp) {
        fp = fopen("courses.dat", "w+b");
        if (!fp) {
            perror("Failed to open file");
            return 1;
        }
    }

    char choice;
    do {
        display_menu();
        scanf(" %c", &choice);
        while (getchar() != '\n');  // Clear input buffer

        switch (choice) {
            case 'C'|'c':
                create_course(fp);
                break;
            case 'U'|'u':
                update_course(fp);
                break;
            case 'R'|'r':
                read_course(fp);
                break;
            case 'D'|'d':
                delete_course(fp);
                break;
            default:
                printf("ERROR: invalid option\n");
        }
    } while (choice != EOF);

    fclose(fp);
    return 0;
}