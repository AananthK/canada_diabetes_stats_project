/*
CPS188 Winter 2023 Term Project
Toronto Metropolitan University
Prof: Dr. Hamelin
Names: Subhan Sajadi, Aananth Kuganendra, Joshua Savunthararajah,  Sami Matar
Date Created: Sunday, March 26, 2023
Last Modified: Sunday, April 7, 2023
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>

#define maxrows 500
#define maxlen 1000

// Define the structure for storing each relavant column of data into an array
//name of column on data file is commented beside eash struct array
typedef struct {
    int year;//REF_DATE
    char* area;//GEO
    char* age_group;//Age Group
    char* sex;//Sex
    double perpopulation;//VALUE
} Column;

//Function to provide the average for question 1a and 1b
double provavg(Column* column_info, int rows, const char* location) {
    double total = 0.0;
    int num_entries = 0;
    
    for (Column* curr_row = column_info; curr_row < column_info + (rows-2); curr_row++) {
        if (strcmp(curr_row->area, location) == 0 && curr_row->perpopulation != 0) {
            total += curr_row->perpopulation;
            num_entries++;
        }
    }

    if (num_entries > 0) {
         double AVG = total / num_entries;
        return AVG;
    } else {
        return 0;
    }
}

//Function to provide averages for question 1c
double provavg_peryear(Column* column_info, Column* column_data, int rows, const char* location, int yr) {
    double total = 0.0;
    int num_entries = 0;
    Column* curr_col;

    for (curr_col = column_info; curr_col < column_info + rows - 2; curr_col++) {
        if (strcmp(curr_col->area, location) == 0 && curr_col->year - yr == 0 && curr_col->perpopulation != 0) {
            total += curr_col->perpopulation;
            num_entries++;
        }
    }

    if (num_entries > 0) {
         double AVG = total / num_entries;
        return AVG;
    } 
    else {
        return 0;
    }
}

//Function to provide averages for question 1d
double provavg_perage(Column* column_info, Column* column_data, int rows, const char* location, const char* target_age) {
    double total = 0.0;
    int num_entries = 0;

    for (Column* info_ptr = column_info; info_ptr < column_info + rows - 2; info_ptr++, column_data++) {
        if (strcmp(info_ptr->area, location) == 0 && strcmp(column_data->age_group, target_age) == 0 && info_ptr->perpopulation != 0) {
            total += info_ptr->perpopulation;
            num_entries++;
        }
    }

    if (num_entries > 0) {
         double AVG = total / num_entries;
        return AVG;
    } 
    else {
        return 0;
    }
}

//Function to eliminate quotation marks that are present in strings
char* clean(char* str) {
    int len = strlen(str);
    if (len < 2 || str[0] != '"' || str[len - 1] != '"') {
        return strdup(str);
    }
    char* new_string = (char*)malloc(len - 1);
    if (!new_string) {
        perror("Memory allocation failed");
        exit(1);
    }
    strncpy(new_string, str + 1, len - 2);
    new_string[len - 2] = '\0';
    return new_string;
}

// Function to read the statscan_diabetes file in CSV and store the relevant columns of data into the C program
int readfile(const char* nof, Column* column_info) {
    FILE* fp = fopen(nof, "r");
    if (fp == NULL) {
        perror("\n\nfile error\n\n");
        return -1;
    }

    char line[maxlen];
    int row = 0;
    while (fgets(line, maxlen, fp)) {
        // This will skip the title of each row
        if (row == 0) {
            row++;
            continue;
        }

        // Cuts the line
char* token = strtok(line, ",");
Column curr_row = {0};

for (int col = 0; token != NULL; col++, token = strtok(NULL, ",")) {
    switch(col) {
        case 0: curr_row.year = atoi(clean(token)); break;
        case 1: curr_row.area = clean(strdup(token)); break;
        case 3: curr_row.age_group = clean(strdup(token)); break;
        case 4: curr_row.sex = clean(strdup(token)); break;
        case 13: curr_row.perpopulation = atof(clean(token)); break;
        default: break;
    }
}

column_info[row-1] = curr_row;
row++;
    }
    fclose(fp);
    return row-1;
}

//Questions will be answered and answers will be outputted in the main function
int main() {
    Column column_info[maxrows];
    int rows = readfile("statscan_diabetes.csv", column_info);
    //These are data files created by the C program to be used later for plotting graphs on GNUplot
  
      //Files for question 5
      FILE* out = fopen ("can1", "w");
      FILE* out2 = fopen ("qu1", "w");
      FILE* out3 = fopen ("on1", "w");
      FILE* out4 = fopen ("al1", "w");
      FILE* out5 = fopen ("bc1", "w");
  
      //File for question 6
      FILE* outb = fopen ("can2", "w");

  printf("Welcome! \nThis C program was designed to extract information from a data file, statscan_diabetes.csv, which contained information on the prevalence of diabetes in Ontario, Quebec, Alberta, British Columbia and Canada. The program outputs percentage averages in varying conditions, providing a better insight into the nation's struggle with diabetes. \n\n ");

  //Question 1a
    const char* areas[] = {"Quebec", "Ontario", "Alberta", "British Columbia"};
    double average1[4];
  
    printf("--------------------------------------------------------------------------------");
    printf("\n\n");
    printf("1.a) Here are the average percentages of the diabetic population in each province (all years, sexes, and ages): \n\n");
    for (int i = 0; i < sizeof(areas) / sizeof(areas[0]); i++) {
        average1[i] = provavg(column_info, rows, areas[i]);
        printf("      Average population in %s: %.2lf %%\n", areas[i], average1[i]);
    }
    printf("\n");
    printf("--------------------------------------------------------------------------------");
    printf("\n");

  //Question 1b

    printf("\n1.b) Here is the national average percentage of the diabetic population (all years, sexes, and ages): \n\n");
        double averagen = provavg(column_info, rows, "Canada (excluding territories)");
        printf("      Average population in Canada (excluding territories): %.2lf %%\n", averagen);
  
    printf("\n");
    printf("--------------------------------------------------------------------------------");
    printf("\n");
  
  //Question 1c
    //Printing headers for files used in question 5
    fprintf (out, "# year area avg \n");
    fprintf (out2, "# year area avg \n");
    fprintf (out3, "# year area avg \n");
    fprintf (out4, "# year area avg \n");
    fprintf (out5, "# year area avg \n");
  
    int yr[] = {2015, 2016, 2017, 2018, 2019, 2020, 2021};
    printf("\n1.c) Here are the yearly average percentages of the diabetic population in each province (all sexes and ages): \n\n");
    const char* areas2[] = {"Canada (excluding territories)", "Quebec", "Ontario", "Alberta", "British Columbia"};
    for (int j = 0; j < sizeof(yr) / sizeof(yr[0]); j++){
        printf("      In the year %d\n", yr[j]);
        for (int i = 0; i < sizeof(areas2) / sizeof(areas2[0]); i++) {
            double average = provavg_peryear(column_info, column_info, rows, areas2[i], yr[j]);
            printf("       Average population in %s: %.2lf %%\n", areas2[i], average);
            if (strcmp(areas2[i], "Canada (excluding territories)") == 0){
                fprintf (out, "%-15d %-15s %-15lf\n", yr[j], areas2[i], average);
            }
            if (strcmp(areas2[i], "Quebec") == 0){
                fprintf (out2, "%-15d %-15s %-15lf\n", yr[j], areas2[i], average);
            }
            if (strcmp(areas2[i], "Ontario") == 0){
                fprintf (out3, "%-15d %-15s %-15lf\n", yr[j], areas2[i], average);
            }    
            if (strcmp(areas2[i], "Alberta") == 0){
                fprintf (out4, "%-15d %-15s %-15lf\n", yr[j], areas2[i], average);
            }
            if (strcmp(areas2[i], "British Columbia") == 0){
                fprintf (out5, "%-15d %-15s %-15lf\n", yr[j], areas2[i], average);
            }
        }
    
        printf("\n");
    }
    printf("--------------------------------------------------------------------------------");
    printf("\n");
  
  //Question 1d
    fprintf (outb, "# age area avg \n");
    const char* ages[] = {"35 to 49 years", "50 to 64 years", "65 years and over"};
    printf("\n1.d) Here is are the average percentages of the diabetic population of each age group in each province (all years and sexes): \n\n");
    for (int j = 0; j < sizeof(ages) / sizeof(ages[0]); j++){
        printf("      For people aged %s\n", ages[j]);
        for (int i = 0; i < sizeof(areas2) / sizeof(areas2[0]); i++) {
            double average = provavg_perage(column_info, column_info, rows, areas2[i], ages[j]);
            printf("       Average population in %s: %.2lf %%\n", areas2[i], average);
            if (strcmp(areas2[i], "Canada (excluding territories)") == 0){
                if (strcmp(ages[j], "35 to 49 years") == 0){
                    fprintf (outb, "%-15d 35-49 %-15lf\n", j, average);
                  }
                  if (strcmp(ages[j], "50 to 64 years") == 0){
                      fprintf (outb, "%-15d 50-64 %-15lf\n", j, average);
                  }
                  if (strcmp(ages[j], "65 years and over") == 0){
                      fprintf (outb, "%-15d >64 %-15lf\n", j, average);
                  }
        
            }
        }
    printf("\n");
    }
    printf("--------------------------------------------------------------------------------");
    printf("\n");

  //Question 2
    for (int j = 0; j < sizeof(areas) / sizeof(areas[0]) - 1; j++) {
        for (int k = 0; k < sizeof(areas) / sizeof(areas[0]) - j - 1; k++) {
            if (average1[k] > average1[k + 1]) {
                double temp = average1[k];
                const char* temp2 = areas[k];
                average1[k] = average1[k + 1];
                areas[k] = areas[k + 1];
                average1[k + 1] = temp;
                areas[k + 1] = temp2;
            }
        }
    }
    printf("\n2. The province with the lowest diabetic percentage average for all years and age groups is %s with %.2lf %%\n\n",         areas[0], average1[0]);    
    printf("   The province with the highest diabetic percentage average for all years and age groups is %s with %.2lf %%\n", areas[3], average1[3]);  

  //Question 3
    printf("\n");
    printf("--------------------------------------------------------------------------------");
    printf("\n");
    printf("\n3. The following provinces have average percentages above the national diabetic percentage, %.2lf %%: \n", averagen);
    for(int i = 0; i < sizeof(areas) / sizeof(areas[0]); i++){
        if (average1[i] > averagen){
            printf("   %s: %.2lf %%\n", areas[i], average1[i]);
        }
    }
    printf("\n   The following provinces have average percentages below the national diabetic percentage, %.2lf %%: \n", averagen);
    for(int i = 0; i < sizeof(areas) / sizeof(areas[0]); i++){
        if (average1[i] < averagen){
            printf("   %s: %.2lf %%\n", areas[i], average1[i]);
        }
    }
//Question 4 
  printf("\n");
  printf("--------------------------------------------------------------------------------");
  printf("\n");
  printf("\n4. Year and province with the highest and lowest percentage of diabetes:\n\n");

  // Variables to store highest and lowest percentages and their corresponding years and provinces
  double permax = -DBL_MAX;
  double permin = DBL_MAX;
  int maxyr = -1;
  int minyr = -1;
  const char* maxprov;
  const char* minprov;
  int i,j;

  for (j = 0; j < sizeof(yr) / sizeof(yr[0]); j++) {
      for (i = 0; i < sizeof(areas) / sizeof(areas[0]); i++) {
          double AVG = provavg_peryear(column_info, column_info, rows, areas[i], yr[j]);

          if (AVG > permax) {
              permax = AVG;
              maxyr = yr[j];
              maxprov = areas[i];
          }

          if (AVG < permin) {
              permin = AVG;
              minyr = yr[j];
              minprov = areas[i];
          }
    }
  }
  printf("  The highest percentage of diabetes was in %s in the year %d with %.2lf %%\n\n", maxprov, maxyr, permax);
  printf("  The lowest percentage of diabetes was in %s in the year %d with %.2lf %%\n", minprov, minyr, permin);
  printf("\n");
  printf("--------------------------------------------------------------------------------");
  return 0;

}