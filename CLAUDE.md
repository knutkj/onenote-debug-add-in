# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with
code in this repository.

## Project Overview

This is a C++ OneNote Debug Add-In project that demonstrates COM DLL development
for Windows. The project builds a DLL that can be registered with Windows as a
COM server for OneNote integration.

## Files and Build Commands

- **Adding files**: The human is adding C++ (`.cpp`) files through Visual Studio
  for now. Claude is editing these files.
- **Build the project:** The human is building the project for now using Visual
  Studio.
- **Register/unregister the DLL:** Convenient through `regsvr32.exe`.

## Architecture

### Core Components

- **DllMain.cpp**: DLL entry point handling Windows DLL lifecycle events.
- **DllReg.cpp**: COM registration implementation with `DllRegisterServer`
  function for registry integration
- **DebugAddIn.def**: Module definition file controlling DLL exports.

### Build Configuration

- **Target**: Dynamic Library (DLL) for x64 Debug configuration.
- **Output**: `out/x64/Debug/DebugAddIn.dll`.
- **Intermediate**: `tmp/x64/Debug/` for build artifacts.
- **Platform**: Windows, Visual Studio 2022.

### Key Technical Details

- Uses C++ with Windows API and COM
- Educational project designed to teach COM development, Windows DLL mechanics,
  and OneNote add-in architecture

### Documentation

Each source file has corresponding `.md` documentation explaining implementation
details, Windows concepts, and C++ fundamentals. The project serves as a
learning resource for developers new to Windows C++ development.

## WSL Environment Notes

Use Windows executables with `.exe` suffix when running build tools and Git
commands in this WSL environment.
