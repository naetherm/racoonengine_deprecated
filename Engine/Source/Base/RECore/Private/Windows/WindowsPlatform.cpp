////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2019 - 2022 RacoonStudios
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this
// software and associated documentation files (the "Software"), to deal in the Software
// without restriction, including without limitation the rights to use, copy, modify, merge,
// publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
// to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
// FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
////////////////////////////////////////////////////////////////////////////////////////////////////


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "RECore/Windows/WindowsPlatform.h"
#include <Tchar.h>
#include <conio.h>
#include <stdlib.h>
#include <shlobj.h>
#include <locale.h>
#include <windows.h>
#include <Userenv.h>
#include <shellapi.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace RECore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
bool WindowsPlatform::getMemoryInformation(MemoryInformation &sMemoryInformation) const
{
  // Initialize memory information
  sMemoryInformation.nTotalPhysicalMemory	= 0;
  sMemoryInformation.nFreePhysicalMemory	= 0;
  sMemoryInformation.nTotalSwapMemory		= 0;
  sMemoryInformation.nFreeSwapMemory		= 0;

  // Error!
  return false;
}

WindowsPlatform::WindowsPlatform() {

}

WindowsPlatform::~WindowsPlatform() {

}

String WindowsPlatform::getPlatform() const {
  static String SPlatform = "Windows";
  return SPlatform;
}

String WindowsPlatform::getOS() const {
  // Get system info
  OSVERSIONINFOEX osvi;
  SYSTEM_INFO si;

  ZeroMemory(&si, sizeof(SYSTEM_INFO));
  ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));

  // Try calling GetVersionEx using the OSVERSIONINFOEX structure.
  // If that fails, try using the OSVERSIONINFO structure.
  osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
  BOOL bOsVersionInfoEx = GetVersionEx(reinterpret_cast<OSVERSIONINFO*>(&osvi));
  if (!bOsVersionInfoEx) {
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    if (!GetVersionEx(reinterpret_cast<OSVERSIONINFO*>(&osvi)))
      return "Windows";
  }

  { // Call GetNativeSystemInfo if supported or GetSystemInfo otherwise
    HMODULE hModule = GetModuleHandle(TEXT("kernel32.dll"));
    if (hModule) {
      PGNSI pGNSI = reinterpret_cast<PGNSI>(GetProcAddress(hModule, "GetNativeSystemInfo"));
      if (pGNSI)
        pGNSI(&si);
      else
        GetSystemInfo(&si);
    } else {
      GetSystemInfo(&si);
    }
  }

  // Compose version string
  String sVersion;

  // Get Windows version
  switch (osvi.dwPlatformId) {
    // Test for the Windows NT product family
    case VER_PLATFORM_WIN32_NT:
      // Test for the specific product
      if (osvi.dwMajorVersion == 10 && osvi.dwMinorVersion == 0) {
        if (osvi.wProductType == VER_NT_WORKSTATION)
          sVersion += "Windows 10 ";
        else
          sVersion += "Windows Server 2020 ";

      } else if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 3) {
        if (osvi.wProductType == VER_NT_WORKSTATION)
          sVersion += "Windows 8.1 ";
        else
          sVersion += "Windows Server 2012 R2 ";

      } else if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 2) {
        if (osvi.wProductType == VER_NT_WORKSTATION)
          sVersion += "Windows 8 ";
        else
          sVersion += "Windows Server 2012 ";

      } else if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 1) {
        if (osvi.wProductType == VER_NT_WORKSTATION)
          sVersion += "Windows 7 ";
        else
          sVersion += "Windows Server 2008 R2 ";

      } else if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 0) {
        if (osvi.wProductType == VER_NT_WORKSTATION)
          sVersion += "Windows Vista ";
        else
          sVersion += "Windows Server \"Longhorn\" ";

      } else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2) {
#ifndef SM_SERVERR2
#define SM_SERVERR2 89
#endif
        if (GetSystemMetrics(SM_SERVERR2))
          sVersion += "Microsoft Windows Server 2003 \"R2\" ";
        else if (osvi.wProductType == VER_NT_WORKSTATION && si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
          sVersion += "Microsoft Windows XP Professional x64 Edition ";
        else
          sVersion += "Microsoft Windows Server 2003, ";

      } else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1)
        sVersion += "Microsoft Windows XP ";

      else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0)
        sVersion += "Microsoft Windows 2000 ";

      else if (osvi.dwMajorVersion <= 4)
        sVersion += "Microsoft Windows NT ";

      // Test for specific product on Windows NT 4.0 SP6 and later
      if (bOsVersionInfoEx) {
        // Test for the workstation type
        if (osvi.wProductType == VER_NT_WORKSTATION && si.wProcessorArchitecture != PROCESSOR_ARCHITECTURE_AMD64) {
          if (osvi.dwMajorVersion == 4)
            sVersion += "Workstation 4.0 ";
          else if (osvi.wSuiteMask & VER_SUITE_PERSONAL)
            sVersion += "Home Edition ";
          else
            sVersion += "Professional ";
        }

          // Test for the server type
        else if (osvi.wProductType == VER_NT_SERVER || osvi.wProductType == VER_NT_DOMAIN_CONTROLLER) {
          if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2) {
            if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64) {
              if (osvi.wSuiteMask & VER_SUITE_DATACENTER)
                sVersion += "Datacenter Edition for Itanium-based Systems";
              else if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE)
                sVersion += "Enterprise Edition for Itanium-based Systems";

            } else if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64) {
              if (osvi.wSuiteMask & VER_SUITE_DATACENTER)
                sVersion += "Datacenter x64 Edition ";
              else if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE)
                sVersion += "Enterprise x64 Edition ";
              else
                sVersion += "Standard x64 Edition ";

            } else {
              if (osvi.wSuiteMask & VER_SUITE_DATACENTER)
                sVersion += "Datacenter Edition ";
              else if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE)
                sVersion += "Enterprise Edition ";
              else if (osvi.wSuiteMask == VER_SUITE_BLADE)
                sVersion += "Web Edition ";
              else
                sVersion += "Standard Edition ";
            }

          } else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0) {
            if (osvi.wSuiteMask & VER_SUITE_DATACENTER)
              sVersion += "Datacenter Server ";
            else if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE)
              sVersion += "Advanced Server ";
            else
              sVersion += "Server ";

          } else { // Windows NT 4.0
            if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE)
              sVersion += "Server 4.0, Enterprise Edition ";
            else
              sVersion += "Server 4.0 ";
          }
        }

        // Test for specific product on Windows NT 4.0 SP5 and earlier
      } else {
        HKEY hKey;
        LONG lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                                 TEXT("SYSTEM\\CurrentControlSet\\Control\\ProductOptions"),
                                 0, KEY_QUERY_VALUE, &hKey);
        if (lRet != ERROR_SUCCESS)
          break;

        TCHAR szProductType[80];
        DWORD dwBufLen = 80*sizeof(TCHAR);
        lRet = RegQueryValueEx(hKey, TEXT("ProductType"), nullptr, nullptr, reinterpret_cast<LPBYTE>(szProductType), &dwBufLen);
        RegCloseKey(hKey);

        if (lRet != ERROR_SUCCESS || dwBufLen > 80*sizeof(TCHAR))
          break;

        if (CompareString(LOCALE_INVARIANT, NORM_IGNORECASE, TEXT("WINNT"), -1, szProductType, -1) == CSTR_EQUAL)
          sVersion += "Workstation ";
        if (CompareString(LOCALE_INVARIANT, NORM_IGNORECASE, TEXT("LANMANNT"), -1, szProductType, -1) == CSTR_EQUAL)
          sVersion += "Server ";
        if (CompareString(LOCALE_INVARIANT, NORM_IGNORECASE, TEXT("SERVERNT"), -1, szProductType, -1) == CSTR_EQUAL)
          sVersion += "Advanced Server ";
        sVersion += static_cast<uint32>(osvi.dwMajorVersion);
        sVersion += '.';
        sVersion += static_cast<uint32>(osvi.dwMinorVersion);
      }

      // Display service pack (if any) and build number
      if (osvi.dwMajorVersion == 4 && CompareString(LOCALE_INVARIANT, NORM_IGNORECASE, osvi.szCSDVersion, -1, TEXT("Service Pack 6"), -1) == CSTR_EQUAL) {
        // Test for SP6 versus SP6a
        HKEY hKey;
        LONG lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                                 TEXT("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Hotfix\\Q246009"),
                                 0, KEY_QUERY_VALUE, &hKey);
        if (lRet == ERROR_SUCCESS)
          sVersion += String("Service Pack 6a (Build ") + static_cast<uint32>(osvi.dwBuildNumber & 0xFFFF) + ')';
        else { // Windows NT 4.0 prior to SP6a
          sVersion += String(osvi.szCSDVersion) + " (Build " + static_cast<uint32>(osvi.dwBuildNumber & 0xFFFF) + ')';
        }

        RegCloseKey(hKey);
      } else { // Not Windows NT 4.0
        sVersion += String(osvi.szCSDVersion) + " (Build " + static_cast<uint32>(osvi.dwBuildNumber & 0xFFFF) + ')';
      }
      break;

      // Test for the Windows Me/98/95
    case VER_PLATFORM_WIN32_WINDOWS:
      if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0) {
        sVersion += "Microsoft Windows 95 ";
        if (osvi.szCSDVersion[1] == 'C' || osvi.szCSDVersion[1] == 'B')
          sVersion += "OSR2 ";

      } else if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10) {
        sVersion += "Microsoft Windows 98 ";
        if (osvi.szCSDVersion[1] == 'A' || osvi.szCSDVersion[1] == 'B')
          sVersion += "SE ";

      } else if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90) {
        sVersion += "Microsoft Windows Millennium Edition";
      }
      break;

    case VER_PLATFORM_WIN32s:
      sVersion += "Microsoft Win32s";
      break;
  }

  // Fallback, if unknown version number combination found
  if (sVersion.length())
    sVersion.trim();
  else
    sVersion = "Windows";

  // Return the found Windows version
  return sVersion;
}

char WindowsPlatform::getSeparator() const {
  return '\\';
}

String WindowsPlatform::getSharedLibraryPrefix() const {
  static String String = "";
  return String;
}

String WindowsPlatform::getSharedLibraryExtension() const {
  static String String = "dll";
  return String;
}

void WindowsPlatform::urgentMessage(const String &sMessage) const {
  MessageBoxW(nullptr, sMessage.cstr(), L"Urgent message", MB_OK | MB_ICONERROR);
}


void WindowsPlatform::sleep(uint64 nMilliseconds) const {
  SleepEx(static_cast<DWORD>(nMilliseconds), false);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // RECore
