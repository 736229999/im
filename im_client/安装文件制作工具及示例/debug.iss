; -- Example1.iss --
; Demonstrates copying 3 files and creating an icon.

; SEE THE DOCUMENTATION FOR DETAILS ON CREATING .ISS SCRIPT FILES!
;文件名相关
[Setup]
AppName=MouMou
AppVersion=1.0.0.3
DefaultDirName={pf}\MouMou
DefaultGroupName=MouMou
UninstallDisplayIcon={app}\MouMou.exe
Compression=lzma2
SolidCompression=yes
SetupIconFile="Release Version\MouMou.ico"
OutputDir="Release"
OutputBaseFilename=MouMou_1.0.0.3


;以下为所需文件所在的路径
[Files]
Source: "Debug Version\MouMou.exe"; DestDir: "{app}"
Source: "Debug Version\fcl.dll"; DestDir: "{app}"
Source: "Debug Version\HmgCrtLib.dll"; DestDir: "{app}"
Source: "Debug Version\HmgSkin.dll"; DestDir: "{app}"
Source: "Debug Version\HmgSkinRes.dll"; DestDir: "{app}"
Source: "Debug Version\Mini139_res.dll"; DestDir: "{app}"
Source: "Debug Version\MiniNetTrans.dll"; DestDir: "{app}"
Source: "Debug Version\serverlist.config"; DestDir: "{app}"
Source: "Debug Version\videocodec.dll"; DestDir: "{app}"
Source: "Debug Version\ImageJK.dll"; DestDir: "{app}"
Source: "Debug Version\MiniAtxControl.dll"; DestDir: "{app}"
Source: "Debug Version\minidata_dll.dll"; DestDir: "{app}"

Source: "Debug Version\mfc100d.dll"; DestDir: "{app}"
Source: "Debug Version\mfc100ud.dll"; DestDir: "{app}"
Source: "Debug Version\mfcm100d.dll"; DestDir: "{app}"
Source: "Debug Version\mfcm100ud.dll"; DestDir: "{app}"
Source: "Debug Version\msvcp100d.dll"; DestDir: "{app}"
Source: "Debug Version\msvcr100d.dll"; DestDir: "{app}"
Source: "Debug Version\peerconnection_client.dll"; DestDir: "{app}"
   
Source: "Debug Version\ImageJK.dll"; DestDir: "{app}"; Flags:ignoreversion regserver
Source: "Debug Version\ImageJK.dll"; DestDir: "{app}"; Flags:regtypelib

Source: "Debug Version\MiniAtxControl.dll"; DestDir: "{app}"; Flags:ignoreversion regserver
Source: "Debug Version\MiniAtxControl.dll"; DestDir: "{app}"; Flags:regtypelib



;Source: "MyProg.chm"; DestDir: "{app}"
;Source: "Readme.txt"; DestDir: "{app}"; Flags: isreadme

[Icons]
;Name: "{group}\MouMou"; Filename: "{app}\MouMou.exe"
Name: "{userdesktop}\MouMou"; Filename: "{app}\MouMou.exe"


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
  Res:= MsgBox('检测到您的系统里已安装此软件,选择"是",将卸载程序.是否继续！', mbInformation, MB_YESNO );
   if  Res <> 7 then 
   begin 
   Exec(RemoveQuotes(uicmd), '', '', SW_SHOW, ewWaitUntilTerminated, ResultCode); 
   end;  
   end;   
   Result:= KeynotExist
 end;
