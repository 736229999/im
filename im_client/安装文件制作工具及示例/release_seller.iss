; -- Example1.iss --
; Demonstrates copying 3 files and creating an icon.

; SEE THE DOCUMENTATION FOR DETAILS ON CREATING .ISS SCRIPT FILES!

[Setup]
AppName=哞哞商家版
AppVersion=1.0.0.4
AppPublisher=飞牛网
DefaultDirName={pf}\MouMou_Bussiness
DefaultGroupName=MouMou_Bussiness
UninstallDisplayName= 哞哞商家版 1.0.0.4
UninstallDisplayIcon={app}\MouMou.exe
Compression=lzma2
SolidCompression=yes
SetupIconFile="ReleaseVersionSeller\MouMou.ico"
OutputDir="Release"
OutputBaseFilename=MouMou_Business_online_1.0.0.4

[dirs]
Name:"{app}\Emotion"
Name:"{app}\Voice"

[Languages]
Name: "cs"; MessagesFile: "compiler:Languages\ChineseSimp.isl"


;以下为所需文件所在的路径
[Files]
Source: "ReleaseVersionSeller\MouMou.exe"; DestDir: "{app}"
Source: "ReleaseVersionSeller\fcl.dll"; DestDir: "{app}"
Source: "ReleaseVersionSeller\GipsVideoEngine.dll"; DestDir: "{app}"
Source: "ReleaseVersionSeller\GipsVoiceEngine.dll"; DestDir: "{app}"
Source: "ReleaseVersionSeller\HeadPhoto.dll"; DestDir: "{app}"
Source: "ReleaseVersionSeller\HmgCrtLib.dll"; DestDir: "{app}"
Source: "ReleaseVersionSeller\HmgSkin.dll"; DestDir: "{app}"
Source: "ReleaseVersionSeller\HmgSkinRes.dll"; DestDir: "{app}"
Source: "ReleaseVersionSeller\ImageJK.dll"; DestDir: "{app}"
Source: "ReleaseVersionSeller\Mini139_res.dll"; DestDir: "{app}"
Source: "ReleaseVersionSeller\minidata_dll.dll"; DestDir: "{app}"
Source: "ReleaseVersionSeller\MiniNetTrans.dll"; DestDir: "{app}"
Source: "ReleaseVersionSeller\MiniAtxControl.dll"; DestDir: "{app}"
Source: "ReleaseVersionSeller\mfc100.dll"; DestDir: "{app}"
Source: "ReleaseVersionSeller\msvcr100.dll"; DestDir: "{app}"
Source: "ReleaseVersionSeller\msvcp100.dll"; DestDir: "{app}"
Source: "ReleaseVersionSeller\MyArticles.dll"; DestDir: "{app}"Source: "ReleaseVersionSeller\MyPictures.dll"; DestDir: "{app}"
Source: "ReleaseVersionSeller\PictureLoadX.dll"; DestDir: "{app}"
Source: "ReleaseVersionSeller\UserInfo.dll"; DestDir: "{app}"
Source: "ReleaseVersionSeller\videocodec.dll"; DestDir: "{app}"
Source: "ReleaseVersionSeller\WebInterface.dll"; DestDir: "{app}"
Source: "ReleaseVersionSeller\WriteArticle.dll"; DestDir: "{app}"
Source: "ReleaseVersionSeller\address.xml"; DestDir: "{app}" 
Source: "ReleaseVersionSeller\DuiLib.dll"; DestDir: "{app}" 
Source: "ReleaseVersionSeller\cryptopp.dll"; DestDir: "{app}"
Source: "ReleaseVersionSeller\msftedit.dll"; DestDir: "{app}"


;加载系统表情
Source: "ReleaseVersionSeller\Emotion\b01.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionSeller\Emotion\b02.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionSeller\Emotion\b03.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionSeller\Emotion\b04.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionSeller\Emotion\b05.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionSeller\Emotion\b06.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionSeller\Emotion\b07.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionSeller\Emotion\b08.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionSeller\Emotion\b09.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionSeller\Emotion\b10.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionSeller\Emotion\b11.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionSeller\Emotion\b12.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionSeller\Emotion\b13.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionSeller\Emotion\b14.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionSeller\Emotion\b15.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionSeller\Emotion\b16.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionSeller\Emotion\b17.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionSeller\Emotion\b18.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionSeller\Emotion\b19.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionSeller\Emotion\b20.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionSeller\Emotion\b21.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionSeller\Emotion\b22.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionSeller\Emotion\b23.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionSeller\Emotion\b24.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionSeller\Emotion\b25.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionSeller\Emotion\b26.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionSeller\Emotion\b27.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionSeller\Emotion\b28.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionSeller\Emotion\b29.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionSeller\Emotion\b30.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionSeller\Emotion\b31.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionSeller\Emotion\b32.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionSeller\Emotion\b33.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionSeller\Emotion\b34.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionSeller\Emotion\b35.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionSeller\Emotion\b36.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionSeller\Emotion\b37.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionSeller\Emotion\b38.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionSeller\Emotion\b39.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionSeller\Emotion\b40.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionSeller\Emotion\b41.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionSeller\Emotion\b42.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionSeller\Emotion\b43.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionSeller\Emotion\b44.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionSeller\Emotion\b45.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionSeller\Emotion\b46.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionSeller\Emotion\b47.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionSeller\Emotion\b48.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionSeller\Emotion\b49.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionSeller\Emotion\b50.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionSeller\Emotion\b51.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionSeller\Emotion\b52.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionSeller\Emotion\b53.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionSeller\Emotion\b54.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionSeller\Emotion\b55.gif"; DestDir: "{app}\Emotion"
Source: "ReleaseVersionSeller\Emotion\waitImage.gif"; DestDir: "{app}\Emotion"
;声音
Source: "ReleaseVersionSeller\Voice\即时消息声-清脆声.wav"; DestDir: "{app}\Voice"
Source: "ReleaseVersionSeller\Voice\联系人上线-敲门声.wav"; DestDir: "{app}\Voice"
Source: "ReleaseVersionSeller\Voice\闪屏抖动声-闪过音效.wav"; DestDir: "{app}\Voice"
Source: "ReleaseVersionSeller\Voice\视频呼叫声-长哞声.wav"; DestDir: "{app}\Voice"
Source: "ReleaseVersionSeller\Voice\系统消息-牛铃4节奏.wav"; DestDir: "{app}\Voice"
Source: "ReleaseVersionSeller\Voice\语音呼叫声-短哞声.wav"; DestDir: "{app}\Voice"


;注册组件

;Source: "ReleaseVersionSeller\ImageJK.dll"; DestDir: "{app}"; Flags:ignoreversion regserver
;Source: "ReleaseVersionSeller\ImageJK.dll"; DestDir: "{app}"; Flags:regtypelib

Source: "ReleaseVersionSeller\MiniAtxControl.dll"; DestDir: "{app}"; Flags:ignoreversion regserver
Source: "ReleaseVersionSeller\MiniAtxControl.dll"; DestDir: "{app}"; Flags:regtypelib

;字体
Source: "ReleaseVersionSeller\msyh.ttf"; DestDir: "{fonts}"; FontInstall: "微软雅黑 常规"; Flags: onlyifdoesntexist uninsneveruninstall
Source: "ReleaseVersionSeller\msyhbd.ttf"; DestDir: "{fonts}"; FontInstall: "微软雅黑 粗体"; Flags: onlyifdoesntexist uninsneveruninstall


;Source: "MyProg.chm"; DestDir: "{app}"
;Source: "Readme.txt"; DestDir: "{app}"; Flags: isreadme

[Icons]
Name: "{group}\MouMou商家版"; Filename: "{app}\MouMou.exe";WorkingDir: "{app}"
Name: "{group}\{cm:UninstallProgram,MouMou商家版}"; Filename: "{uninstallexe}"
Name: "{userdesktop}\MouMou商家版"; Filename: "{app}\MouMou.exe"


[Code]
function InitializeUninstall(): Boolean;
var
  HasRun:HWND;
  Res: Integer;
  ResultCode: Integer;
begin
    HasRun := FindWindowByWindowName('哞哞商家版2015');
    while HasRun<>0 do
    begin
      Res:= MsgBox('检测到哞哞正在运行，请确认退出哞哞', mbInformation, MB_OK );
      if  Res <> 7 then 
      begin
        Exec('cmd','/c taskkill /im MouMou.exe -f', '', SW_HIDE, ewNoWait, ResultCode);
        sleep(1000); 
        HasRun := FindWindowByWindowName('哞哞商家版2015');
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
    DeleteFile(ExpandConstant('{userdocs}\MouMou_Business\GuidePageConfig.ini'));
    HasRun := FindWindowByWindowName('哞哞商家版2015');
    while HasRun<>0 do
    begin
      Res:= MsgBox('检测到哞哞正在运行，请确认退出哞哞', mbInformation, MB_OK );
      if  Res <> 7 then 
      begin
        Exec('cmd','/c taskkill /im MouMou.exe -f', '', SW_HIDE, ewNoWait, ResultCode);
        sleep(1000); 
        HasRun := FindWindowByWindowName('哞哞商家版2015');
      end
    end
    If RegQueryStringValue(HKEY_LOCAL_MACHINE, 'SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\哞哞商家版_is1', 'UninstallString', uicmd) then    begin
        Exec(RemoveQuotes(uicmd), '/SILENT', '', SW_SHOW, ewWaitUntilTerminated, ResultCode);
        sleep(2000);   
    end;   
   Result:= true;end;

procedure CurPageChanged(CurPageID: Integer);
begin
    if CurPageID = wpWelcome then
    WizardForm.NextButton.OnClick(WizardForm);
end;

[run]
Filename: "{app}\MouMou.exe";Description:"立即体验最新哞哞"; StatusMsg:"别急，等一下就好"; Flags: nowait postinstall skipifsilent




