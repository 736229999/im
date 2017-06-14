; -- Example1.iss --
; Demonstrates copying 3 files and creating an icon.

; SEE THE DOCUMENTATION FOR DETAILS ON CREATING .ISS SCRIPT FILES!

[Setup]
AppName=MouMou
AppVersion=1.0.0.5
DefaultDirName={pf}\MouMou
DefaultGroupName=MouMou
UninstallDisplayIcon={app}\MouMou.exe
Compression=lzma2
SolidCompression=yes
SetupIconFile="Release Version\MouMou.ico"
OutputDir="Release"
OutputBaseFilename=MouMou_1.0.0.5

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

;����Ϊ�����ļ����ڵ�·��
[Files]
Source: "Release Version\MouMou.exe"; DestDir: "{app}"
Source: "Release Version\fcl.dll"; DestDir: "{app}"
Source: "Release Version\GipsVideoEngine.dll"; DestDir: "{app}"
Source: "Release Version\GipsVoiceEngine.dll"; DestDir: "{app}"
Source: "Release Version\HeadPhoto.dll"; DestDir: "{app}"
Source: "Release Version\HmgCrtLib.dll"; DestDir: "{app}"
Source: "Release Version\HmgSkin.dll"; DestDir: "{app}"
Source: "Release Version\HmgSkinRes.dll"; DestDir: "{app}"
Source: "Release Version\ImageJK.dll"; DestDir: "{app}"
Source: "Release Version\Mini139_res.dll"; DestDir: "{app}"
Source: "Release Version\minidata_dll.dll"; DestDir: "{app}"
Source: "Release Version\MiniNetTrans.dll"; DestDir: "{app}"
Source: "Release Version\MiniAtxControl.dll"; DestDir: "{app}"
Source: "Release Version\mfc100.dll"; DestDir: "{app}"
Source: "Release Version\msvcr100.dll"; DestDir: "{app}"
Source: "Release Version\msvcp100.dll"; DestDir: "{app}"
Source: "Release Version\MyArticles.dll"; DestDir: "{app}"Source: "Release Version\MyPictures.dll"; DestDir: "{app}"
Source: "Release Version\PictureLoadX.dll"; DestDir: "{app}"
Source: "Release Version\peerconnection_client.dll"; DestDir: "{app}"Source: "Release Version\serverlist.config"; DestDir: "{app}"
Source: "Release Version\UserInfo.dll"; DestDir: "{app}"
Source: "Release Version\videocodec.dll"; DestDir: "{app}"
Source: "Release Version\WebInterface.dll"; DestDir: "{app}"
Source: "Release Version\WriteArticle.dll"; DestDir: "{app}"

Source: "Release Version\ImageJK.dll"; DestDir: "{app}"; Flags:ignoreversion regserver
Source: "Release Version\ImageJK.dll"; DestDir: "{app}"; Flags:regtypelib

Source: "Release Version\MiniAtxControl.dll"; DestDir: "{app}"; Flags:ignoreversion regserver
Source: "Release Version\MiniAtxControl.dll"; DestDir: "{app}"; Flags:regtypelib

;WEB��DLL�ļ�ע��
Source: "Release Version\npIMPlugin.dll"; DestDir: "{app}"; Flags:ignoreversion regserver  
Source: "Release Version\npIMPlugin.dll"; DestDir: "{app}"; Flags:regtypelib

;Source: "MyProg.chm"; DestDir: "{app}"
;Source: "Readme.txt"; DestDir: "{app}"; Flags: isreadme

[Icons]
;Name: "{group}\MouMou"; Filename: "{app}\MouMou.exe"
Name: "{userdesktop}\MouMou"; Filename: "{app}\MouMou.exe"

[Registry]
;HKCR ��HKEY_CLASSES_ROOT��  
;HKCU ��HKEY_CURRENT_USER��  
;HKLM ��HKEY_LOCAL_MACHINE��Ĭ��д����  
;HKU  ��HKEY_USERS��  
;HKCC ��HKEY_CURRENT_CONFIG��
Root: HKCR; Subkey: "feiniu"; ValueType: string; ValueName: "URL PROTOCOL" ; ValueData: "{app}"
Root: HKCR; Subkey: "feiniu"; ValueType: string; ValueName: "@" ; ValueData: "Feiniu Protocol"
Root: HKCR; Subkey: "feiniu\DefaultIcon";ValueType: string; ValueName: "@" ; ValueData: "{app}"
Root: HKCR; Subkey: "feiniu\shell\open\command"; ValueType: string; ValueName: "@" ; ValueData: "\{sd}"

Root: HKCR; Subkey: "feiniu";Flags: uninsdeletekey 
Root: HKCR; Subkey: "feiniu\DefaultIcon";Flags: uninsdeletekey 
Root: HKCR; Subkey: "feiniu\shell\open\command";Flags: uninsdeletekey 


[Code]
function  InitializeSetup():boolean;
var
   KeynotExist:boolean;
   ResultCode: Integer;
   uicmd: String;
   Res: Integer;     
   ErrorCode: Integer;
   IsRunning: Integer; 
 begin
   KeynotExist:= true;
   If RegQueryStringValue(HKEY_LOCAL_MACHINE, 'SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\MouMou_is1', 'UninstallString', uicmd) then
 begin
   KeynotExist:= false;
  Res:= MsgBox('��⵽����ϵͳ���Ѱ�װ�����,ѡ��"��",��ж�س���.�Ƿ������', mbInformation, MB_YESNO );
   if  Res <> 7 then 
   begin 
   Exec(RemoveQuotes(uicmd), '', '', SW_SHOW, ewWaitUntilTerminated, ResultCode); 
   end;  
   end;   
   Result:= KeynotExist
 end;