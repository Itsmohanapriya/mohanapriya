#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "contact.h"

/* Function definitions */
int validate_name(char Name[])
{
    if (isdigit(Name[0]) != 0)
    {
        return 0;
    }
    for (int i = 0; Name[i] != '\0'; i++)
    {
        if (isalnum(Name[i]))
        {
            return 1;
        }
    }
    return 0;
}
int validate_Mobile(char Mobile_number[])
{
    if (strlen(Mobile_number) != 10)
    {
        return 0;
    }
    if(Mobile_number[0] >= '0' && Mobile_number[0] <= '5')  
    {
        return 0;
    }
    for (int i = 0; Mobile_number[i] != '\0'; i++)
    {
        if (!isdigit(Mobile_number[i]))
        {
            return 0;
        }
        return 1;
    }
    
    return 0;
}
int validate_Mail(char Mail_ID[])
{
    int len = strlen(Mail_ID);

    if (isdigit(Mail_ID[0])) 
        return 0;

    for (int i = 0; Mail_ID[i] != '\0'; i++) 
    {
        if (isupper(Mail_ID[i])) 
            return 0;
    }

    int at_count = 0;
    for (int i = 0; Mail_ID[i] != '\0'; i++) 
    {
        if (Mail_ID[i] == '@') 
            at_count++;
    }
    if (at_count != 1) 
        return 0;

    int dotcom_count = 0;
    char *ptr = Mail_ID;
    while ((ptr = strstr(ptr, ".com")) != NULL) 
    {
        dotcom_count++;
        ptr += 4;
    }
    if (dotcom_count != 1) 
        return 0;

    if (len < 4 || strcmp(Mail_ID + len - 4, ".com") != 0) 
        return 0;

    char *at_ptr = strchr(Mail_ID, '@');
    char *dotcom_ptr = strstr(Mail_ID, ".com");

    if (at_ptr == NULL || dotcom_ptr == NULL)
        return 0;

    if (dotcom_ptr - at_ptr <= 1)
        return 0;

    return 1;
}

int is_unique(struct Address_book *addressbook, char *Mobile_no, char *eMailid)
{
    for (int i = 0; i < addressbook->contact_count; i++)
    {
        if (Mobile_no != NULL && strcmp(addressbook->contact_details[i].Mobile_number, Mobile_no) == 0)
            return 1;
        if (eMailid != NULL && strcmp(addressbook->contact_details[i].Mail_ID, eMailid) == 0)
            return 1;
    }
    return 0;
}

void init_intitalization(struct Address_book *addressbook)
{
    addressbook->contact_count = 0;
}
int create_contact(struct Address_book *addressbook)
{
    char Name[20];
    char Mobileno[11];
    char eMailid[30];
    while (1)
    {
        printf("Enter a Name:");
        scanf(" %[^\n]", Name);
        if (validate_name(Name))
        {
            strcpy(addressbook->contact_details[addressbook->contact_count].Name, Name);
            break;
        }
        else
            printf("Invalid name,Try again\n");
    }
    while (1)
    {
        printf("Enter Phone number: ");
        scanf(" %s", Mobileno);
        if (validate_Mobile(Mobileno) && !is_unique(addressbook,Mobileno, NULL))
        {
            strcpy(addressbook->contact_details[addressbook->contact_count].Mobile_number, Mobileno);
            break;
        }
        else
            printf("Invalid mobile number,Try again\n");
    }
    while (1)
    {
        printf("Enter email id: ");
        scanf(" %s", eMailid);
        if (validate_Mail(eMailid) && !is_unique(addressbook,NULL, eMailid))
        {
            strcpy(addressbook->contact_details[addressbook->contact_count].Mail_ID, eMailid);
            break;
        }
        else
            printf("Invalid Mail ID,Try again\n");
    }
    addressbook->contact_count++;
    printf("Contact created successfully\n");
    return 0;
}
void list_contacts(struct Address_book *addressbook)
{
    if (addressbook->contact_count == 0)
      printf("Contact not found");
    for (int i = 0; i < addressbook->contact_count; i++)
    {
        printf("Contact %d:\n", i+1);
        printf("Name: %s\n", addressbook->contact_details[i].Name);
        printf("Mobile Number: %s\n", addressbook->contact_details[i].Mobile_number);
        printf("Email id: %s\n", addressbook->contact_details[i].Mail_ID);
    }
}

int search_contacts(struct Address_book *addressbook)
{
    char keyword[30];
    int option;

    scanf("%d", &option);

    if (option == 1)
    {
        while (1)
        {
            printf("Enter the Name which you want to search: ");
            scanf(" %[^\n]", keyword);

            if (validate_name(keyword))
            {
                for (int i = 0; i < addressbook->contact_count; i++)
                {
                    if (strcmp(addressbook->contact_details[i].Name, keyword) == 0)
                    {
                        printf("\nContact found!!!\n");
                        printf("Name: %s\n", addressbook->contact_details[i].Name);
                        printf("Mobile Number: %s\n", addressbook->contact_details[i].Mobile_number);
                        printf("Email ID: %s\n", addressbook->contact_details[i].Mail_ID);
                        return i;
                    }
                }
                printf("No contact found with that name.\n");
                return -1;
            }
            else
            {
                printf("Invalid name, Try again!\n");
            }
        }
    }

    else if (option == 2)
    {
        while (1)
        {
            printf("Enter the Mobile number which you want to search: ");
            scanf(" %[^\n]", keyword);

            if (validate_Mobile(keyword))
            {
                for (int i = 0; i < addressbook->contact_count; i++)
                {
                    if (strcmp(addressbook->contact_details[i].Mobile_number, keyword) == 0)
                    {
                        printf("\nContact found!!!\n");
                        printf("Name: %s\n", addressbook->contact_details[i].Name);
                        printf("Mobile Number: %s\n", addressbook->contact_details[i].Mobile_number);
                        printf("Email ID: %s\n", addressbook->contact_details[i].Mail_ID);
                        return i;
                    }
                }
                printf("No contact found with that mobile number.\n");
                return -1;
            }
            else
            {
                printf("Invalid Mobile number, Try again!\n");
            }
        }
    }

    else if (option == 3)
    {
        while (1)
        {
            printf("Enter the Mail ID which you want to search: ");
            scanf(" %[^\n]", keyword);

            if (validate_Mail(keyword))
            {
                for (int i = 0; i < addressbook->contact_count; i++)
                {
                    if (strcmp(addressbook->contact_details[i].Mail_ID, keyword) == 0)
                    {
                        printf("\nContact found!!!\n");
                        printf("Name: %s\n", addressbook->contact_details[i].Name);
                        printf("Mobile Number: %s\n", addressbook->contact_details[i].Mobile_number);
                        printf("Email ID: %s\n", addressbook->contact_details[i].Mail_ID);
                        return i;
                    }
                }
                printf("No contact found with that Mail ID.\n");
                return -1;
            }
            else
            {
                printf("Invalid Mail ID, Try again!\n");
            }
        }
    }

    else if (option == 4)
    {
        printf("Exit\n");
        return 0;
    }
    else
    {
        printf("Invalid option. Try again!\n");
        return -1;
    }

    return -1;
}

int edit_contact(struct Address_book *addressbook)
{
    char edit_keyword[30];
    int option;

    scanf("%d", &option);

    if (option >= 1 && option <= 3)
    {
        char keyword[30];
        printf("Enter search keyword: ");
        scanf(" %[^\n]", keyword);

        int found_indexes[100];
        int count = 0;

        for (int i = 0; i < addressbook->contact_count; i++)
        {
            if ((option == 1 && strcmp(addressbook->contact_details[i].Name, keyword) == 0) ||(option == 2 && strcmp(addressbook->contact_details[i].Mobile_number, keyword) == 0) ||(option == 3 && strcmp(addressbook->contact_details[i].Mail_ID, keyword) == 0))
                found_indexes[count++] = i;
        }

        if (count == 0)
        {
            printf("No matching contact found.\n");
            return 0;
        }

        int selected_index;

        if (count == 1)
        {
            selected_index = found_indexes[0];
        }
        else
        {
            printf("Multiple contacts found:\n");
            for (int j = 0; j < count; j++)
            {
                int i = found_indexes[j];
                printf("%d. Name: %s, Mobile: %s, Email: %s\n",j + 1,addressbook->contact_details[i].Name,addressbook->contact_details[i].Mobile_number,
                       addressbook->contact_details[i].Mail_ID);
            }

            printf("Select the contact number to edit (1 to %d): ", count);
            int choice;
            scanf("%d", &choice);

            if (choice < 1 || choice > count)
            {
                printf("Invalid selection.\n");
                return 0;
            }

            selected_index = found_indexes[choice - 1];
        }

        printf("What do you want to edit?\n");
        printf("1. Name\n2. Mobile Number\n3. Email ID\n");
        int edit_option;
        scanf("%d", &edit_option);

        if (edit_option == 1)
        {
            printf("Enter New Name: ");
            scanf(" %[^\n]", edit_keyword);
            if (validate_name(edit_keyword))
                strcpy(addressbook->contact_details[selected_index].Name, edit_keyword);
            else
                printf("Invalid name. Try again!\n");
        }
        else if (edit_option == 2)
        {
            printf("Enter New Mobile number: ");
            scanf(" %[^\n]", edit_keyword);
            if (validate_Mobile(edit_keyword))
                strcpy(addressbook->contact_details[selected_index].Mobile_number, edit_keyword);
            else
                printf("Invalid mobile number. Try again!\n");
        }
        else if (edit_option == 3)
        {
            printf("Enter New Email ID: ");
            scanf(" %[^\n]", edit_keyword);
            if (validate_Mail(edit_keyword))
                strcpy(addressbook->contact_details[selected_index].Mail_ID, edit_keyword);
            else
                printf("Invalid email ID. Try again!\n");
        }
        else
        {
            printf("Invalid edit option.\n");
            return 0;
        }

        printf("Contact updated successfully.\n");
    }
    else if (option == 4)
    {
        printf("Exit\n");
        return 0;
    }
    else
    {
        printf("Invalid option\n");
    }

    return 0;
}


int delete_contact(struct Address_book *addressbook)
{
    char delete_keyword[30];
    int option;
    //printf("Enter the option which you want to delete:\n1. Name\n2. Mobile Number\n3. Email ID\n4. Exit\n");
    scanf("%d", &option);
    if(option >= 1 && option <=3)
    {
    printf("Enter search keyword: ");
    scanf(" %[^\n]", delete_keyword);
    int found_indexes[100]; 
    int count = 0;
    for (int i = 0; i < addressbook->contact_count; i++)
     {
        if ((option == 1 && strcmp(addressbook->contact_details[i].Name, delete_keyword) == 0) ||(option == 2 && strcmp(addressbook->contact_details[i].Mobile_number, delete_keyword) == 0) ||(option == 3 && strcmp(addressbook->contact_details[i].Mail_ID,delete_keyword) == 0))
                found_indexes[count++] = i;
     }
           
    int selected_index;
    if (count == 1)
        selected_index = found_indexes[0];
    else
    {
      printf("Multiple contacts found:\n");
      for (int j = 0; j < count; j++)
       {
         int i = found_indexes[j];
         printf("%d. Name: %s, Mobile: %s, Email: %s\n",j + 1,addressbook->contact_details[i].Name,addressbook->contact_details[i].Mobile_number, addressbook->contact_details[i].Mail_ID);
       }
       printf("Select the choice to delete: ");
       int choice;
       scanf("%d", &choice);
       if (choice < 1 || choice > count)
        {
          printf("Invalid selection.\n");
          return 0;
        }
        selected_index = found_indexes[choice - 1];
      }
        addressbook->contact_count--;
        printf("Contact deleted successfully.\n");
    }
     
    else if (option == 4)
    {
     printf("Exit\n");
     return 0;
    }
    else 
    {
     printf("Invalid option\n");
     return 0;
    }    
}

void save_contacts(struct Address_book *addressbook)
{
    FILE *fp = fopen("data.csv", "w");
    if (fp == NULL)
    {
        printf("Unable to open file for writing.\n");
        return ; 
    }

    fprintf(fp,"#%d\n",addressbook ->contact_count);
    for(int i = 0; i < addressbook -> contact_count;i++)
    {
     fprintf(fp,"%s,%s,%s\n",addressbook->contact_details[i].Name,addressbook->contact_details[i].Mobile_number,addressbook->contact_details[i].Mail_ID);
    }
    fclose(fp);
    printf("Contact saved successfully\n");
}

void load_contacts(struct Address_book *addressbook)
{
    FILE *fp = fopen("data.csv", "r");
    if (fp == NULL)
    {
        printf("No saved contact found\n");
        return ; 
    }
    addressbook->contact_count = 0;
    fscanf(fp, "#%d\n", &addressbook->contact_count);
    for (int i = 0; i < addressbook->contact_count; i++)
    {
      fscanf(fp,"%[^,],%[^,],%[^\n]\n",addressbook->contact_details[i].Name,addressbook->contact_details[i].Mobile_number,addressbook->contact_details[i].Mail_ID);
    }  
    fclose(fp);
    printf("Contact loaded successfully\n");
}

