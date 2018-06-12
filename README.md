# NightLight
NightLight is a Windows system tray application that reduces eye strain produced by staring at a computer screen.
# ![NightLight](https://raw.githubusercontent.com/D-Ermis/D.lux/master/D.lux/logo.png)

## Download 
* [NightLight_v1.1_win.zip](https://github.com/D-Ermis/NightLight/files/2094701/NightLight_v1.0_win.zip)

## Features
* Lightweight - only a few kb in size (Written in C/C++ Win32)
* Extremely low memory usage
* No internet connection needed
* No installation required
* No Web API, no library used
* Automatically checks local time every 5 minutes and sets color temperature accordingly

## Configuration
* **Autostart:** Copy NightLight.exe shortcut to "%APPDATA%\Microsoft\Windows\Start Menu\Programs\Startup" folder
* Run UnlockGamma.reg file and reboot your computer to unlock < 4500K color temperatures on Windows
* Default settings: 
  * Day (08-18): **4200K**
  * Evening (18-21): **3400K**
  * Night (21-24): **2300K**
