devenv /rebuild release "ImClient_seller_online.sln"
xcopy .\binRelease .\安装文件制作工具及示例\ReleaseVersionSeller /s /e /y
"C:\Program Files (x86)\Inno Setup 5\Compil32.exe" .\安装文件制作工具及示例\release_seller_online.iss