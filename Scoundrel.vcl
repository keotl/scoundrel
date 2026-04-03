<html>
<body>
<pre>
<h1>Build Log</h1>
<h3>
--------------------Configuration: Scoundrel - Win32 (WCE MIPS) Debug--------------------
</h3>
<h3>Command Lines</h3>
Creating command line "rc.exe /l 0x409 /fo"MIPSDbg/Scoundrel.res" /d UNDER_CE=300 /d _WIN32_WCE=300 /d "UNICODE" /d "_UNICODE" /d "DEBUG" /d "WIN32_PLATFORM_PSPC" /d "MIPS" /d "_MIPS_" /d "_AFXDLL" /r "C:\Documents and Settings\All Users\Documents\Documents\Scoundrel\Scoundrel.rc"" 
Creating temporary file "C:\DOCUME~1\Atreides\LOCALS~1\Temp\RSP232.tmp" with contents
[
/nologo /W3 /Zi /Od /D "DEBUG" /D "MIPS" /D "_MIPS_" /D UNDER_CE=300 /D _WIN32_WCE=300 /D "WIN32_PLATFORM_PSPC" /D "UNICODE" /D "_UNICODE" /D "_AFXDLL" /FR"MIPSDbg/" /Fp"MIPSDbg/Scoundrel.pch" /Yu"stdafx.h" /Fo"MIPSDbg/" /Fd"MIPSDbg/" /MC /c 
"C:\Documents and Settings\All Users\Documents\Documents\Scoundrel\Card.cpp"
"C:\Documents and Settings\All Users\Documents\Documents\Scoundrel\DrawUtils.cpp"
"C:\Documents and Settings\All Users\Documents\Documents\Scoundrel\Scoundrel.cpp"
"C:\Documents and Settings\All Users\Documents\Documents\Scoundrel\ScoundrelDlg.cpp"
]
Creating command line "clmips.exe @C:\DOCUME~1\Atreides\LOCALS~1\Temp\RSP232.tmp" 
Creating temporary file "C:\DOCUME~1\Atreides\LOCALS~1\Temp\RSP233.tmp" with contents
[
/nologo /W3 /Zi /Od /D "DEBUG" /D "MIPS" /D "_MIPS_" /D UNDER_CE=300 /D _WIN32_WCE=300 /D "WIN32_PLATFORM_PSPC" /D "UNICODE" /D "_UNICODE" /D "_AFXDLL" /FR"MIPSDbg/" /Fp"MIPSDbg/Scoundrel.pch" /Yc"stdafx.h" /Fo"MIPSDbg/" /Fd"MIPSDbg/" /MC /c 
"C:\Documents and Settings\All Users\Documents\Documents\Scoundrel\StdAfx.cpp"
]
Creating command line "clmips.exe @C:\DOCUME~1\Atreides\LOCALS~1\Temp\RSP233.tmp" 
Creating temporary file "C:\DOCUME~1\Atreides\LOCALS~1\Temp\RSP234.tmp" with contents
[
/nologo /base:"0x00010000" /stack:0x10000,0x1000 /entry:"wWinMainCRTStartup" /incremental:yes /pdb:"MIPSDbg/Scoundrel.pdb" /debug /out:"MIPSDbg/Scoundrel.exe" /subsystem:windowsce,3.00 /MACHINE:MIPS 
".\MIPSDbg\Card.obj"
".\MIPSDbg\DrawUtils.obj"
".\MIPSDbg\Scoundrel.obj"
".\MIPSDbg\ScoundrelDlg.obj"
".\MIPSDbg\StdAfx.obj"
".\MIPSDbg\Scoundrel.res"
]
Creating command line "link.exe @C:\DOCUME~1\Atreides\LOCALS~1\Temp\RSP234.tmp"
<h3>Output Window</h3>
Compiling resources...
Compiling...
StdAfx.cpp
Compiling...
Card.cpp
DrawUtils.cpp
Scoundrel.cpp
ScoundrelDlg.cpp
Generating Code...
Linking...
Creating command line "bscmake.exe /nologo /o"MIPSDbg/Scoundrel.bsc"  ".\MIPSDbg\StdAfx.sbr" ".\MIPSDbg\Card.sbr" ".\MIPSDbg\DrawUtils.sbr" ".\MIPSDbg\Scoundrel.sbr" ".\MIPSDbg\ScoundrelDlg.sbr""
Creating browse info file...
<h3>Output Window</h3>




<h3>Results</h3>
Scoundrel.exe - 0 error(s), 0 warning(s)
</pre>
</body>
</html>
