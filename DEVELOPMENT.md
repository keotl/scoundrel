# Development Setup
Here is my workflow, feel free to take some inspiration.

## Software dependencies

Make sure to install:
1. Microsoft ActiveSync (Optional, for connecting to a pocket PC).
2. Microsoft eMbedded Visual Tools 3.0 (MSDN disc X11-49895)
3. Microsoft Pocket PC SDK (MSDN disc X11-26157)

## Include files in vendor/
Copy WinCE and Microsoft eMbedded Tools to vendor/ to get clangd code
completions and diagnostics.

## Sync scripts
See example scripts for which files to include/exclude.
```bash
./push.sh
./pull.sh
```

## Editor config
Make sure to set-up
1. CRLF line endings.
2. Microsoft brace style & MFC macro preservation in clang-format.

## Building

Open `Scoundrel.vcw` in eMbedded Visual C++ on the Windows XP machine and build from there. The project targets WCE 3.0 (MIPS, ARM, x86).


## Debugging
If eMbedded Visual C++ stalls on build when trying to copy to the
device, try having Remote Spy open first.
