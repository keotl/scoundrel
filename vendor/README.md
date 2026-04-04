# Vendor Dependencies

This directory contains Windows CE SDK and eMbedded Visual C++ include files
needed to build the Scoundrel project. These files are not checked into git.

## Directory Structure

```
vendor/
  wce-sdk/
    include/       - Windows CE 3.0 Pocket PC SDK headers
    mfc/include/   - MFC for Windows CE headers (includes locale subdirs)
    atl/include/   - ATL for Windows CE headers
  evc/
    Include/           - (currently empty, top-level EVC dir)
    Include/ObjModel/  - eMbedded Visual C++ OLE Automation headers
    Include/Win32/     - eMbedded Visual C++ Win32 resource headers
```

## How to Populate

Copy the files from a Windows CE 3.0 SDK and Microsoft eMbedded Visual Tools
installation:

1. **wce-sdk/include/** - Copy all files from:
   `Windows CE Tools/wce300/MS Pocket PC/include/`

2. **wce-sdk/mfc/include/** - Copy (recursively) all files from:
   `Windows CE Tools/wce300/MS Pocket PC/mfc/include/`

3. **wce-sdk/atl/include/** - Copy all files from:
   `Windows CE Tools/wce300/MS Pocket PC/atl/include/`

4. **evc/Include/ObjModel/** - Copy all `.H` files from:
   `Microsoft eMbedded Tools/EVC/Include/ObjModel/`

5. **evc/Include/Win32/** - Copy all `.H` and `.RH` files from:
   `Microsoft eMbedded Tools/EVC/Include/Win32/`
