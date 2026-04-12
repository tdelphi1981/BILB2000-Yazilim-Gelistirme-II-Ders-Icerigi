; obys_installer.iss -- OBYS v5 Windows kurulum betigi (Inno Setup)
;
; Bolum 8 - Unite 5 - Kurulum Hazirlama ve Dagitilabilir Paket
;
; Bu betik Inno Setup 6 ile derlenir:
;   ISCC.exe obys_installer.iss
;
; Ciktisi: Output\OBYS-v1.0.0-Setup.exe
; Son kullanici kurulum sirasinda dil secemez, Turkce varsayilan dildir.

#define MyAppName "OBYS"
#define MyAppVersion "1.0.0"
#define MyAppPublisher "KTU Bilgisayar Bilimleri"
#define MyAppURL "https://www.ktu.edu.tr"
#define MyAppExeName "obys_v4.exe"
#define MyBuildDir "02_obys_v5_release\build\release"

[Setup]
; Uygulama kimligi (tekil GUID)
AppId={{8E9B7A21-3C42-4F60-9A1E-2D7C5BCE0031}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}

; Varsayilan kurulum dizini (64-bit Program Files)
DefaultDirName={autopf}\{#MyAppName}
DefaultGroupName={#MyAppName}
DisableProgramGroupPage=yes

; Kaldirici otomatik olusturulur
UninstallDisplayIcon={app}\{#MyAppExeName}
UninstallDisplayName={#MyAppName} {#MyAppVersion}

; Sikistirma ayarlari
Compression=lzma2/ultra64
SolidCompression=yes
LZMAUseSeparateProcess=yes

; 64-bit kurulum
ArchitecturesAllowed=x64
ArchitecturesInstallIn64BitMode=x64

; Cikti
OutputDir=Output
OutputBaseFilename=OBYS-v{#MyAppVersion}-Setup
SetupIconFile=
WizardStyle=modern

; Yonetici yetkisi gerektirmez (per-user yerine per-machine icin admin)
PrivilegesRequired=admin

[Languages]
Name: "turkish"; MessagesFile: "compiler:Languages\Turkish.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; \
    GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; \
    GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
; Ana exe ve windeployqt cikti dizini (tamami)
Source: "{#MyBuildDir}\{#MyAppExeName}"; DestDir: "{app}"; \
    Flags: ignoreversion
Source: "{#MyBuildDir}\*.dll"; DestDir: "{app}"; \
    Flags: ignoreversion
Source: "{#MyBuildDir}\platforms\*"; DestDir: "{app}\platforms"; \
    Flags: ignoreversion recursesubdirs createallsubdirs
Source: "{#MyBuildDir}\styles\*"; DestDir: "{app}\styles"; \
    Flags: ignoreversion recursesubdirs createallsubdirs skipifsourcedoesntexist
Source: "{#MyBuildDir}\imageformats\*"; DestDir: "{app}\imageformats"; \
    Flags: ignoreversion recursesubdirs createallsubdirs skipifsourcedoesntexist
Source: "{#MyBuildDir}\iconengines\*"; DestDir: "{app}\iconengines"; \
    Flags: ignoreversion recursesubdirs createallsubdirs skipifsourcedoesntexist

; Dokumantasyon
Source: "02_obys_v5_release\docs\user-guide.md"; \
    DestDir: "{app}\docs"; Flags: ignoreversion
Source: "02_obys_v5_release\README.md"; \
    DestDir: "{app}"; Flags: ignoreversion
Source: "CHANGELOG.md"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; \
    Filename: "{uninstallexe}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; \
    Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\{#MyAppName}"; \
    Filename: "{app}\{#MyAppExeName}"; Tasks: quicklaunchicon

[Run]
Filename: "{app}\{#MyAppExeName}"; \
    Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; \
    Flags: nowait postinstall skipifsilent
