#ifndef CONTACT_H
#define CONTACT_H

struct Contact_data
{
    char Name[32];
    char Mobile_number[11];
    char Mail_ID[35];
};


struct Address_book
{
    struct Contact_data contact_details[100];
    int contact_count;
};

/* Function declarations */
void init_intitalization(struct Address_book *addressbook);
int create_contact(struct Address_book *);
void list_contacts(struct Address_book *);
int search_contacts(struct Address_book *addressbook);
int edit_contact(struct Address_book *addressbook) ;
int delete_contact(struct Address_book *addressbook);
void save_contacts(struct Address_book *addressbook);
void load_contacts(struct Address_book *addressbook);
int validate_name(char Name[]);
int validate_Mobile(char Mobile_number[]);
int validate_Mail(char Mail_ID[]);
int is_unique(struct Address_book*addressbook,char*Mobile_no,char*eMailid);
#endif // CONTACT_H
// CONTACT_H        