; -- Example1.iss --
; Demonstrates copying 3 files and creating an icon.

; SEE THE DOCUMENTATION FOR DETAILS ON CREATING .ISS SCRIPT FILES!

[Setup]
AppName=����
AppVersion=1.4.0.0AppPublisher=��ţ��
DefaultDirName={pf}\MouMou
DefaultGroupName=MouMou
UninstallDisplayName= ���� 1.4.0.0
UninstallDisplayIcon={app}\MouMou.exe
Compression=lzma2
SolidCompression=yes
SetupIconFile="ReleaseVersionBuyer\MouMou.ico"
OutputDir="Release"
OutputBaseFilename=MouMou_online_1.4.0.0

[dirs]
Name:"{app}\Emotion"
Name:"{app}\Voice"

[Languages]
Name: "cs"; MessagesFile: "compiler:Languages\ChineseSimp.isl"

;����Ϊ�����ļ����ڵ�·��
[Files]
Source: "ReleaseVersionBuyer\MouMou.exe"; DestDir: "{app}"
Source: "ReleaseVersionBuyer\fcl.dll"; DestDir: "{app}"
Source: "ReleaseVersionBuyer\GipsVideoEngine.dll"; DestDir: "{app}"
Source: "ReleaseVersionBuyer\GipsVoiceEngine.dll"; DestDir: "{app}"
Source: "ReleaseVersionBuyer\HeadPhoto.dll"; DestDir: "{app}"
Source: "ReleaseVersionBuyer\HmgCrtLib.dll"; DestDir: "{app}"
Source: "ReleaseVersionBuyer\HmgSkin.dll"; DestDir: "{app}"
Source: "ReleaseVersionBuyer\HmgSkinRes.dll"; DestDir: "{app}"
Source: "ReleaseVersionBuyer\ImageJK.dll"; DestDir: "{app}"
Source: "ReleaseVersionBuyer\Mini139_res.dll"; DestDir: "{app}"
Source: "ReleaseVersionBuyer\minidata_dll.dll"; DestDir: "{app}"
Source: "ReleaseVersionBuyer\MiniNetTrans.dll"; DestDir: "{app}"
Source: "ReleaseVersionBuyer\MiniAtxControl.dll"; DestDir: "{app}"
Source: "ReleaseVersionBuyer\mfc100.dll"; DestDir: "{app}"
Source: "ReleaseVersionBuyer\msvcr100.dll"; DestDir: "{app}"
Source: "ReleaseVersionBuyer\msvcp100.dll"; DestDir: "{app}"
Source: "ReleaseVersionBuyer\MyArticles.dll"; DestDir: "{app}"Source: "ReleaseVersionBuyer\MyPictures.dll"; DestDir: "{app}"
Source: "ReleaseVersionBuyer\PictureLoadX.dll"; DestDir: "{app}"
Source: "ReleaseVersionBuyer\UserInfo.dll"; DestDir: "{app}"
Source: "ReleaseVersionBuyer\videocodec.dll"; DestDir: "{app}"
Source: "ReleaseVersionBuyer\WebInterface.dll"; DestDir: "{app}"
Source: "ReleaseVersionBuyer\WriteArticle.dll"; DestDir: "{app}"    
Source: "ReleaseVersionBuyer\address.xml"; DestDir: "{app}" 
Source: "ReleaseVersionBuyer\DuiLib.dll"; DestDir: "{app}"
Source: "ReleaseVersionBuyer\cryptopp.dll"; DestDir: "{app}"
Source: "ReleaseVersionBuyer\msftedit.dll"; DestDir: "{app}"   

;����ϵͳ����
Source: "ReleaseVersionBuyer\Emotion\b01.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionBuyer\Emotion\b02.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionBuyer\Emotion\b03.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionBuyer\Emotion\b04.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionBuyer\Emotion\b05.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionBuyer\Emotion\b06.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionBuyer\Emotion\b07.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionBuyer\Emotion\b08.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionBuyer\Emotion\b09.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionBuyer\Emotion\b10.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionBuyer\Emotion\b11.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionBuyer\Emotion\b12.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionBuyer\Emotion\b13.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionBuyer\Emotion\b14.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionBuyer\Emotion\b15.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionBuyer\Emotion\b16.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionBuyer\Emotion\b17.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionBuyer\Emotion\b18.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionBuyer\Emotion\b19.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionBuyer\Emotion\b20.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionBuyer\Emotion\b21.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionBuyer\Emotion\b22.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionBuyer\Emotion\b23.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionBuyer\Emotion\b24.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionBuyer\Emotion\b25.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionBuyer\Emotion\b26.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionBuyer\Emotion\b27.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionBuyer\Emotion\b28.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionBuyer\Emotion\b29.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionBuyer\Emotion\b30.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionBuyer\Emotion\b31.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionBuyer\Emotion\b32.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionBuyer\Emotion\b33.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionBuyer\Emotion\b34.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionBuyer\Emotion\b35.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionBuyer\Emotion\b36.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionBuyer\Emotion\b37.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionBuyer\Emotion\b38.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionBuyer\Emotion\b39.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionBuyer\Emotion\b40.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionBuyer\Emotion\b41.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionBuyer\Emotion\b42.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionBuyer\Emotion\b43.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionBuyer\Emotion\b44.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionBuyer\Emotion\b45.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionBuyer\Emotion\b46.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionBuyer\Emotion\b47.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionBuyer\Emotion\b48.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionBuyer\Emotion\b49.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionBuyer\Emotion\b50.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionBuyer\Emotion\b51.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionBuyer\Emotion\b52.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionBuyer\Emotion\b53.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionBuyer\Emotion\b54.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionBuyer\Emotion\b55.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionBuyer\Emotion\waitImage.gif"; DestDir: "{app}\Emotion"


;����
Source: "ReleaseVersionBuyer\Voice\��ʱ��Ϣ��-�����.wav"; DestDir: "{app}\Voice"
Source: "ReleaseVersionBuyer\Voice\��ϵ������-������.wav"; DestDir: "{app}\Voice"
Source: "ReleaseVersionBuyer\Voice\����������-������Ч.wav"; DestDir: "{app}\Voice"
Source: "ReleaseVersionBuyer\Voice\��Ƶ������-������.wav"; DestDir: "{app}\Voice"
Source: "ReleaseVersionBuyer\Voice\ϵͳ��Ϣ-ţ��4����.wav"; DestDir: "{app}\Voice"
Source: "ReleaseVersionBuyer\Voice\����������-������.wav"; DestDir: "{app}\Voice"

;��ݶ���ģ��
Source: "ReleaseVersionSeller\ExcelTemplate\FastWordCustom.xls"; DestDir: "{app}\ExcelTemplate"

;ע�����
Source: "ReleaseVersionBuyer\ImageJK.dll"; DestDir: "{app}"; Flags:ignoreversion regserver
Source: "ReleaseVersionBuyer\ImageJK.dll"; DestDir: "{app}"; Flags:regtypelib

Source: "ReleaseVersionBuyer\MiniAtxControl.dll"; DestDir: "{app}"; Flags:ignoreversion regserver
Source: "ReleaseVersionBuyer\MiniAtxControl.dll"; DestDir: "{app}"; Flags:regtypelib


;WEB��DLL�ļ�ע��
;Source: "ReleaseVersionBuyer\npIMPlugin.dll"; DestDir: "{app}"; Flags:ignoreversion regserver  
;Source: "ReleaseVersionBuyer\npIMPlugin.dll"; DestDir: "{app}"; Flags:regtypelib

;����
Source: "ReleaseVersionBuyer\msyh.ttf"; DestDir: "{fonts}"; FontInstall: "΢���ź� ����"; Flags: onlyifdoesntexist uninsneveruninstall
Source: "ReleaseVersionBuyer\msyhbd.ttf"; DestDir: "{fonts}"; FontInstall: "΢���ź� ����"; Flags: onlyifdoesntexist uninsneveruninstall


;Source: "MyProg.chm"; DestDir: "{app}"
;Source: "Readme.txt"; DestDir: "{app}"; Flags: isreadme

[Icons]
Name: "{group}\MouMou"; Filename: "{app}\MouMou.exe";WorkingDir: "{app}"
Name: "{group}\{cm:UninstallProgram,MouMou}"; Filename: "{uninstallexe}"
Name: "{userdesktop}\MouMou"; Filename: "{app}\MouMou.exe"


[Registry]
;HKCR ��HKEY_CLASSES_ROOT��  
;HKCU ��HKEY_CURRENT_USER��  
;HKLM ��HKEY_LOCAL_MACHINE��Ĭ��д����  
;HKU  ��HKEY_USERS��  
;HKCC ��HKEY_CURRENT_CONFIG��
Root: HKCR; Subkey: "feiniu"; ValueType: string; ValueName: "URL Protocol" ; ValueData: "{app}\MouMou.exe"
Root: HKCR; Subkey: "feiniu"; ValueType: string; ValueData: "FeiniuProtocol"
Root: HKCR; Subkey: "feiniu\DefaultIcon";ValueType: string; ValueData: "{app}\MouMou.exe,1"
Root: HKCR; Subkey: "feiniu\shell\open\command"; ValueType: string;ValueData: "{app}\MouMou.exe %1"

Root: HKCR; Subkey: "feiniu";Flags: uninsdeletekey 
Root: HKCR; Subkey: "feiniu\DefaultIcon";Flags: uninsdeletekey 
Root: HKCR; Subkey: "feiniu\shell\open\command";Flags: uninsdeletekey 

[Code]

function InitializeUninstall(): Boolean;
var
  HasRun:HWND;
  Res: Integer;
  ResultCode: Integer;
begin
    HasRun := FindWindowByWindowName('����2015');
    while HasRun<>0 do
    begin
      Res:= MsgBox('��⵽�����������У���ȷ���˳�����', mbInformation, MB_OK );
      if  Res <> 7 then 
      begin
        Exec('cmd','/c taskkill /im MouMou.exe -f', '', SW_HIDE, ewNoWait, ResultCode);
        sleep(1000); 
        HasRun := FindWindowByWindowName('����2015');
      end
    end
    Result:= true;
end;


function  InitializeSetup():boolean;
  var
    ResultCode: Integer;
    uicmd: String;
    Res: Integer;     
    ErrorCode: Integer;
    HasRun:HWND;    
  begin
    DeleteFile(ExpandConstant('{userdocs}\MouMou\GuidePageConfig.ini'));
    HasRun := FindWindowByWindowName('����2015');
    while HasRun<>0 do
    begin
      Res:= MsgBox('��⵽�����������У���ȷ���˳�����', mbInformation, MB_OK );
      if  Res <> 7 then 
      begin
        Exec('cmd','/c taskkill /im MouMou.exe -f', '', SW_HIDE, ewNoWait, ResultCode);
        sleep(1000); 
        HasRun := FindWindowByWindowName('����2015');
      end
    end
    If RegQueryStringValue(HKEY_LOCAL_MACHINE, 'SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\����_is1', 'UninstallString', uicmd) then
    begin
        Exec(RemoveQuotes(uicmd), '/SILENT', '', SW_SHOW, ewWaitUntilTerminated, ResultCode);
        sleep(2000);   
    end;   
    Result:= true;
  end;

procedure CurPageChanged(CurPageID: Integer);
begin
    if CurPageID = wpWelcome then
    WizardForm.NextButton.OnClick(WizardForm);
end;

[run]
Filename: "{app}\MouMou.exe";Description:"����������������"; StatusMsg:"�𼱣���һ�¾ͺ�"; Flags: nowait postinstall skipifsilent




