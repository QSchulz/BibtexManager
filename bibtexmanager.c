/****************************************************************************/
/* C library for manipulating bibtex file (sort, extract per authors/date)  */
/* Written by SCHULZ Quentin, quentin.schulz@utbm.fr , Jan. 2013            */
/****************************************************************************/

#include <bibtexmanager.h>

/*PRINT FUNCTIONS*/
/*****************/
void printEntryField (EntryField* entryField)
{
	/*Print field name and content of the EntryField starting with a tabulation*/
	printf("\t %s :", entryField->fieldName);
	printf(" %s\n", entryField->content);
}

void printAuthor (Author* author)
{
    /*Print first name and name separated by a space (can dissociate when reading thanks to formatFirstName and formatName)*/
    printf("%s ", author->firstName);
    printf("%s", author ->name);
}

void printAuthorList (AuthorList* AL)
{
	AuthorElem* AE;
	int i;
	printf("\tAuthors = ");
	/*Test if the AutorList exists*/
	if (AL!=NULL)
	{
		/*Retrieve the first AuthorElem*/
		AE = AL->head;
		/*Traverse whole AuthorList*/
		for(i=0; i<AL->nb;i++)
		{
			/*For each AuthorElem, print its author*/
			printAuthor(AE->author);
			/*Separate each author by a comma if it's not the last one. If it's, end the line by a point*/
			if(i!=AL->nb-1)
			{
				printf(", ");
			}
			else
			{
				printf(".\n");
			}
			/*Go to the next element*/
			AE = AE->next;
		}
	}
}
void printEntryFieldList(EntryFieldList* EFList)
{
	ElemEntryField* p;
	int i;
	/*Test if the EntryFieldList exists*/
	if (EFList !=NULL)
	{
		/*Retrieve the first ElemEntryField*/
		p = EFList->head;
		/*Traverse whole EntryFieldList*/
		for(i=0; i< EFList->nb; i++)
		{
			/*For each ElemEntryField, print its entryField*/
			printEntryField(p->entryField);
			/*Go to the newt element*/
			p = p-> next;
		}
	}
}
void printEntry (Entry* entry)
{
	/*Test if the entry exists, even if this case is already deleted by printLibrary(AP). (Traverse whole Library, should not lead to an empty Entry*/
	if (entry != NULL)
	{
		/*Print authorList and EntryFieldList for this Entry*/
		printAuthorList(entry->authorList);
		printEntryFieldList(entry->entryFieldList);
	}
}
/*Print Library especially for AuthorsPublication to print the year of the entry if it's different from the year of the previous entry*/
void printLibraryAP (Library* lib)
{
	EntryElem* p;
	char* year = NULL;
	int i;
	/*test if the library exists*/
	if(lib!=NULL)
	{
		/*Retrieve first EntryElem*/
		p = lib->head;
		/*Stock the year of the first EntryElem*/
		year = malloc(sizeof(char)*5);
		year = getYear(p->entry);
		/*Print it*/
		printf("****\n%s\n****\n", year);
		/*Traverse whole library*/
		for (i=0; i< lib->nb;i++)
		{
			/*Test if the year of the current entry is the same of the previous one*/
			if (strcmp(getYear(p->entry), year)!=0)
			{
				/*If not, stock in year the year of the current entry*/
				year = getYear(p->entry);
				/*Print it*/
				printf("****\n%s\n****\n", year);
			}
			/*Print entry*/
			printEntry(p->entry);
			printf("\n\n");
			/*Go to the next EntryElem*/
			p = p->next;
		}
	}
}

void printLibrary (Library* lib)
{
	EntryElem* p;
	int i;
	/*Test if the library exists*/
	if(lib!=NULL)
	{
		/*Retrieve the first EntryElem*/
		p = lib->head;
		/*Traverse whole library*/
		for (i=0; i< lib->nb;i++)
		{
			/*For each entry, print it*/
			printEntry(p->entry);
			printf("\n\n");
			/*Go to the next EntryElem*/
			p = p->next;
		}
	}
}

void printAuthorsPublications(AuthorsPublications* AP)
{
	APList* APL;
	int i;
	/*Test if the AuthorsPublications exists*/
	if (AP!= NULL)
	{
		/*Retrieve the first APList*/
		APL = AP->head;
		/*Traverse whole AuthorsPublications (DLList)*/
		for (i=0; i<AP->nb; i++)
		{
			/*For each APElem, print its author and its library*/
			printf("******************\n");
			printAuthor(APL->APElem->author);
			printf("\n******************\n");
			printLibraryAP(APL->APElem->lib);
			/*Go to the next APList*/
			APL = APL->next;
		}
	}
}

void printDatePublications(DatePublications* DP)
{
	DPList* DPL;
	int i;
	/*Test if the DatePublications exists*/
	if (DP!= NULL)
	{
		/*Retrive the first DPList*/
		DPL = DP->head;
		/*Traverse whole DatePublications (DLList)*/
		for (i=0; i<DP->nb; i++)
		{	
			/*For each DPElem, print its year and its library*/
			printf("******************\n%s\n******************\n", DPL->DPElem->year);
			printLibrary(DPL->DPElem->lib);
			/*Go to the next DPList*/
			DPL = DPL->next;
		}
	}
}


/*INSERT FUNCTIONS*/
/******************/
AuthorList* insertAuthor(AuthorList* AL, Author* author)
{
	AuthorElem* newAE;
	AuthorElem* p;
	/*Create new AuthorElem which contains the author in parameters*/
	newAE = (AuthorElem*) malloc (sizeof(AuthorElem));
	newAE->author = author;
	/*Test if the list is empty. The NULL case isn't a problem, because we initialize the AuthorList before sending it to this function*/
	if (AL->nb == 0)
	{
		/*If it's empty, the new AuthorElem is the only AuthorElem in the AuthorList. Then, the head and tail of the DLList point on newAE and both prev and next of newAE are NULL (no successor or predecessor)*/
		AL->head = AL->tail = newAE;
		newAE->prev = newAE->next = NULL;		
	}
	else
	{
		/*If it's not empty, go to the end of the AuthorList. Store the value of the pointer in p. The next value of newAE is NULL (no successor) and prev value is p. The next of p is newAE and the new tail of the AuthorList is newAE*/
		p = AL->tail;
		newAE->next = NULL;
		newAE->prev = p;
		p->next = newAE;
		AL->tail = newAE;
	}
	/*Succeed to add an author in the authorlist, so increment the number of author the authorlist contains*/
	AL->nb++;
	return AL;
}
EntryFieldList* insertEntryField(EntryFieldList* EFList, EntryField* entryField)
{
	ElemEntryField* newEEF;
	ElemEntryField* p;
	/*Create new ElemEntryField which contains the entryField in parameters*/
	newEEF = (ElemEntryField*) malloc (sizeof(ElemEntryField));
	newEEF->entryField = entryField;
	/*Test if the list exist.*/
	if (EFList == NULL)
	{
		/*If not, create a new EntryFieldList with 1 for its number of ElemEntryField it contains.*/
		EFList = malloc(sizeof(EntryFieldList));
		EFList->nb = 1;
		/*Insert it as the only ElemEntryField of the EntryFieldList*/
		EFList->head = EFList->tail = newEEF;
		newEEF->prev = newEEF->next = NULL;		
	}
	else
	{
		/*Increment the number of ElemEntryField of the EntryFieldList*/
		EFList->nb++;
		/*Go to the end of the AuthorList. Store the value of the pointer in p. The next value of newEEF is NULL (no successor) and prev value is p. The next of p is newEEF and the new tail of the EntryFieldList is newEEF*/
		p = EFList->tail;
		newEEF->next = NULL;
		newEEF->prev = p;
		p->next = newEEF;
		EFList->tail = newEEF;
	}
	return EFList;
}
Library* insertEntry(Library* lib, Entry* newEntry)
{
	EntryElem* testedEntry;
	EntryElem* p;
	AuthorElem* testAlist;
	AuthorElem* newEntryAlist;
	BOOL sameEntry = FALSE;
	BOOL sameAuthors = TRUE;
	const char* title;
	int i = 0;
	int j = 0;
	/*If newEntry doesn't exist, not interesting to insert it*/
	if (newEntry != NULL)
	{
		/*Create a new EntryElem which contains newEntry*/
		p = malloc(sizeof(EntryElem));
		p->entry = newEntry;
		/*Test if the library is empty. The NULL case isn't a problem, because we initialize the Library before sending it to this function*/
		if (lib->nb == 0)
		{
			/*If it's empty, the new EntryElem is the only EntryElem in the library. Then, the head and tail of the DLList point on p and both prev and next of p are NULL (no successor or predecessor)*/
			lib->head = p;
			lib->nb++;
			lib->tail = p;
			p->next = p->prev = NULL;
		}
		else
		{
			/*It's not empty. Retrieve the first EntryElem of the DLList*/
			testedEntry = lib->head;
			/*Store the title of the entry we want to insert in the library*/
			title = getTitle(newEntry);
			/*Store the AuthorList of the entry we want to insert in the library*/
			newEntryAlist = newEntry->authorList->head;
			/*Traverse whole library until we reach the end of the library or we found the entry in the library before inserting it (duplicate entry)*/  
			while(i<lib->nb && sameEntry == FALSE)
			{
				/*Store the AuthorList of the current entry*/
				testAlist = testedEntry->entry->authorList->head;
				/*Test if title of both entry are equals. If not, it's not a same entry*/
				if (strcmp(getTitle(testedEntry->entry),title)==0)
				{
					/*Same title. Test if each AuthorList has the same number of authors (if not, it means that at least one Author is in an AuthorList and not in the other -> not same Entry*/
					if(testedEntry->entry->authorList->nb == newEntry->authorList->nb)
					{
						/*Same number of authors. Traverse whole AuthorList until we reach the end or we found a different author*/
						while(sameAuthors == TRUE && j<testedEntry->entry->authorList->nb)
						{
							/*Same author. If */
							if(compareAuthors(testAlist->author, newEntryAlist->author)==FALSE)
							{
								/*Different author*/
								sameAuthors = FALSE;
							}
							else
							{
								/*Same author. Go on.*/
								j++;
								/*Go to the next author*/
								testAlist = testAlist->next;
								newEntryAlist = newEntryAlist->next;	
							}
						}
						/*If we reach this line, same title, same authors so same entry*/
						sameEntry = sameAuthors;
					}
				}
				/*Go to the next EntryElem*/
				testedEntry = testedEntry->next;
				i++;
			}
			/*Test if it is the same Entry*/
			if (sameEntry == FALSE)
			{
				/*It's not the same entry*/
				/*Go to the end of the library. Store the value of the pointer in testedEntry. The next value of p is NULL (no successor) and prev value is testedEntry. The next of testedEntry is p and the new tail of the library is p*/
				testedEntry = lib->tail;
				testedEntry->next = p;
				p->prev = testedEntry;
				lib->tail = p;
				lib->nb++;
			}
			else
			{
				/*It's the same entry, inform the user*/
				printf("\nWarning: duplicate entry\n");
				printEntry(newEntry);
			}
		}
	}
	return lib;
}

AuthorsPublications* insertAPElem(AuthorsPublications* AP, APElem* APE)
{
	APList* newAPL;
	APList* p;
	/*Create new APList which contains the APElem*/
	newAPL = (APList*) malloc (sizeof(APList));
	newAPL->APElem = APE;
	/*Test if the AuthorsPublications is empty. The NULL case isn't a problem, because we initialize the AuthorPublications before sending it to this function*/
	if (AP->nb == 0)
	{
		/*If it's empty, the new APList is the only APList in the AuthorsPublications. Then, the head and tail of the DLList point on newAPL and both prev and next of newAPL are NULL (no successor or predecessor)*/
		AP->head = AP->tail = newAPL;
		newAPL->prev = newAPL->next = NULL;		
	}
	else
	{
		/*If it's not empty, go to the end of the AuthorsPublications. Store the value of the pointer in p. The next value of newAPL is NULL (no successor) and prev value is p. The next of p is newAPL and the new tail of the AuthorsPublications is newAPL*/
		p = AP->tail;
		newAPL->next = NULL;
		newAPL->prev = p;
		p->next = newAPL;
		AP->tail = newAPL;
	}
	/*Succeed to add an APElem in the AuthorsPublications, so increment the number of APElem the AuthorsPublications contains*/
	AP->nb++;
	return AP;	
}
DatePublications* insertDPElem(DatePublications* DP, DPElem* DPE)
{
	DPList* newDPL;
	DPList* p;
	/*Create new DPList which contains the DPElem*/
	newDPL = (DPList*) malloc (sizeof(DPList));
	newDPL->DPElem = DPE;
	/*Test if the DatePublications is empty. The NULL case isn't a problem, because we initialize the DatePublications before sending it to this function*/
	if (DP->nb == 0)
	{
		/*If it's empty, the new DPList is the only DPList in the DatePublications. Then, the head and tail of the DLList point on newDPL and both prev and next of newDPL are NULL (no successor or predecessor)*/
		DP->head = DP->tail = newDPL;
		newDPL->prev = newDPL->next = NULL;		
	}
	else
	{
		/*If it's not empty, go to the end of the DatePublications. Store the value of the pointer in p. The next value of newDPL is NULL (no successor) and prev value is p. The next of p is newDPL and the new tail of the DatePublications is newDPL*/
		p = DP->tail;
		newDPL->next = NULL;
		newDPL->prev = p;
		p->next = newDPL;
		DP->tail = newDPL;
	}
	/*Succeed to add a DPElem in the AuthorsPublications, so increment the number of APElem the AuthorsPublications contains*/
	DP->nb++;
	return DP;	
}

/*REMOVE FUNCTION*/
/*****************/
/*removeEntry search for the string eKey in the key field in each entry of the given library. If found, remove this entry from the given library and return the edited library. If not found, return the same library and print an error in the console.
**lib is the library where to search for the entry key
**eKey is the EntryKey to search for in the given library*/
Library* removeEntry (Library* lib, EntryKey* eKey)
{
	EntryElem *p, *l;
	int i = 0;
	/*Retrieve the first EntryElem of the library*/               
	p = lib->head;
	/*Test if the library is empty. The NULL case isn't a problem because we don't allow the user to remove an entry if the lib is NULL*/
	if (lib->nb != 0)
	{
		/*Traverse whole list until we reach the element before the last one or until we find the key*/
		while(i<lib->nb-1 && strcmp(eKey,p->entry->key)!=0)
		{
			/*Go to the next EntryElem*/
			p = p->next;
			i++;
		}
		/*If the loop has stopped because we found the key*/
		if (strcmp(eKey, p->entry->key)==0)
		{
			/*If the match entry is the head*/
			if (p == lib->head)
			{
				l = p->next;
				/*if p is the only element*/
				if(l!=NULL)
				{
					l->prev=NULL;
				}
				lib->head = l;   
				free(p);
				p=NULL;
			}
			/*If the match entry is the tail*/
			else if (p == lib->tail)
			{
				l = p->prev;
				lib->tail = l;
				l->next = NULL;   
				free(p);
				p=NULL;
			}
			/*If it's neither the tail nor the head */
			else
			{
				l = p->prev;
				l->next = p->next;
				l = p->next;
				l->prev = p->prev;
				free(p);              
			}
			/*Decrement the value because we succeed to remove it*/
			lib->nb--;
			return lib;
		}
		/*No match*/
		else
		{
			printf("Sorry, an error occured: cannot find the given key in the given library\n");
			return NULL;
		}
	}
	else
	{
		printf("Sorry, an error occured: empty library\n");
		return NULL;
	}
}

/*TEST FUNCTIONS*/
/****************/
/*Test if the given sentence is a common entry field or not ("	fieldName = {(content)}," with fieldName != author)
**If yes, return the index of matching sequences for the fieldName and the content field of the line. If no, return NULL
**r is the formatted regular expression to test if the line is formatted as a common entry field or not
**line is the sequence to be tested*/
regmatch_t* isCommonEntryField(regex_t r, char line[])
{
	regmatch_t* m=NULL;
	const char* p = line;
	/*m will stock matches + sentence of the result of regular expression process*/
	m = (regmatch_t*) malloc (3*sizeof(regmatch_t));
	/*Test if there is matches. If yes, return index of matches via m, else return NULL*/
	if(regexec (&r, p, 3, m, 0)==0)
		{
			return m;
		}
		else
		{
			free(m);
			m = NULL;
			return NULL;
		}
	
}

/*Test if the given sentence is an author field or not ("	author = {(content)},")
**If yes, return the index of matching sequences for the content field of the line. If no, return NULL
**r is the formatted regular expression to test if the line is formatted as an author field or not
**line is the sequence to be tested*/
regmatch_t* isAuthorList(regex_t r,char line[])
{

	regmatch_t* m=NULL;
	const char* p = line;
	/*m will stock matches + sentence of the result of regular expression process*/
	m = (regmatch_t*) malloc (3*sizeof(regmatch_t));
	/*Test if there is matches. If yes, return index of matches via m, else return NULL*/
		if(regexec (&r, p, 2, m, 0)==0)
		{
			return m;
		}
		else
		{
			free(m);
			m = NULL;
			return NULL;
		}
}

/*Test if the given sentence is the first line of an entry or not ("@(type){(key),")
**If yes, return the index of matching sequences for the type and the key field of the line. If no, return NULL
**r is the formatted regular expression to test if the line is formatted as a first line of an entry or not
**line is the sequence to be tested*/
regmatch_t* isFirstLineEntry(regex_t r,char line[])
{
	regmatch_t* m=NULL;
	const char* p = line;
	/*m will stock matches + sentence of the result of regular expression process*/
	m = (regmatch_t*) malloc (3*sizeof(regmatch_t));
	/*Test if there is matches. If yes, return index of matches via m, else return NULL*/
		if(regexec (&r, p, 3, m, 0)==0)
		{
			return m;
		}
		else
		{
			free(m);
			m = NULL;
			return NULL;
		}
}

/*Test if author1 = author2 (i.e. first name and name are equals).
**If yes, return TRUE. If not, return FALSE
**author1 and author2 are both given authors to be tested*/
BOOL compareAuthors(Author* author1, Author* author2)
{
	/*Test if first name and name of first author are both equal to first name and name of second author*/
	if (strcmp(author1->firstName, author2->firstName)==0 && strcmp(author1->name, author2->name)==0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/*FORMAT FUNCTIONS*/
/******************/
/*Remove special chararacters (especially in name and first name of authors)
**return the edited string without special characters*/
char* removeSpecialChar(char* string)
{
	char* tmp;
	char temp[2048];
	int length;
	/*Copy the string into an array of char, because it's easier to change only some characters*/
	strcpy(temp, string);
	/*THE NEXT TWO ARE FOR AVOIDING STRCMP BUG WITH ACCENTS (for example, 'é' isn't between 'd' et 'f'!)*/
	/*Search for an 'é' in the sentence. If found, replace it by "e"*/
	tmp = strstr(temp, "é");
	if (tmp != NULL)
	{
		length = strlen(tmp);
		strcpy(&temp[strlen(temp)-length], "e");
		strcat(temp, &tmp[2]);
	}
	/*Search for an 'ç' in the sentence. If found, replace it by "c"*/
	tmp = strstr(temp, "ç");
	if (tmp != NULL)
	{
		length = strlen(tmp);
		strcpy(&temp[strlen(temp)-length], "c");
		strcat(temp, &tmp[2]);
	}
	/*Search for an '{\'e}' in the sentence. If found, replace it by "é"*/
	tmp = strstr(temp, "{\\'e}");
	if (tmp != NULL)
	{
		length = strlen(tmp);
		strcpy(&temp[strlen(temp)-length], "é");
		strcat(temp, &tmp[5]);
	}
	/*Search for an '{\c{c}}' in the sentence. If found, replace it by "ç"*/
	tmp = strstr(temp, "{\\c{c}}");
	if (tmp != NULL)
	{
		length = strlen(tmp);
		strcpy(&temp[strlen(temp)-length], "ç");
		strcat(temp, &tmp[7]);
	}
	/*Change the string in parameters to the modified string*/
	string = NULL;
	string = malloc(sizeof(char)*(strlen(temp)+1));
	strcpy(string, temp);
	return string;
}

/*Format the given string into a well-formatted string for a first name (Quentin -> first letter in upper case and the rest in lower case) without special characters
**return the well edited string*/
char* formatFirstName(char* string)
{
	int i;
	char tmp[2048];
	/*Start to remove special characters from the string*/
	string = removeSpecialChar(string);
	/*First letter of the first name is in upper case*/
	tmp[0] = toupper(string[0]);
	/*The rest is in lower case*/
	for (i=1; i<strlen(string); i++)
	{
		tmp[i] = tolower(string[i]);
	}
	tmp[strlen(string)]='\0';
	/*Change the string in parameters to the modified string*/
	string = NULL;
	string = malloc(sizeof(char)*(strlen(tmp)+1));
	strcpy(string, tmp);
	return string;
}

/*Format the given string into a well-formatted string for a name (SCHULZ -> all letters in upper case) without special characters
**return the well edited string*/
char* formatName(char* string)
{
	int i;
	char tmp[2048];
	/*Start to remove special characters from the string*/
	string = removeSpecialChar(string);
	/*All letters of the name is in upper case*/
	for (i=0; i<strlen(string); i++)
	{
		tmp[i] = toupper(string[i]);
	}
	tmp[strlen(string)]='\0';
	/*Change the string in parameters to the modified string*/
	string = NULL;
	string = malloc(sizeof(char)*(strlen(tmp)+1));
	strcpy(string, tmp);
	return string;
}

/*GET FUNCTIONS*/
/***************/
/*Return a string which is the title of the given entry. If can't find, return NULL*/
char* getTitle (Entry* entry)
{
	/*Use two temporar pointer on head and tail of the Entryfieldlist*/
	ElemEntryField* p1 = entry->entryFieldList->head;
	ElemEntryField* p2 = entry->entryFieldList->tail;
	int i = entry->entryFieldList->nb;
	BOOL isOdd = i%2;
	const char* title = "title";
	i = i/2;
	/*Look for an EntryField with a fieldname "title". Traverse in both sense until we reach the end (counter<=0) or find a match*/
	while(strcmp(p1->entryField->fieldName, title)!=0 && strcmp(p2->entryField->fieldName, title)!=0 && i>0)
	{
		p1=p1->next;
		p2=p2->prev;
		i--;
	}
	/*The loop has stopped before reahcing the end*/
	if(i>0)
	{
		/*Two options: first pointer has a fieldname "title"*/
		if (strcmp(p1->entryField->fieldName, title)==0)
		{
			return p1->entryField->content;
		}
		/*Second pointer has a fieldname "title"*/
		else
		{
			return p2->entryField->content;
		}
	}
	/*We reach the end*/
	else
	{
		/*If the number of entryfield is odd, we have to test the value of the medium*/
		if (isOdd == TRUE)
		{
			p1 = p1->next;
			if (strcmp(p1->entryField->fieldName, title)==0)
			{
				return p1->entryField->content;
			}
			else
			{
				return NULL;
			}
		}
		/*If not, there is no entryfield with a fieldname "title"*/
		else
		{
			return NULL;
		}
	}
}

/*Return a string which is the year of the given entry. If can't find, return NULL*/
char* getYear (Entry* entry)
{
	/*Same as getTitle with year instead of title*/
	ElemEntryField* p1 = entry->entryFieldList->head;
	ElemEntryField* p2 = entry->entryFieldList->tail;
	int i = entry->entryFieldList->nb;
	BOOL isOdd = i%2;
	const char* title = "year";
	i = i/2;
	while(strcmp(p1->entryField->fieldName, title)!=0 && strcmp(p2->entryField->fieldName, title)!=0 && i>0)
	{
		p1=p1->next;
		p2=p2->prev;
		i--;
	}
	if(i>0)
	{
		if (strcmp(p1->entryField->fieldName, title)==0)
		{
			return p1->entryField->content;
		}
		else
		{
			return p2->entryField->content;
		}
	}
	else
	{
		if (isOdd == TRUE)
		{
			p1 = p1->next;
			if (strcmp(p1->entryField->fieldName, title)==0)
			{
				return p1->entryField->content;
			}
			else
			{
				return NULL;
			}
		}
		else
		{
			return NULL;
		}
	}
}

/*Return a string which is the name of the first author in the authorlist of the given entry. If can't find, return NULL*/
char* getNameFirstAuthor(Entry* entry)
{
	/*Test if there is at least one element in the authorlist of the entry*/
	if (entry->authorList->nb !=0)
	{
		/*If yes, return the name of the first author*/
		return entry->authorList->head->author->name;
	}
	else
	{
		/*If no, inform the user*/
		printf("Weird: no author in this AuthorList!\n");
		return NULL;
	}
}

AuthorsPublications* getAuthorsPublications(Library* lib)
{
	EntryElem* EE = lib->head;
	APList* APL1;
	APList* APL2;
	APElem* APE;
	AuthorList* AL = EE->entry->authorList;
	AuthorElem* AE;
	int i, j, k, m;
	BOOL isOdd;
	/*Create a new AuthorsPublications*/
	AuthorsPublications* AP;
	AP = malloc(sizeof(AuthorsPublications));
	AP->nb = 0;
	AP->head = AP->tail= NULL;
	/*Traverse the whole library*/
	for(i=0; i<lib->nb; i++)
	{
		AL = EE->entry->authorList;
		AE = AL->head;
		/*Traverse AuthorList*/
		for(m=0; m<AL->nb; m++)
		{
			k = AP->nb;
			/*Same principle as getTitle and getYear, looking for authors instead of title or year*/
			if (k!=0)
			{
				APL1 = AP->head;
				APL2 = AP->tail;
				isOdd = k%2;
				j = k/2;
				/*Traverse the DPList until we reach the end or a match*/
				while(!compareAuthors(AE->author, APL1->APElem->author) && !compareAuthors(AE->author, APL2->APElem->author) && j>0)
				{
					APL1 = APL1->next;
					APL2 = APL2->prev;
					j--;
				}
				if (j>0)
				{
					if(compareAuthors(AE->author, APL1->APElem->author)==TRUE)
					{
						APL1->APElem->lib = insertEntry(APL1->APElem->lib, EE->entry);
					}
					else
					{
						APL2->APElem->lib = insertEntry(APL2->APElem->lib, EE->entry);
					}
				}
				else
				{
					if(isOdd == TRUE)
					{
						if(compareAuthors(AE->author, APL1->APElem->author))
						{
							APL1->APElem->lib = insertEntry(APL1->APElem->lib, EE->entry);
						}
						else
						{
							/*Create a new APElem and insert the entry in its library*/
							APE = malloc(sizeof(APElem));
							APE->author = AE->author;
							APE->lib = malloc(sizeof(Library));
							APE->lib->nb = 0;
							APE->lib->head = APE->lib->tail = NULL;
							APE->lib = insertEntry(APE->lib, EE->entry);
							AP = insertAPElem(AP, APE);
						
						}
					}
					else
					{
						APE = malloc(sizeof(APElem));
						APE->author = AE->author;
						APE->lib = malloc(sizeof(Library));
						APE->lib->nb = 0;
						APE->lib->head = APE->lib->tail = NULL;
						APE->lib = insertEntry(APE->lib, EE->entry);
						AP = insertAPElem(AP, APE);
					}		
				}
			}
			else
			{
				/*CREATE NEW APElem AND INSERT IN THE AP*/
				APE = malloc(sizeof(APElem));
				APE->author = AE->author;
				APE->lib = malloc(sizeof(Library));
				APE->lib->nb = 0;
				APE->lib->head = APE->lib->tail = NULL;
				APE->lib = insertEntry(APE->lib, EE->entry);
				AP = insertAPElem(AP, APE);
			}
			AE = AE->next;
			APE = NULL;
		}
		
		EE = EE->next;
	}
	return AP;
}

DatePublications* getDatePublications(Library* lib)
{
	EntryElem* EE = lib->head;
	DPList* DPL1;
	DPList* DPL2;
	DPElem* DPE;
	int i, j, k;
	char* year;
	BOOL isOdd;
	DatePublications* DP;
	DP = malloc(sizeof(DatePublications));
	DP->nb = 0;
	DP->head = DP->tail= NULL;
	/*Traverse the library lib*/
	for(i=0; i<lib->nb; i++)
	{
		year = getYear(EE->entry);
		k = DP->nb;
		if (k!=0)
		{
			DPL1 = DP->head;
			DPL2 = DP->tail;
			isOdd = k%2;
			j = k/2;
			/*Traverse the DPList until we reach the end or a match*/
			while(strcmp(year, DPL1->DPElem->year)!=0 && strcmp(year, DPL2->DPElem->year)!=0 && j>0)
			{
				DPL1 = DPL1->next;
				DPL2 = DPL2->prev;
				j--;
			}
			if (j>0)
			{
				if(strcmp(year, DPL1->DPElem->year)==0)
				{
					DPL1->DPElem->lib = insertEntry(DPL1->DPElem->lib, EE->entry);
				}
				else
				{
					DPL2->DPElem->lib = insertEntry(DPL2->DPElem->lib, EE->entry);
				}
			}
			else
			{
				if(isOdd == TRUE)
				{
					if(strcmp(year, DPL1->DPElem->year)==0)
					{
						DPL1->DPElem->lib = insertEntry(DPL1->DPElem->lib, EE->entry);
					}
					else
					{
						/*CREATE NEW DPElem AND INSERT IN THE DP*/
						DPE = malloc(sizeof(DPElem));
						DPE->year = year;
						DPE->lib = malloc(sizeof(Library));
						DPE->lib->nb = 0;
						DPE->lib->head = DPE->lib->tail = NULL;
						DPE->lib = insertEntry(DPE->lib, EE->entry);
						DP = insertDPElem(DP, DPE);
						
					}
				}
				else
				{
					DPE = malloc(sizeof(DPElem));
					DPE->year = year;
					DPE->lib = malloc(sizeof(Library));
					DPE->lib->nb = 0;
					DPE->lib->head = DPE->lib->tail = NULL;
					DPE->lib = insertEntry(DPE->lib, EE->entry);
					DP = insertDPElem(DP, DPE);
				}		
			}
		}
		else
		{
			/*CREATE NEW DPElem AND INSERT IN THE DP*/
			DPE = malloc(sizeof(DPElem));
			DPE->year = year;
			DPE->lib = malloc(sizeof(Library));
			DPE->lib->nb = 0;
			DPE->lib->head = DPE->lib->tail = NULL;
			DPE->lib = insertEntry(DPE->lib, EE->entry);
			DP = insertDPElem(DP, DPE);			
		}
		DPE = NULL;
		EE = EE->next;
	}
	return DP;
}

AuthorsPublications* getAuthorReferences(Library* lib, Author* author)
{
	AuthorsPublications* APtmp;
	AuthorsPublications* AP;
	APList* APL;
	APElem* APE;
	APtmp = sortLibraryAuthorDate(lib);
	APL = APtmp->head;
	
	while(APL!= NULL && strcmp(removeSpecialChar(author->name), removeSpecialChar(APL->APElem->author->name))>0 && compareAuthors(author, APL->APElem->author)==FALSE)
	{
		APL = APL->next;
	}
	if(APL == NULL || strcmp(removeSpecialChar(author->name), removeSpecialChar(APL->APElem->author->name))<0)
	{
		printf("Can't find\n");
		return NULL;
	}
	else
	{
		AP = malloc(sizeof(AuthorsPublications));
		AP->nb = 0;
		APE = APL->APElem;
		AP = insertAPElem(AP, APE);
		return AP;
	}
}

DatePublications* getDateReferences(Library* lib, long input)
{
	char year[5];
	DatePublications* DPtmp;
	DatePublications* DP;
	DPList* DPL;
	DPElem* DPE;
	DPtmp = sortLibraryDateAuthor(lib);
	DPL = DPtmp->head;
	sprintf(year, "%ld", input);
	while(DPL!= NULL && strcmp(year, DPL->DPElem->year)<0)
	{
		DPL = DPL->next;
	}
	if(DPL == NULL || strcmp(year, DPL->DPElem->year)>0)
	{
		printf("Can't find any occurence for year %s\n", year);
		return NULL;
	}
	else
	{
		DP = malloc(sizeof(DatePublications));
		DP->nb = 0;
		DPE = DPL->DPElem;
		DP = insertDPElem(DP, DPE);
		return DP;
	}
}
/*PARSE FUNCTIONS*/
/*****************/
/*Create a new AuthorList with all authors of an entry thanks to the given line. Then it will cut the given sequence in several authors and fill in the AuthorList
**m contains the index of the beginning and the end of the sentences containing the authorlist found with isCommonEntryField function
**line is the sequence to be cut into several parts corresponding to the number of authors in the sequence in order to fill the new AuthorList
**return the new AuthorList*/
AuthorList* parseAuthorField(char line[], regmatch_t* m)
{
	int start = m[1].rm_so;
	int finish = m[1].rm_eo;
	int length = 0;
	int len = 0;
	const int original_len = finish-start;
	char* field;
	char* name = NULL;
	char* author = NULL;
	char* firstname = NULL;
	AuthorList* authorList = NULL;
	Author* newAuthor = NULL;
	char* authorField;
	
	authorField = malloc(sizeof(char)*(original_len+1));
	/*Create a new AuthorList containing all authors of this entry*/
	authorList = malloc (sizeof(AuthorList));
	authorList->nb = 0;
	authorList->head = authorList->tail = NULL;
	/*Keep the list of authors not the whole line*/
	strncpy(authorField, &line[start], original_len);
	authorField[original_len]='\0';
	field = strstr(authorField, " and ");
	if (field != NULL)
	{
		while(field!=NULL)
		{
			newAuthor = NULL;
			newAuthor = malloc(sizeof(Author));
			length = strlen(field);
			author = malloc(sizeof(char)*(original_len-length-len+1));
			strncpy(author, &authorField[len], original_len-length-len);
			author[original_len-length-len]='\0';
			/*malloc*/
			name = malloc(sizeof(char)*(strlen(author)+1));
			strcpy(name, author);
			strtok(name, ",");
			firstname = malloc(sizeof(char)*(original_len-length-len-strlen(name)-2));
			strcpy(firstname, &author[strlen(name)+2]);
			len = original_len - length + 5;
			field = strstr(&authorField[len], " and ");
			firstname = formatFirstName(firstname);
			newAuthor->firstName = malloc(sizeof(char)*(strlen(firstname)+1));
			strcpy(newAuthor->firstName, firstname);
			name = formatName(name);
			newAuthor->name = malloc(sizeof(char)*(strlen(name)+1));
			strcpy(newAuthor->name, name);
			authorList = insertAuthor(authorList, newAuthor);
		}
		newAuthor = NULL;
		newAuthor = malloc(sizeof(Author));
		length = original_len - len;
		author = malloc(sizeof(char)*(length+1));
		strncpy(author, &authorField[len], length);
		author[length]='\0';
		name = malloc(sizeof(char)*(strlen(author)+1));
		strcpy(name, author);
		strtok(name, ",");
		firstname = malloc(sizeof(char)*(original_len-length-strlen(name)-2));
		strcpy(firstname, &author[strlen(name)+2]);
		firstname = formatFirstName(firstname);
		newAuthor->firstName = malloc(sizeof(char)*(strlen(firstname)+1));
		strcpy(newAuthor->firstName, firstname);
		name = formatName(name);
		newAuthor->name = malloc(sizeof(char)*(strlen(name)+1));
		strcpy(newAuthor->name, name);
		authorList = insertAuthor(authorList, newAuthor);
	}
	else
	{
		newAuthor = NULL;
		newAuthor = malloc(sizeof(Author));
		name = malloc(sizeof(char)*(strlen(authorField)+1));
		strcpy(name, authorField);
		strtok(name, ", ");
		firstname = malloc(sizeof(char)*(original_len-strlen(name)-2));
		strcpy(firstname, &authorField[strlen(name)+2]);
		newAuthor->firstName = malloc(sizeof(char)*(strlen(firstname)+1));
		firstname = formatFirstName(firstname);
		strcpy(newAuthor->firstName, firstname);
		name = formatName(name);
		newAuthor->name = malloc(sizeof(char)*(strlen(name)+1));
		strcpy(newAuthor->name, name);
		authorList = insertAuthor(authorList, newAuthor);
	}
	free(m);
	return authorList;
}


/*Fill in the type and key fields of the given entry thanks to the given line
**m contains the index of the matches sequences for a first line of an entry found with isFirstLineEntry function
**line is the sequence to be cut into two parts in order to fill both fields
**entry is the entry on which type and key fields have to be filled*/
void parseFirstLineEntry(char line[], Entry* entry, regmatch_t* m)
{
	int start, finish;
	
    	/*start and finish are stockage values to stock the start and end of the match 1*/
	start = m[1].rm_so;
	finish = m[1].rm_eo;
	/*Fill in entry's type*/
	entry->type = malloc(sizeof(char)*(finish-start+1));
	strncpy(entry->type, &line[start], finish-start);
	entry->type[finish-start]='\0';
	
	/*start and finish are now stockage values to stock the start and end of the match 2*/
	start = m[2].rm_so;
	finish = m[2].rm_eo;
	/*Fill in entry's key*/
	entry->key= malloc(sizeof(char)*(finish-start+1));
	strncpy(entry->key, &line[start], finish-start);
	entry->key[finish-start]='\0';
	
	/*printf("type = >%s<\nkey = >%s<\n\n\n",entry->type,entry->key);*/
	/*No need anymore for matches' position*/
	free(m);
}

/*Create a new EntryField with the fieldName and content filled with the given line
**m contains the index of the matches sequences for a common entry field found with isCommonEntryField function
**line is the sequence to be cut into two parts in order to fill the new EntryField
**return the new EntryField*/
EntryField* parseCommonEntryField(char line[], regmatch_t* m)
{
	int start, finish;
	EntryField* newEntryField;
    	/*start and finish are stockage value to stock the start and end of the match 1*/
	start = m[1].rm_so;
	finish = m[1].rm_eo;
	/*Create new EntryField*/
	newEntryField = malloc(sizeof(EntryField));
	/*Fill in newEntryField's "fieldName"*/
	newEntryField->fieldName = malloc(sizeof(char)*(finish-start+1));
	strncpy(newEntryField->fieldName, &line[start], finish-start);
	newEntryField->fieldName[finish-start]='\0';
	/*See above. Same process for match 2*/
	start = m[2].rm_so;
	finish = m[2].rm_eo;
	/*Fill in newEntryField's "content"*/
	newEntryField->content= malloc(sizeof(char)*(finish-start+1));
	strncpy(newEntryField->content, &line[start], finish-start);
	newEntryField->content[finish-start]='\0';
	/*printf("fieldName = >%s<\ncontent = >%s<\n\n\n",newEntryField->fieldName,newEntryField->content);*/
	/*No need anymore for matches' position*/
	free(m);
	return newEntryField;
}

/*Create a new library and fill in it with parseCommonEntryField, parseFirstLineEntry and parseAuthorField functions
**path is the given path to the file needed to create the library (a bibtex file)
**return the created library*/
Library* parseBibtexFile(char* path){

    char line[2048];
    char entryfield[2048];
    int len = 0;
    regex_t r,r1,r2;
    const char *regex, *regex1, *regex2;
    Entry* newEntry=NULL;
    /*POINTER?*/
    Library* lib;
    EntryField* newEntryField;
    regmatch_t *m1, *m2, *m3;
    FILE* file = NULL;
    file = fopen(path,"r");
    if (file != NULL)
    {
    	printf("Succeed to open the file %s\n", path);
        /*Succeed to find the file and open it*/
	lib = (Library*) malloc (sizeof(Library));
	lib->nb = 0;
        /*Expressions of regular expressions*/
	regex = "([[:alnum:]]+) = \\{([[:print:][:space:]’–]+)\\},\n";
	regex1 = "\tauthor = \\{([[:print:]]+)\\},\n";
	regex2 = "@([[:alpha:]]+)\\{([[:alnum:]_]+),\n";
	/*Compile regex and test if it succeeded or not*/
	if(regcomp (&r, regex, REG_EXTENDED)!=0 || regcomp (&r1, regex1, REG_EXTENDED)!=0 || regcomp (&r2, regex2, REG_EXTENDED)!=0)
	{
		printf("At least one regular expression have an error while compiling\n");
	}
	else
	{
		/*Traverse whole file until the end of the file*/
        	while(fgets(line,sizeof(line),file)!=NULL)
		{	
			/*The process is useless if line is empty or only contains '}'*/
			if (strcmp(line, "\n")!=0 && strncmp(line, "}", 1)!=0)
			{
				len = strlen(line)-1;
				/*Copy the whole line in the entryfield stockage value*/
				strcpy(entryfield, line);
				/*To be sure to stock a whole entryfield, we need to know if the line is finished by a ",\n"
				**If it isn't, it means that the entryfield isn't complete so we need to add a line until we reach a line ended by ",\n"*/
				while (strncmp(&line[len-1], ",\n",2)!=0)
				{
				    fgets(line,sizeof(line),file);
				    len = strlen(line)-1;
				    /*Add the line to the entryfield*/
				    strcat(entryfield, line);
				    entryfield[strlen(entryfield)]='\0';
				}
				m1 = isFirstLineEntry(r2, entryfield);
				/*entryfield is the first line of an Entry*/
				if (m1 != NULL)
				{
					/*Insert the new Entry in the Library lib
					**Case of newEntry is NULL is considered in insertEntry*/
					lib = insertEntry(lib, newEntry);
					/*Re-initialize the newEntry*/
					newEntry = NULL;
					newEntry = malloc (sizeof(Entry));
					newEntry->entryFieldList = NULL; 
					newEntry->authorList = NULL;
					/*Launch the parse of the first line of the Entry
					**It will fill the key and type field in the Entry and fill in it*/
					parseFirstLineEntry(entryfield, newEntry, m1); 
				}
				else 
				{
					m2 = isAuthorList(r1, entryfield);
					/*entryfield is a field containing a list of authors*/
					if (m2 != NULL)
					{
						/*Launch the parse of the author field
						**It will fill the AuthorList field in the Entry*/
						newEntry->authorList = parseAuthorField(entryfield, m2);
							/*printAuthorList(newEntry->authorList);*/
					}
					else
					{
						m3 = isCommonEntryField(r, entryfield);
						/*entryfield is a field containing a common entry field*/
						if (m3 != NULL)
						{
							/*Launch the parse of the EntryField
							**It will fill all the EntryFields in the Entry*/
							newEntryField = parseCommonEntryField(entryfield, m3);
							newEntry->entryFieldList = insertEntryField(newEntry->entryFieldList, newEntryField);
						}
						else
						{
							printf(">%s< is neither a first line of an Entry formatted like '@Abstract = {[...]},' nor a common EntryField like 'type = {[...]},' nor an AuthorList like 'author = {NAME, Firstname},' \n", entryfield);
						}
					}
				}
			}
		}
		/*For the last one*/
		insertEntry(lib, newEntry);
	}
        fclose(file);
    }
    else
    {
        printf("Can't find %s\n", path);
    }
    regfree(&r);
    regfree(&r1);
    regfree(&r2);
    return lib;
}

/*EXPORT FUNCTIONS*/
/******************/
/*Print the Entry/Author/AuthorList/EntryFieldList in the file in parameters*/
void fprintAuthor (FILE* file, Author* author)
{
	fprintf(file,"%s ", author->firstName);
	fprintf(file,"%s", author ->name);
}

void fprintAuthorList (FILE* file, AuthorList* AL)
{
	AuthorElem* AE;
	int i;
	AE = AL->head;
	fprintf(file, "\t\t");
	for(i=0; i<AL->nb-1;i++)
	{
		fprintAuthor(file,AE->author);
		fprintf(file,", ");
		AE = AE->next;
	}
	fprintAuthor(file,AE->author);
	fprintf(file,".\n");
}

void fprintEntryFieldList(FILE* file, EntryFieldList* EFList)
{
	ElemEntryField* EEF = EFList->head;
	int i = 0;
	fprintf(file, "\t\t");
	for (i=0; i<EFList->nb; i++)
	{
		if(strcmp(EEF->entryField->fieldName, "title")!=0 && strcmp(EEF->entryField->fieldName, "year")!=0)
        	{
        		fprintf(file, "%s, ", EEF->entryField->content);
        	}
        	EEF = EEF->next;
	}
}

void fprintEntry(FILE* file,Entry* entry)
{
	fprintAuthorList(file,entry->authorList);
	fprintEntryFieldList(file,entry->entryFieldList);
	fprintf(file,"%s.\n", getYear(entry));
}

/*Print the library in the file in parameters*/
void fprintLibrary(FILE* file, Library* library)
{
	EntryElem* p;
	int i;
	if (library !=NULL)
	{
		p=library->head;
		for (i=0; i<library->nb;i++)
		{
			fprintf(file,"\t\t%s \n", getTitle(p->entry));
			fprintEntry(file, p->entry);
			fprintf(file, "\n\n");
			p=p->next;
		}
	}
}

/*Special function for AuthorsPublications. Needed because we've to print a year each time it's different from the previous one (coming with a new Entry)
**Print the library in the file in parameters*/
void fprintLibraryAP(FILE* file, Library* library)
{
	EntryElem* p;
	int i;
	char* year = NULL;
	year = malloc(sizeof(char)*(5));
	if (library !=NULL)
	{
		p=library->head;
		for (i=0; i<library->nb;i++)
		{
			if (strcmp(getYear(p->entry), year)!=0)
			{
				strcpy(year,getYear(p->entry));
				fprintf(file, "\t****\n\t%s\n\t****\n", year);
			}
			fprintf(file,"\t\t%s\n", getTitle(p->entry));
			fprintEntry(file, p->entry);
			fprintf(file, "\n\n");
			p=p->next;
		}
	}
}

/*Print AuthorsPublications in a file named with the name parameter
**authorpub the AuthorsPublications to be printed
**name the name to give to the created file in which we'll print*/
void exportAuthorsPublications (AuthorsPublications* authorspub, char* name)
{
	int i = 0;
	/*char* prev = NULL;*/
	APList* APL = authorspub->head;
	FILE* exportAP = NULL;
	exportAP = fopen(name,"w");
	if (exportAP != NULL)
	{
		printf("Number of authors: %d\n", authorspub->nb);
		for(i=0; i<authorspub->nb; i++)
		{
			fprintAuthor(exportAP, APL->APElem->author);
			fprintf(exportAP, "\n################################\n");
			fprintLibraryAP(exportAP,APL->APElem->lib);
			APL = APL->next;
		}
		fclose(exportAP);
	}
	else
	{
		printf("Can’t create %s\n", name);
	}
}

/*Print DatePublications in a file named with the name parameter
**datepub the DatePublications to be printed
**name the name to give to the created file in which we'll print*/
void exportDatePublications(DatePublications* datepub, char* name)
{
	int i = 0;
	DPList* DPL = datepub->head;
	FILE* exportDP = NULL;
	exportDP = fopen(name,"w");
	if (exportDP != NULL)
	{
		printf("Number of years: %d\n", datepub->nb);
		for(i=0; i<datepub->nb; i++)
		{
			fprintf(exportDP, "****\n%s\n****\n", DPL->DPElem->year);
			fprintLibrary(exportDP,DPL->DPElem->lib);
			DPL = DPL->next;
		}
		fclose(exportDP);
	}
	else
	{
		printf("Can’t create %s\n", name);
	}
}

void quicksortAuthorsLibrary(Library* lib, int left, int right)
{
	int newpivot;
	/*UTILISER ALEATOIRE*/
	if (left<right)
	{
		newpivot = partitionAuthorsLibrary(lib, left, right);
		quicksortAuthorsLibrary(lib, left, newpivot);
		quicksortAuthorsLibrary(lib, newpivot + 1, right);
	}
}
int partitionAuthorsLibrary(Library* lib, int left, int right)
{
	int store, i;
	EntryElem* EEleft = lib->head;
	EntryElem* EEright = lib->tail;
	EntryElem* EEpivot = lib->head;
	EntryElem* EEstore;
	Entry* tmpEntry;
	int nb = lib->nb;
	for (i=0; i<left; i++)
	{
		EEleft = EEleft->next;
	}
	for (i=nb; i>right; i--)
	{
		EEright = EEright->prev;
	}
	EEpivot = EEright;
	tmpEntry = EEpivot->entry;
	EEpivot->entry = EEright->entry;
	EEright->entry = tmpEntry;
	EEstore = EEleft;
	store = left;
	for (i=left; i<right; i++)
	{
		if (strcmp(removeSpecialChar(getNameFirstAuthor(EEleft->entry)), removeSpecialChar(getNameFirstAuthor(EEright->entry)))<0)
		{
			tmpEntry = EEstore->entry;
			EEstore->entry = EEleft->entry;
			EEleft->entry = tmpEntry;
			EEstore = EEstore->next;
			store ++;
		}
		EEleft = EEleft->next;
	}
	tmpEntry = EEright->entry;
	EEright->entry = EEstore->entry;
	EEstore->entry = tmpEntry;
	return store;
}

void quicksortDateLibrary(Library* lib, int left, int right)
{
	int newpivot;
	/*UTILISER ALEATOIRE*/
	if (left<right)
	{
		newpivot = partitionDateLibrary(lib, left, right);
		quicksortDateLibrary(lib, left, newpivot);
		quicksortDateLibrary(lib, newpivot + 1, right);
	}
}

int partitionDateLibrary(Library* lib, int left, int right)
{
	int store, i;
	EntryElem* EEleft = lib->head;
	EntryElem* EEright = lib->tail;
	EntryElem* EEstore;
	Entry* tmpEntry;
	int nb = lib->nb;
	for (i=0; i<left; i++)
	{
		EEleft = EEleft->next;
	}
	for (i=nb; i>right; i--)
	{
		EEright = EEright->prev;
	}
	EEstore = EEleft;
	store = left;
	for (i=left; i<right; i++)
	{
		if (strcmp(getYear(EEleft->entry), getYear(EEright->entry))>0)
		{
			tmpEntry = EEstore->entry;
			EEstore->entry = EEleft->entry;
			EEleft->entry = tmpEntry;
			EEstore = EEstore->next;
			store ++;
		}
		EEleft = EEleft->next;
	}
	tmpEntry = EEright->entry;
	EEright->entry = EEstore->entry;
	EEstore->entry = tmpEntry;
	return store;
}

AuthorsPublications* sortLibraryAuthorDate(Library* lib)
{
	AuthorsPublications* AP;
	quicksortDateLibrary(lib, 0, lib->nb);
	AP = getAuthorsPublications(lib);
	quicksortAP(AP, 0, AP->nb);
	return AP;
}

DatePublications* sortLibraryDateAuthor(Library* lib)
{
	DatePublications* DP;
	quicksortAuthorsLibrary(lib, 0, lib->nb);
	DP = getDatePublications(lib);
	quicksortDP(DP, 0, DP->nb);
	return DP;
	
}


int partitionAP(AuthorsPublications* AP, int left, int right)
{
	int store, i;
	APList* APLleft = AP->head;
	APList* APLright = AP->tail;
	APList* APLstore;
	APElem* tmpAPE;
	int nb = AP->nb;
	for (i=0; i<left; i++)
	{
		APLleft = APLleft->next;
	}
	for (i=nb; i>right; i--)
	{
		APLright = APLright->prev;
	}
	APLstore = APLleft;
	store = left;
	for (i=left; i<right; i++)
	{
		if (strcmp(removeSpecialChar(APLleft->APElem->author->name), removeSpecialChar(APLright->APElem->author->name))<0)
		{
			tmpAPE = APLstore->APElem;
			APLstore->APElem = APLleft->APElem;
			APLleft->APElem = tmpAPE;
			APLstore = APLstore->next;
			store ++;
		}
		APLleft = APLleft->next;
	}
	tmpAPE = APLright->APElem;
	APLright->APElem = APLstore->APElem;
	APLstore->APElem = tmpAPE;
	return store;
}

void quicksortAP(AuthorsPublications* AP, int left, int right)
{
	int newpivot;
	if (left<right)
	{
		newpivot = partitionAP(AP, left, right);
		quicksortAP(AP, left, newpivot);
		quicksortAP(AP, newpivot + 1, right);
	}
}

int partitionDP(DatePublications* DP, int left, int right)
{
	int store, i;
	DPList* DPLleft = DP->head;
	DPList* DPLright = DP->tail;
	DPList* DPLpivot = DP->head;
	DPList* DPLstore;
	DPElem* tmpDPE;
	int nb = DP->nb;
	for (i=0; i<left; i++)
	{
		DPLleft = DPLleft->next;
	}
	for (i=nb; i>right; i--)
	{
		DPLright = DPLright->prev;
	}
	DPLpivot = DPLright;
	tmpDPE = DPLpivot->DPElem;
	DPLpivot->DPElem = DPLright->DPElem;
	DPLright->DPElem = tmpDPE;
	DPLstore = DPLleft;
	store = left;
	for (i=left; i<right; i++)
	{
		if (strcmp(DPLleft->DPElem->year, DPLright->DPElem->year)>0)
		{
			tmpDPE = DPLstore->DPElem;
			DPLstore->DPElem = DPLleft->DPElem;
			DPLleft->DPElem = tmpDPE;
			DPLstore = DPLstore->next;
			store ++;
		}
		DPLleft = DPLleft->next;
	}
	tmpDPE = DPLright->DPElem;
	DPLright->DPElem = DPLstore->DPElem;
	DPLstore->DPElem = tmpDPE;
	return store;
}

void quicksortDP(DatePublications* DP, int left, int right)
{
	int newpivot;
	if (left<right)
	{
		newpivot = partitionDP(DP, left, right);
		quicksortDP(DP, left, newpivot);
		quicksortDP(DP, newpivot + 1, right);
	}
}


void menu(void)
{
    int answer = 0;
    Library* lib = NULL;
    AuthorsPublications* AP = NULL;
    DatePublications* DP = NULL;
    Author* author = NULL;
    FILE* file = NULL;
    long year = 0;
    char temp[2048];
    char* tmp = NULL;
    EntryKey* tempo = NULL;
    printf("\033[2J\033[1;1H");
    while(answer!=13)
    {
	    printf("################################################################\n");
	    printf("#                      CHOOSE YOUR OPTION                      #\n");
	    printf("################################################################\n");
	    printf("# ( 1) Create library                                          #\n");
	    printf("# ( 2) Remove entry                                            #\n");
	    printf("# ( 3) Create sorted AuthorsPublications                       #\n");
	    printf("# ( 4) Create sorted DatePublications                          #\n");
	    printf("# ( 5) Create AuthorPublications for a specified author        #\n");
	    printf("# ( 6) Create DatePublications for a specified year            #\n");
	    printf("# ( 7) Print library                                           #\n");
	    printf("# ( 8) Print current AuthorsPublications                       #\n");
	    printf("# ( 9) Print current DatePublications                          #\n");
	    printf("# (10) Export library                                          #\n");
	    printf("# (11) Export current AuthorsPublications                      #\n");
	    printf("# (12) Export current DatePublications                         #\n");
	    printf("# (13) Exit program                                            #\n");
	    printf("################################################################\n");
	    scanf("%d", &answer);
    	    printf("\033[2J\033[1;1H");
	    switch(answer)
	    {
	    	case 1: 
		    	printf("I need the complete path (path + name of file) to the bibtex file you want me to create a library of\n");
		    	scanf("%s", temp);
		    	tmp = malloc(sizeof(char)*(strlen(temp)+1));
		    	strcpy(tmp, temp); 
		    	lib = parseBibtexFile(tmp);
		    	break;
		case 2: 
	    		if (lib != NULL)
	    		{
	    			printf("Provide the key of the Enter you want to delete: \n");
	    			scanf("%s", temp);
	    			tempo = malloc(sizeof(char)*(strlen(temp)+1));
	    			strcpy(tempo, temp);
	    			removeEntry(lib, tempo);
	    		}
	    		else
	    		{
	    			printf("Try to create the library before removing an entry (option 1)\n");
	    		}
	    		break;
		case 3:
		    	if(lib != NULL)
		    	{
		    		AP = sortLibraryAuthorDate(lib);
		    	}
		    	else
		    	{
		    		printf("You've to create library first (option 1)\n");
		    	} 
		    	break;
		case 4:
		    	if(lib != NULL)
		    	{
		    		DP = sortLibraryDateAuthor(lib);
		    	}
		    	else
		    	{
		    		printf("You've to create library first (option 1)\n");
		    	} 
		    	break;
		case 5:
		    	if(lib != NULL)
		    	{
		    		printf("Provide the author (use special chars like ç or é if needed) :\nName:");
		    		scanf("%s", temp);
		    		author = malloc(sizeof(Author));
		    		author->name = malloc(sizeof(char)*(strlen(temp)+1));
		    		strcpy(author->name, temp);
		    		printf("First Name:");
		    		scanf("%s", temp);
		    		author->firstName = malloc(sizeof(char)*(strlen(temp)+1));
		    		strcpy(author->firstName, temp);
		    		AP = getAuthorReferences(lib, author);
		    	}
		    	else
		    	{
		    		printf("You've to create library first (option 1)\n");
		    	} 
		    	break;
		case 6:
		    	if(lib != NULL)
		    	{
		    		printf("Provide the year:");
		    		scanf("%ld", &year);
		    		while (year>2013)
		    		{
		    			printf("Ahah. Nice try.\nTry with a year < 2013 maybe?\n");
		    			scanf("%ld", &year); 
		    		}
		    		DP = getDateReferences(lib, year);
		    	}
		    	else
		    	{
		    		printf("You've to create library first (option 1)\n");
		    	} 
		    	break;
	    	case 7:
		    	if(lib != NULL)
		    	{
		    		printLibrary(lib);
		    	}
		    	else
		    	{
		    		printf("You've to create library first (option 1)\n");
		    	}
		    	break;
	    	case 8:
		    	if(AP != NULL)
		    	{
		    		printAuthorsPublications(AP);
		    	}
		    	else
		    	{
		    		printf("You've to create sorted AuthorsPublications first (option 3)\n");
		    	}
		    	break;
	    	case 9:
		    	if(DP != NULL)
		    	{
		    		printDatePublications(DP);
		    	}
		    	else
		    	{
		    		printf("You've to create sorted DatePublications first (option 4)\n");
		    	}
		    	break;
	    	case 10:
		    	if(lib!= NULL)
		    	{
		    		printf("Provide the complete name (with extension) you want for the exported library: \n");
		    		scanf("%s", temp);
		    		tmp = malloc(sizeof(char)*(strlen(temp)+1));
		    		strcpy(tmp, temp);
		    		file = fopen(tmp, "w");
		    		if (file != NULL)
		    		{
					fprintLibrary(file, lib);
					fclose(file);
			    		printf("DONE.\n\nYou can find your file under %s\n", tmp);		    		
					openit(tmp);
		    		}
				else
				{
					printf("Failed to open/create %s\n", temp);
				}		    		
		    	}
		    	else
		    	{
		    		printf("You've to create library first (option 1)\n");
		    	}
		    	break;
	    	case 11:
		    	if(AP != NULL)
		    	{
		    		printf("Provide the complete name (with extension) you want for the exported AuthorsPublications: ");
		    		scanf("%s", temp);
		    		tmp = malloc(sizeof(char)*(strlen(temp)+1));
		    		strcpy(tmp, temp);
		    		exportAuthorsPublications(AP, temp);
		    		printf("DONE.\n\nYou can find your file under %s\n", tmp);
		    		openit(tmp);
		    	}
		    	else
		    	{
		    		printf("You've to create sorted AuthorsPublications first (option 3)\n");
		    	}
		    	break;
	    	case 12:
		    	if(DP != NULL)
		    	{
		    		printf("Provide the complete name (with extension) you want for the exported DatePublications: ");
		    		scanf("%s", temp);
		    		tmp = malloc(sizeof(char)*(strlen(temp)+1));
		    		strcpy(tmp, temp);
		    		exportDatePublications(DP, tmp);
		    		printf("DONE.\n\nYou can find your file under %s\n", tmp);
		    		openit(tmp);
		    	}
		    	else
		    	{
		    		printf("You've to create sorted DatePublications first (option 4)\n");
		    	}
		    	break;
	    	case 13:
			/*if (AP != NULL)
			{ freeAuthorsPublications(AP);}
			if (DP != NULL)
			{ freeDatePublications(DP);}
			if (lib != NULL)
			{ freeLibrary(lib);}
			if (author != NULL)
			{ freeAuthor(author);}
			if (tmp != NULL)
			{ free(tmp);}
			path = NULL;*/
		    	break;
	    	default: printf("Please enter a number between 1 and 13\n");break;
	    }
	    if(answer>0 && answer<13)
	    {
	    	printf("Enter any number and press enter to continue\n");
	    	scanf("%d", &answer);
	    	answer = 0;
	    	printf("\033[2J\033[1;1H");
	    }
    }	
}

void openit(char* filename)
{
	char answer;
	char instruction[40] = "gedit ./";
	strcat(instruction, filename);
	instruction[strlen(filename)+8]='\0';
	printf("Do you want to open the file? Y/n\n");
	scanf(" %c", &answer);
	if (toupper(answer) == 'Y')
	{
		system(instruction);
	}
}
