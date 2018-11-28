cd..
cd..

xcopy /s /y /d .\DoomServer\Include\*.h  .\ServerEngine\Include\
xcopy /s /y /d .\DoomServer\Include\*.inl  .\ServerEngine\Include\

xcopy /s /y /d /exclude:Out.txt .\DoomServer\Bin_Debug\*.* .\ServerEngine\Bin_Debug\
xcopy /s /y /d /exclude:out.txt .\DoomServer\Bin\*.* .\ServerEngine\Bin\
xcopy /s /y /d /exclude:out.txt .\DoomServer\Bin64_Debug\*.* .\ServerEngine\Bin64_Debug\
xcopy /s /y /d /exclude:out.txt .\DoomServer\Bin64\*.* .\ServerEngine\Bin64\


xcopy /s /y /d .\ServerEngine\Bin_Debug\*.* .\TestServer\Bin_Debug\
xcopy /s /y /d .\ServerEngine\Bin\*.* .\TestServer\Bin\
xcopy /s /y /d .\ServerEngine\Bin64\*.* .\TestServer\Bin64\
xcopy /s /y /d .\ServerEngine\Bin64_Debug\*.* .\TestServer\Bin64_Debug\
