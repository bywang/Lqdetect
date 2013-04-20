<html>
<body>
<pre>
<h1>Build Log</h1>
<h3>
--------------------Configuration: LQDetect - Win32 (WCE ARMV4I) Release--------------------
</h3>
<h3>Command Lines</h3>
Creating command line "rc.exe /l 0x409 /fo"ARMV4IRel/LQDetect.res" /d UNDER_CE=500 /d _WIN32_WCE=500 /d "UNICODE" /d "_UNICODE" /d "NDEBUG" /d "WCE_PLATFORM_mini2440" /d "THUMB" /d "_THUMB_" /d "ARM" /d "_ARM_" /d "ARMV4I" /r "Z:\source\lcd\LQDetect\LQDetect.rc"" 
Creating temporary file "C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSPD7.tmp" with contents
[
/nologo /W3 /D "ARM" /D "_ARM_" /D "ARMV4I" /D UNDER_CE=500 /D _WIN32_WCE=500 /D "WCE_PLATFORM_mini2440" /D "UNICODE" /D "_UNICODE" /D "NDEBUG" /FR"ARMV4IRel/" /Fp"ARMV4IRel/LQDetect.pch" /Yu"stdafx.h" /Fo"ARMV4IRel/" /QRarch4T /QRinterwork-return /O2 /MC /c 
"Z:\source\lcd\LQDetect\CppSQLite3U.cpp"
"Z:\source\lcd\LQDetect\DialogContainer.cpp"
"Z:\source\lcd\LQDetect\DialogDemarcate.cpp"
"Z:\source\lcd\LQDetect\DialogDetect.cpp"
"Z:\source\lcd\LQDetect\DialogLiquid.cpp"
"Z:\source\lcd\LQDetect\DialogOK.cpp"
"Z:\source\lcd\LQDetect\DialogOKCANCEL.cpp"
"Z:\source\lcd\LQDetect\DialogOtherParameter.cpp"
"Z:\source\lcd\LQDetect\DialogParameter.cpp"
"Z:\source\lcd\LQDetect\DialogPassword.cpp"
"Z:\source\lcd\LQDetect\DialogSetDisplay.cpp"
"Z:\source\lcd\LQDetect\DialogSplash.cpp"
"Z:\source\lcd\LQDetect\DialogTest.cpp"
"Z:\source\lcd\LQDetect\LQDetect.cpp"
"Z:\source\lcd\LQDetect\LQDetectDlg.cpp"
"Z:\source\lcd\LQDetect\PSerialPort.cpp"
"Z:\source\lcd\LQDetect\PSerialPortX.cpp"
]
Creating command line "clarm.exe @C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSPD7.tmp" 
Creating temporary file "C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSPD8.tmp" with contents
[
/nologo /W3 /D "ARM" /D "_ARM_" /D "ARMV4I" /D UNDER_CE=500 /D _WIN32_WCE=500 /D "WCE_PLATFORM_mini2440" /D "UNICODE" /D "_UNICODE" /D "NDEBUG" /FR"ARMV4IRel/" /Fp"ARMV4IRel/LQDetect.pch" /Yc"stdafx.h" /Fo"ARMV4IRel/" /QRarch4T /QRinterwork-return /O2 /MC /c 
"Z:\source\lcd\LQDetect\StdAfx.cpp"
]
Creating command line "clarm.exe @C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSPD8.tmp" 
Creating temporary file "C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSPD9.tmp" with contents
[
/nologo /base:"0x00010000" /stack:0x10000,0x1000 /entry:"wWinMainCRTStartup" /incremental:no /pdb:"ARMV4IRel/LQDetect.pdb" /out:"ARMV4IRel/LQDetect.exe" /subsystem:windowsce,5.00 /MACHINE:THUMB 
.\ARMV4IRel\CppSQLite3U.obj
.\ARMV4IRel\DialogContainer.obj
.\ARMV4IRel\DialogDemarcate.obj
.\ARMV4IRel\DialogDetect.obj
.\ARMV4IRel\DialogLiquid.obj
.\ARMV4IRel\DialogOK.obj
.\ARMV4IRel\DialogOKCANCEL.obj
.\ARMV4IRel\DialogOtherParameter.obj
.\ARMV4IRel\DialogParameter.obj
.\ARMV4IRel\DialogPassword.obj
.\ARMV4IRel\DialogSetDisplay.obj
.\ARMV4IRel\DialogSplash.obj
.\ARMV4IRel\DialogTest.obj
.\ARMV4IRel\LQDetect.obj
.\ARMV4IRel\LQDetectDlg.obj
.\ARMV4IRel\PSerialPort.obj
.\ARMV4IRel\PSerialPortX.obj
.\ARMV4IRel\StdAfx.obj
.\ARMV4IRel\LQDetect.res
.\SQLite3.lib
]
Creating command line "link.exe @C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSPD9.tmp"
<h3>Output Window</h3>
Compiling resources...
Compiling...
StdAfx.cpp
Compiling...
CppSQLite3U.cpp
Unicode Selected
Z:\source\lcd\LQDetect\CppSQLite3U.cpp(372) : warning C4530: C++ exception handler used, but unwind semantics are not enabled. Specify -GX
DialogContainer.cpp
Unicode Selected
Z:\source\lcd\LQDetect\DialogContainer.cpp(202) : warning C4530: C++ exception handler used, but unwind semantics are not enabled. Specify -GX
DialogDemarcate.cpp
Unicode Selected
Z:\source\lcd\LQDetect\DialogDemarcate.cpp(358) : warning C4530: C++ exception handler used, but unwind semantics are not enabled. Specify -GX
DialogDetect.cpp
Unicode Selected
Z:\source\lcd\LQDetect\DialogDetect.cpp(419) : warning C4530: C++ exception handler used, but unwind semantics are not enabled. Specify -GX
DialogLiquid.cpp
Unicode Selected
Z:\source\lcd\LQDetect\DialogLiquid.cpp(252) : warning C4530: C++ exception handler used, but unwind semantics are not enabled. Specify -GX
DialogOK.cpp
DialogOKCANCEL.cpp
Unicode Selected
DialogOtherParameter.cpp
Unicode Selected
Unicode Selected
Z:\source\lcd\LQDetect\DialogOtherParameter.cpp(146) : warning C4530: C++ exception handler used, but unwind semantics are not enabled. Specify -GX
DialogParameter.cpp
Unicode Selected
Z:\source\lcd\LQDetect\DialogParameter.cpp(629) : warning C4530: C++ exception handler used, but unwind semantics are not enabled. Specify -GX
Z:\source\lcd\LQDetect\DialogParameter.cpp(827) : warning C4018: '>=' : signed/unsigned mismatch
Z:\source\lcd\LQDetect\DialogParameter.cpp(827) : warning C4018: '<' : signed/unsigned mismatch
DialogPassword.cpp
DialogSetDisplay.cpp
Unicode Selected
Unicode Selected
Z:\source\lcd\LQDetect\DialogSetDisplay.cpp(184) : warning C4530: C++ exception handler used, but unwind semantics are not enabled. Specify -GX
DialogSplash.cpp
DialogTest.cpp
Unicode Selected
LQDetect.cpp
Unicode Selected
Unicode Selected
Z:\source\lcd\LQDetect\LQDetect.cpp(160) : warning C4530: C++ exception handler used, but unwind semantics are not enabled. Specify -GX
LQDetectDlg.cpp
PSerialPort.cpp
Unicode Selected
PSerialPortX.cpp
Unicode Selected
Generating Code...
Unicode Selected
Linking...
   Creating library ARMV4IRel/LQDetect.lib and object ARMV4IRel/LQDetect.exp
LINK : warning LNK4089: all references to 'WININET.dll' discarded by /OPT:REF
Creating temporary file "C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSPDF.tmp" with contents
[
/nologo /o"ARMV4IRel/LQDetect.bsc" 
.\ARMV4IRel\StdAfx.sbr
.\ARMV4IRel\CppSQLite3U.sbr
.\ARMV4IRel\DialogContainer.sbr
.\ARMV4IRel\DialogDemarcate.sbr
.\ARMV4IRel\DialogDetect.sbr
.\ARMV4IRel\DialogLiquid.sbr
.\ARMV4IRel\DialogOK.sbr
.\ARMV4IRel\DialogOKCANCEL.sbr
.\ARMV4IRel\DialogOtherParameter.sbr
.\ARMV4IRel\DialogParameter.sbr
.\ARMV4IRel\DialogPassword.sbr
.\ARMV4IRel\DialogSetDisplay.sbr
.\ARMV4IRel\DialogSplash.sbr
.\ARMV4IRel\DialogTest.sbr
.\ARMV4IRel\LQDetect.sbr
.\ARMV4IRel\LQDetectDlg.sbr
.\ARMV4IRel\PSerialPort.sbr
.\ARMV4IRel\PSerialPortX.sbr]
Creating command line "bscmake.exe @C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSPDF.tmp"
Creating browse info file...
<h3>Output Window</h3>




<h3>Results</h3>
LQDetect.exe - 0 error(s), 12 warning(s)
</pre>
</body>
</html>
