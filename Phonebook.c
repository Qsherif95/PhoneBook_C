
/***************************************************************
Author: Elsherif Mahmoud
Section: 502
Date: 4/11/2015
Description:This is the source code for the final project application for ECEN210.
This is a Phonebook application designed to create,store,copy,delete,search and display list of contacts.
Further explanation of the building blocks of the application is provided on top of each function.
****************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
/* List of defined aliases  */
#define CPUFACTOR 3.75
#define load 1 //for "Options" function
#define close 0 //for "Options" function
#define Project_Title -1 //for "Options" function
#define yes 1 //for "isSure" function
#define no 0//for "isSure" function

#define StringLength 40 //maximum number of characters per contact entry
#define capacity 100 //maximum umber of contacts


FILE *fRead;//pointer for reading the contact list file
FILE *fWrite;//pointer for writing the contact list file
int counter=0;/*Counter for entries. it is set global due to multiple references.*/
int contact_matched[capacity];/*array of indexes for matched contacts.
it is set global due to multiple references.It's used for SearchForContact function*/
/////////////

/*Definition of a data structure of type phonebook_record
which is abbreviated to "record"*/
typedef  struct Phonebook_records{
      int exist;/*contact existence (when 1) or removal(when0) signal*/
      char FirstName[StringLength];
      char LastName[StringLength];
      char EmailAdress[StringLength];
   }record;/*structure type*/

void main(){
    system("cls");
    record *phonebook[capacity];//initialize the instance
    load_data(phonebook);//loading the file containing the contact list
    int task;
    transition(Project_Title);//displays the project title message
    while (1){
        ResetMatches();//reseting the array of indexes of the matched results
        system("cls");
        header();//the interface
        scan_int(&task);
        switch (task){
            case 0:/**about me**/
                transition(load);//transition function //Project title
                aboutMe();//About Me
                system("pause");
                transition(close);
                break;
            case 1:/**Add contact**/
                transition(load);//transition function
                AddContact(phonebook);//New contact function
                system("pause");
                transition(close);//transition function //closing
                break;
            case 2:/**Search for contact**/
                transition(load);//transition function
                SearchForContact(phonebook);//Search function
                options(phonebook);//options function [delete-copy]
                transition(close);
                break;
            case 3:/**List all contacts**/
                transition(load);//transition function
                ListAllContacts(phonebook);
                options(phonebook);//options function [delete-copy]
                transition(close);
                break;
            case 4:/**delete all contacts**/
                DeleteAllContacts(phonebook);//Delete All
                system("pause");
                break;
            case 5:/**Save changes**/
                if(AreYouSure("save the changes")==1){save_data(phonebook);}
                delay(1,50);
                break;
            case 6:/**exit**/
                if(AreYouSure("save the changes")==1){save_data(phonebook);};

                if(AreYouSure("exit")==1){
                    transition(close);
                    return;
                }

        }

    }
}

/******************>>>The functions in the order of their usage<<<***********************/

/*This function must be at the start of the program
It loads all the contact and stores them in the array phonebook for processing*/
void load_data(record * phonebook){
    int i;
    char tempContainer[StringLength*3];//this will temperorally carry strings on each line of the file
    fRead = fopen("phonebook_contacts.dat", "r");
   /*This error will happen the first time you run the program,because the file won't have been created*/
   if ( fRead == NULL){
        printf("\n\n\t\t\t***Creating a File***");
        delay(2,0);
        fWrite=fopen("phonebook_contacts.dat", "w");//Created Initially
        fclose(fWrite);
   }
   else{
        //Reads line by line until the end of file
        for(i=0;feof(fRead)==0 ;i++){
            fgets (tempContainer, StringLength, fRead);
            sscanf(tempContainer,"%d\t%s\t%s\t%s\n",&phonebook[i].exist,&phonebook[i].FirstName, &phonebook[i].LastName, &phonebook[i].EmailAdress);
        }
   }
   counter=i-1;//Because there's always an empty line at the beginning of the file
   fclose(fRead);
}
/**************************************************************/
/*This function resets all the contact_matched array indexes for matches to zero So that the array can be reusable*/
void ResetMatches(void){
    int k;for (k=0;k<capacity;k++){contact_matched[k]=-1;}
}
/**************************************************************/
/*This function displays a transition message (e.g. loading, closing etc.) */
void transition(int type){
    system("cls");
    char option;int i;
    printf("\n\n\n\n\n               ");
    if (type==close){//closing
        char message[]="* * * * * * * * C L O S I N G * * * * * * *";
        int x = strlen(message);
        for (i=0;i<=x;i++)
        {
            printf("%c",message[i]);
            delay(0,02);
        }
    }
    if (type==Project_Title){//Project
        char message[]="\n\n\n\t\t**Final Project For ECEN 210 course | Spring'16 **";
        int x = strlen(message);
        for (i=0;i<=x;i++)
        {
            printf("%c",message[i]);
            delay(0,03);
        }
        delay(2,0);
    }
    if (type==load){//loading
        char message[]="* * * * * * * * L O A D I N G * * * * * * *";
        int x = strlen(message);
        for (i=0;i<=x;i++)
        {
            printf("%c",message[i]);
            delay(0,02);
        }
    }
    delay(0,50);
    system("cls");
}
/**************************************************************/
/*This function displays the main user interface */
int header(){
    system("cls");
    printf("                 ******* P H O N E   B O O K ********      \n");
    printf("\n\n\n  About Me                 (0)\n");
    printf(      "  Add New Contact          (1)\n");
    printf(      "  Search For a Contact     (2)\n");
    printf(      "  List All Contacts        (3)\n");
    printf(      "  Delete All Contacts      (4)\n");
    printf(      "  Save Changes             (5)\n");
    printf(      "  Close The Application    (6)\n\n");
    printf("So, What do you feel like doing? >> ");
}
/**************************************************************/
/*This function checks the scanned integers and performs error analysis*/
int scan_int(int* value){
    int status,flag=0;
    while(flag!=1){
        status=scanf("%d",value);
        fflush(stdin);
        if (status==0) {
            printf("\n\n\t***********  ERROR..Enter a Number Please ***********\n");
            printf("\n\t>>");

        }
        else {flag=0;return value;}
    }
}
/**************************************************************/
/*This function freezes the compiler for specified time.
it takes two inputs (number of integer seconds, fractional seconds)
e.g. delay(1,33) delays the program by 1.33 seconds*/
void delay(unsigned  time,unsigned decimal){
    int i,j;
    float fraction=(float)(decimal)/100;//for milliseconds
    for (j=0;time!=0 && decimal!=0 && j<time;j++)//e.g. 1.35 second
        for (i=0;i<CPUFACTOR*100000000*(1+fraction);i++);
    for (j=0;time!=0 && decimal==0 && j<time;j++) //e.g. 1.00 second
        for (i=0;i<CPUFACTOR*100000000;i++);
    if (time==0 && decimal!=0) //e.g. 0.21 second
        for (i=0;i<CPUFACTOR*100000000*fraction;i++);
}
/**************************************************************/
/*This function displays my info*/
void aboutMe(){
    system("cls");
    printf("\n\n\n\n         Elsherif Mahmoud '17 \n\n         Electrical & Computer Engineering | Student \n\n         Texas A&M University at Qatar \n\n\n\n\n\n\n\n\n\n\n\n\n\n         ");
}
/**************************************************************/
/*This function adds a new contact to the array of phonebook
It asks the user to enter the First and Last Names as well as the email address. It then creates a signal of existents (x.exists=1)  */
void AddContact (record * phonebook){
    printf("\n\t        ********* A D D  a  C O N T A C T *********      \n");
    printf("\n\t         >>>>>> Your Memory Usage is %d%% <<<<<<      \n\n",(counter*100)/capacity);
    int i,k;
    record TempContact;//temporary instant, until the user confirms
    counter++;//number of contact increases by 1
    i=counter-1;/*index for values assignment.
    it will keep its value unless there exists a contact with
    removal signal (x[k].exist==0), then i=k and the counter should not increment
    as the memory is the same*/
    for (k=0;k<counter;k++){
        if (phonebook[k].exist==0){--counter;i=k;break;}
    }
    ///////////////USER Prompt//////////////////
    printf("\n\tFirst Name: ");
    scanf("%s", TempContact.FirstName);
    printf("\tLast Name: ");
    scanf("%s", TempContact.LastName);
    printf("\tEmail: ");
    scanf("%s", TempContact.EmailAdress);
    /*USER CONFIRMATION REQUEST*/
    if(AreYouSure("Add Contacts")==1){
        printf("\n\t         $$$$$$$$$$ Contact Successfully Added $$$$$$$$$\n\n\t");
        phonebook[i].exist=1;
        strcpy(phonebook[i].FirstName,TempContact.FirstName);
        strcpy(phonebook[i].LastName,TempContact.LastName);
        strcpy(phonebook[i].EmailAdress,TempContact.EmailAdress);
    }
    else{//if the user doesn't ant to add
        printf("\n\t No contacts will be added...\n\t");
        --counter;//the counter must be reset to the previous value
    }

}
/**************************************************************/
/*Tis function asks if the user is sure of the operation he commanded*/
int AreYouSure(char x[]){
    char isSure='y';
    printf("\tAre you sure you want to %s? y/n \n\t>>",x);
    fflush(stdin); //flushing carriage return
    scanf("%c",&isSure);
    if (isSure=='y'||isSure=='Y'){return(yes);}// if not sure
    else if (isSure=='n'||isSure=='N'){return(no);}
    else {AreYouSure(x);}

}

/**************************************************************/
/*This function is executed after listing All or searched contacts.
The user is asked to select the contact of interest.
The user is then asked to select the action to be performed on the contact:
1)go to homepage
2)Copy the contact's email to clipboard
3)Delete the contact
4)Back to the list of contact*/
void options(record * phonebook){
    char action;int i;fflush(stdin);
    int k; //index for match of interest
    //if there's no contacts/ no matches
    if (contact_matched[0]==-1){system("pause");return;}

    /**____________Contact of interest____________**/
    //This is executed  if  there's more than 1 match
    if (contact_matched[1]!=-1){
        printf("\n\t**Enter 0 to go back**\n\tYour contact of interest is #");
        scan_int(&k);//scanning the user choice of the contact
        i=contact_matched[k-1];//index of the selected matched query
        if (k==0){
            return;
        }
        else if (i==-1){//checking that the user chose a number within the range
            printf("\n\t****ERROR..Select From The contacts On The List*****\n\n\t");
            options(phonebook);
        }
        else if(phonebook[i].exist==0){//checking that the user didn't choose a contact he already deleted
            printf("\n\t****ERROR...you have already deleted this contact****\n\n\t");
            options(phonebook);
        }
        printf("\n\t *[ %s %s's email is: %s ]*\n",phonebook[i].FirstName, phonebook[i].LastName, phonebook[i].EmailAdress);
    }else{i=contact_matched[0];}//index of the only matched contact

    /**______________Action Type________________**/

    printf("\n\n\t *Homepage(H)   *Copy(C)  *Delete(D)  *Select another(S)\n\t>>");
    fflush(stdin);
    scanf("%c",&action);
    if (action=='H'||action=='h'){//go to home screen
        return;
    }
    else if (action=='C'||action=='c'){//copy to clipboard
        copy_to_clipboard(&phonebook[i].EmailAdress);
        options(phonebook);
    }
    else if (action=='D'||action=='d'){//delete the contact
        DeleteContact(phonebook,i);
        options(phonebook);
    }
    else if (action=='S'||action=='s'){//go back to the list of all matched contacts
        options(phonebook);
    }
    else {
        printf("\n\tplease select one of the options!\n\n");
        options(phonebook);
    }
}
/**************************************************************/
/*This function copies the email of the contact to the clipboard of the user*/
void copy_to_clipboard(char * email){
    const size_t len = strlen(email) + 1;
    /*************BLACK BOX*****************/
    /**/HGLOBAL hMem =  GlobalAlloc(GMEM_MOVEABLE, len);
    /**/memcpy(GlobalLock(hMem), email, len);
    /**/GlobalUnlock(hMem);
    /**/OpenClipboard(0);
    /**/EmptyClipboard();
    /**/SetClipboardData(CF_TEXT, hMem);
    /**/CloseClipboard();
    /**************************************/

    printf("\n\t***Email copied Successfully***\n");
    delay(1,0);
}
/**************************************************************/
/*This function deletes specific contact. It takes two inputs
the phonebook instance array and the specific index.
It then sends a removal signal by assigning 0 to "exist" the parameter (i.e.x[i].exist=0)*/
void DeleteContact (record * phonebook,int i){

    if(AreYouSure("delete this contact")==yes){
        //sending a signal of removal
        phonebook[i].exist=0;
        printf("\n\t $$$$Contact Deleted, He wasn't nice anyways!!\n\t");
        printf("\n\t ---------------------------------------------\n\t");
    }
    else{
        printf("\n\t$$$ oops.. my bad !!\n\t");
        printf("\n\t ---------------------------------------------\n\t");
    }
}
/**************************************************************/
/*This function searches for contact by matching first and last names [CASE SENSITIVE]
It takes two inputs 1)phonebook array 2)action type (0 (delete) or 1 (display))  */
void SearchForContact(record *phonebook){
    int i;//index for contacts
    int j=0;//counter for matches
    int firstNameMatch,lastNameMatch;
    char QueryFirstName[StringLength];//search query
    char QueryLastName[StringLength];//search query
    ///////////////InterFace//////////////////
    printf("\n\t            ******* S E A R C H   F O R   C O N T A C T *******      \n");
    printf("\n\n\n\n\tPlease type the name of the contact");
    printf("\n\n\tFirst Name: ");
    scanf("%s",&QueryFirstName);//First Name Query
    printf("\tLast Name: ");
    scanf("%s",&QueryLastName);//Last Name Query
    ////////////////////////////////////////
    for (i=0;i<counter;i++){
        //Case Sensitive comparison
        firstNameMatch=strcmp(QueryFirstName, phonebook[i].FirstName) == 0;//1 if matches 0 otherwise
        lastNameMatch=strcmp(QueryLastName, phonebook[i].LastName) == 0;//1 if matches 0 otherwise
        /*&&&&&&&&&&&&&&& Display &&&&&&&&&&&&&&&&&&&&*/
        if (phonebook[i].exist==1 && firstNameMatch && lastNameMatch){
            contact_matched[j]=i;/*reserving the index of the matched contact
            for further references */
            j++;//counter for matches
            printf("\n\t (%d)\n\t %s %s's email is: %s\n\n",j,phonebook[i].FirstName, phonebook[i].LastName, phonebook[i].EmailAdress);
        }
    }//for loop
    if (j==0){printf("\n\n\t*********** No Matches Found ***********\n\n\t");}
    else if(j>1) {//if there's more than 1 match
        printf("\t*********** %d Matches Found ***********\n\n\t",j);
    }
}
/**************************************************************/
/* This function lists all "existing" contacts*/
void ListAllContacts(record * phonebook){
    int i,j=0;
    printf("\n\t            ********* A L L   C O N T A C T S ********      \n");
    for(i=0;i<counter;i++){
        if (phonebook[i].exist==1){
            contact_matched[j]=i;/*reserving the index of the matched contact*/
            j++;
            printf("\n\t (%d)\n\t %s %s's email is: %s\n\n\t",j,phonebook[i].FirstName, phonebook[i].LastName, phonebook[i].EmailAdress);
        }
    }
    printf("\n\n\t\t\t  ***You have %d Contacts***\n\n",j);
    //if no contacts
    if (j==0){printf("\n\n\t*********** It doesn't mean I'm lonely when I'm alone ***********\n\n\t");}
}
/**************************************************************/
/*This function deletes all the contacts by
sending a removal signal (x.exist=0) to all contact*/
void DeleteAllContacts(record * phonebook){
    int i;
    if (AreYouSure("delete ALL contacts")==1){
        for(i=0;i<counter;i++){phonebook[i].exist=0;}
        printf("\n\t\t***********I KILLED THEM ALL !!!***********\n\n\t");
    }
    else{
        printf("\n\t$$$ oops.. my bad !!\n\n\t");
    }
}
/**************************************************************/
/*This function stores the updated contacts list in a file ready for retrieval after closing the application.
**It only stores the contacts which have existence signal of 1 (x.exist=1)*/
void save_data(record * phonebook){
    int i=0;
    fWrite = fopen("phonebook_contacts.dat", "w");
    if ( fWrite == NULL ){
        perror("The following error occurred ");
        exit(EXIT_FAILURE);
    }
    else{
        for (i=0;i<counter;i++){
            if(phonebook[i].exist==1)
                fprintf(fWrite, "%d\t%s\t%s\t%s\n",phonebook[i].exist,phonebook[i].FirstName, phonebook[i].LastName, phonebook[i].EmailAdress);
        }

    }
    printf("\n\t****Data Saved****\n\n");
    fclose(fWrite);
}
