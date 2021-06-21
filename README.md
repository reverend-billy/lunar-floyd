# lunar-core

The Infineon XMC microcontrollers inside Lunar Energy's products are based on Arm's Cortex-M series of 32-bit embedded processors. Since we chose Cortex-M for our embedded processors, selecting Arm's Keil tools made the most sense for our embedded software development.

The smart inverter has an XMC4800 micro inside. Each battery block has an XMC4400 micro inside. Each photovoltaic optimizer each has an XMC1400 micro inside. Both the XMC4800 and XMC4400 are based on the Cortex-M4 processor core. The XMC1400 is based on the Cortex-M0 processor core.

## Prerequisites

If you can view this README then you already have access to this private Git repo on [Lunar Energy's BitBucket](https://bitbucket.org/lunarenergy/).

### Hardware

Developing firmware for Lunar Energy requires the following hardware.

- machine capable of running Windows 10
- USB port
- micro USB cable
- [Infineon XMC4400 Platform2Go](https://www.infineon.com/cms/en/product/evaluation-boards/kit_xmc_plt2go_xmc4400/)
- [Infineon XMC4800 IoT Amazon FreeRTOS Connectivity kit WIFI](https://www.digikey.com/en/products/detail/infineon-technologies/KITXMC48IOTAWSWIFITOBO1/9816150)
- [Infineon XMC1400 Boot Kit](https://www.infineon.com/cms/en/product/evaluation-boards/kit_xmc14_boot_001/)

Each of the XMC dev boards comes with a micro USB port for powering, flashing and debugging so a free USB port is needed on the Windows end to connect the two.

### Software

Developing firmware for Lunar Energy requires the following software.

- VirtualBox on Linux or Parallels on macOS
- Windows 10
- Git
- [J-Link drivers](https://www.segger.com/downloads/jlink/#J-LinkSoftwareAndDocumentationPack)
- [Keil MDK](https://www2.keil.com/mdk5/)

Windows is needed to run the Keil MDK required for microcontroller software development. [Git for Windows](https://gitforwindows.org/) can be installed using [Chocolatey](https://community.chocolatey.org/packages/git) or by downloading and running the latest installer from gitforwindows.org. Windows 10 comes with an SSH client built-in so there is no need to install one for authenticating to BitBucket.

## Getting started

Here are the steps to provision a Windows environment for firmware development.

1. Download and install SEGGER's J-Link Software and Documentation Pack for Windows from https://www.segger.com/downloads/jlink/.

2. Download and install the Keil MDK from https://www2.keil.com/mdk5/. The MDK-Lite edition is sufficient if you don't have an MDK-Essential license yet since the 32 KB code size restriction is not an issue.

3. Clone this repo from a PowerShell terminal:

```PowerShell
PS C:\Users\Frank> git clone git@bitbucket.org:lunarenergy/lunar-core.git
Cloning into 'lunar-core'...
Enter passphrase for key '/c/Users/Frank/.ssh/id_rsa':
remote: Counting objects: 734, done.
remote: Compressing objects: 100% (485/485), done.
Receiving objects: 100% (734/734)used 519 (delta 224) receiving objects:  92% (676/734)
Receiving objects: 100% (734/734), 2.33 MiB | 4.73 MiB/s, done.
Resolving deltas: 100% (328/328), done.
```

Access to this Git repo is restricted to select Lunar Energy employees, so you must add your public SSH key (e.g. `~/.ssh/id_rsa.pub`) to your personal settings at https://bitbucket.org/lunarenergy/ before you can clone it.

## Directory structure

The C source and header files are in the `Src` directory. Code for each XMC dev board is located in its respective subdirectory under `Src/Boards`. Code shared across all XMC micros is located in the top-level `Src` directory and the `Infineon/XMC` directory under `Src/Devices`. Think of this common code as a HAL that wraps the underlying board-specific implementations.

```
lunar-core
│   README.md
│
├───Src
│   │   GPIO_Drv.h
│   │   ...
│   │   main.c
│   │   main.h
│   │   ...
│   │   UART_Drv.h
│   │
│   ├───Boards
│   │   ├───XMC1400_Boot_Kit
│   │   │       Control.c
│   │   │       ...
│   │   │       UART_Drv_ConfigTable.h
│   │   │
│   │   ├───XMC4400_Platform2Go
│   │   │       Control.c
│   │   │       ...
│   │   │       UART_Drv_ConfigTable.h
│   │   │
│   │   └───XMC4800_AWS
│   │           Control.c
│   │           ...
│   │           UART_Drv_ConfigTable.h
│   │
│   └───Devices
│       │
│       ├───Infineon
│       │   └───XMC
│       │       │   DevicePin_Config.h
│       │       │   GPIO_Drv.c
│       │       │   Reset_Drv.c
│       │       │   RTC_Drv.c
│       │       │   SysTick_Drv.c
│       │       │   Timebase_Drv.c
│       │       │   UART_Drv.c
│       │       │
│       │       ├───XMC1400_series
│       │       │
│       │       ├───XMC4400_series
│       │       │
│       │       ├───XMC4800_series
│       │       │
│       │       └───XMClib
│       │
│       └───Newlib
│               syscalls.c
│
└───Toolchain
    └───Keil
        ├───XMC1400_Boot
        │   │   XMC1400_Boot_Kit.uvoptx
        │   │   XMC1400_Boot_Kit.uvprojx
        │   │
        │   ├───Debug
        │   └───Release
        │
        ├───XMC4400_Platform2Go
        │   │   XMC4400_Platform2Go.uvoptx
        │   │   XMC4400_Platform2Go.uvprojx
        │   │
        │   ├───Debug
        │   └───Release
        │
        └───XMC4800_AWS
            │   XMC4800_AWS.uvoptx
            │   XMC4800_AWS.uvprojx
            │
            ├───Debug
            └───Release
```

Keil uVision project and options files (`.uvprojx` and `.uvoptx`) for each XMC dev board are located under `Toolchain`. uVision is the name of the IDE part of the Keil MDK.

Detailed documentation covering the contents of each firmware release can be found [here](https://drive.google.com/drive/folders/1_bH25hwK9uLD3OuWYe7Ob0dE_AA1WrSv) on Google Drive.

## Building

Keil uVision is a graphical IDE so a developer typically builds a project manually by clicking with their mouse.

To build code targeting the XMC4400 Platform2Go dev board for the first time:

1. Launch Keil uVision5
2. Select **Open Project...** from the **Project** menu
3. Locate the `XMC4400_Platform2Go` project file under this repo's `Toolchain` directory
4. Install the Infineon XMC4000 Device Family Pack when prompted
5. Click on the **Build** button on the toolbar to build the target files

Steps 2 through 4 can be skipped the next time around. Many menu options like **Build Target** have keyboard shortcuts associated with them to speed up the build and debug cycle.

The process for building the `XMC4800_AWS` and `XMC1400_Boot` project files is equivalent.

## Flashing

To flash a dev board after building the project for that target:

1. Connect a micro USB cable to your Windows machine
2. Plug the micro USB end of the cable into the dev board
3. Click on the **Download** button on the toolbar to flash the dev board
4. Hit the reset button on the dev board to begin executing the newly flashed code

**Note**: Continue reading if flashing fails with **Cannot load device description!**

The `.flm` files for the device packs install to your user directory not `C:/Keil_v5/ARM/Flash`. The subdirectory containing the missing XMC4000 series `.flm` files is `AppData\Local\Arm\Packs\Infineon\XMC4000_DFP\2.13.0\Flash`. Copy the contents of that directory into `C:/Keil_v5/ARM/Flash` and flashing an XMC4400 Platform2Go should succeed.

## Debugging

Ed Hatfield has created a SerialCom tool for communicating with devices over UART. The tool is a .NET desktop application so it requires Windows to run.

A Zip archive of the SerialCom application and DLLs can be found [here](https://drive.google.com/drive/folders/1hGxYy2s3axgjWrBKq_vd7IuC12rK6jHC) on Google Drive. Download and extract the Zip archive to your Windows machine.

The Personality XML file can be found [here](https://drive.google.com/drive/folders/1Wa868_I3dZEuS4hb66f0QnIQh5uuFFH0) on Google Drive. Download the Personality XML file to your Windows machine.

To use the SerialCom tool:

1. Launch the SerialCom application
2. Click on the **Open Personality** (folder) button
3. Locate the Personality XML file for the latest firmware release
4. Click on the **Port Settings** (gears) button
5. Set the COM port and baud rate to 115200 for the built-in debugger
6. Click the **Build and Send** button or double-click commands on the left tree view

The tool saves your settings when exiting for next time.
