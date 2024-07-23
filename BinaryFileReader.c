#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_NAME_LENGTH 20
#define MAX_EMAIL_LENGTH 40
#define MAX_CITY_LENGTH 20
#define MAX_PREFECTURE_LENGTH 15
#define NUM_RECORDS 1000000

typedef struct {
    char first_name[MAX_NAME_LENGTH + 1];
    char last_name[MAX_NAME_LENGTH + 1];
    uint8_t age;
    char email[MAX_EMAIL_LENGTH + 1];
    float height;
    char city[MAX_CITY_LENGTH + 1];
    char prefecture[MAX_PREFECTURE_LENGTH + 1];
} Person;

void read_string(FILE* file, char* str, int max_length) {
    uint8_t length;
    if (fread(&length, sizeof(uint8_t), 1, file) != 1) {
        printf("Error reading string length\n");
        return;
    }
    if (length > max_length) {
        printf("Warning: String length %d exceeds max length %d\n", length, max_length);
        length = max_length;
    }
    if (fread(str, sizeof(char), length, file) != length) {
        printf("Error reading string content\n");
        return;
    }
    str[length] = '\0';
}

Person read_person(FILE* file) {
    Person person;
    read_string(file, person.first_name, MAX_NAME_LENGTH);
    read_string(file, person.last_name, MAX_NAME_LENGTH);
    if (fread(&person.age, sizeof(uint8_t), 1, file) != 1) {
        printf("Error reading age\n");
    }
    read_string(file, person.email, MAX_EMAIL_LENGTH);
    if (fread(&person.height, sizeof(float), 1, file) != 1) {
        printf("Error reading height\n");
    }
    read_string(file, person.city, MAX_CITY_LENGTH);
    read_string(file, person.prefecture, MAX_PREFECTURE_LENGTH);
    return person;
}

void print_person(Person* person) {
    printf("Name: %s %s\n", person->first_name, person->last_name);
    printf("Age: %d\n", person->age);
    printf("Email: %s\n", person->email);
    printf("Height: %.2f m\n", person->height);
    printf("City: %s\n", person->city);
    printf("Prefecture: %s\n", person->prefecture);
}

int main() {
    FILE* file = fopen("large_dataset_optimized.bin", "rb");
    if (file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    // Check file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    printf("File size: %ld bytes\n", file_size);

    int num_records;
    if (fread(&num_records, sizeof(int), 1, file) != 1) {
        printf("Error reading number of records\n");
        fclose(file);
        return 1;
    }
    printf("Number of records in file: %d\n", num_records);

    if (num_records != NUM_RECORDS) {
        printf("Warning: Number of records in file (%d) doesn't match expected (%d)\n", num_records, NUM_RECORDS);
    }

    Person* people = (Person*)malloc(NUM_RECORDS * sizeof(Person));
    if (people == NULL) {
        printf("Error allocating memory\n");
        fclose(file);
        return 1;
    }

    // Read all records into memory
    for (int i = 0; i < NUM_RECORDS; i++) {
        people[i] = read_person(file);
        if (feof(file)) {
            printf("Reached end of file after reading %d records\n", i);
            break;
        }
    }

    fclose(file);

    // Print first 10 records
    printf("\nFirst 10 records:\n");
    for (int i = 0; i < 10 && i < NUM_RECORDS; i++) {
        printf("\nRecord %d:\n", i + 1);
        print_person(&people[i]);
    }

    // Print 100,000th record
    if (NUM_RECORDS >= 100000) {
        printf("\n100,000th record:\n");
        print_person(&people[99999]);
    }
    else {
        printf("\nNot enough records to show the 100,000th record\n");
    }

    free(people);
    return 0;
}


// Dynamic memory allocation
//#define _CRT_SECURE_NO_WARNINGS
//
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <stdint.h>
//
//#define MAX_NAME_LENGTH 20
//#define MAX_EMAIL_LENGTH 40
//#define MAX_CITY_LENGTH 20
//#define MAX_PREFECTURE_LENGTH 15
//
//typedef struct {
//    char first_name[MAX_NAME_LENGTH + 1];
//    char last_name[MAX_NAME_LENGTH + 1];
//    uint8_t age;
//    char email[MAX_EMAIL_LENGTH + 1];
//    float height;
//    char city[MAX_CITY_LENGTH + 1];
//    char prefecture[MAX_PREFECTURE_LENGTH + 1];
//} Person;
//
//void read_string(FILE* file, char* str, int max_length) {
//    uint8_t length;
//    if (fread(&length, sizeof(uint8_t), 1, file) != 1) {
//        printf("Error reading string length\n");
//        str[0] = '\0';
//        return;
//    }
//    if (length > max_length) {
//        printf("String length (%d) exceeds maximum (%d)\n", length, max_length);
//        length = max_length;
//    }
//    if (fread(str, sizeof(char), length, file) != length) {
//        printf("Error reading string content\n");
//        str[0] = '\0';
//        return;
//    }
//    str[length] = '\0';  // Null-terminate the string
//}
//
//Person read_person(FILE* file) {
//    Person person;
//    memset(&person, 0, sizeof(Person));  // Initialize to zeros
//
//    if (feof(file)) {
//        printf("End of file reached unexpectedly\n");
//        return person;
//    }
//
//    read_string(file, person.first_name, MAX_NAME_LENGTH);
//    read_string(file, person.last_name, MAX_NAME_LENGTH);
//    if (fread(&person.age, sizeof(uint8_t), 1, file) != 1) {
//        printf("Error reading age\n");
//        return person;
//    }
//    read_string(file, person.email, MAX_EMAIL_LENGTH);
//    if (fread(&person.height, sizeof(float), 1, file) != 1) {
//        printf("Error reading height\n");
//        return person;
//    }
//    read_string(file, person.city, MAX_CITY_LENGTH);
//    read_string(file, person.prefecture, MAX_PREFECTURE_LENGTH);
//
//    return person;
//}
//
//int main() {
//    FILE* file = fopen("large_dataset_optimized.bin", "rb");
//    if (file == NULL) {
//        printf("Error opening file!\n");
//        return 1;
//    }
//
//    // Check file size
//    fseek(file, 0, SEEK_END);
//    long file_size = ftell(file);
//    fseek(file, 0, SEEK_SET);
//    printf("File size: %ld bytes\n", file_size);
//
//    int num_records;
//    if (fread(&num_records, sizeof(int), 1, file) != 1) {
//        printf("Error reading number of records\n");
//        fclose(file);
//        return 1;
//    }
//    printf("Number of records: %d\n\n", num_records);
//
//    // Calculate record size
//    size_t record_size = 4 * sizeof(uint8_t) +  // 4 string lengths
//        MAX_NAME_LENGTH +
//        MAX_NAME_LENGTH +
//        MAX_EMAIL_LENGTH +
//        MAX_CITY_LENGTH +
//        MAX_PREFECTURE_LENGTH +
//        sizeof(uint8_t) +  // age
//        sizeof(float);     // height
//    printf("Calculated record size: %zu bytes\n\n", record_size);
//
//    // Read and print the first 10 records
//    for (int i = 0; i < 10 && i < num_records; i++) {
//        Person person = read_person(file);
//        printf("Record %d:\n", i + 1);
//        printf("  Name: %s %s\n", person.first_name, person.last_name);
//        printf("  Age: %d\n", person.age);
//        printf("  Email: %s\n", person.email);
//        printf("  Height: %.2f m\n", person.height);
//        printf("  City: %s\n", person.city);
//        printf("  Prefecture: %s\n\n", person.prefecture);
//    }
//
//    fclose(file);
//    return 0;
//}
