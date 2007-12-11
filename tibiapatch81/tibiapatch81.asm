; Tibia Patch for Tibia 8.1
; Multi-Client, Link Copy Warning, and Name Spy Patch

.386
.model flat,stdcall
option casemap:none

include windows.inc
include user32.inc
include kernel32.inc
include comdlg32.inc 
	
includelib user32.lib
includelib kernel32.lib
includelib comdlg32.lib

.const
MAXSIZE equ 260

addyMultiClient		equ 000F8944h
addyLinkCopyWarning	equ 0010C077h
addyNameSpy			equ 000DF469h
addyNameSpy2		equ 000DF473h

.data
ofn OPENFILENAME <>
buffer db MAXSIZE dup(0)
fileFilter	db "Tibia.exe",0,"Tibia.exe",0
			db "Executable Files",0,"*.exe",0,0

textBackup		db "Tibia.exe.bak",0

textAppName		db "Multi-Client, Link Copy Warning, and Name Spy Patch for Tibia 8.1",0
textError		db "Error",0
textErrorMsg	db "Could not patch Tibia.exe!",0
textSuccess		db "Success",0
textSuccessMsg	db "Tibia.exe successfully patched!",0

patchMultiClient		db 0EBh
patchLinkCopyWarning	db 0EBh
patchNameSpy			db 090h, 090h
patchNameSpy2			db 090h, 090h

.data?
exeFile dd ?
exeBackup dd ?
bytesWritten dd ?

.code
main:
	mov ofn.lStructSize,sizeof ofn
	mov ofn.lpstrFilter,offset fileFilter
	mov ofn.lpstrFile,offset buffer
	mov ofn.nMaxFile,MAXSIZE
	mov ofn.lpstrTitle,offset textAppName
	mov ofn.Flags,OFN_FILEMUSTEXIST or OFN_PATHMUSTEXIST or OFN_LONGNAMES or OFN_EXPLORER or OFN_HIDEREADONLY
	
	invoke GetOpenFileName,addr ofn
	.if eax==TRUE
		invoke CreateFile,addr buffer,GENERIC_WRITE,0,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0
		.if eax==INVALID_HANDLE_VALUE
			invoke MessageBox,0,addr textErrorMsg,addr textError,MB_ICONHAND
			jmp exit
		.endif
		mov exeFile,eax

		invoke SetFilePointer,exeFile,addyMultiClient,0,FILE_BEGIN
		invoke WriteFile,exeFile,addr patchMultiClient,1,addr bytesWritten,0
		
		invoke SetFilePointer,exeFile,addyLinkCopyWarning,0,FILE_BEGIN
		invoke WriteFile,exeFile,addr patchLinkCopyWarning,1,addr bytesWritten,0
		
		invoke SetFilePointer,exeFile,addyNameSpy,0,FILE_BEGIN
		invoke WriteFile,exeFile,addr patchNameSpy,2,addr bytesWritten,0
		
		invoke SetFilePointer,exeFile,addyNameSpy2,0,FILE_BEGIN
		invoke WriteFile,exeFile,addr patchNameSpy2,2,addr bytesWritten,0
		
		invoke CloseHandle,exeFile
		invoke MessageBox,0,addr textSuccessMsg, addr textSuccess,MB_ICONINFORMATION
	.endif

exit:
	invoke ExitProcess,0

end main