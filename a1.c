/*
  COMP 2510 Assignment 1
  The program allows the user to save student records and display them.

  Name: Jessica Luong
  Student ID:
  Set 2B
 */
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#define LINESIZE 512

int user_interface(FILE *fp);
void append_record(FILE *fp);
int get_id(char *id);
int check_id(const char id[]);
int get_name(char *first_name, char *last_name);
int check_name(const char name[]);
int get_grade(int *grade);
int check_grade(int grade);
int display_record(FILE *fp, int n);
void display_all(FILE *fp, int n);

/*
  Driver. Opens file, calls the interface, and closes file.
 */
int main(int argc, char*argv[]) {
  FILE *fp;
  FILE file;
  fp = &file;

  if(argc == 2) {
    if((fp = fopen(argv[1], "wb+")) == 0) {
     perror("fopen");
    }
  }

  user_interface(fp);

  if(fclose(fp) != 0) {
    perror("close");
  }
}

/*
   Prompts for and accepts user input to append records or display records.
 */
int user_interface(FILE *fp) {
  char input[LINESIZE];
  int choice = 0;
  int num_inputs = 0;
  char prompt[] = "\n\nChoices:\n0 to add student records\nn (positive integer) to display record n\n-n (negative integer) to view display all student starting from record n\nPlease enter your choice: ";

  while(1) {
    fprintf(stderr, "%s", prompt);

    if(!fgets(input, LINESIZE, stdin)) {
      clearerr(stdin);
      break; /* quits program when user presses end-of-file key (ctrl+d) */
    }

    if((num_inputs = sscanf(input, "%d", &choice)) == 1) {
      if(choice == 0) {
        append_record(fp);
      } else if (choice > 0){
        display_record(fp, choice);
      } else if (choice < 0){
        display_all(fp, abs(choice));
      }
    }
  }
  return -2; /* function should not reach here */
}
/*
   Adds student record (id, name, and grade of a student) to a file.
 */
void append_record(FILE *fp) {
  char id[10];
  char first_name[21];
  char last_name[21];
  int grade = 0;

  if (get_id(id) == 0 && get_name(first_name, last_name) == 0 && get_grade(&grade) == 0) {
      fprintf(fp, "%-10s%-20s%-20s%-4d", id, last_name, first_name, grade);
  }
}

/*
   Accepts ID if it is valid.
   Parameter: ID pointer
   Returns: 0 if a valid ID is recieved
 */
int get_id(char id[]) {
  char input[LINESIZE];
  int num_inputs = 0;
  char prompt[] = "\nPlease enter an ID in the format a########: ";

  while(1) {
    fprintf(stderr, "%s", prompt);

    if(!fgets(input, LINESIZE, stdin)) {
      clearerr(stdin);
      break; /* aborts append operation when user presses end-of-file key (ctrl+d) */
    }

    if((num_inputs = sscanf(input, "%s", id)) == 1) {
      if(check_id(id) == 0) {
        return 0;
      }
    } else if (num_inputs == EOF) {
      return -1; /* aborts append operation when user enters all whitespaces */
    }
  }
  return -2; /* function should not reach here */
}

/*
   Checks whether ID is valid: lowercase 'a' followed by 8 digits.
   Parameter: string that represents ID
   Returns: 0 if ID is valid; -1 if ID is invalid
 */
int check_id(const char id[]) {
  size_t i;

  if(strlen(id) == 9 && id[0] == 'a') {
    for(i = 1; id[i] != '\0'; i++) {
      if(isdigit(id[i])) {
        return 0;
      }
    }
  }
  return -1;
}

/*
   Accepts name if it is valid and converts all characters to lowercase.
   Parameter: first name pointer and last name pointer
   Returns: 0 if a valid name is recieved
 */
int get_name(char first_name[], char last_name[]) {
  char input[LINESIZE];
  int num_inputs = 0;
  size_t i;
  char prompt[] = "Please enter a first name and a last name separated by a space: ";

  while(1) {
    fprintf(stderr, "%s", prompt);

    if(!fgets(input, LINESIZE, stdin)) {
      clearerr(stdin);
      break; /* aborts append operation when user presses end-of-file key (ctrl+d) */
    }

    if((num_inputs = sscanf(input, "%s%s", first_name, last_name)) == 2) {
      if(check_name(first_name) == 0 && check_name(last_name) == 0) {
        for(i = 0; first_name[i] != '\0'; i++) {
          first_name[i] = tolower(first_name[i]);
        }
        for(i = 0; last_name[i] != '\0'; i++) {
          last_name[i] = tolower(last_name[i]);
        }
        return 0;
      }
    } else if (num_inputs == EOF) {
      return -1; /* aborts append operation when user enters all whitespaces */
    }
  }
  return -2; /* function should not reach here */
}

/*
   Checks whether name is valid: only contain letters or hypen and between the
   length of 2 and 20.
   Parameter: string that represents name
   Returns: 0 if name is valid; -1 if name is invalid
 */
 int check_name(const char name[]) {
   size_t i;

   if(strlen(name) >= 2 && strlen(name) < 20) {
     for(i = 0; name[i] != '\0'; i++) {
       if(isalpha(name[i]) || name[i] == '-')
        if(name[0] != '-' && name[strlen(name)-1] != '-')
         return 0;
     }
  }
   return -1;
 }



/*
   Accepts grade if it is valid.
   Parameter: grade pointer
   Returns: 0 if a valid grade is recieved
 */
int get_grade(int *grade) {
  char input[LINESIZE];
  int num_inputs = 0;
  char prompt[] = "Please enter a score between 1 and 100: ";

  while(1) {
    fprintf(stderr, "%s", prompt);

    if(!fgets(input, LINESIZE, stdin)) {
      clearerr(stdin);
      break; /* aborts append operation when user presses end-of-file key (ctrl+d) */
    }

    if((num_inputs = sscanf(input, "%d", grade)) == 1) {
      if(check_grade(*grade) == 0) {
        return 0;
      }
    } else if (num_inputs == EOF) {
      return -1; /* aborts append operation when user enters all whitespaces */
    }
  }
  return -2; /* function should not reach here */
}

/*
   Checks whether grade is valid: integer between 0 and 100 inclusive.
   Parameter: integer that represents grade
   Returns: 0 if grade is valid; -1 if grade is invalid
 */
int check_grade(int grade) {
  if(grade >= 0 && grade <= 100) {
    return 0;
  }
  return -1;
}

/*
   Displays a file at position indicated by an integer n.
   Parameter: file pointer and integer n represeting the file to read
 */
 int display_record(FILE *fp, int n) {
   char id[10];
   char first_name[21];
   char last_name[21];
   int grade = 0;
   int number_of_records = 0;

   n = (n - 1) * 54;
   rewind(fp);

   if (fseek(fp, n, SEEK_CUR) == -1) {
     perror("fseek");
     printf("\n%d", number_of_records);
   } else {
     fscanf(fp, "%s%s%s%d", id, last_name, first_name, &grade);
     number_of_records = 1;
     printf("%s : %s, %s : %d\n", id, last_name, first_name, grade);
     printf("%d\n", number_of_records);
   }
   return -1;
 }

/*
   Displays all files starting from a position indicated by an integer n.
   Parameter: file pointer and integer n represeting the file to start reading from
 */
 void display_all(FILE *fp, int n) {
   char id[10];
   char first_name[21];
   char last_name[21];
   int grade = 0;
   int number_of_records = 0;

   n = (n - 1) * 54;
   rewind(fp);

   if (fseek(fp, n, SEEK_CUR) == -1) {
     perror("fseek");
   }

   while (fscanf(fp, "%s%s%s%d", id, last_name, first_name, &grade) != EOF) {
     printf("%s : %s, %s : %d\n", id, last_name, first_name, grade);
     number_of_records++;
   }
   printf("%d\n", number_of_records);
 }
