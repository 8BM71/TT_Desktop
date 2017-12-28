#define MyAppPublisher "TPU 8VM71 group"
#define MyAppURL "http://tttpu.tk/"
#define MyAppExeName "tputt.exe"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{C8D0FDAB-EE09-4C39-AFFA-93C69820DEB9}
AppName={#APPVEYOR_PROJECT_NAME}
AppVersion={#APPVEYOR_BUILD_VERSION}
AppVerName={#INSTALLER_NAME}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={sd}\TTTPU
DisableProgramGroupPage=yes
OutputDir={#APPVEYOR_BUILD_FOLDER}\bin\installs
OutputBaseFilename={#INSTALLER_NAME}
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "russian"; MessagesFile: "compiler:Languages\Russian.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: {#APPVEYOR_BUILD_FOLDER}\bin\release\*; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{commonprograms}\{#APPVEYOR_PROJECT_NAME}"; Filename: "{app}\{#MyAppExeName}"
Name: "{commondesktop}\{#APPVEYOR_PROJECT_NAME}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; WorkingDir: "{app}"; Description: "{cm:LaunchProgram,{#StringChange(INSTALLER_NAME, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

