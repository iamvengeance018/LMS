#include <stdio.h>
#include <string.h>

#define MAX_BOOKS 100
#define MAX_STUDENTS 50

#define MATHS 1
#define PHYSICS 2
#define CHEMISTRY 3

typedef struct
{
    int id;
    char name[50];
    char author[50];
    int department;
    int available;
} Book;

typedef struct
{
    int id;
    char name[50];
    char password[20];
    int borrowedBook;
} Student;

Book books[MAX_BOOKS];
Student students[MAX_STUDENTS];

int bookCount = 0;
int studentCount = 0;

// Function declarations
void loadData();
void saveData();
void addDefaultData();
void studentLogin();
void adminLogin();
void studentMenu(int index);
void adminMenu();
void viewBooks(int department);
void viewAllBooks();
void borrowBook(int index);
void returnBook(int index);
void addBook();
void deleteBook();
void updateBook();
void viewStudentRecords();

const char *departmentName(int dept)
{
    switch (dept)
    {
        case MATHS: return "Mathematics";
        case PHYSICS: return "Physics";
        case CHEMISTRY: return "Chemistry";
        default: return "Unknown";
    }
}

// Save data into files
void saveData()
{
    FILE *fp;

    fp = fopen("books.txt", "w");

    if (fp != NULL)
    {
        fprintf(fp, "%d\n", bookCount);

        for (int i = 0; i < bookCount; i++)
        {
            fprintf(fp, "%d %s %s %d %d\n",
                    books[i].id,
                    books[i].name,
                    books[i].author,
                    books[i].department,
                    books[i].available);
        }

        fclose(fp);
    }

    fp = fopen("students.txt", "w");

    if (fp != NULL)
    {
        fprintf(fp, "%d\n", studentCount);

        for (int i = 0; i < studentCount; i++)
        {
            fprintf(fp, "%d %s %s %d\n",
                    students[i].id,
                    students[i].name,
                    students[i].password,
                    students[i].borrowedBook);
        }

        fclose(fp);
    }
}

// Load saved data
void loadData()
{
    FILE *fp;

    fp = fopen("books.txt", "r");

    if (fp != NULL)
    {
        fscanf(fp, "%d", &bookCount);

        for (int i = 0; i < bookCount; i++)
        {
            fscanf(fp, "%d %49s %49s %d %d",
                   &books[i].id,
                   books[i].name,
                   books[i].author,
                   &books[i].department,
                   &books[i].available);
        }

        fclose(fp);
    }

    fp = fopen("students.txt", "r");

    if (fp != NULL)
    {
        fscanf(fp, "%d", &studentCount);

        for (int i = 0; i < studentCount; i++)
        {
            fscanf(fp, "%d %49s %19s %d",
                   &students[i].id,
                   students[i].name,
                   students[i].password,
                   &students[i].borrowedBook);
        }

        fclose(fp);
    }
}

// Add initial data
void addDefaultData()
{
    if (studentCount == 0)
    {
        students[0] = (Student){101, "Student1", "1234", 0};
        students[1] = (Student){102, "Student2", "1234", 0};
        studentCount = 2;
    }

    if (bookCount == 0)
    {
        books[0] = (Book){1, "Algebra", "John", MATHS, 1};
        books[1] = (Book){2, "Calculus", "David", MATHS, 1};
        books[2] = (Book){3, "Mechanics", "Newton", PHYSICS, 1};
        books[3] = (Book){4, "Optics", "Young", PHYSICS, 1};
        books[4] = (Book){5, "Organic", "Morrison", CHEMISTRY, 1};
        books[5] = (Book){6, "Inorganic", "Jain", CHEMISTRY, 1};

        bookCount = 6;
    }

    saveData();
}

// Display books by department
void viewBooks(int department)
{
    int found = 0;

    printf("\n--- %s Books ---\n", departmentName(department));

    for (int i = 0; i < bookCount; i++)
    {
        if (books[i].department == department)
        {
            printf("\nID: %d | Name: %s | Author: %s | Status: %s",
                   books[i].id,
                   books[i].name,
                   books[i].author,
                   books[i].available ? "Available" : "Borrowed");

            found = 1;
        }
    }

    if (!found)
        printf("\nNo books available in this department.\n");
}

void viewAllBooks()
{
    printf("\n========== ALL BOOKS ==========\n");

    for (int i = 0; i < bookCount; i++)
    {
        printf("\nID: %d | Name: %s | Author: %s",
               books[i].id, books[i].name, books[i].author);

        printf("\nDepartment: %s",
               departmentName(books[i].department));

        printf("\nStatus: %s\n",
               books[i].available ? "Available" : "Borrowed");
    }
}

// Borrow book
void borrowBook(int index)
{
    int id;

    if (students[index].borrowedBook != 0)
    {
        printf("\nYou have already borrowed one book!");
        printf("\nPlease return it before borrowing another.\n");
        return;
    }

    viewAllBooks();

    printf("\nEnter Book ID to borrow: ");
    scanf("%d", &id);

    for (int i = 0; i < bookCount; i++)
    {
        if (books[i].id == id)
        {
            if (!books[i].available)
            {
                printf("\nBook is already borrowed!\n");
                return;
            }

            books[i].available = 0;
            students[index].borrowedBook = id;

            saveData();

            printf("\nBook borrowed successfully!\n");
            return;
        }
    }

    printf("\nInvalid Book ID!\n");
}

// Return book
void returnBook(int index)
{
    int id = students[index].borrowedBook;

    if (id == 0)
    {
        printf("\nYou have not borrowed any book.\n");
        return;
    }

    for (int i = 0; i < bookCount; i++)
    {
        if (books[i].id == id)
        {
            books[i].available = 1;
            students[index].borrowedBook = 0;

            saveData();

            printf("\nBook returned successfully!\n");
            return;
        }
    }
}

// Student menu
void studentMenu(int index)
{
    int choice, dept;

    do
    {
        printf("\n\n========== STUDENT MENU ==========");
        printf("\n1. View Departments");
        printf("\n2. View Books");
        printf("\n3. Borrow Book");
        printf("\n4. Return Book");
        printf("\n5. My Borrowed Book");
        printf("\n6. Logout");
        printf("\nEnter choice: ");

        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                printf("\n1. Mathematics");
                printf("\n2. Physics");
                printf("\n3. Chemistry");
                break;

            case 2:
                printf("\nSelect Department:");
                printf("\n1. Mathematics");
                printf("\n2. Physics");
                printf("\n3. Chemistry");
                printf("\nEnter choice: ");
                scanf("%d", &dept);

                if (dept >= 1 && dept <= 3)
                    viewBooks(dept);
                else
                    printf("\nInvalid department!");
                break;

            case 3:
                borrowBook(index);
                break;

            case 4:
                returnBook(index);
                break;

            case 5:
                if (students[index].borrowedBook == 0)
                    printf("\nNo book borrowed.");
                else
                {
                    for (int i = 0; i < bookCount; i++)
                    {
                        if (books[i].id == students[index].borrowedBook)
                        {
                            printf("\nBorrowed Book: %s",
                                   books[i].name);
                        }
                    }
                }
                break;

            case 6:
                printf("\nStudent logged out.");
                break;

            default:
                printf("\nInvalid choice!");
        }

    } while (choice != 6);
}

// Add new book
void addBook()
{
    if (bookCount >= MAX_BOOKS)
    {
        printf("\nBook storage full!");
        return;
    }

    Book b;

    printf("\nEnter Book ID: ");
    scanf("%d", &b.id);

    for (int i = 0; i < bookCount; i++)
    {
        if (books[i].id == b.id)
        {
            printf("\nBook ID already exists!");
            return;
        }
    }

    printf("Enter Book Name (single word): ");
    scanf("%49s", b.name);

    printf("Enter Author Name (single word): ");
    scanf("%49s", b.author);

    printf("Select Department (1-Maths, 2-Physics, 3-Chemistry): ");
    scanf("%d", &b.department);

    if (b.department < 1 || b.department > 3)
    {
        printf("\nInvalid department!");
        return;
    }

    b.available = 1;

    books[bookCount++] = b;

    saveData();

    printf("\nBook added successfully!");
}

// Delete book
void deleteBook()
{
    int id, i;

    printf("\nEnter Book ID to delete: ");
    scanf("%d", &id);

    for (i = 0; i < bookCount; i++)
    {
        if (books[i].id == id)
        {
            if (!books[i].available)
            {
                printf("\nCannot delete a borrowed book!");
                return;
            }

            for (int j = i; j < bookCount - 1; j++)
                books[j] = books[j + 1];

            bookCount--;

            saveData();

            printf("\nBook deleted successfully!");
            return;
        }
    }

    printf("\nBook not found!");
}

// Update book
void updateBook()
{
    int id;

    printf("\nEnter Book ID to update: ");
    scanf("%d", &id);

    for (int i = 0; i < bookCount; i++)
    {
        if (books[i].id == id)
        {
            printf("Enter new Book Name: ");
            scanf("%49s", books[i].name);

            printf("Enter new Author Name: ");
            scanf("%49s", books[i].author);

            printf("\nSelect Department (1-Maths, 2-Physics, 3-Chemistry): ");
            scanf("%d", &books[i].department);

            if (books[i].department < 1 ||
                books[i].department > 3)
            {
                printf("\nInvalid department!");
                return;
            }

            saveData();

            printf("\nBook updated successfully!");
            return;
        }
    }

    printf("\nBook not found!");
}

// View student records
void viewStudentRecords()
{
    printf("\n========== STUDENT RECORDS ==========\n");

    for (int i = 0; i < studentCount; i++)
    {
        printf("\nStudent ID: %d", students[i].id);
        printf("\nStudent Name: %s", students[i].name);

        if (students[i].borrowedBook == 0)
            printf("\nBorrowed Book: None\n");
        else
            printf("\nBorrowed Book ID: %d\n",
                   students[i].borrowedBook);
    }
}

// Admin menu
void adminMenu()
{
    int choice;

    do
    {
        printf("\n\n========== ADMIN MENU ==========");
        printf("\n1. View All Books");
        printf("\n2. Add Book");
        printf("\n3. Delete Book");
        printf("\n4. Update Book");
        printf("\n5. View Student Records");
        printf("\n6. Logout");
        printf("\nEnter choice: ");

        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                viewAllBooks();
                break;

            case 2:
                addBook();
                break;

            case 3:
                deleteBook();
                break;

            case 4:
                updateBook();
                break;

            case 5:
                viewStudentRecords();
                break;

            case 6:
                printf("\nAdmin logged out.");
                break;

            default:
                printf("\nInvalid choice!");
        }

    } while (choice != 6);
}

// Student login
void studentLogin()
{
    int id;
    char password[20];

    printf("\nEnter Student ID: ");
    scanf("%d", &id);

    printf("Enter Password: ");
    scanf("%19s", password);

    for (int i = 0; i < studentCount; i++)
    {
        if (students[i].id == id &&
            strcmp(students[i].password, password) == 0)
        {
            printf("\nLogin successful. Welcome %s!",
                   students[i].name);

            studentMenu(i);
            return;
        }
    }

    printf("\nInvalid Student ID or Password!");
}

// Admin login
void adminLogin()
{
    char username[30];
    char password[20];

    printf("\nEnter Admin Username: ");
    scanf("%29s", username);

    printf("Enter Admin Password: ");
    scanf("%19s", password);

    if (strcmp(username, "admin") == 0 &&
        strcmp(password, "admin123") == 0)
    {
        printf("\nAdmin Login Successful!");
        adminMenu();
    }
    else
    {
        printf("\nInvalid Admin Credentials!");
    }
}

// Main function
int main()
{
    int choice;

    loadData();
    addDefaultData();

    do
    {
        printf("\n\n================================");
        printf("\n    LIBRARY MANAGEMENT SYSTEM");
        printf("\n================================");

        printf("\n1. Student Login");
        printf("\n2. Admin Login");
        printf("\n3. Exit");

        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                studentLogin();
                break;

            case 2:
                adminLogin();
                break;

            case 3:
                saveData();
                printf("\nExiting Library System...");
                break;

            default:
                printf("\nInvalid choice!");
        }

    } while (choice != 3);

    return 0;
}