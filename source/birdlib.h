#ifndef BIRDLIB_H
#define BIRDLIB_H

#include <math.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#define MAX_SIZE 101 // Macro for Max name size. 100 characters + NULL character
#define MAX_CAPACITY 800
#define MINIMUM_AGE 5
#define MAX_STATUS_SIZE 10
#define SUCCESS 1
#define FAILURE 0
#define BUFFER_SIZE 1000
#define CANCEL 0
#define DELETE 1

// Structs

    // Auxiliary Structs
    typedef struct number{
        char letters[5];
        int numbers;
    }Number; 

    typedef struct date{
        int day;
        int month;
        int year;
    }Date;

    typedef struct time{
        int hour;
        int minutes;
    }Time;

    // Main Structs
    typedef struct flight{
        Number number; // AAA-XXXX format
        int max_capacity;
        Date date; // DD/MM/YYYY model flight date
        Time time; // HH:MM model flight departure time
        char status[MAX_STATUS_SIZE]; // Active or Canceled
    }Flight;

    typedef struct booking{
        Number number; // AAA-XXXX format
        Number flight_number; // AA-XXXXXX format
        int CPF[4]; // XXX.XXX.XXX-XX model
        char name[MAX_SIZE];
        char sex; // 'M', 'F' or 'O'
        Date birth_date; // DD/MM/YYYY model
        char status[MAX_STATUS_SIZE]; // Active or Canceled
    }Booking;

// Miscellaneous Operations

    // Number struct to String
    const char *ntos(Number number){
        char ch[7], str[10];
        memset(ch, 0, 7); memset(str, 0, 10); // Setting strings to NULL
        strcpy(str, number.letters);
        strcat(str, "-");
        if(strlen(number.letters) == 3){
            if(number.numbers < 10){ch[0] = 48; ch[1] = 48; ch[2] = 48;  ch[3] = 48 + number.numbers;}
            else if(number.numbers < 100){ch[0] = 48; ch[1] = 48; ch[2] = 48 + ((number.numbers - number.numbers%10)/10); ch[3] = 48 + number.numbers%10;}
            else if(number.numbers < 1000){ch[0] = 48; ch[1] = 48 + ((number.numbers - number.numbers%100)/100); 
                    ch[2] = 48 + ((number.numbers%100 - number.numbers%10)/10); ch[3] = 48 + number.numbers%10;}
            else{itoa(number.numbers, ch, 10);}
        }else if(strlen(number.letters) == 2){
            if(number.numbers < 10){ch[0] = 48; ch[1] = 48; ch[2] = 48; ch[3] = 48; ch[4] = 48;  ch[5] = 48 + number.numbers;}
            else if(number.numbers < 100){ch[0] = 48; ch[1] = 48; ch[2] = 48; ch[3] = 48; ch[4] = 48 + ((number.numbers - number.numbers%10)/10); ch[5] = 48 + number.numbers%10;}
            else if(number.numbers < 1000){ch[0] = 48; ch[1] = 48; ch[2] = 48; ch[3] = 48 + ((number.numbers - number.numbers%100)/100); 
                    ch[4] = 48 + ((number.numbers%100 - number.numbers%10)/10); ch[5] = 48 + number.numbers%10;}
            else if(number.numbers < 10000){ch[0] = 48; ch[1] = 48; ch[2] = 48 + ((number.numbers - number.numbers%1000)/1000); 
                    ch[3] = 48 + ((number.numbers - number.numbers%100)/100); ch[4] = 48 + ((number.numbers%100 - number.numbers%10)/10); ch[5] = 48 + number.numbers%10;}
            else if(number.numbers < 100000){ch[0] = 48; ch[1] = 48 + ((number.numbers - number.numbers%10000)/10000); ch[2] = 48 + ((number.numbers - number.numbers%1000)/1000); 
                    ch[3] = 48 + ((number.numbers - number.numbers%100)/100); ch[4] = 48 + ((number.numbers%100 - number.numbers%10)/10); ch[5] = 48 + number.numbers%10;}
            else{itoa(number.numbers, ch, 10);}
        }
        strcat(str, ch);
        char *String = malloc(15);
        strcpy(String, str); 
        return String; // Returns the Number in String format
    }

//Functions handling Files

    // Function to write Status in file. If newline is true it prints a new line char
    void WrittingStatus(char str[], FILE** fp, bool newline){
        if(strcmp(str, "ACTIVE") == 0){ // Checks if it matches ACTIVE Status
            strlwr(str);
            str[0] = 'A';               // Formatting to "Active"
            fprintf(*fp, "   %s   |", str);
            if(newline == true){
                fputs("\n", *fp);
            }
        }
        if(strcmp(str, "CANCELED") == 0){ // Checks if it matches CANCELED Status
            strlwr(str);
            str[0] = 'C';                 // Formatting to "Canceled"
            fprintf(*fp, "  %s  |", str);
            if(newline == true){
                fputs("\n", *fp);
            }
        }
    }

    // Finds in which line of the inputted text File the inputted String is
    int Find_String_In_File(const char *dir, char str[]){
        int len = strlen(str);
        char strMatch[MAX_SIZE] = "\000", ch;

        int i = 0, line = 0;

        FILE *fp;
        fp = fopen(dir, "r");

        do{
            ch = fgetc(fp);
            if(feof(fp)){
                fclose(fp);
                return FAILURE; // Returns 0 if it reaches the end of the File without a Match
            }

            if(ch == str[0] || i != 0){ // If the first character of the inputted String matches the current File Pointer character
                strMatch[i] = ch;       // Starts to build a String 
                i++;
                if(ch == '|'){
                    i = 0;
                    while(strMatch[i] != '\0'){
                        strMatch[i] = '\0';
                        i++;
                    }
                    i = 0;
                }
                if(i == len){
                    if(strcmp(str, strMatch) == 0){ // Checks if the built String matches the inputted String
                        fclose(fp);
                        return line;
                    }else{ // If built String doesn't match inputted String it resets variables and keeps searching for matching Numbers
                        i = 0;
                        while(strMatch[i] != '\0'){
                            strMatch[i] = '\0';
                            i++;
                        }
                        i = 0;
                    }
                }
            }
           
            if(ch == '\n'){ // If it reaches the end of the line without a Match it resets the variables and cleans the String
                i = 0;
                while(strMatch[i] != '\0'){
                    strMatch[i] = '\0';
                    i++;
                }
                i = 0;
                line++;
            }

        }while(1);
    }

    // Finds if both inputted Strings are in the same line in text File
    // Retuns 1 if true, 0 if false
    int Find_Two_Strings_In_File_Line(const char *dir, char str1[], char str2[]){
        int len1 = strlen(str1), len2 = strlen(str2);
        char strMatch1[MAX_SIZE] = "\000", strMatch2[MAX_SIZE] = "\000", ch;
        bool match1 = false, match2 = false;

        int i = 0, j = 0, line = 0;

        FILE *fp;
        fp = fopen(dir, "r");

        do{
            ch = fgetc(fp);
            if(feof(fp)){
                fclose(fp);
                return FAILURE; // Returns 0 if it reaches the end of the File without a Match
            }

            if(ch == '|'){
                i = 0;
                while(strMatch1[i] != '\0'){
                    strMatch1[i] = '\0';
                    strMatch2[i] = '\0';
                    i++;
                }
                i = 0;
                j = 0;
            }

            if(ch == str1[0] || i != 0){ // If the first character of the First inputted String matches the current File Pointer character
                strMatch1[i] = ch;       // Starts to build a String 
                i++;
                if(i == len1){
                    if(strcmp(str1, strMatch1) == 0){ // Checks if the built String matches the inputted String
                        match1 = true;
                    }else{ // If built String doesn't match inputted String it resets variables and keeps searching for matching Numbers
                        i = 0;
                        while(strMatch1[i] != '\0'){
                            strMatch1[i] = '\0';
                            i++;
                        }
                        i = 0;
                    }
                }
            }

            if(ch == str2[0] || j != 0){ // If the first character of the Second inputted String matches the current File Pointer character
                strMatch2[j] = ch;       // Starts to build a String 
                j++;
                if(j == len2){
                    if(strcmp(str2, strMatch2) == 0){ // Checks if the built String matches the inputted String
                        match2 = true;
                    }else{ // If built String doesn't match inputted String it resets variables and keeps searching for matching Numbers
                        j = 0;
                        while(strMatch2[j] != '\0'){
                            strMatch2[i] = '\0';
                            j++;
                        }
                        j = 0;
                    }
                }
            }
           
            if(match1 == true && match2 == true){
                return SUCCESS;
            }

            if(ch == '\n'){ // If it reaches the end of the line without a Match it resets the variables and cleans the String
                i = 0;
                while(strMatch1[i] != '\0' || strMatch2[i] != '\0'){
                    strMatch1[i] = '\0';
                    strMatch2[i] = '\0';
                    i++;
                }
                match1 = false;
                match2 = false;
                i = 0;
                j = 0;
                line++;
            }

        }while(1);
    }

    // Finds and prints on Console the Bookings of inputted Flight
    void Print_Flight_Bookings(char Flight_Number[]){

        typedef struct booking_string{ // Struct that reads Strings from File
        char Bnum[10];
        char Fnum[10];
        char CPF[15];
        char sex;
        char Bday[11];
        char Stats[MAX_STATUS_SIZE];
        char name[MAX_SIZE];
        } Booking_String;

        Booking_String BS;

        char dir[] = "Booking_Data_Base.txt"; // Directory to read from
        char ch, str[200], strMatch[MAX_SIZE];
        memset(str, 0, 200); memset(strMatch, 0, MAX_SIZE); // Setting Strings to NULL
        int bytes = 0, line = 0, i = 0, number_of_passengers = 0, Max_Capacity;
        bool match = false;

        char status[MAX_STATUS_SIZE], fdate[11], hour[6];
        memset(status, 0, MAX_STATUS_SIZE); memset(fdate, 0, MAX_STATUS_SIZE); memset(hour, 0, MAX_STATUS_SIZE);

        char dir2[] =  "Flight_Data_Base.txt"; // Directory to be read from

        FILE *fp;
        fp = fopen(dir2, "r");

        char Num[10];
        float Max;

        line = 0;
        bytes = 0;
         do{
            ch = fgetc(fp);
            bytes++;
            if(feof(fp)){
                fclose(fp);
                break; // Returns 0 if it reaches the end of the File without a Match
            }
            if(ch == '\n'){
                bytes = 0;
                line++;
                if(line > 1){
                    fscanf(fp, "|   %[^ ]    |     %f      | %[^ ] |  %[^ ]   |   %[^ ]   |", Num, &Max, fdate, hour, status);
                    if(strcmp(Num, Flight_Number) == 0){
                        Max_Capacity = Max; // Finds Flight Max Capacity
                        printf("Information on Flight %s\n\n", Num);
                        printf("The Flight will leave on %s at %s\n", fdate, hour);
                        printf("The status of this Flight: %s\n\n", status);
                    }
                }
            }

        }while(1);

        fp = fopen(dir, "r");
        line = 0;
        do{
            ch = fgetc(fp);
            bytes++; // Counts number of bytes read in Line
            if(feof(fp)){
                fclose(fp);
                break; // Returns 0 if it reaches the end of the File without a Match
            }

            if(ch == '\n'){ // If it reaches the end of the line without a Match it resets the variables and cleans the String
                if(line == 0){
                    printf("List of Active bookings for Flight %s:\n\n", Flight_Number);
                    puts("| Booking Number |      CPF       | Full Name\n"); // Setting up User interface
                    memset(str, 0, strlen(str)); // Sets String back to NULL
                }
                if(match == true){
                    fseek(fp, -(bytes+1), SEEK_CUR); // Returns to the start of the Line and scans Line elements
                    fscanf(fp, "|   %[^ ]    |   %[^ ]    | %[^ ] |  %c  | %[^ ] |   %[^ ]   | %[^\n]", BS.Bnum, BS.Fnum, BS.CPF, &BS.sex, BS.Bday, BS.Stats, BS.name);
                    if(strcmp(BS.Stats, "Active") == 0){ // Checks if Booking is active
                        printf("|   %s    | %s | %s\n", BS.Bnum, BS.CPF, BS.name); // Prints Active Flight Booking's booking number, CPF and Name
                        number_of_passengers += 1; // Counts number of Passengers
                    }
                    match = false;
                }
                bytes = 0;
    
                memset(strMatch, 0, strlen(strMatch));
                i = 0;
                line++;
            }

            if(ch == Flight_Number[0] || i != 0){ // If the first character of the inputted String matches the current File Pointer character
                strMatch[i] = ch;       // Starts to build a String 
                i++;
                if(ch == '|'){
                    memset(strMatch, 0, strlen(strMatch));
                    i = 0;
                }
            }
            if(i == strlen(Flight_Number)){
                if(strcmp(Flight_Number, strMatch) == 0){
                    match = true;
                    memset(strMatch, 0, strlen(strMatch));
                    i = 0;
                }
            }

        }while(1);

        printf("\nFlight's Max Capacity: %d\n", Max_Capacity);
        printf("Current number of Passengers: %d\n", number_of_passengers);
        printf("Percentage of ocupation: %.2f%%\n\n", (float)number_of_passengers/Max_Capacity*100);

        puts("Press any key to continue\n");
        getch();

    }

    // Prints on console the information of an inputed Booking Number
    void Print_Booking(char Booking_Number[]){
        typedef struct booking_string{ // Struct that reads Strings from File
        char Bnum[10];
        char Fnum[10];
        char CPF[15];
        char sex;
        char Bday[11];
        char Stats[10];
        char name[MAX_SIZE];
        } Booking_String;

        Booking_String BS;

        char dir[] = "Booking_Data_Base.txt"; // Directory to read from

        char ch, str[200], strMatch[MAX_SIZE];
        memset(str, 0, 200); memset(strMatch, 0, MAX_SIZE); // Setting Strings to NULL
        int bytes = 0, line = 0, i = 0;
        bool match = false;

        FILE *fp;
        fp = fopen(dir, "r");

        do{
            ch = fgetc(fp);
            bytes++; // Counts number of bytes read in Line
            if(feof(fp)){
                fclose(fp);
                break; // Returns 0 if it reaches the end of the File without a Match
            }

            if(ch == '\n'){ // If it reaches the end of the line without a Match it resets the variables and cleans the String
                if(line == 0){
                    printf("Information of Booking number %s:\n\n\n", Booking_Number);
                    puts("| Flight Number |      CPF       | Sex | Birth Date |   Status   | Full Name\n"); // Setting up User interface
                    memset(str, 0, strlen(str)); // Sets String back to NULL
                }
                if(match == true){
                    fseek(fp, -(bytes+1), SEEK_CUR); // Returns to the start of the Line and scans Line elements
                    fscanf(fp, "|   %[^ ]    |   %[^ ]    | %[^ ] |  %c  | %[^ ] |%*[^AC]%[^ ]%*[^|]| %[^\n]", BS.Bnum, BS.Fnum, BS.CPF, &BS.sex, BS.Bday, BS.Stats, BS.name);
                    if(strcmp(BS.Stats, "Active") == 0){ // Checks if Booking is active
                        printf("|   %s    | %s |  %c  | %s |   %s   | %s\n", BS.Fnum, BS.CPF, BS.sex, BS.Bday, BS.Stats, BS.name); // Prints Booking Info
                    }else{
                        printf("|   %s    | %s |  %c  | %s |  %s  | %s\n",  BS.Fnum, BS.CPF, BS.sex, BS.Bday, BS.Stats, BS.name); // Prints Booking Info
                    }
                    fclose(fp);
                    break;
                    match = false;
                }
                bytes = 0;
    
                memset(strMatch, 0, strlen(strMatch));
                i = 0;
                line++;
            }

            if(ch == Booking_Number[0] || i != 0){ // If the first character of the inputted String matches the current File Pointer character
                strMatch[i] = ch;       // Starts to build a String 
                i++;
                if(ch == '|'){
                    memset(strMatch, 0, strlen(strMatch));
                    i = 0;
                }
            }
            if(i == strlen(Booking_Number)){
                if(strcmp(Booking_Number, strMatch) == 0){
                    match = true;
                    memset(strMatch, 0, strlen(strMatch));
                    i = 0;
                }
            }

        }while(1);

    }

    // Prints on console all the Flights of the Passenger with the inputted PDF
    void Print_Passenger(char CPF[]){
        typedef struct booking_string{ // Struct that reads Strings from File
        char Bnum[10];
        char Fnum[10];
        char CPF[15];
        char sex;
        char Bday[11];
        char Stats[10];
        char name[MAX_SIZE];
        } Booking_String;

        Booking_String BS;

        char dir[] = "Booking_Data_Base.txt"; // Directory to read from

        char ch, str[200], strMatch[MAX_SIZE];
        memset(str, 0, 200); memset(strMatch, 0, MAX_SIZE); // Setting Strings to NULL
        int bytes = 0, line = 0, i = 0;
        bool match = false;

        FILE *fp;
        fp = fopen(dir, "r");

        do{
            ch = fgetc(fp);
            bytes++; // Counts number of bytes read in Line
            if(feof(fp)){
                fclose(fp);
                break; // Returns 0 if it reaches the end of the File without a Match
            }

            if(ch == '\n'){ // If it reaches the end of the line without a Match it resets the variables and cleans the String
                if(line == 0){
                    printf("Information and Bookings of CPF %s:\n\n\n", CPF);
                    puts("| Booking Number | Flight Number | Sex | Birth Date |   Status   | Full Name\n"); // Setting up User interface
                    memset(str, 0, strlen(str)); // Sets String back to NULL
                }
                if(match == true){
                    fseek(fp, -(bytes+1), SEEK_CUR); // Returns to the start of the Line and scans Line elements
                    fscanf(fp, "|   %[^ ]    |   %[^ ]    | %[^ ] |  %c  | %[^ ] |   %[^ ]   | %[^\n]", BS.Bnum, BS.Fnum, BS.CPF, &BS.sex, BS.Bday, BS.Stats, BS.name);
                    if(strcmp(BS.Stats, "Active") == 0){
                        printf("|   %s    |   %s    |  %c  | %s |   %s   | %s\n", BS.Bnum, BS.Fnum, BS.sex, BS.Bday, BS.Stats, BS.name); // Prints Booking Info
                    }else{
                        printf("|   %s    |   %s    |  %c  | %s |  %s  | %s\n", BS.Bnum, BS.Fnum, BS.sex, BS.Bday, BS.Stats, BS.name); // Prints Booking Info
                    }
                    match = false;
                }
                bytes = 0;
    
                memset(strMatch, 0, strlen(strMatch));
                i = 0;
                line++;
            }

            if(ch == CPF[0] || i != 0){ // If the first character of the inputted String matches the current File Pointer character
                strMatch[i] = ch;       // Starts to build a String 
                i++;
                if(ch == '|'){
                    memset(strMatch, 0, strlen(strMatch));
                    i = 0;
                }
            }
            if(i == strlen(CPF)){
                if(strcmp(CPF, strMatch) == 0){
                    match = true;
                    memset(strMatch, 0, strlen(strMatch));
                    i = 0;
                }
            }

        }while(1);
    }

    // Cancel or Delete inputted Booking
    int Modify_Booking_File(const char *Booking_Number, int action, int Flight_Modified){

        Booking booking;
        char number[10];

        /* File pointer to hold reference of input file */
        FILE * fp;
        FILE * fTemp;
        char dir[] = "Booking_Data_Base.txt";
        
        char buffer[BUFFER_SIZE];
        int line, count;

        line = Find_String_In_File(dir, Booking_Number);
        if(!line){
            return FAILURE;
        }

        /* Remove extra new line character from stdin */
        fflush(stdin);

        /*  Open all required files */
        fp  = fopen(dir, "r");
        fTemp = fopen("rep.tmp", "w"); 

        /* fopen() return NULL if unable to open file in given mode. */
        if (fp == NULL || fTemp == NULL)
        {
            /* Unable to open file hence exit */
            printf("\n\t----------ERROR!----------\n");
            printf("\nError opening the Data File. Please check if there are any programs impeding the creation of or access to Files\n\n");
            return FAILURE;
        }


        /*
        * Read line from source file and write to destination 
        * file after replacing given line.
        */
        count = 0;

        switch(action){
            case CANCEL:
                while ((fgets(buffer, BUFFER_SIZE, fp)) != NULL){

                    if(!Flight_Modified){
                        /* If current line is line to replace */
                        if (count == line){
                            fseek(fp, -(strlen(buffer)+1), SEEK_CUR);
                            fscanf(fp, "|   %*[^ ]    |   %[^-]%*c%d    | %d.%d.%d-%d |  %c  | %d/%d/%d |%*[^AC]%[^ ]%*[^|]| %[^\n]\n", booking.number.letters, &booking.number.numbers, &booking.CPF[0], &booking.CPF[1], &booking.CPF[2], &booking.CPF[3], &booking.sex, &booking.birth_date.day, &booking.birth_date.month, &booking.birth_date.year, booking.status, booking.name);
                            if(strcmp(booking.status, "Canceled") == 0){
                                puts("-------------- ERROR ------------\n");
                                puts("Booking has already been canceled\n\n");
                                fclose(fp);
                                fclose(fTemp);
                                return FAILURE;
                            }else{
                                fprintf(fTemp, "|   %s    |   %s-%04d    | %03d.%03d.%03d-%02d |  %c  | %02d/%02d/%04d |  Canceled  | %s\n", Booking_Number, booking.number.letters, booking.number.numbers, booking.CPF[0], booking.CPF[1], booking.CPF[2], booking.CPF[3], booking.sex, booking.birth_date.day, booking.birth_date.month, booking.birth_date.year, booking.name);
                                count++;
                            }
                            #//fputs(newline, fTemp);
                        }else{
                            fputs(buffer, fTemp);
                        }
                        count++;
                    }else{
                        if (count > 1){
                            fseek(fp, -(strlen(buffer)+1), SEEK_CUR);
                            fscanf(fp, "|   %[^ ]    |   %[^-]%*c%d    | %d.%d.%d-%d |  %c  | %d/%d/%d |%*[^AC]%[^ ]%*[^|]| %[^\n]\n", number, booking.number.letters, &booking.number.numbers, &booking.CPF[0], &booking.CPF[1], &booking.CPF[2], &booking.CPF[3], &booking.sex, &booking.birth_date.day, &booking.birth_date.month, &booking.birth_date.year, booking.status, booking.name);
                            if(strcmp(ntos(booking.number), Booking_Number) == 0){
                                fprintf(fTemp, "|   %s    |   %s-%04d    | %03d.%03d.%03d-%02d |  %c  | %02d/%02d/%04d |  Canceled  | %s\n", number, booking.number.letters, booking.number.numbers, booking.CPF[0], booking.CPF[1], booking.CPF[2], booking.CPF[3], booking.sex, booking.birth_date.day, booking.birth_date.month, booking.birth_date.year, booking.name);
                                count++;
                            }else{
                                fputs(buffer, fTemp);
                            }
                            //fputs(newline, fTemp);
                        }else{
                            fputs(buffer, fTemp);
                        }
                        count++;
                    }
                }
                break;
            case DELETE:
                while ((fgets(buffer, BUFFER_SIZE, fp)) != NULL){

                    /* If current line is line to replace */
                    if (count == line){
                        count++;
                    }else{
                        fputs(buffer, fTemp);
                    }
                    count++;
                }
                break;
            default:
                puts("------ ERROR ------\n");
                puts("Action not accepted\n\n");
                fclose(fp);
                fclose(fTemp);
                return FAILURE;
        }

        /* Close all files to release resource */
        fclose(fp);
        fclose(fTemp);


        /* Delete original source file */
        remove(dir);

        /* Rename temporary file as original file */
        rename("rep.tmp", dir);

        return SUCCESS;

    }

    // Cancel of Delete inputted Flight
    int Modify_Flight_File(const char *Flight_Number, int action){

        Flight flight;

        /* File pointer to hold reference of input file */
        FILE * fp;
        FILE * fTemp;
        char dir[] = "Flight_Data_Base.txt";
        
        char buffer[BUFFER_SIZE];
        int line, count;

        line = Find_String_In_File(dir, Flight_Number);
        if(!line){
            return FAILURE;
        }

        /* Remove extra new line character from stdin */
        fflush(stdin);

        /*  Open all required files */
        fp  = fopen(dir, "r");
        fTemp = fopen("replace.tmp", "w"); 

        /* fopen() return NULL if unable to open file in given mode. */
        if (fp == NULL || fTemp == NULL)
        {
            /* Unable to open file hence exit */
            printf("\n\t----------ERROR!----------\n");
            printf("\nError opening the Data File. Please check if there are any programs impeding the creation of or access to Files\n\n");
            return FAILURE;
        }


        /*
        * Read line from source file and write to destination 
        * file after replacing given line.
        */
        count = 0;

        switch(action){
            case CANCEL:
                while ((fgets(buffer, BUFFER_SIZE, fp)) != NULL)
                {

                    /* If current line is line to replace */
                    if (count == line){
                        fseek(fp, -(strlen(buffer)+1), SEEK_CUR);
                        fscanf(fp, "|   %*[^ ]    |     %d      | %d/%d/%d |  %d:%d   |%*[^AC]%[^ ]%*[^|]|\n", &flight.max_capacity, &flight.date.day, &flight.date.month, &flight.date.year, &flight.time.hour, &flight.time.minutes, flight.status);
                        if(strcmp(flight.status, "Canceled") == 0){
                            puts("------------- ERROR ------------\n");
                            puts("Flight has already been canceled\n\n");
                            fclose(fp);
                            fclose(fTemp);
                            return FAILURE;
                        }else{
                            fprintf(fTemp, "|   %s    |     %03d      | %02d/%02d/%04d |  %02d:%02d   |  Canceled  |\n", Flight_Number, flight.max_capacity, flight.date.day, flight.date.month, flight.date.year, flight.time.hour, flight.time.minutes);
                            count++;
                        }
                        #//fputs(newline, fTemp);
                    }else{
                        fputs(buffer, fTemp);
                    }
                    count++;
                }
                
                Modify_Booking_File(Flight_Number, CANCEL, true);

                break;
            case DELETE:
                while ((fgets(buffer, BUFFER_SIZE, fp)) != NULL){

                    /* If current line is line to replace */
                    if (count == line){
                        count++;
                    }else{
                        fputs(buffer, fTemp);
                    }
                    count++;
                }
                while(Find_String_In_File("Booking_Data_Base.txt", Flight_Number) != 0){
                    Modify_Booking_File(Flight_Number, DELETE, true);
                }
                break;
            default:
                puts("------ ERROR ------\n");
                puts("Action not accepted\n\n");
                fclose(fp);
                fclose(fTemp);
                return FAILURE;
        }

        /* Close all files to release resource */
        fclose(fp);
        fclose(fTemp);


        /* Delete original source file */
        remove(dir);

        /* Rename temporary file as original file */
        rename("replace.tmp", dir);

        return SUCCESS;

    }

// Checking if there are Flights of Bookings stored in the Data Base

    //Checks if at least one Booking or Flight has been registered. Returns 1 if it has not and returns 0 if it has
    int ChkNullFile(const char *dir){
        char ch = 0;
        int line = 1;

        FILE *fp;
        fp = fopen(dir, "r");
    
        while(!feof(fp) && line < 4){
            ch = fgetc(fp);
            if(ch == '\n'){
                line++; // Counts File lines
            }
        }
        fclose(fp);

        if(line == 4){ // Existing a 4th line in the File means there is at least 1 registered Flight. Since by default 3 lines are created when File is created.
            return FAILURE;
        }else{return SUCCESS;}
    }

// Checking Inputs
    // Takes in a string and an Integer and checks if a flight number in the format AAA-XXXX can be formed, where AAA is the String and X are integers. Returns 1 if it can, 0 if cannot
    int ChkFlightNumber(char str[], int X){
        int i;
        
        if(strlen(str) == 3 && X <= 9999 && X >= 0){ // Checks if Flight Number exists
            for(i = 0; i < 3; i++){
                if(!isupper(str[i])){ // Checks if inputted characters are Upper Case letters
                    return FAILURE; 
                }
                if(str[0] == 'J' && str[1] == 'E' && str[2] == 'B'){ // Check if flight is from Galactic Birds (JEB-XXXX)
                    return SUCCESS;
                }else{return FAILURE;}
            }
        }else{return SUCCESS;}
    }

    // Returns 1 if inputted int is in the range [MAX_CAPACITY, 0], if not, returns 0
    int ChkFlightCapacity(int capacity){
        if(capacity > 0 && capacity <= MAX_CAPACITY){return SUCCESS;} // Checks if the inputted capacity is compatible with the company's spacecrafts
        else{return FAILURE;}
    }

    // Takes in 3 integers, representing date inputs, a boolean to whether it is the same day or not, and a bool to know if the function should check if the date has passed or not.
    // Returns 1 if date exists(or has not passed) and 0 if date does not exist(or has passed). And changes Same_Day boolean to true, if the date is at the current live date
    int ChkDate(int day, int month, int year, bool *Same_Day, bool CHECK_IF_DAY_PASSED){
        int r = year % 4;

        time_t Current_Time = time(NULL);
        struct tm tm = *localtime(&Current_Time); // Getting local time

        if(day == tm.tm_mday && month == tm.tm_mon && year == tm.tm_year+1909){ // Checks if Flight is booked for current day
            *Same_Day = true;
            return SUCCESS;
        }

        if(day < 1 || day > 31){return FAILURE;} // Checks if generic day is valid
        if(month < 1 || month > 12){return FAILURE;}
        if(CHECK_IF_DAY_PASSED == true){
            if(year < tm.tm_year + 1909){return FAILURE;} // Checks if inputted year is equal or greater than current year (I added 9 so it assumes we are in 2030)
            if(year == tm.tm_year + 1909){if(month < tm.tm_mon+1){return FAILURE;}} // If flight is for the current year, checks if the month hasn't already passed
            if(year == tm.tm_year + 1909){if(month == tm.tm_mon+1){if(day < tm.tm_mday){return FAILURE;}}} // If flight is for the current year and month, checks if the day hasn't already passed, if flight booked for same day, activates boolean
        }
        if(month == 2){if(r == 0){ // February
            if(year % 100 == 0){ // Checks if it is Leap Year multiple of 100
                if(year % 400 == 0){ // Checks if Leap Year is also a multiple of 400
                    if(day <= 29){
                        return SUCCESS;
                    }else{return FAILURE;}
                }if(day <= 28){
                    return SUCCESS;
                }else{return FAILURE;}
            }else{if(day <= 29){return SUCCESS;}else{return FAILURE;}} // For Leap Years that are not multiples of 100
            }else{if(day <= 28){return SUCCESS;}else{return FAILURE;}} // If not a Leap Year
        }
        r = year % 2; // Checks if months are Multiple of 2 
        if(month <= 7){
            if(r == 0){
                if(day <= 30){return SUCCESS;}else{return FAILURE;} // Checks for April and June
            }else{
                if(day <= 31){return SUCCESS;}else{return FAILURE;} // Checks for January, March, May, July
            }
        }else{
            if(r != 0){
                if(day <= 30){return SUCCESS;}else{return FAILURE;} // Checks for September and November
            }else{
                if(day <= 31){return SUCCESS;}else{return FAILURE;} // Checks for August, October, December
            }
        }
    }

    // Takes in the Flight Number and checks if the Flight has already departed
    int Flight_Has_Departed(char *Flight_Number){

        FILE *fp;
        char dir[] =  "Flight_Data_Base.txt"; // Directory to be read from
        fp = fopen(dir, "r");

        char Num[10], ch;
        int day, month, year;

        int line = 0;
        int bytes = 0;
         do{
            ch = fgetc(fp);
            bytes++;
            if(feof(fp)){
                fclose(fp);
                break; // Returns 0 if it reaches the end of the File without a Match
            }
            if(ch == '\n'){
                bytes = 0;
                line++;
                if(line > 1){
                    fscanf(fp, "|   %[^ ]%*[^|]|     %*f      | %d/%d/%d |  %*d:%*d   |%*[^AC]%*[^ ]%*[^|]|", Num, &day, &month, &year);
                    if(strcmp(Num, Flight_Number) == 0){
                        if(ChkDate(day, month, year, false, true)){
                            return FAILURE; // The date hasn't passed
                        }else{return SUCCESS;} // The date has passed
                    }
                }
                memset(Num, 0, strlen(Num)); // Sets String back to NULL
            }

        }while(1);

        return FAILURE; // Returns 0 if no Flight was found
    }

    // Checks if inputted Flight is canceled, returns 1 if it is and 0 if not
    int Flight_Is_Full(char *Flight_Number){

        char dir[] =  "Flight_Data_Base.txt"; // Directory to be read from

        char dir2[] = "Booking_Data_Base.txt"; // Directory to read from
        char ch, str[200], strMatch[MAX_SIZE];
        memset(str, 0, 200); memset(strMatch, 0, MAX_SIZE); // Setting Strings to NULL
        int bytes = 0, line = 0, i = 0, number_of_passengers = 0, Max_Capacity;
        bool match = false;
        char status[MAX_STATUS_SIZE];

        FILE *fp;
        fp = fopen(dir, "r");

        char Num[10];
        float Max;

        line = 0;
        bytes = 0;
        
        do{
            ch = fgetc(fp);
            bytes++;
            if(feof(fp)){
                fclose(fp);
                break; // Returns 0 if it reaches the end of the File without a Match
            }
            if(ch == '\n'){
                bytes = 0;
                line++;
                if(line > 1){
                    fscanf(fp, "|   %[^ ]    |     %f      | %*[^ ] |  %*[^ ]   |   %*[^ ]   |", Num, &Max);
                    if(strcmp(Num, Flight_Number) == 0){
                        Max_Capacity = Max; // Finds Flight Max Capacity
                    }
                }
            }

        }while(1);

        fp = fopen(dir2, "r");
        line = 0;
        do{
            ch = fgetc(fp);
            bytes++; // Counts number of bytes read in Line
            if(feof(fp)){
                fclose(fp);
                break; // Returns 0 if it reaches the end of the File without a Match
            }

            if(ch == '\n'){ // If it reaches the end of the line without a Match it resets the variables and cleans the String

                if(match == true){
                    fseek(fp, -(bytes+1), SEEK_CUR); // Returns to the start of the Line and scans Line elements
                    fscanf(fp, "|   %*[^ ]    |   %*[^ ]    | %*[^ ] |  %*c  | %*[^ ] |   %[^ ]   | %*[^\n]", status);
                    if(strcmp(status, "Active") == 0){ // Checks if Booking is active
                        number_of_passengers += 1; // Counts number of Passengers
                    }
                    match = false;
                }
                bytes = 0;
    
                memset(strMatch, 0, strlen(strMatch));
                i = 0;
                line++;
            }

            if(ch == Flight_Number[0] || i != 0){ // If the first character of the inputted String matches the current File Pointer character
                strMatch[i] = ch;       // Starts to build a String 
                i++;
                if(ch == '|'){
                    memset(strMatch, 0, strlen(strMatch));
                    i = 0;
                }
            }
            if(i == strlen(Flight_Number)){
                if(strcmp(Flight_Number, strMatch) == 0){
                    match = true;
                    memset(strMatch, 0, strlen(strMatch));
                    i = 0;
                }
            }

        }while(1);

       if(number_of_passengers ==  Max_Capacity){
           return true;
       }else{
           return false;
       }
    }

    int Flight_Is_Canceled(char *Flight_Number){

        char dir[] =  "Flight_Data_Base.txt"; // Directory to be read from

        int bytes = 0, line = 0, i = 0, number_of_passengers = 0, Max_Capacity;
        bool match = false;
        char status[MAX_STATUS_SIZE];
        char ch;

        FILE *fp;
        fp = fopen(dir, "r");

        char Num[10];
        float Max;

        line = 0;
        bytes = 0;
        
        do{
            ch = fgetc(fp);
            bytes++;
            if(feof(fp)){
                fclose(fp);
                break; // Returns 0 if it reaches the end of the File without a Match
            }
            if(ch == '\n'){
                bytes = 0;
                line++;
                if(line > 1){
                    fscanf(fp, "|   %*[^ ]    |     %*d      | %*[^ ] |  %*[^ ]   |   %[^ ]   |", status);
                    if(strcmp(status, "Canceled") == 0){
                        return true; // Finds Flight Max Capacity
                    }
                }
            }

        }while(1);

        return false;
    }

    // Takes in a Date. Checks if it has passed 5 years from day. Returns 1 if true and 0 if not
    int ChkMininumFlightAge(Date Birth_Date){
        time_t Live_Time = time(NULL);
        struct tm tm = *localtime(&Live_Time);
        
        if((tm.tm_year + 1909 - Birth_Date.year) > MINIMUM_AGE){ // Checks if person was born more than 5 years ago
            return SUCCESS;
        }
        if(Birth_Date.year > tm.tm_year){
            return FAILURE;
        }
        if((tm.tm_year + 1909 - Birth_Date.year) == MINIMUM_AGE){ // If person turns 5 in the year of Flight
            if(Birth_Date.month < tm.tm_mon + 1){
                return SUCCESS;
            }
            if(Birth_Date.month > tm.tm_mon + 1){
                return FAILURE;
            }
            if(Birth_Date.month == tm.tm_mon + 1){
                if(Birth_Date.day <= tm.tm_mday){
                    return SUCCESS;    
                }else{return FAILURE;} 
            }
        }
        

    }

    // Takes in time inputs (hours and minutes) and same day boolean.
    // Returns 1 if time is exists (24 hours) and 0 if it does not. If Same_Day boolean is true, returns 1 if time has not passed and 0 if it has
    int ChkTime(int hour, int minutes, bool *Same_Day){

        time_t Current_Time = time(NULL);
        struct tm tm = *localtime(&Current_Time); // Getting local time

        if(hour < 0 || hour > 23 || minutes < 0 || minutes > 59){
            return FAILURE;
        }else{
            if(*Same_Day == true){
                if(hour < tm.tm_hour || minutes < tm.tm_min){return 0;} // If Flight is for the same day, Checks if the time hasn't already passed
            }else{
                return SUCCESS;
            }
        }
    }

    // Takes in the inputted Status and Checks if it matches "Active" or "Canceled" (It is not case sensitive)
    // Returns 1 if Status is accepted and 0 if not
    int ChkStatus(char str[]){
        strupr(str); // Converts String to Upper Case to make Checks
        int a = strcmp(str, "ACTIVE");
        int b = strcmp(str, "CANCELED");
        if(a == 0 || b == 0){
            return SUCCESS;
        }else{
            return FAILURE;
        }
    }

    // Takes in the inputted Booking number and checks if it is in the AA-XXXXXX format
    //Returns 1 if it is and 0 if not
    int ChkBookingNumber(char str[], int X){
        int i;

        if(strlen(str) == 2 && X <= 999999 && X >= 0){ // Checks if Flight Number exists
            for(i = 0; i < 2; i++){
                if(!isupper(str[i])){return 0; // Checks if inputted characters are Upper Case letters
                }
            }
            if(str[0] == 'G' && str[1] == 'B'){ // Check if Passenger is from Galactic bird (GB-XXXXXX)
                return 1;
            }}else{return 0;}
        
    }

    //Functions regarding CPF number

        // CPF int Array to String
        const char *CPFiatos(int CPF[]){

        int i, j, n = 0;
        char CPFs[15] = "\000", ch[4], temp;

        for(i = 0; i < 4; i++){
            itoa(CPF[i], ch, 10); // Converting int to String

            // Formatting CPF
            if(i != 3 && strlen(ch) == 1){
                temp = ch[0];
                ch[0] = '0';
                ch[1] = '0';
                ch[2] = temp;
            }else if(i != 3 && strlen(ch) == 2){
                temp = ch[0];
                ch[0] = '0';
                ch[2] = ch[1];
                ch[1] = temp;
            }
            if(i == 3 && strlen(ch) == 1){
                temp = ch[0];
                ch[0] = '0';
                ch[1] = temp;
            }
            for(j = 0; j < 3; j++){
                if(n == 3 || n == 7){
                    CPFs[n] = '.';
                    n++;
                    j--;
                }else if(n == 11){
                    CPFs[n] = '-';
                    n++;
                    j--;
                }else{
                    CPFs[n] = ch[j];
                    n++;
                }
                if(n == 14){
                    j = 3;
                }
            }
        }
        char *cpf = malloc(15);
        strcpy(cpf, CPFs); 
        return cpf; // Returns the CPF in String format
        }

        // Validate CPF int array
        int ValidateCPFia(int CPF[]){
        int i;
        for(i = 0; i < 4; i ++){
            if(i == 3 && CPF[i] > 99 || CPF[i] < 0){
                return FAILURE;
            }
            if(CPF[i] < 0 || CPF[i] > 999){
                return FAILURE;
            }
        }
        return SUCCESS;
        }

    // Checks in CPF inputted by User is true
    int ChkCPF(int CPF[]){

        if(!ValidateCPFia(CPF)){
            return FAILURE;
        }

        int i, multiplication = 0;
        bool verifier = false;

             /*    Numbers before '-'    */           /*    Numbers After '-'   */
        char CPF_Number_Before_Verifier[10] = "\000", CPF_Number_After_Verifier[3] = "\000";

        char CPFs[15];
        strcpy(CPFs, CPFiatos(CPF));

        // Checking if CPF is a 111.111.111-11 type
        if(CPFs[0] == CPFs[1] && CPFs[1] == CPFs[2] && CPFs[2] == CPFs[4] && CPFs[4] == CPFs[5] && CPFs[5] == CPFs[6] 
        && CPFs[6] == CPFs[8] && CPFs[8] == CPFs[9] && CPFs[9] == CPFs[10] && CPFs[10] == CPFs[12] && CPFs[12] == CPFs[13]){
            return FAILURE;
        }

        int n = 0;

        while(1){
            for(i = 0; i < 14; i++){
                if(CPFs[i] == '-'){
                    verifier = true;
                    n = 0;
                }
                if(CPFs[i] != '.' && CPFs[i] != '-' && verifier == false){
                    CPF_Number_Before_Verifier[n] = CPFs[i];
                    n++;
                }
                if(CPFs[i] != '-' && verifier == true){
                    CPF_Number_After_Verifier[n] = CPFs[i];
                    n++;
                }
            }

            for(i = 0; i < 9; i++){
                multiplication += (CPF_Number_Before_Verifier[i] - '0') * (10 - i);
            }

            multiplication = (multiplication * 10)%11;

            if(multiplication == 10){
                multiplication = 0;
            }
        
            if(multiplication != (CPF_Number_After_Verifier[0] - '0')){
                return 0;
            }
   
            multiplication = 0;
            for(i = 0; i < 9; i++){
                multiplication += (CPF_Number_Before_Verifier[i] - '0') * (11 - i);
            }

            multiplication += (CPF_Number_After_Verifier[0] - '0') * 2;

            multiplication = (multiplication * 10)%11;


            if(multiplication == 10){
                multiplication = 0;
            }

            if(multiplication != (CPF_Number_After_Verifier[1] - '0')){
                return FAILURE;
            }
            return SUCCESS;
        }
    }

    // Check if inputted name has especial characters or numbers
    int ChkName(char name[]){
        int len = strlen(name), i;
        for(i = 0; i < len; i++){
            if(!((name[i] >= 65 && name[i] <= 90) || (name[i] >= 97 && name[i] <= 122) || name[i] == 32 || name[i] == '\n')){ // Searches if the name has Special characters or Numbers
                return FAILURE;
            }
        }
        return SUCCESS; // Return 1 if no Special characters of Numbers were found
    }

    // Checks if Sex is 'M', 'F' or 'O'
    int ChkSex(char sex){
        
        if(sex == 'F' || sex == 'f' || sex == 'M' || sex == 'm' || sex == 'O' || sex == 'o'){ // Checks if inputted Sex is accepted
            return SUCCESS;
        }
        return FAILURE; // Returns 0 if sex is invalid
    }

#endif