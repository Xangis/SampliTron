; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{10216C99-86D8-452F-9667-6A85EC2ED5B2}
AppName=SampliTron
AppVerName=SampliTron 1.1
AppPublisher=Zeta Centauri
AppPublisherURL=http://zetacentauri.com
AppSupportURL=http://zetacentauri.com
AppUpdatesURL=http://zetacentauri.com
DefaultDirName={pf}\Zeta Centauri\SampliTron
DefaultGroupName=Zeta Centauri\SampliTron
LicenseFile=C:\Users\Xangis\code\SampliTron\Release\License.txt
OutputDir=C:\Users\Xangis\code\SampliTron\installer
OutputBaseFilename=SampliTron1.1Setup
SetupIconFile=C:\Users\Xangis\code\SampliTron\Release\samplitron.ico
UninstallDisplayIcon={app}\samplitron.ico
Compression=lzma
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked; OnlyBelowVersion: 0,6.1

[Files]
Source: "C:\Users\Xangis\code\SampliTron\Release\SampliTron.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\Xangis\code\SampliTron\Release\libsndfile-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\Xangis\code\SampliTron\Release\License.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\Xangis\code\SampliTron\Release\samplitron.htb"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\Xangis\code\SampliTron\Release\samplitron.ico"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\Xangis\code\SampliTron\Samples\*"; DestDir: "{app}\Samples"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:\Users\Xangis\code\SampliTron\Release\Patches\*"; DestDir: "{app}\Patches"; Flags: ignoreversion recursesubdirs createallsubdirs
; NOTE: Don't use "Flags: ignoreversion" on any shared system files
Source: "C:\Users\Xangis\code\SampliTron\Release\vcredist_x86.exe"; DestDir: "{app}"; Flags: ignoreversion deleteafterinstall
Source: "C:\Users\Xangis\code\SampliTron\Release\DirectX\DXSETUP.exe"; DestDir: "{app}"; Flags: ignoreversion deleteafterinstall
Source: "C:\Users\Xangis\code\SampliTron\Release\DirectX\DSETUP.dll"; DestDir: "{app}"; Flags: ignoreversion deleteafterinstall
Source: "C:\Users\Xangis\code\SampliTron\Release\DirectX\dsetup32.dll"; DestDir: "{app}"; Flags: ignoreversion deleteafterinstall
Source: "C:\Users\Xangis\code\SampliTron\Release\DirectX\dxupdate.cab"; DestDir: "{app}"; Flags: ignoreversion deleteafterinstall
Source: "C:\Users\Xangis\code\SampliTron\Release\DirectX\Aug2009_XAudio_x86.cab"; DestDir: "{app}"; Flags: ignoreversion deleteafterinstall

[Icons]
Name: "{group}\SampliTron"; Filename: "{app}\SampliTron.exe"; WorkingDir: "{app}"
Name: "{group}\{cm:ProgramOnTheWeb,SampliTron}"; Filename: "http://zetacentauri.com/software_samplitron.htm"
Name: "{commondesktop}\SampliTron"; Filename: "{app}\SampliTron.exe"; Tasks: desktopicon; WorkingDir: "{app}"
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\SampliTron"; Filename: "{app}\SampliTron.exe"; Tasks: quicklaunchicon; WorkingDir: "{app}"

[Run]
Filename: "{app}\vcredist_x86.exe"; Parameters: "/q"; WorkingDir: "{app}";  StatusMsg: "Installing Visual C++ 2010 Redistributable..."; Flags: waituntilterminated
Filename: "{app}\DXSETUP.exe"; Parameters: "/SILENT"; WorkingDir: "{app}";  StatusMsg: "Installing DirectX XAudio2 Redistributable..."; Flags: waituntilterminated
Filename: "{app}\SampliTron.exe"; Description: "{cm:LaunchProgram,SampliTron}"; Flags: nowait postinstall skipifsilent

