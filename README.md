# Thunder JS Runtime

Thunder JS Runtime is a lightweight, high-performance JavaScript execution environment built with C++ and QuickJS-NG. It provides a simple C++ wrapper around the amalgamated QuickJS engine, tailored for Windows systems (compiled with MinGW gcc/g++).

## Features
- **Fast Startup**: Sub-millisecond startup times powered by QuickJS-NG.
- **Embedded Console API**: Fully working `console.log`, `console.error`, and `console.warn` implementations out of the box.
- **Microtask Queue**: Full support for Promise resolution and async execution via pending job loops.
- **Lightweight**: Minimal runtime size and memory overhead.

## Requirements
- Windows OS
- MinGW (GCC/G++ compiler) installed and added to your `PATH`

## Building
Run the provided batch script to compile the QuickJS engine and link the C++ wrapper:
```cmd
build.bat
```
This produces `thunder-js.exe` in the root folder.

## Running JavaScript Files
To execute a JavaScript file, run:
```cmd
thunder-js.exe <filename.js>
```
Or stream input via stdin:
```cmd
echo console.log("Hello, Thunder!") | thunder-js.exe -
```

## Running Tests
A suite of 5 verification tests is provided. Run the following command to execute them:
```cmd
test.bat
```
All test cases should pass successfully (100/100 score).
