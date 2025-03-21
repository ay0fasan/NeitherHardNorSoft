# NeitherHardNorSoft

## Overview

NHNS.


## Cloning the Repository

To properly clone the repository with all required submodules, follow these steps:

Open a terminal and run:

```bash
git clone --recurse-submodules <repository-url>
```
This ensures all submodules, including the HAL driver, are fetched during the initial clone.

If you’ve already cloned the repository without submodules, initialize and update the submodules with:

```bash
git submodule update --init --recursive
```


## Environment Setup

### 1. Install Git

1. Download Git from [git-scm.com](https://git-scm.com/downloads).
2. Follow the installation instructions for your operating system.

### 2. Install ARM GCC Compiler

1. Download the ARM GCC compiler from the [ARM Developer website](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm).
2. Extract the archive to a directory of your choice.
3. Add the bin directory of the extracted files to your system's PATH.

   - **Windows**:
     1. Open the Start Menu, search for `Environment Variables`, and select "Edit the system environment variables".
     2. Click `Environment Variables` in the System Properties window.
     3. Under `System variables`, select `Path`, and click `Edit`.
     4. Click `New`, and add the path to your ARM GCC bin directory (e.g., `C:\gcc-arm-none-eabi\bin`).
     5. Click `OK` to close all windows.

   - **Linux/Mac**:
     1. Open a terminal and run:
        ```bash
        echo 'export PATH=<path-to-gcc-arm>/bin:$PATH' >> ~/.bashrc
        source ~/.bashrc
        ```
     2. Replace `<path-to-gcc-arm>` with the actual path to your ARM GCC bin directory.

### 3. Install MinGW (Windows Only)

1. Download MinGW from the [MinGW SourceForge page](https://sourceforge.net/projects/mingw/).
2. Run the installer and follow the instructions to install `mingw32-base` and `mingw32-gcc-g++`.
3. Add the MinGW bin directory to your system's PATH (similar to the ARM GCC Compiler setup).

### 4. Install Python

1. Download Python from [python.org](https://www.python.org/downloads/).
2. Run the installer, ensuring you check "Add Python to PATH" before proceeding.
3. Verify the installation by opening a terminal and running:
   ```bash
   python --version
   ```

### 5. Set Up Environment Variables

Ensure the following environment variables are set:

- **PATH**: Should include the paths to your Git, ARM GCC Compiler, and MinGW (if on Windows).

### 6. Install Clang-Format

1. Download Clang-Format [Clang-Format](https://github.com/llvm/llvm-project/releases/download/llvmorg-18.1.8/LLVM-18.1.8-win64.exe).
2. Follow the installation steps and add the bin directory to your system's `PATH`.
3. Verify the installation by running:
   ```bash
   clang-format --version
   ```


## Building

To build the firmware, follow these steps:

1. Open a terminal or command prompt.
2. Navigate to the `NHNS` root directory.
3. Run the `make` command to start the build process:

   ```bash
   cd NHNS
   make
   ```


## Programming

### Using an ST-Link Programmer

1. Download [STM32CubeProg](https://www.st.com/en/development-tools/stm32cubeprog.html) and follow the installation steps.
2. Connect your ST-Link programmer to the target device.
3. Run the following command to program the device:
    ```bash
    make flash
    ```


## Clang Format

To ensure consistent code formatting, use Clang-Format. Download Clang-Format from [LLVM GitHub Releases](https://github.com/llvm/llvm-project/releases/tag/llvmorg-18.1.8).

After installing Clang-Format, you can format your source files with the following command:

```bash
clang-format -i <NAME OF FILE>
```

Replace <NAME OF FILE> with the path to the file you wish to format.

OR

````bash
./format.sh <directory>
```


## Additional Notes

- Make sure your build environment is properly configured before building the project.
- Keep your tools and dependencies up to date to avoid compatibility issues.
