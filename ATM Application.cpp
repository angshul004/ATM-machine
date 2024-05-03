#include<stdio.h>
#include<stdlib.h>
#include<time.h>

void atm(int *balance, int *pintemp);
void deposit(int *balance);
void withdraw(int *balance);
void chbal(int *balance);
void change_pin(int *pin);
int viw_statement();

int main(){
    int pintemp, pin, chance=0, balance=50000;
    printf("Welcome to ATM!\n");

    FILE *pinfile = fopen("pinfile.txt", "r");              //pin file
    if (pinfile == NULL) {
        perror("Error opening pinfile.txt");
        return 1;
    }
    if (fscanf(pinfile, "%d", &pintemp) != 1) {
        perror("Error reading pin from pinfile.txt");
        fclose(pinfile);
        return 1;
    }
    fclose(pinfile);
    
        
    while (chance < 3) {
        printf("Enter your pin: \n");
        scanf("%d", &pin);
        if (pin == pintemp) {
            printf("\nWelcome!\n\n");
            atm(&balance, &pintemp);
            break;
        } 
        else {
            printf("\nIncorrect Pin\n");
            chance++;
            if (chance >= 3) {
                printf("Maximum attempts reached.\n");
                break;
            }
        }
    }   
    printf("\nThank you!");
    return 0;
}

void atm(int *balance, int *pintemp){
    int c;
    char choice;
    do {
        printf("Please select your choice:\n");
        printf(" Deposit - press 1\n Withdraw - press 2\n Check Balance - press 3\n Change Pin - press 4\n View statement - press 5\n");
        scanf("%d",&c);
        switch(c){
            case 1:
                deposit(balance);
                break;
            case 2:
                withdraw(balance);
                break;
            case 3:
                chbal(balance);
                break;
            case 4:
                change_pin(pintemp);
                break;
            case 5:
                viw_statement();
                break;                                 
            default:
                printf("Invalid choice. Enter between 1 to 5.\n");
        }
        printf("\nDo you want to continue? (y/n):\n");
        scanf(" %c", &choice);
        system("cls");
    } while(choice == 'y' || choice == 'Y');
}

void deposit(int *balance){
    int amt;
    printf("Enter amount to deposit: ");
    scanf("%d",&amt);
    *balance+=amt;
    
    time_t t = time(NULL);
    struct tm *date = localtime(&t);  
    printf("Amount deposited on %d-%02d-%02d at %02d:%02d\n", date->tm_year + 1900, date->tm_mon + 1, date->tm_mday, date->tm_hour, date->tm_min);
    
    FILE *statement = fopen("statement.txt", "a");
    if (statement == NULL) {
        perror("Error opening statement.txt for appending");
    } else {
        fprintf(statement, "\nDate: %d-%02d-%02d Time: %02d:%02d \t RS. %d Cr",date->tm_year + 1900, date->tm_mon + 1, date->tm_mday, date->tm_hour, date->tm_min,amt);
        fclose(statement);
    }
}

void withdraw(int *balance){
    int amt;
    printf("Enter amount to withdraw: ");
    scanf("%d",&amt);
    if(amt > 25000){
        printf("You can withdraw maximum rs. 25000");
    } else {
        if (amt > *balance) {
            printf("Insufficient balance.\n");
        } else {
            *balance -= amt;
            time_t t = time(NULL);
            struct tm *date = localtime(&t);  
            printf("Amount withdrawn on %d-%02d-%02d at %02d:%02d\n", date->tm_year + 1900, date->tm_mon + 1, date->tm_mday, date->tm_hour, date->tm_min);
            
            FILE *statement = fopen("statement.txt", "a");
            if (statement == NULL) {
                perror("Error opening statement.txt for appending");
            } else {
                fprintf(statement, "\nDate: %d-%02d-%02d Time: %02d:%02d \t RS. %d Dr",date->tm_year + 1900, date->tm_mon + 1, date->tm_mday, date->tm_hour, date->tm_min,amt);
                fclose(statement);
            }
        }
    }
}

void chbal(int *balance){
    printf("Your balance is: %d\n",*balance);
}

void change_pin(int *pintemp){
    int prepin,newpin;
    printf("\nEnter previous pin first: ");
    scanf("%d",&prepin);
    
    if(prepin==*pintemp){
        printf("Enter new pin: ");
        scanf("%d",&newpin);
        FILE *fptr = fopen("pinfile.txt", "w");
        if (fptr == NULL) {
            perror("Error opening pinfile.txt for writing");
        } else {
            fprintf(fptr, "%d", newpin);
            fclose(fptr);
            
            time_t t = time(NULL);
            struct tm *date = localtime(&t);  
            printf("Pin changed successfully on %d-%02d-%02d at %02d:%02d\n", date->tm_year + 1900, date->tm_mon + 1, date->tm_mday, date->tm_hour, date->tm_min);
        }
    }
    else{
        printf("Incorrect previous pin. Pin remains unchanged.\n");
    }   
}

int viw_statement(){
    FILE *fptr = fopen("statement.txt", "r");
    char myString[100];
    if(fptr != NULL) {
        while(fgets(myString, 100, fptr)) {
            printf("%s", myString);
        }
        fclose(fptr);
    } else {
        perror("Error opening statement.txt for reading");
    }
    return 0;   
}
