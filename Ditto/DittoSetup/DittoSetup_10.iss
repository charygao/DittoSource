#define MyAppName "Ditto"
#define MyAppVersion GetFileVersion("..\Release\DittoU.exe")
#define MyAppVerName MyAppName + " " + MyAppVersion

;#define bit64
  

[Setup]
AppName={#MyAppName}
AppVerName={#MyAppVerName}
OutputBaseFilename=DittoSetup_{#MyAppVersion}
AppPublisher=Scott Brogden
AppPublisherURL=ditto-cp.sourceforge.net
AppSupportURL=ditto-cp.sourceforge.net
AppUpdatesURL=ditto-cp.sourceforge.net
#ifdef bit64
  ArchitecturesInstallIn64BitMode=x64
  ArchitecturesAllowed=x64
#endif
DefaultDirName={pf}\{#MyAppName}
DefaultGroupName={#MyAppName}
;UsePreviousTasks=no
;DisableDirPage=yes
DisableProgramGroupPage=yes
DisableReadyPage=yes
DirExistsWarning=no
UninstallLogMode=overwrite
ChangesAssociations=yes
CloseApplications=yes
;vista forward
MinVersion=6.0.6000

[Languages]
Name: English; MessagesFile: compiler:Default.isl


Name: Deutsch; MessagesFile: German.isl
Name: Italiano; MessagesFile: Italian.isl
Name: French; MessagesFile: French.isl
Name: Portuguese; MessagesFile: Portuguese.isl
Name: Spanish; MessagesFile: Spanish.isl
Name: Polski; MessagesFile: Polish.isl
Name: Dutch; MessagesFile: Dutch.isl
Name: Swedish; MessagesFile: Swedish.isl
Name: Croatian; MessagesFile: Croatian.isl
Name: Turkish; MessagesFile: Turkish.isl
Name: Japanese; MessagesFile: Japanese.isl
Name: Chinese; MessagesFile: ChineseSimp.isl
Name: Romanian; MessagesFile: Romanian.isl
Name: Korean; MessagesFile: Korean.isl
Name: Russian; MessagesFile: Russian.isl
Name: Slovenian; MessagesFile: Slovenian.isl
Name: Czech; MessagesFile: Czech.isl
Name: Danish; MessagesFile: Danish.isl
Name: Greek; MessagesFile: Greek.isl
Name: Ukrainian; MessagesFile: Ukrainian.isl
Name: Hebrew; MessagesFile: Hebrew.isl
Name: Finnish; MessagesFile: Finnish.isl

[Tasks]
Name: RunAtStartup; Description: Run Ditto on windows startup
Name: AddFireWallException; Description: Add Windows Firewall exception for Ditto on port 23443;  Flags: unchecked

[Files]
#ifdef bit64
	Source: ..\Release64\Ditto.exe; DestDir: {app}; DestName: Ditto.exe; Flags: ignoreversion; AfterInstall: AddProgramToFirewall(ExpandConstant('{app}\Ditto.exe'), 'Ditto_FromInstaller_64');
	Source: ..\Release64\Addins\DittoUtil.dll; DestDir: {app}\Addins; Flags: ignoreversion
	Source: mfc-crt64\vcredist_x64_2017.exe; Flags: dontcopy;      
  Source: ..\Release64\icuuc58.dll; DestDir: {app}; Flags: ignoreversion
  Source: ..\Release64\icuin58.dll; DestDir: {app}; Flags: ignoreversion
  Source: ..\Release64\icutu58.dll; DestDir: {app}; Flags: ignoreversion
  Source: ..\Release64\icuio58.dll; DestDir: {app}; Flags: ignoreversion
  Source: ..\Release64\icudt58.dll; DestDir: {app}; Flags: ignoreversion
#endif
#ifndef bit64
	Source: ..\Release\Ditto.exe; DestDir: {app}; DestName: Ditto.exe; Flags: ignoreversion; AfterInstall: AddProgramToFirewall(ExpandConstant('{app}\Ditto.exe'), 'Ditto_FromInstaller_32');
  Source: ..\Release\Addins\DittoUtil.dll; DestDir: {app}\Addins; Flags: ignoreversion
	Source: mfc-crt\vcredist_x86_2017.exe; Flags: dontcopy;
  Source: ..\Release\icuuc58.dll; DestDir: {app}; Flags: ignoreversion
  Source: ..\Release\icuin58.dll; DestDir: {app}; Flags: ignoreversion
  Source: ..\Release\icutu58.dll; DestDir: {app}; Flags: ignoreversion
  Source: ..\Release\icuio58.dll; DestDir: {app}; Flags: ignoreversion
  Source: ..\Release\icudt58.dll; DestDir: {app}; Flags: ignoreversion
#endif

Source: Changes.txt; DestDir: {app}
Source: ..\Debug\Language\*; DestDir: {app}\Language; BeforeInstall: BeforeLanguageInstall()
Source: ..\Debug\Themes\*; DestDir: {app}\Themes
Source: ..\Help\*.htm; DestDir: {app}\Help; Flags: ignoreversion

[Icons]
Name: {group}\Ditto; Filename: {app}\Ditto.exe
Name: {group}\Ditto Help; Filename: {app}\Help\DittoGettingStarted.htm
Name: {group}\Uninstall; Filename: {uninstallexe}

[Run]
Filename: {app}\Ditto.exe; Description: Launch Ditto; Flags: nowait postinstall skipifsilent
Filename: {app}\Help\DittoGettingStarted.htm; Description: View Help; Flags: nowait postinstall skipifsilent shellexec unchecked
Filename: {app}\Changes.txt; Description: View Change History; Flags: nowait postinstall skipifsilent shellexec unchecked

[Registry]
Root: HKCU; Subkey: Software\Ditto; Flags: uninsdeletekey
Root: HKCU; Subkey: SOFTWARE\Microsoft\Windows\CurrentVersion\Run; ValueType: string; ValueName: Ditto; flags: uninsdeletevalue; ValueData: {app}\Ditto.exe; Tasks: RunAtStartup

Root: HKCU; Subkey: Software\Ditto; ValueType: dword; ValueName: SetFocus_iexplore.exe; ValueData: 00000001

Root: HKCU; Subkey: Software\Ditto; ValueType: string; ValueName: LanguageFile; ValueData: {language}

Root: HKCU; Subkey: Software\Ditto\PasteStrings; ValueType: string; ValueName: gvim.exe; ValueData: """{{PLUS}gP"
Root: HKCU; Subkey: Software\Ditto\CopyStrings; ValueType: string; ValueName: gvim.exe; ValueData: """{{PLUS}y"
Root: HKCU; Subkey: Software\Ditto\CutStrings; ValueType: string; ValueName: gvim.exe; ValueData: """{{PLUS}x"

Root: HKCU; Subkey: Software\Ditto\PasteStrings; ValueType: string; ValueName: cmd.exe; OnlyBelowVersion: 10; ValueData: % {{Delay100}ep
Root: HKCU; Subkey: Software\Ditto\CopyStrings; ValueType: string; ValueName: cmd.exe; OnlyBelowVersion: 10; ValueData: % {{Delay100}ey

Root: HKCU; Subkey: Software\Ditto\PasteStrings; ValueName: cmd.exe; MinVersion: 10; Flags: deletevalue
Root: HKCU; Subkey: Software\Ditto\CopyStrings; ValueName: cmd.exe; MinVersion: 10; Flags: deletevalue

;associate .dto with Ditto
Root: HKCR; Subkey: .dto; ValueType: string; ValueName: ; ValueData: Ditto; Flags: uninsdeletevalue
Root: HKCR; Subkey: Ditto; ValueType: string; ValueName: ; ValueData: Ditto; Flags: uninsdeletekey
Root: HKCR; Subkey: Ditto\DefaultIcon; ValueType: string; ValueName: ; ValueData: {app}\Ditto.exe,0
Root: HKCR; Subkey: Ditto\shell\open\command; ValueType: string; ValueName: ; ValueData: """{app}\Ditto.exe"" ""%1"""


[Code]
procedure BeforeLanguageInstall();
var
  sDir: String;
begin
    sDir := ExpandConstant('{app}');

    RenameFile(sDir+'\Language\Italian.xml', sDir+'\Language\Italian.xml.old')
end;

procedure CleanupOldFiles();
var
  sDir: String;
  begin
    sDir := ExpandConstant('{app}');

    DeleteFile(sDir+'\mfc100u.dll')
    DeleteFile(sDir+'\mfcm100u.dll')
    DeleteFile(sDir+'\msvcp100.dll')
    DeleteFile(sDir+'\msvcr100.dll')

    DeleteFile(sDir+'\iculx55.dll')
    DeleteFile(sDir+'\icule55.dll')
    DeleteFile(sDir+'\icuuc55.dll')
    DeleteFile(sDir+'\icutu55.dll')
    DeleteFile(sDir+'\icuio55.dll')
    DeleteFile(sDir+'\icuin55.dll')
    DeleteFile(sDir+'\icudt55.dll')
end;


procedure RegisterForCrashDump(theApp : String);
var
   theExe : String;
begin
  theExe := theApp + '.exe';
  if IsWin64() then
    begin
      if RegValueExists(HKLM64, 'Software\Microsoft\Windows\Windows Error Reporting\LocalDumps\' + theExe, 'DumpFolder') = false then
        RegWriteStringValue(HKLM64, 'Software\Microsoft\Windows\Windows Error Reporting\LocalDumps\' + theExe, 'DumpFolder', ExpandConstant('{userappdata}') + '\Ditto\Dumps');
      if RegValueExists(HKLM64, 'Software\Microsoft\Windows\Windows Error Reporting\LocalDumps\' + theExe, 'DumpType') = false then
        RegWriteDWordValue(HKLM64, 'Software\Microsoft\Windows\Windows Error Reporting\LocalDumps\' + theExe, 'DumpType', 2);
      if RegValueExists(HKLM64, 'Software\Microsoft\Windows\Windows Error Reporting\LocalDumps\' + theExe, 'DumpCount') = false then
        RegWriteDWordValue(HKLM64, 'Software\Microsoft\Windows\Windows Error Reporting\LocalDumps\' + theExe, 'DumpCount', 3);
    end
  else
    begin
      if RegValueExists(HKEY_LOCAL_MACHINE, 'Software\Microsoft\Windows\Windows Error Reporting\LocalDumps\' + theExe, 'DumpFolder') = false then
        RegWriteStringValue(HKEY_LOCAL_MACHINE, 'Software\Microsoft\Windows\Windows Error Reporting\LocalDumps\' + theExe, 'DumpFolder', ExpandConstant('{userappdata}') + '\Ditto\Dumps');
      if RegValueExists(HKEY_LOCAL_MACHINE, 'Software\Microsoft\Windows\Windows Error Reporting\LocalDumps\' + theExe, 'DumpType') = false then
        RegWriteDWordValue(HKEY_LOCAL_MACHINE, 'Software\Microsoft\Windows\Windows Error Reporting\LocalDumps\' + theExe, 'DumpType', 2);
      if RegValueExists(HKEY_LOCAL_MACHINE, 'Software\Microsoft\Windows\Windows Error Reporting\LocalDumps\' + theExe, 'DumpCount') = false then
        RegWriteDWordValue(HKEY_LOCAL_MACHINE, 'Software\Microsoft\Windows\Windows Error Reporting\LocalDumps\' + theExe, 'DumpCount', 3);
    end;
end;

function IsVC2017CRuntimeInstalled(): Boolean;
var
  Installed: Boolean;
  IsInstalled: Cardinal;
begin
  Installed := false
  IsInstalled := 0;

  #ifdef bit64

    if RegQueryDWordValue(HKLM, 'SOFTWARE\Microsoft\VisualStudio\14.0\VC\Runtimes\x64', 'Installed', IsInstalled) then
    begin
      if (IsInstalled = 1) then
      begin
        Installed := true;
      end;
    end;  

    //double check the HKLM64 key
    if (IsInstalled <> 1) and IsWin64() then 
    begin
      if RegQueryDWordValue(HKLM64, 'SOFTWARE\Microsoft\VisualStudio\14.0\VC\Runtimes\x64', 'Installed', IsInstalled) then
      begin
        if (IsInstalled = 1) then
        begin
          Installed := true;
        end;
      end;  
    end;

  #endif
  #ifndef bit64
    if RegQueryDWordValue(HKLM, 'SOFTWARE\Microsoft\VisualStudio\14.0\VC\Runtimes\x86', 'Installed', IsInstalled) then
    begin
      if (IsInstalled = 1) then
      begin
        Installed := true;
      end;
    end;  

    //double check the HKLM64 key
    if (IsInstalled <> 1) and IsWin64() then 
    begin
      if RegQueryDWordValue(HKLM64, 'SOFTWARE\Microsoft\VisualStudio\14.0\VC\Runtimes\x86', 'Installed', IsInstalled) then
      begin
        if (IsInstalled = 1) then
        begin
          Installed := true;
        end;
      end;  
    end;
  #endif

  Result := Installed;
end;

function InstallVc2017Runtime(): Boolean;
var
  nErrorCode: Integer;
begin
  Log('Installing VS 2017 C++ redistributable');

  #ifdef bit64
    ExtractTemporaryFile('vcredist_x64_2017.exe');
    ShellExec('', ExpandConstant('{tmp}\vcredist_x64_2017.exe'), '/q', '' , SW_HIDE, ewWaitUntilTerminated, nErrorCode);
  #endif
  #ifndef bit64
    ExtractTemporaryFile('vcredist_x86_2017.exe');
    ShellExec('', ExpandConstant('{tmp}\vcredist_x86_2017.exe'), '/q', '' , SW_HIDE, ewWaitUntilTerminated, nErrorCode);
  #endif
end;

procedure CheckForPreReqs();
var
  VCRuntime2017Installed: Boolean;
  nReturnCode: Integer;
begin
  VCRuntime2017Installed := IsVC2017CRuntimeInstalled();

  if VCRuntime2017Installed = true then
    Log('Microsoft VS 2017 C++ redistributable is already installed.')
  else
  begin
    InstallVc2017Runtime();
    if not IsVC2017CRuntimeInstalled then
    begin
      Log('VS 2017 C++ redistributable install failed!');
      SuppressibleMsgBox(CustomMessage('VCRuntimeInstallFailed'), mbInformation, MB_OK, idOK);
    end;
  end;

end;

procedure CurStepChanged(CurStep: TSetupStep);
var
  ProgressPage: TOutputProgressWizardPage;
  ErrorCode : Integer;
  AbortNeeded: Boolean;
begin
  AbortNeeded := false;
  case CurStep of
    ssInstall:      
    begin
      CheckForPreReqs();
    end;

	  ssPostInstall:
  	begin            
    end;
	
    ssDone:
    begin
      RegisterForCrashDump('Ditto')
      CleanupOldFiles()
    end;

  end;
end;


function RuleExistsInFirewall(RuleName : String) : Boolean;
var
  ErrorCode : Integer;
begin
  Exec('>', 'netsh advfirewall firewall show rule name="' + RuleName + '"', '', SW_HIDE, ewWaitUntilTerminated, ErrorCode);
  if ErrorCode = 0 then
    Result := True
  else
    Result := False;
end;

procedure AddProgramToFirewall(ProgramName : String; RuleName : String);
var
  ErrorCode : Integer;
  Success : Boolean;
  WindowsVersion : TWindowsVersion;
begin
  if IsTaskSelected('AddFireWallException') then
    begin
    GetWindowsVersionEx(WindowsVersion);
    if (WindowsVersion.Major < 6) then
      begin
        Success := Exec('>', 'netsh firewall add allowedprogram "' + ProgramName + '" "' + RuleName + '" ENABLE ALL', '', SW_HIDE, ewWaitUntilTerminated, ErrorCode);
      end
    else
      begin
        if (not RuleExistsInFirewall(RuleName)) then
          begin
            Success := Exec('>', 'netsh advfirewall firewall add rule name="' + RuleName + '" dir=in action=allow protocol=TCP localport=23443 program="' + ProgramName + '" enable=yes', '', SW_HIDE, ewWaitUntilTerminated, ErrorCode);
            Success := Exec('>', 'netsh advfirewall firewall add rule name="' + RuleName + '" dir=out action=allow protocol=TCP localport=23443 program="' + ProgramName + '" enable=yes', '', SW_HIDE, ewWaitUntilTerminated, ErrorCode);
          end
      end;
     
    if not Success then
       Log('Error - Unable to add ' + RuleName + ' to List of Windows firewall exceptions. ErrorCode: ' + IntToStr(ErrorCode))
    else
       Log(RuleName + ' successfully added to list of Windows firewall exceptions. ErrorCode: ' + IntToStr(ErrorCode))
  end
end;



[CustomMessages]
VCRuntimeInstallFailed=VCRuntime prerequisite install failed.