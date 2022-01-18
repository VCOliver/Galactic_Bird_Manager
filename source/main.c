#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "birdlib.h"

#define FLIGHT_BUFFER 4
#define BOOKING_BUFFER 3



// Function to create Data Base Files
int creatFiles(){
    FILE *fpFlight, *fpBooking;
    fpFlight = fopen("Flight_Data_Base.txt", "a+"); // Opens the file to which the Flight data will be written
    fpBooking = fopen("Booking_Data_Base.txt", "a+"); // Opens the file to which the Booking data will be written
    if(!(fpFlight && fpBooking)){printf("\t----------ERROR!----------\n\nError opening the Data File. The program has been terminated\n\nPlease check if there are any programs impeding the creation of the Data Base Files\n\n"); // Checks if the files were created
        getch();
        return 0; // Returns 0 if it failed
    }else{

        fseek(fpFlight, 0, SEEK_END);
        int size = ftell(fpFlight); // Checks if Flight Data File is Empty and iniciates formatting
        if(size == 0){
            fputs("| Flight Number | Max Capacity |    Data    |   Time   |   Status   |\n\n", fpFlight);
        }

        fseek(fpBooking, 0, SEEK_END);
        size = ftell(fpBooking); // Checks if Flight Data File is Empty and iniciates formatting
        if(size == 0){
            fputs("| Booking Number | Flight Number |      CPF       | Sex | Birth Date |   Status   | Full Name\n\n", fpBooking);
        }
            fclose(fpFlight);
            fclose(fpBooking);
        return 1; // Returns 1 if succeeded
        } 
}

// Registering Options
void registerFlight(bool *null_flight){
    
    bool Same_Day = false; // For Flights bought for the same day

    Flight flight;
    int True_Input, chkscan;

    // Register Flight Number
    do{
        fflush(stdin);
        printf("Please inform the Flight Number(JEB-XXXX format, where X a number): ");
        chkscan = scanf("%[^-\n ]%*c%d", flight.number.letters, &flight.number.numbers); // User Input

        True_Input = ChkFlightNumber(flight.number.letters, flight.number.numbers);
        if(!True_Input){ // Checks if the Input is correct
            printf("\n\t\t--------ERROR--------\n\n--------Please input a valid Flight Number!--------\n\n");
        }else if(Find_String_In_File("Flight_Data_Base.txt", ntos(flight.number)) != 0){
            printf("\n\t\t--------ERROR--------\n\n--------Flight Number has already been registered!--------\n\n");
            True_Input = 0;
        }
    }while(True_Input != 1 || chkscan != 2);

    // Registers Flight's Max Capacity
    do{
        printf("Please inform the Flight Capacity: ");
        chkscan = scanf("%d", &flight.max_capacity); // User Input

        True_Input = ChkFlightCapacity(flight.max_capacity);
        if(!True_Input){ // Checks if the Input is correct
            printf("\n\t\t--------ERROR--------\n\n--------There are no Spacecrafts with the inputted Capacity!--------\n\n");
        }
    }while(True_Input != 1 || chkscan != 1);

    // Registers Flight Date
    do{
        printf("Please inform the Flight Date(DD/MM/YYYY format): ");
        chkscan = scanf("%d/%d/%d", &flight.date.day, &flight.date.month, &flight.date.year); // User Input

        True_Input = ChkDate(flight.date.day, flight.date.month, flight.date.year, &Same_Day, true);
        if(!True_Input){ // Checks if the Input is correct
            printf("\n\t\t--------ERROR--------\n\n--------Please inform an existing Date!--------\n\n");
            fflush(stdin);
        }
    }while(True_Input != 1 || chkscan != 3);

    // Registers Flight Time
    do{
        printf("Please inform the Flight Time(HH:MM format): ");
        chkscan = scanf("%d:%d", &flight.time.hour, &flight.time.minutes); // User Input

        True_Input = ChkTime(flight.time.hour, flight.time.minutes, &Same_Day);
        if(!True_Input){ // Checks if the Input is correct
            printf("\n\t\t--------ERROR--------\n\n--------Please inform an existing Time!--------\n\n");
        }
    }while(True_Input != 1 || chkscan != 2);

    // Registers Flight Status
    do{
        fflush(stdin);
        printf("Please inform the Flight Status(Active or Canceled): ");
        chkscan = fgets(flight.status, MAX_STATUS_SIZE, stdin); // User Input
        flight.status[strcspn(flight.status, "\n")] = 0;

        True_Input = ChkStatus(flight.status);
        if(!True_Input){ // Checks if the Input is correct
            printf("\n\t\t--------ERROR--------\n\n--------Please inform an accepted Status!--------\n\n");
        }

    }while(True_Input != 1 || chkscan == 0);

    // Writing to Data File
        FILE *fp;
        fp = fopen("Flight_Data_Base.txt", "a");

        fprintf(fp, "|   %s-%04d    |", flight.number.letters, flight.number.numbers); // Writes the Flight Number
        fprintf(fp, "     %03d      |", flight.max_capacity); // Writes the Flight's Max Capacity
        fprintf(fp, " %02d/%02d/%04d |", flight.date.day, flight.date.month, flight.date.year); // Writes the Flight Date
        fprintf(fp, "  %02d:%02d   |", flight.time.hour, flight.time.minutes); // Writes the Flight Time of Departure
        WrittingStatus(flight.status, &fp, true); // Writes the Flight Status
        fclose(fp);

    *null_flight = false;
    puts("\nFlight Register Completed! Press any key to continue\n"); // Informs User the Action was a success
    getch();
    
}

int registerBooking(bool *null_booking){

    Booking booking;
    int True_Input, chkscan;
    char c;

    // Inputs and validate User Booking number input
    do{
        fflush(stdin);
        printf("Please inform the Booking Number(GB-XXXXXX format, where X a number): ");
        chkscan = scanf("%[^-\n ]%*c%d", booking.number.letters, &booking.number.numbers); // User Input

        True_Input = ChkBookingNumber(booking.number.letters, booking.number.numbers);
        if(!True_Input){ // Checks if the Input is correct
            printf("\n\t\t--------ERROR--------\n\n--------Please input a valid Booking Number!--------\n\n");
        }else if(Find_String_In_File("Booking_Data_Base.txt", ntos(booking.number)) != 0){
            printf("\n\t\t--------ERROR--------\n\n--------Booking number has already been registered!--------\n\n");
            True_Input = 0;
        }
    }while(True_Input != 1 || chkscan != 2);

    // Inputs and validate User Flight number input
    do{
        fflush(stdin);
        printf("Please inform the Flight Number(JEB-XXXX format, where X a number): ");
        chkscan = scanf("%[^-\n ]%*c%d", booking.flight_number.letters, &booking.flight_number.numbers); // User Input

        True_Input = ChkFlightNumber(booking.flight_number.letters, booking.flight_number.numbers);
        if(!True_Input){ // Checks if the Input is correct
            printf("\n\t\t--------ERROR--------\n\n--------Please input a valid Flight Number!--------\n\n");
        }else if(!Find_String_In_File("Flight_Data_Base.txt", ntos(booking.flight_number))){
            printf("\n\t\t--------ERROR--------\n\n--------Inputted Flight does not exist!--------\n\n");
            puts("Do you wish to register a New Flight? (Y or N) ");
            fflush(stdin);
            c = getchar();
            if(c == 'Y' || c == 'y'){
                return 0; // Returns to Menu
            }
            True_Input = 0;

        }
        if(Flight_Has_Departed(ntos(booking.flight_number))){
            puts("\n\t\t--------ERROR--------\n\n--------Inputted Flight has already departed!--------\n\nDo you wish to return to the Menu? (Y or N) ");
            fflush(stdin);
            c = getchar();
            if(c == 'Y' || c == 'y'){
                return 0; // Returns to Menu
            }
            True_Input = 0;
        }
        if(Flight_Is_Full(ntos(booking.flight_number))){
            puts("\n\t--------ERROR--------\n\n--------Inputted Flight is Full!--------\n\nDo you wish to return to the Menu? (Y or N) ");
            fflush(stdin);
            c = getchar();
            if(c == 'Y' || c == 'y'){
                return 0; // Returns to Menu
            }
            True_Input = 0;
        }
        if(Flight_Is_Canceled(ntos(booking.flight_number))){
            puts("\n\t--------ERROR--------\n\n--------Inputted Flight is Canceled!--------\n\nDo you wish to return to the Menu? (Y or N) ");
            fflush(stdin);
            c = getchar();
            if(c == 'Y' || c == 'y'){
                return 0; // Returns to Menu
            }
            True_Input = 0;
        }
    }while(True_Input != 1 || chkscan != 2);

    // Inputs and validate User CPF input
    do{
        fflush(stdin);
        printf("Please inform your CPF(AAA.BBB.CCC-DD format): ");
        chkscan = scanf("%d.%d.%d-%d", &booking.CPF[0], &booking.CPF[1], &booking.CPF[2], &booking.CPF[3]); // User Input

        True_Input = ChkCPF(booking.CPF);
        if(!True_Input){ // Checks if the Input is correct
            printf("\n\t\t--------ERROR--------\n\n--------Please input a valid CPF!--------\n\n");
        }else if(Find_Two_Strings_In_File_Line("Booking_Data_Base.txt", ntos(booking.flight_number), CPFiatos(booking.CPF))){
            printf("\n\t\t--------ERROR--------\n\n--------This CPF has already been booked to this Flight!--------\n\n");
        }
    }while(True_Input != 1 || chkscan != 4);

    // Inputs and validate User name input
    do{
        fflush(stdin);
        printf("Please inform your Full Name: ");
        chkscan = fgets(booking.name, MAX_SIZE, stdin); // User Input
        booking.name[strcspn(booking.name, "\n")] = 0;

        True_Input = ChkName(booking.name);
        if(!True_Input){ // Checks if the Input is correct
            printf("\n\t\t--------ERROR--------\n\n--------I am quite sure you are not Elon Musk's son. Please input a name without special characters of numbers!--------\n\n");
        }
    }while(True_Input != 1 || chkscan == 0);

    // Inputs and validate User Sex input
    do{
        fflush(stdin);
        printf("Please inform your Sex(M, F, O): ");
        booking.sex = getchar(); // User Input

        True_Input = ChkSex(booking.sex);
        if(!True_Input){ // Checks if the Input is correct
            printf("\n\t\t--------ERROR--------\n\n--------Please input a valid Sex!--------\n\n");
        }
    }while(True_Input != 1);

    // Inputs and validate User Birth Date input
    do{
        fflush(stdin);
        printf("Please inform your Birth date(DD/MM/YYYY format): ");
        chkscan = scanf("%d/%d/%d", &booking.birth_date.day, &booking.birth_date.month, &booking.birth_date.year); // User Input

        True_Input = ChkDate(booking.birth_date.day, booking.birth_date.month, booking.birth_date.year, false, false);
        True_Input = ChkMininumFlightAge(booking.birth_date);
        if(!True_Input){ // Checks if the Input is correct
            printf("\n\t\t--------ERROR--------\n\n--------Please input a valid Date!--------\n\n");
        }
    }while(True_Input != 1 || chkscan != 3);

    // Inputs and validate User Status input
    do{
        fflush(stdin);
        printf("Please inform the Booking Status(Active or Canceled): ");
        chkscan = fgets(booking.status, MAX_STATUS_SIZE, stdin); // User Input
        booking.status[strcspn(booking.status, "\n")] = 0;

        True_Input = ChkStatus(booking.status);
        if(!True_Input){ // Checks if the Input is correct
            printf("\n\t\t--------ERROR--------\n\n--------Please inform an accepted Status!--------\n\n");
        }

    }while(True_Input != 1 || chkscan == 0);

    // Writes to Data File
        FILE *fp;
        fp = fopen("Booking_Data_Base.txt", "a");

        fprintf(fp, "|   %s-%06d    |", booking.number.letters, booking.number.numbers);
        fprintf(fp, "   %s-%04d    |", booking.flight_number.letters, booking.flight_number.numbers);
        fprintf(fp, " %03d.%03d.%03d-%02d |", booking.CPF[0], booking.CPF[1], booking.CPF[2], booking.CPF[3]);
        fprintf(fp, "  %c  |", toupper(booking.sex));
        fprintf(fp, " %02d/%02d/%02d |", booking.birth_date.day, booking.birth_date.month, booking.birth_date.year);
        WrittingStatus(booking.status, &fp, false); // Writes the Flight Status
        fprintf(fp, " %s\n", booking.name);
        fclose(fp);

    // Leaves Option
        *null_booking = false;
        puts("\nFlight Booking Completed! Press any key to continue\n"); // Informs User the Action was a success
        getch();


}

int consultFlight(){

    Number number;
    int True_Input;

    // Inputs and validate User Flight number input
    do{
        fflush(stdin);
        printf("Please inform the Flight Number(JEB-XXXX format, where A is an Uppercase Letter and X a number): ");
        scanf("%[^-\n ]%*c%d", number.letters, &number.numbers); // User Input
        puts("\n");

        True_Input = ChkFlightNumber(number.letters, number.numbers);
        if(!True_Input){ // Checks if the Input is correct
            printf("\n\t\t--------ERROR--------\n\n--------Please input a valid Flight Number!--------\n\n");
        }else if(!Find_String_In_File("Flight_Data_Base.txt", ntos(number))){ // Checks if Flight exists in Data Base
            printf("\n\t\t--------ERROR--------\n\n--------Inputted Flight does not exist!--------\n\n");
            puts("Do you wish to register a New Flight? (Y or N) ");
            fflush(stdin);
            char c = getchar();
            if(c == 'Y' || c == 'y'){
                return 0; // Returns to Menu
            }
            True_Input = 0;
        }
        
    }while(True_Input != 1);

    Print_Flight_Bookings(ntos(number));

}

int consultBooking(){

    Number number;
    int True_Input, chkscan;

    // Inputs and validate User Flight number input
    do{
        fflush(stdin);
        printf("Please inform the Booking Number(GB-XXXXXX format, where A is an Uppercase Letter and X a number): ");
        chkscan = scanf("%[^-\n ]%*c%d", number.letters, &number.numbers); // User Input
        puts("\n");

        True_Input = ChkBookingNumber(number.letters, number.numbers);
        if(!True_Input){ // Checks if the Input is correct
            printf("\n\t\t--------ERROR--------\n\n--------Please input a valid Flight Number!--------\n\n");
        }else if(!Find_String_In_File("Booking_Data_Base.txt", ntos(number))){ // Checks if Flight exists in Data Base
            printf("\n\t\t--------ERROR--------\n\n--------Inputted Booking does not exist!--------\n\n");
            puts("Do you wish to register a New Booking? (Y or N) ");
            fflush(stdin);
            char c = getchar();
            if(c == 'Y' || c == 'y'){
                return 0; // Returns to Menu
            }
            True_Input = 0;
        }
        
    }while(True_Input != 1 || chkscan != 2);

    Print_Booking(ntos(number));

    puts("\n\nPress any key to continue\n");
    getch();

}

int consultPassenger(){

    int CPF[4];
    int True_Input, chkscan;

    // Inputs and validate User Flight number input
    do{
        fflush(stdin);
        printf("Please inform the passenger's CPF(AAA.BBB.CCC-DD format): ");
        chkscan = scanf("%d.%d.%d-%d", &CPF[0], &CPF[1], &CPF[2], &CPF[3]); // User Input
        puts("\n");

        True_Input = ChkCPF(CPF);
        if(!True_Input){ // Checks if the Input is correct
            printf("\n\t\t--------ERROR--------\n\n--------Please input a valid Flight Number!--------\n\n");
        }else if(!Find_String_In_File("Booking_Data_Base.txt", CPFiatos(CPF))){ // Checks if CPF exists in Data Base
            printf("\n\t\t--------ERROR--------\n\n--------Inputted CPF has no booked Flights!--------\n\n");
            puts("Do you wish to register a New Booking? (Y or N) ");
            fflush(stdin);
            char c = getchar();
            if(c == 'Y' || c == 'y'){
                return 0; // Returns to Menu
            }
            True_Input = 0;
        }
        
    }while(True_Input != 1 || chkscan != 4);

    Print_Passenger(CPFiatos(CPF));

    puts("\nPress any key to continue\n");
    getch();

}

int cancelFlight(){

    Number number;
    int True_Input, chkscan;

   // Inputs and validate User Flight number input
    do{
        fflush(stdin);
        printf("Please inform the Flight Number(JEB-XXXX format, where A is an Uppercase Letter and X a number): ");
        chkscan = scanf("%[^-\n ]%*c%d", number.letters, &number.numbers); // User Input
        puts("\n");

        True_Input = ChkFlightNumber(number.letters, number.numbers);
        if(!True_Input){ // Checks if the Input is correct
            printf("\n\t\t--------ERROR--------\n\n--------Please input a valid Flight Number!--------\n\n");

        }else if(!Find_String_In_File("Flight_Data_Base.txt", ntos(number))){ // Checks if Flight exists in Data Base
            printf("\n\t\t--------ERROR--------\n\n--------Inputted Flight does not exist!--------\n\n");
            puts("Do you wish to register a New Flight? (Y or N) ");
            fflush(stdin);
            char c = getchar();
            if(c == 'Y' || c == 'y'){
                return 0; // Returns to Menu
            }
            True_Input = 0;
        }
        
    }while(True_Input != 1 || chkscan != 2); 

    if(!Modify_Flight_File(ntos(number), CANCEL)){
        puts("Do you wish to return to the Menu? (Y or N) ");
            fflush(stdin);
            char c = getchar();
            if(c == 'Y' || c == 'y'){
                return 0; // Returns to Menu
            }
            cancelFlight();
    }else{
        printf("Successfully canceled flight: %s\n\n", ntos(number));
        puts("Press any key to continue...\n");
        getchar();
    }
}

int cancelBooking(){

    Number number;
    int True_Input, chkscan;

   // Inputs and validate User Flight number input
    do{
        fflush(stdin);
        printf("Please inform the Booking Number(GB-XXXXXX format, where X a number): ");
        chkscan = scanf("%[^-\n ]%*c%d", number.letters, &number.numbers); // User Input

        True_Input = ChkBookingNumber(number.letters, number.numbers);
        if(!True_Input){ // Checks if the Input is correct
            printf("\n\t\t--------ERROR--------\n\n--------Please input a valid Booking Number!--------\n\n");
        }else if(!Find_String_In_File("Booking_Data_Base.txt", ntos(number))){ // Checks if Booking exists in Data Base
            printf("\n\t\t--------ERROR--------\n\n--------Inputted Booking does not exist!--------\n\n");
            puts("Do you wish to register a New Booking? (Y or N) ");
            fflush(stdin);
            char c = getchar();
            if(c == 'Y' || c == 'y'){
                return 0; // Returns to Menu
            }
            True_Input = 0;
        }
        
    }while(True_Input != 1 || chkscan != 2); 

    if(!Modify_Booking_File(ntos(number), CANCEL, false)){
        puts("Do you wish to return to the Menu? (Y or N) ");
            fflush(stdin);
            char c = getchar();
            if(c == 'Y' || c == 'y'){
                return 0; // Returns to Menu
            }
            cancelBooking();
    }else{
        printf("Successfully canceled booking: %s\n\n", ntos(number));
        puts("Press any key to continue...\n");
        getchar();
    }
}

int deleteFlight(){

    Number number;
    int True_Input, chkscan;

   // Inputs and validate User Flight number input
    do{
        fflush(stdin);
        printf("Please inform the Flight Number(JEB-XXXX format, where A is an Uppercase Letter and X a number): ");
        chkscan = scanf("%[^-\n ]%*c%d", number.letters, &number.numbers); // User Input
        puts("\n");

        True_Input = ChkFlightNumber(number.letters, number.numbers);
        if(!True_Input){ // Checks if the Input is correct
            printf("\n\t\t--------ERROR--------\n\n--------Please input a valid Flight Number!--------\n\n");

        }else if(!Find_String_In_File("Flight_Data_Base.txt", ntos(number))){ // Checks if Flight exists in Data Base
            printf("\n\t\t--------ERROR--------\n\n--------Inputted Flight does not exist!--------\n\n");
            puts("Do you wish to register a New Flight? (Y or N) ");
            fflush(stdin);
            char c = getchar();
            if(c == 'Y' || c == 'y'){
                return 0; // Returns to Menu
            }
            True_Input = 0;
        }
        
    }while(True_Input != 1 || chkscan != 2); 

    if(!Modify_Flight_File(ntos(number), DELETE)){
        puts("Do you wish to return to the Menu? (Y or N) ");
            fflush(stdin);
            char c = getchar();
            if(c == 'Y' || c == 'y'){
                return 0; // Returns to Menu
            }
            deleteFlight();
    }else{
        printf("Successfully Deleted flight: %s\n\n", ntos(number));
        puts("Press any key to continue...\n");
        getchar();
    }
}

// Menu and Options callings
void cases(int option, bool *null_flight, bool *null_booking){
    system("clear || cls");
    if(!(*null_flight == false)){
        *null_flight = ChkNullFile("Flight_Data_Base.txt"); // Checks if there are not Flights already created in the Data File
    }
    if(!(*null_booking == false)){
        *null_booking = ChkNullFile("Booking_Data_Base.txt"); // Checks if there are not Flights already created in the Data File
    }
    
    switch(option){
        case 1:
            printf("---------------------------------------------\n");
            printf("\t--------REGISTER FLIGHT-------\n");
            printf("---------------------------------------------\n\n");
            registerFlight(null_flight);
            break;
        case 2:
            if(*null_flight){ // Checks if there are any Registered Flights
                system("clear || cls");
                printf("There are no registered Flights!\n");
                puts("\nPress any key to continue\n");
                getch();
            }else{ // If there are Flights registered, allows User to input Bookings
                printf("----------------------------------------------\n");
                printf("\t-------- REGISTER BOOKING -------\n");
                printf("----------------------------------------------\n\n");
                registerBooking(null_booking);
            }
            break;
        case 3:
            if(*null_flight && *null_booking){ // Checks if there are any Registered Flights
                system("clear || cls");
                printf("There are no registered Flights or Bookings!\n");
                puts("\nPress any key to continue\n");
                getch();
            }else{ // If there are Flights registered, allows User to input Bookings
                printf("-------------------------------------------\n");
                printf("\t-------- CONSULT FLIGHT -------\n");
                printf("-------------------------------------------\n\n");
                consultFlight();
            }
            break;
        case 4:
            if(*null_flight && *null_booking){ // Checks if there are any Registered Flights
                system("clear || cls");
                printf("There are no registered Flights or Bookings!\n");
                puts("\nPress any key to continue\n");
                getch();
            }else{ // If there are Flights registered, allows User to input Bookings
                printf("---------------------------------------------\n");
                printf("\t-------- CONSULT BOOKING -------\n");
                printf("---------------------------------------------\n\n");
                consultBooking();
            }
            break;
        case 5:
            if(*null_flight && *null_booking){ // Checks if there are any Registered Flights
                system("clear || cls");
                printf("There are no registered Flights or Bookings!\n");
                puts("\nPress any key to continue\n");
                getch();
            }else{ // If there are Flights registered, allows User to input CPF
                printf("-------------------------------------------------\n");
                printf("\t-------- CONSULT PASSENGER -------\n");
                printf("-------------------------------------------------\n\n");
                consultPassenger();
            }
            break;
        case 6:
            if(*null_flight){ // Checks if there are any Registered Flights
                system("clear || cls");
                printf("There are no registered Flights!\n");
                puts("\nPress any key to continue...\n");
                getch();
            }else{ // If there are Flights registered, allows User to input CPF
                printf("------------------------------------------------\n");
                printf("         -------- CANCEL FLIGHT -------\n");
                printf("------------------------------------------------\n\n");
                cancelFlight();
            }
            break;
        case 7:
            if(*null_flight){ // Checks if there are any Registered Flights
                system("clear || cls");
                printf("There are no registered Flights!\n");
                puts("\nPress any key to continue...\n");
                getch();
            }else{ // If there are Flights registered, allows User to input CPF
                printf("------------------------------------------------\n");
                printf("       -------- CANCEL BOOKING -------\n");
                printf("------------------------------------------------\n\n");
                cancelBooking();
            }
            break;
        case 8:
            if(*null_flight){ // Checks if there are any Registered Flights
                system("clear || cls");
                printf("There are no registered Flights!\n");
                puts("\nPress any key to continue...\n");
                getch();
            }else{ // If there are Flights registered, allows User to input CPF
                printf("------------------------------------------------\n");
                printf("        -------- DELETE FLIGHT -------\n");
                printf("------------------------------------------------\n\n");
                deleteFlight();
            }
            break;
        case 9:
            printf("------------------------------------------");
            printf("\nThanks for flying Galactic! See you soon!\n");
            printf("------------------------------------------\n");
            break;
        default:
            printf("----------ERROR!----------\nInvalid option, please choose a valid number\n");
    }
}

int menu(){
    char option;

    system("clear || cls");

    puts("---------------------------------------");
    puts("\t-------- MENU ---------");
    puts("---------------------------------------\n");


    printf("Choose an option:\n");
    printf("1 - Register Flight \n");
    printf("2 - Register Booking \n");
    printf("3 - Consult Flight\n");
    printf("4 - Consult Booking\n");
    printf("5 - Consult Passenger\n");
    printf("6 - Cancel Flight\n");
    printf("7 - Cancel Booking\n");
    printf("8 - Delete Flight\n");
    printf("9 - EXIT\n\n");
    fflush(stdin);   
    option = getch();
    while(!isdigit(option) || option == '0'){
        puts("\nOption not existent. Please choose another option!\n");
        fflush(stdin);
        option = getch();
    }

return option - '0';    
}

// Main
int main(void){

    int files_created = creatFiles();
    if(!files_created){exit(EXIT_FAILURE);} // Terminates programs if files are not created

    // Declaring main variables
    bool null_flight = true, null_booking = true;
    int option;

    // Main loop
    do{
        option = menu(); // Opens the menu for the User
        cases(option, &null_flight, &null_booking); // Checks what option was chosen by the User
        if(option != 9){
            fflush(stdin); // Cleans input buffer
        } 
    }while(option != 9);
    
}

