/* See: http://msdn.microsoft.com/library/en-us/shellcc/platform/shell/programmersguide/versions.asp */
#include <windows.h>
#include <shlwapi.h>
__declspec(dllexport) HRESULT DllGetVersion(DLLVERSIONINFO2 *pdvi);

HRESULT DllGetVersion (DLLVERSIONINFO2 *pdvi)
{
	if ( !pdvi || (pdvi->info1.cbSize != sizeof (*pdvi)) )
		return (E_INVALIDARG);
	pdvi->info1.dwMajorVersion = 5;
	pdvi->info1.dwMinorVersion = 2;
	pdvi->info1.dwBuildNumber = 1544;
	pdvi->info1.dwPlatformID = DLLVER_PLATFORM_WINDOWS;
	if (pdvi->info1.cbSize == sizeof (DLLVERSIONINFO2))
		pdvi->ullVersion = MAKEDLLVERULL (5, 2, 1544, 40270);
	return S_OK;
}
