# Development Setup

## Prerequisites

- A Windows XP machine with **Microsoft eMbedded Visual C++ 3.0** installed (used for building)
- A Linux machine with **clangd** (v18+) and **Emacs** with eglot (used for editing)
- SMB share between the two machines

## Repository layout

```
├── Microsoft eMbedded Tools/   # eMbedded Visual C++ IDE files (from install)
├── wce-sdk/                    # Windows CE SDK headers and libs (see below)
│   └── wce300/
│       ├── MS Pocket PC/       # Pocket PC SDK
│       ├── Pocket PC 2002/
│       └── Smartphone 2002/
└── Scoundrel/                  # Project source code
```

## Setting up the Windows CE SDK headers

The `wce-sdk/` directory must contain the contents of `C:\Windows CE Tools` (or `C:\Program Files\Windows CE Tools`) from the Windows XP build machine. These are installed by the Pocket PC SDKs alongside eMbedded Visual C++.

Copy the entire directory:

```
C:\Windows CE Tools\  →  wce-sdk/
```

The critical paths for clangd are:

- `wce-sdk/wce300/MS Pocket PC/include` — Windows CE platform headers (windows.h, wingdi.h, etc.)
- `wce-sdk/wce300/MS Pocket PC/mfc/include` — MFC headers (afxwin.h, afxext.h, etc.)
- `wce-sdk/wce300/MS Pocket PC/atl/include` — ATL headers

## clangd setup

The `Scoundrel/compile_flags.txt` configures clangd with the correct include paths and preprocessor defines. No additional setup is needed — clangd picks it up automatically.

**Case sensitivity note:** The source files use lowercase `#include "stdafx.h"` but the original filenames are `StdAfx.h`/`StdAfx.cpp`. On Linux, rename them to match the include directives:

```bash
cd Scoundrel
mv StdAfx.h stdafx.h
mv StdAfx.cpp stdafx.cpp
```

## Emacs

`.dir-locals.el` and `.clang-format` in `Scoundrel/` configure:

- CRLF line endings (required for the Windows CE compiler)
- Tab indentation (4-wide)
- Microsoft brace style
- MFC macro preservation in clang-format

## Syncing with the build machine

The Windows XP build machine is accessed via SMB. From the `Scoundrel/` directory:

```bash
./push.sh   # sync local → build machine
./pull.sh   # sync build machine → local
```

These scripts exclude workspace-only files (.git, .clang-format, compile_flags.txt, etc.) from the sync.

## Building

Open `Scoundrel.vcw` in eMbedded Visual C++ on the Windows XP machine and build from there. The project targets WCE 3.0 (MIPS, ARM, x86).
