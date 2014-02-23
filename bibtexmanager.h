/****************************************************************************/
/* C library for manipulating bibtex file (sort, extract per authors/date)  */
/* Written by SCHULZ Quentin, quentin.schulz@utbm.fr , Jan. 2013            */
/****************************************************************************/

#ifndef __BIBTEX_MANAGER_H__
#define __BIBTEX_MANAGER_H__
#include <stdlib.h>
#include <stdio.h>
/*For manipulating char* (string)*/
#include <string.h>
/*For using regular expressions*/
#include <regex.h>
/*For toupper and tolower operations on string*/
#include <ctype.h>

/*Define two states in Boolean*/
#define TRUE 1
#define FALSE 0

/*DEFINITION OF RECORDS*/
/***********************/
/*Boolean type, TRUE = 1 and FALSE = 0*/
typedef int BOOL;

/*Special type for the key of entry*/
typedef char EntryKey;

/*Author type
**firstName contains a string of the firstName of the Author
**name contains a string of the name of the Author*/
typedef struct {
   char* firstName;
   char* name;
}Author;

/*Element of the doubly linked list AuthorList
**author is a pointer on an Author type
**prev and next are pointing on previous and next AuthorElem*/
typedef struct elemAuthor {
   Author* author;
   struct elemAuthor* prev;
   struct elemAuthor* next;
}AuthorElem;

/*Doubly linked list of authors
**nb is the number of authors in this DLList
**head and tail are pointing on the first and last AuthorElem it contains*/
typedef struct {
   int nb;
   AuthorElem* head;
   AuthorElem* tail;
}AuthorList;
/*Type for a common field of entry
**fieldName contains a string of the field's name (like abstract, title, year, ...)
**content contains a string of the content of the field*/
typedef struct {
   char* fieldName;
   char* content;
}EntryField;

typedef struct elemEntryField {
   EntryField* entryField;
   struct elemEntryField* prev;
   struct elemEntryField* next;
}ElemEntryField;
/*Like the DLList AuthorList*/
typedef struct {
   int nb;
   ElemEntryField* head;
   ElemEntryField* tail;
}EntryFieldList;

/*An entry type which contains whole informations on one entry
**key contains a string of the key of the entry (first line of entry after @(TYPE)
**type contains a string of the type of the entry (first line of entry @(TYPE)
**authorList is pointing on the DLList of authors from this entry
**entryFieldList is pointing on the DLList of EntryFields from this entry*/
typedef struct{
   char* key;
   char* type;
   AuthorList* authorList;
   EntryFieldList* entryFieldList;
}Entry;
/*Element of the DLList Library
**entry is pointing on the entry
**prev and next are pointing on previous and next EntryElem*/
typedef struct elemEntry{
   Entry* entry;
   struct elemEntry* prev;
   struct elemEntry* next;
}EntryElem;

/*Doubly linked list of authors
**nb is the number of authors in this DLList
**head and tail are pointing on the first and last EntryElem it contains*/
typedef struct {
   int nb;
   EntryElem* head ;
   EntryElem* tail;
}Library;

/*DPElem is an indirect element of the DLList DatePublications
**year contains a string which is the year of all entries in the library
**lib is the library that contains only entries from the year of the year string*/
typedef struct {
   char* year;
   Library* lib;
}DPElem;

/*Element of the DLList DatePublications*/
typedef struct elemDPList{
   DPElem* DPElem;
   struct elemDPList* prev;
   struct elemDPList* next;
}DPList;
/*DatePublications is a type for gathering all entries and sorting them by year
**nb is the number of differents years in the library
**head and tail are pointing on the first and last DPList it contains*/
typedef struct {
   int nb;
   DPList* head;
   DPList* tail;    
}DatePublications;

/*APElem is an indirect element of the DLList AuthorsPublications
**author is pointing on an author who has done all entries in the library
**lib is the library that contains only entries from the author*/
typedef struct {
   Author* author;
   Library* lib;
}APElem;

typedef struct elemAPList{
   APElem* APElem;
   struct elemAPList* prev;
   struct elemAPList* next;
}APList;
/*Same as DatePublications, but now for sorting entries by authors*/
typedef struct {
   int nb;
   APList* head;
   APList* tail;    
}AuthorsPublications;

/*PRINT FUNCTIONS*/
/*****************/
void printEntryField (EntryField* entryField);
void printAuthor (Author* author);
void printAuthorList (AuthorList* AL);
void printEntryFieldList(EntryFieldList* EFList);
void printEntry (Entry* entry);
/*Print Library especially for AuthorsPublication to print the year of the entry if it's different from the year of the previous entry*/
void printLibraryAP (Library* lib);
void printLibrary (Library* lib);
void printAuthorsPublications(AuthorsPublications* AP);
void printDatePublications(DatePublications* DP);

/*INSERT FUNCTIONS*/
/******************/
/*Insert entryField in an EntryFieldList
**If the EntryFieldList is empty, create a new EntryFieldList
**Create a new ElemEntryField which contains the entryField and editing both pointers (from the new ElemEntryField and from the EntryFieldList)
**Return the EntryFieldList*/
EntryFieldList* insertEntryField(EntryFieldList* EFList, EntryField* entryField);
AuthorList* insertAuthor(AuthorList* AL, Author* author);
Library* insertEntry(Library* lib, Entry* newEntry);
AuthorsPublications* insertAPElem(AuthorsPublications* AP, APElem* APE);
DatePublications* insertDPElem(DatePublications* DP, DPElem* DPE);

/*REMOVE FUNCTION*/
/*****************/
/*removeEntry search for the string eKey in the key field in each entry of the given library. If found, remove this entry from the given library and return the edited library. If not found, return the same library and print an error in the console.
**lib is the library where to search for the entry key
**eKey is the EntryKey to search for in the given library*/
Library* removeEntry (Library* lib, EntryKey* eKey);

/*TEST FUNCTIONS*/
/****************/
/*Test if the given sentence is a common entry field or not ("	fieldName = {(content)}," with fieldName != author)
**If yes, return the index of matching sequences for the fieldName and the content field of the line. If no, return NULL
**r is the formatted regular expression to test if the line is formatted as a common entry field or not
**line is the sequence to be tested*/
regmatch_t* isCommonEntryField(regex_t r,char line[]);

/*Test if the given sentence is an author field or not ("	author = {(content)},")
**If yes, return the index of matching sequences for the content field of the line. If no, return NULL
**r is the formatted regular expression to test if the line is formatted as an author field or not
**line is the sequence to be tested*/
regmatch_t* isAuthorList(regex_t r,char line[]);

/*Test if the given sentence is the first line of an entry or not ("@(type){(key),")
**If yes, return the index of matching sequences for the type and the key field of the line. If no, return NULL
**r is the formatted regular expression to test if the line is formatted as a first line of an entry or not
**line is the sequence to be tested*/
regmatch_t* isFirstLineEntry(regex_t r,char line[]);

/*Test if author1 = author2 (i.e. first name and name are equals).
**If yes, return TRUE. If not, return FALSE
**author1 and author2 are both given authors to be tested*/
BOOL compareAuthors(Author* author1, Author* author2);

/*FORMAT TEXT*/
/*************/
/*Remove special chararacters (especially in name and first name of authors)
**return the edited string without special characters*/
char* removeSpecialChar(char* string);

/*Format the given string into a well-formatted string for a first name (Quentin -> first letter in upper case and the rest in lower case) without special characters
**return the well edited string*/
char* formatFirstName(char* string);
/*Format the given string into a well-formatted string for a name (SCHULZ -> all letters in upper case) without special characters
**return the well edited string*/
char* formatName(char* string);

/*GET FUNCTIONS*/
/***************/
/*Return a string which is the title of the given entry. If can't find, return NULL*/
char* getTitle(Entry* entry);
/*Return a string which is the year of the given entry. If can't find, return NULL*/
char* getYear(Entry* entry);
/*Return a string which is the name of the first author in the authorlist of the given entry. If can't find, return NULL*/
char* getNameFirstAuthor(Entry* entry);

AuthorsPublications* getAuthorsPublications(Library* lib);
DatePublications* getDatePublications(Library* lib);
AuthorsPublications* getAuthorReferences(Library* lib, Author* author);
DatePublications* getDateReferences(Library* lib, long input);

/*PARSE FUNCTIONS*/
/*****************/
/*Create a new AuthorList with all authors of an entry thanks to the given line. Then it will cut the given sequence in several authors and fill in the AuthorList
**m contains the index of the beginning and the end of the sentences containing the authorlist found with isCommonEntryField function
**line is the sequence to be cut into several parts corresponding to the number of authors in the sequence in order to fill the new AuthorList
**return the new AuthorList*/
AuthorList* parseAuthorField(char line[], regmatch_t* m);
/*Fill in the type and key fields of the given entry thanks to the given line
**m contains the index of the matches sequences for a first line of an entry found with isFirstLineEntry function
**line is the sequence to be cut into two parts in order to fill both fields
**entry is the entry on which type and key fields have to be filled*/
void parseFirstLineEntry(char line[], Entry* entry, regmatch_t* m);
/*Create a new EntryField with the fieldName and content filled with the given line
**m contains the index of the matches sequences for a common entry field found with isCommonEntryField function
**line is the sequence to be cut into two parts in order to fill the new EntryField
**return the new EntryField*/
EntryField* parseCommonEntryField(char line[], regmatch_t* m);
/*Create a new library and fill in it with parseCommonEntryField, parseFirstLineEntry and parseAuthorField functions
**path is the given path to the file needed to create the library (a bibtex file)
**return the created library*/
Library* parseBibtexFile(char* path);

/*EXPORT FUNCTIONS*/
/******************/
/*Print the Entry/Author/AuthorList/EntryFieldList in the file in parameters*/
void fprintAuthor(FILE* file, Author* author);
void fprintAuthorList (FILE* file, AuthorList* AL);
void fprintEntryFieldList(FILE* file, EntryFieldList* EFList);
void fprintEntry(FILE* file, Entry* entry);
/*Print the library in the file in parameters*/
void fprintLibrary(FILE* file, Library* library);
/*Special function for AuthorsPublications. Needed because we've to print a year each time it's different from the previous one (coming with a new Entry)
**Print the library in the file in parameters*/
void fprintLibraryAP(FILE* file, Library* library);
/*Print AuthorsPublications in a file named with the name parameter
**authorpub the AuthorsPublications to be printed
**name the name to give to the created file in which we'll print*/
void exportAuthorsPublications (AuthorsPublications* authorpub, char* name);
/*Print DatePublications in a file named with the name parameter
**datepub the DatePublications to be printed
**name the name to give to the created file in which we'll print*/
void exportDatePublications (DatePublications* datepub, char* name);

/*SORT FUNCTIONS*/
AuthorsPublications* sortAuthorsPublications(AuthorsPublications* AP);
AuthorsPublications* sortLibraryAuthorDate(Library* lib);
DatePublications* sortDatePublications(DatePublications* DP);
DatePublications* sortLibraryDateAuthor(Library* lib);
void quicksortAP(AuthorsPublications* AP, int left, int right);
int partitionAP(AuthorsPublications* AP, int left, int right);
void quicksortDP(DatePublications* DP, int left, int right);
int partitionDP(DatePublications* DP, int left, int right);
void quicksortDateLibrary(Library* lib, int left, int right);
int partitionDateLibrary(Library* lib, int left, int right);
void quicksortAuthorsLibrary(Library* lib, int left, int right);
int partitionAuthorsLibrary(Library* lib, int left, int right);

/*FREE FUNCTIONS*/
void freeLibrary (Library* lib);
void freeEntryField (EntryField* entryField);
void freeAuthor (Author* author);
void freeAuthorList (AuthorList* AL);
void freeEntryFieldList(EntryFieldList* EFList);
void freeEntry (Entry* entry);
void freeAuthorsPublications(AuthorsPublications* AP);
void freeDatePublications(DatePublications* DP);
void freeAPElem(APElem* APE);
void freeDPElem(DPElem* DPE);

/*MENU*/
void menu(void);
void openit(char* filename);


#endif
