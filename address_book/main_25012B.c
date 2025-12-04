#include <stdio.h>
#include "contact.h"
/* Structure declaration */

int main()
{
    /* Variable and structure defintion */
    char option;
    struct Address_book addressbook;
    
    init_intitalization(&addressbook);
    load_contacts(&addressbook);
    while (1)
    {
        printf("\nAddress book menu\n"); /* Give a prompt message for a user */
        printf("1.Add contact\n2.Edit contact\n3.Delete contact\n4.search contact\n5.Display contact\n6.Save contact\n7.Exit\n");
        printf("Enter the option : ");
        scanf("%hhd", &option);

        switch (option) /* Based on choosed option */
        {
            case 1:
            {
                create_contact(&addressbook);
                break;
            }

            case 2:
            {
                printf("Edit Contact menu : \n1.Name \n2.Mobile number\n3.Mail ID\n4. Exit\nEnter the option : "); /* Providing menu */
                edit_contact(&addressbook);
                break;
            }
            case 3:
            {
                printf("Delete Contact menu : \n1.Name \n2.Mobile number\n3.Mail ID\n4.Exit\nEnter the option : "); /* Providing menu */
                delete_contact(&addressbook);
                break;
            }
            case 4:
            {
                printf("Search Contact menu : \n1.Name \n2.Mobile number\n3.Mail ID\n4.Exit\nEnter the option : "); /* Providing menu */
                search_contacts(&addressbook);
                break;
            }
            case 5:
            {
                printf("List contacts:");
                list_contacts(&addressbook);
                break;
            }

            case 6:
            {
                printf("Saving contacts\n");
                save_contacts(&addressbook);
                break;
            }
            case 7:
            {
                printf("INFO : Save and Exit...\n");
                return 0;
                break;
            }
            default:
            {
                printf("Invalid option \n");
                break;
            }
        }
    }

    return 0;
}
