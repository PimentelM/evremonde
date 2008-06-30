/*
Tibia Patch for Tibia 8.1
Multi-Client and Link Copy Warning
by Evremonde
Last updated on April 3rd, 2008
*/

#include <windows.h>
#include <commdlg.h>

// memory addresses
const int MULTI_CLIENT		= 0x000F8944;
const int LINK_COPY_WARNING	= 0x0010C077;

// patch byte
const int PATCH_BYTE = 0xEB;

// globals
OPENFILENAME ofn;
HWND hwnd;
HANDLE file;
DWORD buffer;
char tempFile[1024];

int main()
{
	// initialize open file dialog
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrTitle = "Multi-Client and Link Copy Warning Patch for Tibia 8.1";
	ofn.lpstrFilter = "Tibia.exe\0Tibia.exe\0"; // file extension filter
	ofn.lpstrFile = tempFile;
	ofn.nMaxFile = sizeof(tempFile);
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_LONGNAMES | OFN_EXPLORER | OFN_HIDEREADONLY;

	// show open file dialog
	if (GetOpenFileName(&ofn))
	{
		// create file
		file = CreateFile(ofn.lpstrFile, GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (file == INVALID_HANDLE_VALUE)
		{
			// patch failed
			MessageBox(0,
				"Could not patch Tibia.exe!",
				"Error", MB_ICONERROR | MB_OK);

			return 0;
		}

		// patch multi-client
		SetFilePointer(file, MULTI_CLIENT, 0, FILE_BEGIN);
		WriteFile(file, &PATCH_BYTE, 1, &buffer, 0);

		// patch link copy warning
		SetFilePointer(file, LINK_COPY_WARNING, 0, FILE_BEGIN);
		WriteFile(file, &PATCH_BYTE, 1, &buffer, 0);

		// close file
		CloseHandle(file);

		// patch succeeded
		MessageBox(0,
			"Tibia.exe successfully patched!",
			"Success", MB_ICONINFORMATION | MB_OK);
	}

	return 0;
}
