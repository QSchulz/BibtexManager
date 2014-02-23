#Bibtex Manager

The objective of this project is to provide a library of functions for managing a library of scientific books and articles. This library will be structured and initialized using a Bibtex file.<br>

After creating the library, the user has a control of it through twelve functions:

1. Create library
2. Remove an Entry from the library
3. Create a sorted AuthorPublications (sorted by author's name and then by date)
4. Create a sorted DatePublications (sorted by date and then by author's name)
5. Create an AuthorPublications for a specified author (retrieve all publications for this author)
6. Create a DatePublications for a specified year (retrieve all publications written the specified year)
7. Print the library
8. Print the current AuthorPublications
9. Print the current DatePublications
10. Export the library
11. Export the current AuthorPublications
12. Export the current DatePublications


For further information, please read *Subject.pdf* and *Report.pdf* files.

####Author:
Quentin SCHULZ (quentin.schulz@utbm.fr)

####Usage:

Download all files (except PDF files if you do not need them) and run the make file with: make.<br>
You can run your program with: /path/to/project/bibtexmain.exe<br>
or with: make launch<br>
If unfortunately it prints errors, try what says the result of: make init<br>
and relaunch the program.

#####Changelog

**v0.1 - 2014-02-23**
Initial release.<br>
