#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <wchar.h>

#define MAX_PROCESSES 1024

typedef struct {
    DWORD processID;
    char name[MAX_PATH];
} ProcessList;

int processCount;
char input[256];
ProcessList processList[MAX_PROCESSES];

void getInput(void) {

    if (fgets(input, sizeof(input), stdin) != NULL) {

        size_t len = strlen(input);

        if (len > 0) {
            char * lnbreak_chr = strchr(input, '\n'); //strchr search for first occurence and create a pointer to it
            *lnbreak_chr = '\0';
        }
    }
}

int getProcessList() {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); //SNAPPROCESS  0x00000002
    PROCESSENTRY32 pe = { 0 };

    int count = 0;
    pe.dwSize = sizeof(PROCESSENTRY32);

    char multiByteString[MAX_PATH];


    if (Process32First(hSnapshot, &pe)) {
        do {
             
            if (count >= MAX_PROCESSES) break;

            processList[count].processID = pe.th32ProcessID;
            WideCharToMultiByte(CP_UTF8, 0, pe.szExeFile, -1, multiByteString, MAX_PATH, NULL, NULL); // convert WCHAR * to char * in mutliByteString

            strcpy_s(processList[count].name, sizeof(processList[count].name), multiByteString);

            count++;

        } while (Process32Next(hSnapshot, &pe));
        
    }

    CloseHandle(hSnapshot);
    processCount = count;
    
    return 0 ; 
}

char* toLowerCase(char* str) {
    for (int i = 0; i < strlen(str); i++) {
        str[i] = tolower(str[i]);
    }

    return str;
}

int searchProcess() {

    printf("Process Name: ");
    getInput() ;
   
    int match = false;

    getProcessList();
    
   
    printf("\nNo PID NAME\n");
    printf("------------\n");


    for (int i = 0; i < processCount; i++) {

        if (strstr( toLowerCase( processList[i].name ), toLowerCase(input) ) != NULL) {

            printf("%d %lu %s\n", i, processList[i].processID, processList[i].name);
            match = true;
        }
    }

    if (!match) puts("No matches");

    return 0;
}

void killProcess() {
    
    //kill PID
}

int main()
{
   
    searchProcess();
    killProcess();

    return 0;
}


