//#include "stdafx.h"
#include "windows.h"
#include <iostream>
#include "ExtendedHook.h"

using namespace std;

typedef int (WINAPI* pMessageBox)(HWND myhandle, LPCWSTR text, LPCWSTR caption, UINT types); //function prototype
int WINAPI MessageBoxWHooker(HWND myhandle, LPCTSTR text, LPCTSTR caption, UINT types); //function hook
EHOOKSTRUCT myApi; //essential structure

pMessageBox myMessageBox = NULL; //optional, but i think it is useful


int main(int argc, char* argv[])
{
    if (InstallEHook("MessageBoxW", L"User32.dll", &myApi, &MessageBoxWHooker) == false) {
        wcout << "Error hooking" << endl;
        return 1;
    }

    myMessageBox = (pMessageBox)myApi.adr_new_api; //[optional] this will be a MessageBox without hook  
    myMessageBox(0, L"Hooking is my speciality!", L"ROSDEVIL", MB_OK | MB_ICONWARNING);

    if (MessageBox(0, L"Hi, did you understand?", L"ehi", MB_YESNO) == IDYES) {//this will be hooked!
        wcout << "You have pressed yes" << endl;
    }
    else {
        wcout << "You have pressed no" << endl;
    }

    UninstallEHook(&myApi);
    cin.get();
    return 0;
}

int WINAPI MessageBoxWHooker(HWND myhandle, LPCWSTR text, LPCWSTR caption, UINT types) {
    wcout << "-- MessageBoxW hooked!" << endl;
    wcout << "HWND: " << myhandle << endl;
    wcout << "Text: " << text << endl;
    wcout << "Caption: " << caption << endl;
    wcout << "Buttons/Icon: " << types << endl;

    return ((pMessageBox)myApi.adr_new_api)(myhandle, text, caption, types);
}