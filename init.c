#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char ** argv)
{
	char cwd[1024];
	char init[1024] = "export LD_LIBRARY_PATH=\"";
        char* endInit = "\":{LD_LIBRARY_PATH}";
        /*Retrieve the path to the library to include in the LD_LIBRARY_PATH*/
        if (getcwd(cwd, sizeof(cwd)) != NULL)
        {
        	strcat(init, cwd);
		strcat(init, endInit);
		init[strlen(cwd)+46]='\0';
		printf("*****************************************************************\nTap what is written the line below in the console:\n\n%s\n\n\nNow tap:\nmake launch\nCheck if there is spaces or special characters if it doesn't work. In the worst case, try:\nexport LD_LIBRARY_PATH=.:{LD_LIBRARY_PATH}\n\n*****************************************************************\n", init);
        }
        else
        {
        	printf("getcwd() error");
        }
        return 0;
}
