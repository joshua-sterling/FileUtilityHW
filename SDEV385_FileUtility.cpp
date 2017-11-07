/*This program is a utility to give information about a file system
 * Users can select files, view information about them, change their 
 * permissions, and copy them to a new directory
 * Joshua Sterling
 * Written using Ubuntu 16.04.3 LTS
 * compiled using g++
 * e.g g++ JSterling_SDEV385_HW10.cpp -o JSHW10
 * extra info for test update push
*/
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <vector>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/sysmacros.h>
#include <errno.h>
#include <unistd.h>
#include <math.h>


using namespace std;

string path = "/home";  //starting directory
vector<string> files;	//will hold filenames to be viewed

void testFunction()
{
	printf("This is just to give me something to push to github");
}


/* This function will copy a file.  It will take a file from teh current 
 * directory and copy it to a destination directory, keeping the same file 
 * name */
void FileCopy() {
	string fileName, //name of file to be captured from user
		fromFullPath, //orginating path including file name
		toFullPath, 	//desitnation path including file name
		destinationPath;	//destination directory
	char theBuffer[100];
	int count, 				//will hold how many lines to write
		bytesRead;			//how many bytes read to be rewritten in copy
	//file descriptors	
	int inputFileDescriptor, outputFileDescriptor;
		
	printf("Please enter file to copy: ");
	cin >> fileName;
	printf("Please enter the destination directory: ");
	cin >> destinationPath;
	
	//create the full paths based on user entry
	fromFullPath = path;
	fromFullPath += "/";
	fromFullPath += fileName;
	toFullPath = destinationPath;
	toFullPath += "/";
	toFullPath += fileName;
	
	//display teh full from and to
	printf("from path: %s\n",fromFullPath.c_str());
	printf("to path: %s\n", toFullPath.c_str());
	
	//open the input file	
	inputFileDescriptor = open(fromFullPath.c_str(), O_RDONLY);
	//show if error 
	if(inputFileDescriptor == -1){
		printf("Open originating file result: %s\n",strerror(errno));
	}
	//open output location
	outputFileDescriptor = open(toFullPath.c_str(), O_WRONLY | O_CREAT);
	//show if error
	if(outputFileDescriptor == -1) {
		printf("Open desitnation result: %s\n", strerror(errno));
	}
	
	//total number of lines to read
	off_t fileLen=lseek(inputFileDescriptor,0,SEEK_END);
	
	//number of lines
	int lines = ceil(fileLen/100.0);
	
	//reset to front of input file	
	lseek(inputFileDescriptor,0,SEEK_SET);
	
	//write to teh new file
	for(count = 0; count < lines; count++)
	{
		bytesRead=read(inputFileDescriptor,theBuffer,100);
		write(outputFileDescriptor,theBuffer,bytesRead);		
	}
	//close files
	close(inputFileDescriptor);
	close(outputFileDescriptor);
}


/*Menu option for changing file permissions
 * based on option selected will add that permission to the files
 * existing permission*/
void FilePermsMenu(int i){
	int permMenu = -1;
	struct stat theBuffer;
	int theFileDescriptor;
	mode_t theMode;			//will hold the existing mode
	string newPath = path;
	newPath += "/";
	newPath += files[i];
	//Open the file, throw an error if open fails
	if((theFileDescriptor=open(newPath.c_str(),O_RDONLY))<0)
	{
	perror("Fail on Open");
	}
	cout << newPath << endl;
	//Grab the file statistics
	if((fstat(theFileDescriptor,&theBuffer))<0)
	{
		perror("Unable to get Statistics");
		
	}
	else{
		theMode= theBuffer.st_mode;
	}
	
	while(permMenu==-1){
		printf("*******Add File Permissions******\n");
		printf("*1.  Give everyone RWX          *\n");	
		printf("*2.  Give owner Read            *\n");	
		printf("*3.  Give owner Write           *\n");
		printf("*4.  Give group Read            *\n");	
		printf("*5.  Give group Write           *\n");
		printf("*6.  Give other Read            *\n");	
		printf("*7.  Give other Write           *\n");		
		printf("*0.  Return to Main Menu        *\n");
		printf("*********************************\n");
		printf("Please select a menu option:");
		
		cin >> permMenu;
		
		
			switch(permMenu){
				case 1: {
					int test = chmod(newPath.c_str(),S_IRWXU|S_IRWXG|S_IRWXO);
					//check for errors
					if(test==-1){
						printf("chmod results: %s\n", strerror(errno));
					}
					break;
				}
				case 2: {
					//keep existing perms and add owner read
					int test = chmod(newPath.c_str(),theMode|S_IRUSR);
					//check for errors
					if(test==-1){
						printf("chmod results: %s\n", strerror(errno));
					}
					break;
				}
				case 3: {
					//keep existing perms and add owner write
					int test = chmod(newPath.c_str(),theMode|S_IWUSR);
					//check for errors
					if(test==-1){
						printf("chmod results: %s\n", strerror(errno));
					}
					break;
				}
				case 4: {
					//keep existing perms and add group read
					int test = chmod(newPath.c_str(),theMode|S_IRGRP);
					//check for errors
					if(test==-1){
						printf("chmod results: %s\n", strerror(errno));
					}
					break;
				}
				case 5: {
					//keep existing perms and add group write
					int test = chmod(newPath.c_str(),theMode|S_IWGRP);
					//check for errors
					if(test==-1){
						printf("chmod results: %s\n", strerror(errno));
					}
					break;
				}
				case 6: {
					//keep existing perms and add other read
					int test = chmod(newPath.c_str(),theMode|S_IROTH);
					//check for errors
					if(test==-1){
						printf("chmod results: %s\n", strerror(errno));
					}
					break;
				}
				case 7: {
					//keep existing perms and add other write
					int test = chmod(newPath.c_str(),theMode|S_IWOTH);
					//check for errors
					if(test==-1){
						printf("chmod results: %s\n", strerror(errno));
					}
					break;
				}
				case 0: //do nothing - will exit sub menu
				break;
				default: {
					printf("Invalid option\n");
					permMenu = -1;
					break;
				}
			}			
	}
	
}

/* This function receives the struct stat and will display 
 * the current permissions on that file */
void ViewFilePerms(struct stat theBuffer){
	printf("File Permissions: \t");
    printf( (S_ISDIR(theBuffer.st_mode)) ? "d" : "-");
    printf( (theBuffer.st_mode & S_IRUSR) ? "r" : "-");
    printf( (theBuffer.st_mode & S_IWUSR) ? "w" : "-");
    printf( (theBuffer.st_mode & S_IXUSR) ? "x" : "-");
    printf( (theBuffer.st_mode & S_IRGRP) ? "r" : "-");
    printf( (theBuffer.st_mode & S_IWGRP) ? "w" : "-");
    printf( (theBuffer.st_mode & S_IXGRP) ? "x" : "-");
    printf( (theBuffer.st_mode & S_IROTH) ? "r" : "-");
    printf( (theBuffer.st_mode & S_IWOTH) ? "w" : "-");
    printf( (theBuffer.st_mode & S_IXOTH) ? "x" : "-");
    printf("\n\n");
}

/*This function allows users to enter the file names they want to view
 * */
void getFiles()
{
	string inputFile;
	char inputMore = 'y';
	while(inputMore == 'y' || inputMore == 'Y'){
		cout << "Please enter a file name: ";
		cin >> inputFile; 		
		files.push_back(inputFile);  //store in vector
		
		//continue as long as user wants to keep adding
		cout << "Add another file to view?(Y/N): ";
		cin >> inputMore;
	}
}

/*This function will show specific information about each file
 * in the files vector*/
void viewFileInfo()
{
	int viewInfoMenu;
	mode_t theMode;
	
	getFiles();
	for(size_t i = 0; i < files.size(); i++){
		cout << files[i] << endl;		//debugging
		int theFileDescriptor;
		struct stat theBuffer;
		string newPath = path;
		newPath += "/";
		newPath += files[i];
		//Open the file, throw an error if open fails
		if((theFileDescriptor=open(newPath.c_str(),O_RDONLY))<0)
		{
		perror("Fail on Open");
		}
		cout << newPath << endl;
		//Grab the file statistics
		if((fstat(theFileDescriptor,&theBuffer))<0)
		{
			perror("Unable to get Statistics");
			
		}
		else{
			theMode= theBuffer.st_mode;
			//present menu for each file selected
			printf("The File: %s\n",files[i].c_str());
			printf("*******File Information to View******\n");
			printf("*1.  View the mode                  *\n");	
			printf("*2.  View file permissions          *\n");	
			printf("*3.  View last access time          *\n");
			printf("*4.  View last update time          *\n");
			printf("*************************************\n");
			printf("Please select a menu option:");
		
			cin >> viewInfoMenu;
			//display selected item
			switch(viewInfoMenu){
				case 1: printf("The mode is %0X\n", theMode & ~(S_IFMT));
				break;
				case 2: ViewFilePerms(theBuffer);
				break;
				case 3: printf("Last access time: %s",ctime(&theBuffer.st_atim.tv_sec));
				break;
				case 4: printf("Last modification time: %s",ctime(&theBuffer.st_mtim.tv_sec));
				break;
				//invalid enty will just move on and do nothing
				default: printf("Not a valid option");  
				break;					
			
			}
		}
		printf("***********\n");
	}
	
}


bool getPath()
{
	string temp = path;		//hold path just in case
	cout << "Please enter a directory path: ";
	cin >> path;
	
	DIR *dir=opendir(path.c_str());	
	
	//make sure this is a valid directory
	if(dir)
	{		
		closedir(dir);
		return true;
	
	}
	else
	{
		cout << "Could not find directory" << endl;
		path = temp;		//reset path 
		closedir(dir);
		return false;
	}
	
	
}

/*This function will list teh contents of the current directory*/
void fileList()
{
	
	DIR *dir=opendir(path.c_str());	
	struct dirent *entry = readdir(dir);
	while(entry != NULL)
	{
		printf("%s\n", entry->d_name);
		entry = readdir(dir);		
	}
	 
	 closedir(dir);
	 printf("\n");
	
}



int main()
 {
	 int mainMenu = -1;			//user menu option
	 
	 	 
	 printf("Welcome to the File Utility\n");
	 
	 
	 //generate menu until user wants to quit
	 while(mainMenu != 6)
	 {
		 printf("***********Main Menu*****************\n");
		 printf("*1.  Change directory               *\n");
		 printf("*2.  View directory contents        *\n");
		 printf("*3.  View file information          *\n");
		 printf("*4.  Add file permissions           *\n");
		 printf("*5.  Copy file to new directory     *\n");
		 printf("*6.  Exit Program                   *\n");
		 printf("*************************************\n");
		 printf("Current directory is %s\n", path.c_str());
		 printf("Please select a menu option:");
		 
		 cin >> mainMenu;
		 		 
		 switch(mainMenu)
		 {
			 
			 case 1 : {
				 printf("Changing file path:\n");
				 getPath();
				 
				 break;
			 }
			 case 2 : {
				 printf("Display list of files:\n");
				 fileList();
				 break;
			 }
			 case 3 : {
				 printf("Display file information\n");
				 viewFileInfo();
				 files.clear();
				 break;
			 }
			 case 4 : {
				 printf("Change file permissions\n");
				 getFiles();
				 for(size_t i = 0; i < files.size(); i++){
					cout << files[i] << endl;		//debugging
					FilePermsMenu(i);
				 }
				 files.clear();
				 break;
			 }
			 case 5 : {
				 printf("Copy a file\n");
				 FileCopy();
				 break;
			}
			 case 6 : //do nothing - will exit
			 break;
			 default: {
				 printf("Invalid option.  Please select 1-5\n");
				 mainMenu = -1;	//reset option
				 break;	
			 }
		 }
		 //clear cin for next entry
		 cin.clear();
		 cin.ignore();
	 } 
 }
