; packet.dll by Azura
; for Tibia 8.40

.486
.model flat,stdcall
option casemap:none

include windows.inc
include kernel32.inc
includelib kernel32.lib

; the call to WS2_32.send
SEND_POINTER equ 005AB600h

; the value in the mov ecx before the call to WS2_32.send
SOCKET_POINTER equ 00784CE0h

; the key used to encrypt/decrypt the packets
XTEA_KEY equ SOCKET_POINTER + 2Ch

.code
DllEntry proc DllInstance:HINSTANCE,Reason:DWORD,Reserved1:DWORD
    mov eax,TRUE
    ret
DllEntry endp

SendPacket proc ProcessId:DWORD,Packet:DWORD
    ; general variables
    local Process               :DWORD
    local Socket                :DWORD

    ; packet variables
    local XteaKey[16]           :BYTE
    local OldMemoryProtection   :DWORD
    local NumberOfBlocks        :DWORD
    local PacketLength          :DWORD
    local NewPacketPointer      :DWORD
    local EncryptionPointer     :DWORD
    local OriginalPacketPointer :DWORD

    ; code injection variables
    local TotalLength           :DWORD
    local RemoteAllocatedMemory :DWORD
    local PacketInformation     :DWORD
    local RemoteThread          :DWORD

    jmp @SendPacket

; the code that is injected together with the packet in Tibia's process
@SendPacketThread:
    push 0
    push DWORD ptr ds:[ebx]
    add ebx,4
    push ebx
    mov eax,DWORD ptr ds:[SOCKET_POINTER]
    push DWORD ptr ds:[eax+4]
    call DWORD ptr ds:[SEND_POINTER]
    retn

; code start
@SendPacket:
    ; open a handle to Tibia's process
    invoke OpenProcess,PROCESS_CREATE_THREAD+PROCESS_QUERY_INFORMATION+PROCESS_VM_OPERATION+PROCESS_VM_WRITE+PROCESS_VM_READ,NULL,ProcessId
    .if eax!=0
        mov Process,eax

        ; get the socket pointer and control so we only send a packet when there is a connection
        invoke ReadProcessMemory,Process,SOCKET_POINTER,addr Socket,4,NULL
        .if Socket!=0
            ; read the XTEA key from Tibia's memory
            invoke ReadProcessMemory,Process,XTEA_KEY,addr XteaKey,16,NULL

            ; calculate how many blocks of 8-bytes that is going to be encrypted
            mov eax,Packet
            movzx eax,WORD ptr [eax]
            add eax,2
            mov ecx,8
            xor edx,edx
            div ecx
            .if edx!=0
                inc eax
            .endif
            mov NumberOfBlocks,eax

            ; make space for the encrypted packet
            lea eax,[eax*8+2+4]
            mov PacketLength,eax
            invoke VirtualAlloc,NULL,eax,MEM_COMMIT,PAGE_READWRITE
            mov NewPacketPointer,eax

            ; calculate and write the header
            mov eax,PacketLength
            sub eax,2
            mov ecx,NewPacketPointer
            mov WORD ptr [ecx],ax

            ; fix the pointer variables
            add ecx,6
            mov EncryptionPointer,ecx
            mov eax,Packet
            mov OriginalPacketPointer,eax

            ; XTEA encryption loop
            push ebx
            push edi
            push esi
            .repeat
                xor eax,eax
                mov edx,OriginalPacketPointer
                mov ecx,DWORD ptr [edx]
                mov edx,DWORD ptr [edx+4]
                mov edi,32
                lea esi,XteaKey
                push ebp
                .repeat
                    mov ebx,edx
                    shr ebx,5
                    mov ebp,edx
                    shl ebp,4
                    xor ebx,ebp
                    add ebx,edx
                    mov ebp,eax
                    AND ebp,3
                    mov ebp,DWORD ptr ds:[esi+ebp*4]
                    add ebp,eax
                    xor ebx,ebp
                    add ecx,ebx
                    sub eax,61C88647h
                    mov ebx,ecx
                    shr ebx,5
                    mov ebp,ecx
                    shl ebp,4
                    xor ebx,ebp
                    add ebx,ecx
                    mov ebp,eax
                    shr ebp,0Bh
                    AND ebp,3
                    mov ebp,DWORD ptr ds:[esi+ebp*4]
                    add ebp,eax
                    xor ebx,ebp
                    add edx,ebx
                    dec edi
                .until edi==0
                pop ebp
                mov eax,EncryptionPointer
                mov DWORD ptr [eax],ecx
                mov DWORD ptr [eax+4],edx
                add EncryptionPointer,8
                add OriginalPacketPointer,8
                dec NumberOfBlocks
            .until NumberOfBlocks==0

            ; calculate Adler-32 checksum
            mov eax,1
            xor ebx,ebx
            mov ecx,NewPacketPointer
            add ecx,6
            mov edi,PacketLength
            sub edi,6
            .if edi>5552
                mov edi,5552
            .endif
            .repeat
                movzx edx,BYTE ptr ds:[ecx]
                add eax,edx
                add ebx,eax
                inc ecx
                dec edi
            .until edi==0
            pop esi
            pop edi
            mov ecx,65521
            xor edx,edx
            div ecx
            mov eax,ebx
            mov ebx,edx
            xor edx,edx
            div ecx
            mov ecx,ebx
            pop ebx

            ; write checksum to packet
            mov eax,NewPacketPointer
            mov WORD ptr [eax+2],cx
            mov WORD ptr [eax+4],dx

            ; calculate total length of packet and the code to inject
            mov eax,PacketLength
            add eax,((offset @SendPacket - offset @SendPacketThread) + sizeof DWORD)
            mov TotalLength,eax

            ; make space in Tibia's process
            invoke VirtualAllocEx,Process,NULL,TotalLength,MEM_COMMIT+MEM_RESERVE,PAGE_EXECUTE_READWRITE
            mov RemoteAllocatedMemory,eax
            mov PacketInformation,eax

            ; write packet and code to Tibia's Process
            invoke WriteProcessMemory,Process,RemoteAllocatedMemory,offset @SendPacketThread,(offset @SendPacket - offset @SendPacketThread),NULL
            add PacketInformation,(offset @SendPacket - offset @SendPacketThread)
            invoke WriteProcessMemory,Process,PacketInformation,addr PacketLength,4,NULL
            add PacketInformation,4
            invoke WriteProcessMemory,Process,PacketInformation,NewPacketPointer,PacketLength,NULL
            sub PacketInformation,4

            ; create remote thread
            invoke CreateRemoteThread,Process,NULL,NULL,RemoteAllocatedMemory,PacketInformation,NULL,NULL
            mov RemoteThread,eax
            invoke WaitForSingleObject,RemoteThread,INFINITE

            ; clean up
            invoke CloseHandle,RemoteThread
            invoke VirtualFree,NewPacketPointer,0,MEM_RELEASE
            invoke VirtualFreeEx,Process,RemoteAllocatedMemory,0,MEM_RELEASE
        .endif
        invoke CloseHandle,Process
    .endif
    ret
SendPacket endp

end DllEntry
