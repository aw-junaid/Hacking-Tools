#include <stdio.h>
#include <Windows.h>
#include <winternl.h>

#pragma comment(lib,"ntdll.lib") // Need to link with ntdll.lib import library. You can find the ntdll.lib from the Windows DDK.

typedef struct _SYSTEM_PROCESS_INFO
{
    ULONG                   NextEntryOffset;
    ULONG                   NumberOfThreads;
    LARGE_INTEGER           Reserved[3];
    LARGE_INTEGER           CreateTime;
    LARGE_INTEGER           UserTime;
    LARGE_INTEGER           KernelTime;
    UNICODE_STRING          ImageName;
    ULONG                   BasePriority;
    HANDLE                  ProcessId;
    HANDLE                  InheritedFromProcessId;
} SYSTEM_PROCESS_INFO, *PSYSTEM_PROCESS_INFO;

int main()
{
    NTSTATUS status;
    PVOID buffer;
    PSYSTEM_PROCESS_INFO spi;
    OFSTRUCT _buffer;
    // OpenFile avec en parametres : le nom du fichier(LPCSTR), la struct que t'as créé, et les droits que tu veux (OF_EXIST)

    int testOpen = 1;
    if (testOpen == 0)
    {
        getchar();
        MessageBox(NULL, "Message from Application", "Hook.dll", 0);
    }
    else if (testOpen == 1)
    {
        typedef NTSTATUS(__stdcall *NT_OPEN_FILE)(OUT PHANDLE FileHandle, IN ACCESS_MASK DesiredAccess, IN POBJECT_ATTRIBUTES ObjectAttributes, OUT PIO_STATUS_BLOCK IoStatusBlock, IN ULONG ShareAccess, IN ULONG OpenOptions);
        NT_OPEN_FILE NtOpenFileStruct;
        /* load the ntdll.dll */
        PVOID Info;
        HMODULE hModule = LoadLibrary(("ntdll.dll"));
        NtOpenFileStruct = (NT_OPEN_FILE)GetProcAddress(hModule, "NtOpenFile");
        if (NtOpenFileStruct == NULL) {
            printf("Error: could not find the function NtOpenFile in library ntdll.dll.");
            exit(-1);
        }
        printf("NtOpenFile is located at 0x%08x in ntdll.dll.\n", (unsigned int)NtOpenFileStruct);

        while (getchar())
        {
            /* create the string in the right format */
            UNICODE_STRING filename;
            RtlInitUnicodeString(&filename, L"C:\\temp.txt");

            /* initialize OBJECT_ATTRIBUTES */
            OBJECT_ATTRIBUTES obja;
            InitializeObjectAttributes(&obja, &filename, OBJ_CASE_INSENSITIVE, NULL, NULL);

            /* call NtOpenFile */
            IO_STATUS_BLOCK iostatusblock;
            HANDLE file = NULL;
            NTSTATUS stat = NtOpenFileStruct(&file, FILE_WRITE_DATA, &obja, NULL, NULL, NULL);
            printf("%d", stat);
            /*if (NT_SUCCESS(stat)) {
                printf("File successfully opened.\n");
                }
                else {
                printf("File could not be opened %d.\n", NT_SUCCESS(stat));
                }*/
        }
    }
    else
    {
        while (getchar() != 0)
        {
            buffer = VirtualAlloc(NULL, 1024 * 1024, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE); // We need to allocate a large buffer because the process list can be large.

            if (!buffer)
            {
                printf("\nError: Unable to allocate memory for process list (%d)\n", GetLastError());
                return -1;
            }

            printf("\nProcess list allocated at address %#x\n", buffer);
            spi = (PSYSTEM_PROCESS_INFO)buffer;

            if (!NT_SUCCESS(status = NtQuerySystemInformation(SystemProcessInformation, spi, 1024 * 1024, NULL)))
            {
                printf("\nError: Unable to query process list (%#x)\n", status);

                VirtualFree(buffer, 0, MEM_RELEASE);
                return -1;
            }

            while (spi->NextEntryOffset) // Loop over the list until we reach the last entry.
            {
                printf("\nProcess name: %ws | Process ID: %d\n", spi->ImageName.Buffer, spi->ProcessId); // Display process information.
                spi = (PSYSTEM_PROCESS_INFO)((LPBYTE)spi + spi->NextEntryOffset); // Calculate the address of the next entry.
            }

            printf("\n %d Press any key to continue.\n", status);
            getchar();

            VirtualFree(buffer, 0, MEM_RELEASE); // Free the allocated buffer.
        }
    }

    return 0;
}