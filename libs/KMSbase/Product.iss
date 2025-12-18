
; Author    KMS - Martin Dubois, P.Eng.
; Copyright (C) 2020 KMS
; License   http://www.apache.org/licenses/LICENSE-2.0
; Product   KmsBase
; File      Product.iss

; CODE REVIEW 2020-10-27 KMS - Martin Dubois, P.Eng.

[Setup]
AllowNetworkDrive=no
AllowUNCPath=no
AppCopyright=Copyright (C) 2020 KMS.
AppName=KmsBase
AppPublisher=KMS
AppPublisherURL=http://www.kms-quebec.com
AppSupportURL=http://www.kms-quebec.com
AppVersion=3.0.30
ArchitecturesAllowed=x64 x86
ArchitecturesInstallIn64BitMode=x64
DefaultDirName={pf}\KmsBase
LicenseFile=License.txt
MinVersion=10.0
OutputBaseFilename=KmsBase_3.0.30
OutputDir=Installer

[Files]
Source: "_DocUser\KmsBase.ReadMe.txt"                      ; DestDir: "{app}"; Flags: isreadme
Source: "Debug\KmsLib.lib"                                 ; DestDir: "{app}\Libraries\Debug_32"
Source: "Debug\KmsLib.pdb"                                 ; DestDir: "{app}\Libraries\Debug_32"
Source: "Debug_DLL\KmsLib.lib"                             ; DestDir: "{app}\Libraries\Debug_32_DLL"
Source: "DoxyFile_en.txt"                                  ; DestDir: "{app}"
Source: "DoxyFile_fr.txt"                                  ; DestDir: "{app}"
Source: "Includes\KmsBase.h"                               ; DestDir: "{app}\Includes"
Source: "Includes\KmsLib\CmdLineParser.h"                  ; DestDir: "{app}\Includes\KmsLib"
Source: "Includes\KmsLib\DebugLog.h"                       ; DestDir: "{app}\Includes\KmsLib"
Source: "Includes\KmsLib\DriverHandle.h"                   ; DestDir: "{app}\Includes\KmsLib"
Source: "Includes\KmsLib\Dump.h"                           ; DestDir: "{app}\Includes\KmsLib"
Source: "Includes\KmsLib\Exception.h"                      ; DestDir: "{app}\Includes\KmsLib"
Source: "Includes\KmsLib\File.h"                           ; DestDir: "{app}\Includes\KmsLib"
Source: "Includes\KmsLib\FileHandle.h"                     ; DestDir: "{app}\Includes\KmsLib"
Source: "Includes\KmsLib\IgnoreList.h"                     ; DestDir: "{app}\Includes\KmsLib"
Source: "Includes\KmsLib\KmsLib.h"                         ; DestDir: "{app}\Includes\KmsLib"
Source: "Includes\KmsLib\MemTester.h"                      ; DestDir: "{app}\Includes\KmsLib"
Source: "Includes\KmsLib\NetworkAddress.h"                 ; DestDir: "{app}\Includes\KmsLib"
Source: "Includes\KmsLib\RLE.h"                            ; DestDir: "{app}\Includes\KmsLib"
Source: "Includes\KmsLib\Socket.h"                         ; DestDir: "{app}\Includes\KmsLib"
Source: "Includes\KmsLib\String.h"                         ; DestDir: "{app}\Includes\KmsLib"
Source: "Includes\KmsLib\TextFile.h"                       ; DestDir: "{app}\Includes\KmsLib"
Source: "Includes\KmsLib\TextFileEditor.h"                 ; DestDir: "{app}\Includes\KmsLib"
Source: "Includes\KmsLib\ThreadBase.h"                     ; DestDir: "{app}\Includes\KmsLib"
Source: "Includes\KmsLib\ToolBase.h"                       ; DestDir: "{app}\Includes\KmsLib"
Source: "Includes\KmsLib\ValueVector.h"                    ; DestDir: "{app}\Includes\KmsLib"
Source: "Includes\KmsLib\Walker.h"                         ; DestDir: "{app}\Includes\KmsLib"
Source: "Includes\KmsLib\Walker_Smart.h"                   ; DestDir: "{app}\Includes\KmsLib"
Source: "Includes\KmsLib\Windows\ComPortHandle.h"          ; DestDir: "{app}\Includes\KmsLib\Windows"
Source: "Includes\KmsLib\Windows\RegistryKey.h"            ; DestDir: "{app}\Includes\KmsLib\Windows"
Source: "Includes\KmsLib\Windows\Service.h"                ; DestDir: "{app}\Includes\KmsLib\Windows"
Source: "Includes\KmsLib\Windows\SystemLog.h"              ; DestDir: "{app}\Includes\KmsLib\Windows"
Source: "Includes\KmsLib\Windows\Windows.h"                ; DestDir: "{app}\Includes\KmsLib\Windows"
Source: "Includes\KmsTest.h"                               ; DestDir: "{app}\Includes"
Source: "Includes\KmsTool.h"                               ; DestDir: "{app}\Includes"
Source: "Includes\Ring.h"                                  ; DestDir: "{app}\Includes"
Source: "Includes\SafeAPI.h"                               ; DestDir: "{app}\Includes"
Source: "Includes\WindowsToLinux.h"                        ; DestDir: "{app}\Includes"
Source: "KmsCopy\_DocUser\KmsBase.KmsCopy.ReadMe.txt"      ; DestDir: "{app}"
Source: "KmsLib\_DocUser\KmsBase.KmsLib.ReadMe.txt"        ; DestDir: "{app}"
Source: "KmsVersion\_DocUser\KmsBase.KmsVersion.ReadMe.txt"; DestDir: "{app}"
Source: "Release\KmsCopy.exe"                              ; DestDir: "{app}"
Source: "Release\KmsCopy.exe"                              ; DestDir: "{app}\Binaries\Release_32"
Source: "Release\KmsLib.lib"                               ; DestDir: "{app}\Libraries\Release_32"
Source: "Release\KmsVersion.exe"                           ; DestDir: "{app}"
Source: "Release\KmsVersion.exe"                           ; DestDir: "{app}\Binaries\Release_32"
Source: "Release_DLL\KmsLib.lib"                           ; DestDir: "{app}\Libraries\Release_32_DLL"
Source: "RunDoxygen.cmd"                                   ; DestDir: "{app}"
Source: "Scripts\Import.cmd"                               ; DestDir: "{app}"
Source: "Scripts\Import.cmd.txt"                           ; DestDir: "{app}"
Source: "Scripts\Import.txt"                               ; DestDir: "{app}"
Source: "Scripts\KmsBase_Build_Begin.cmd"                  ; DestDir: "{app}\Scripts"
Source: "Scripts\KmsBase_Build_Installer.cmd"              ; DestDir: "{app}\Scripts"
Source: "Scripts\KmsBase_CreateCab.cmd"                    ; DestDir: "{app}\Scripts"
Source: "Scripts\KmsBase_Export.cmd"                       ; DestDir: "{app}\Scripts"
Source: "Scripts\KmsBase_Export_Signed.cmd"                ; DestDir: "{app}\Scripts"
Source: "Scripts\KmsBase_Sign.cmd"                         ; DestDir: "{app}\Scripts"
Source: "x64\Debug\KmsLib.lib"                             ; DestDir: "{app}\Libraries\Debug_64"
Source: "x64\Debug\KmsLib.pdb"                             ; DestDir: "{app}\Libraries\Debug_64"
Source: "x64\Debug_DLL\KmsLib.lib"                         ; DestDir: "{app}\Libraries\Debug_64_DLL"
Source: "x64\Release\KmsCopy.exe"                          ; DestDir: "{app}\Binaries\Release_64"
Source: "x64\Release\KmsLib.lib"                           ; DestDir: "{app}\Libraries\Release_64"
Source: "x64\Release_DLL\KmsLib.lib"                       ; DestDir: "{app}\Libraries\Release_64_DLL"
