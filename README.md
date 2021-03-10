# Checkra1n.py
 checkra1n for windows, written in python

well it's checkm8 only

# Testers
<a href= https://github.com/Sakurai07/Checkra1n.py/blob/main/testers.md>Testers</a>


# Must do
after cloning cd into the folder and type `python main.py`
type 0 into `$ ` to install dependencies
then copy libusb0.dll into c:/windows/system32/

# Steps
For the menu, simply follow installation
for the instant checkra1n cd to the folder and type `cpbin.lnk` (allows you to just type checkra1n from anywhere, boot flags not supported yet)
For a bad GUI version cd to the folder and type `python gui.py`
For webra1n (css + html to come soon) cd to the folder and type `python index.py`

# Installation:
```
git clone https://github.com/Sakurai07/Checkra1n.py/
cd Checkra1n.py
python main.py
```

# TO DO:
- [ ] Implement pyboot
- [x] Add mydearpygui
- [ ] Make theming

# Compatability
s5l8947x - Apple TV 3rd Gen

s5l8950x: iPhone 5/5C

s5l8955x: iPad 4th Gen

s5l8960x: iPad Air, iPhone 5S, iPad mini 2, iPad mini 3

t8002: Apple Watch Series 1/2

t8004: Apple Watch Series 3

t8010: iPad 6/7th Gen, iPhone 7, iPhone 7 Plus, iPod Touch 7th Gen

t8011: iPad Pro (10.5 inch), iPad Pro (12.9 inch) 2nd Gen, Apple TV 4k

t8015: iPhone 8/8+, iPhone X

Or anything else that supports the checkm8 exploit

# Troubleshooting:
1. restart your computer at least 3 times after installing drivers
2. re-try the jailbreak

# Errors:
 **1.** File "C:\Users\users\Downloads\ipwndfu-master\ipwndfu-master\ipwndfu", line 47, in <module>
    device = dfu.acquire_device()
  File "C:\Users\users\Downloads\ipwndfu-master\ipwndfu-master\dfu.py", line 16, in acquire_device
    for device in usb.core.find(find_all=True, idVendor=0x5AC, idProduct=0x1227, backend=backend):
  File "C:\Users\users\Downloads\ipwndfu-master\ipwndfu-master\usb\core.py", line 1263, in find
    raise NoBackendError('No backend available')
usb.core.NoBackendError: No backend available

_with this error you must run install dependencies in main.py_

# Credits:
 axi0mX for original exploit
 
 geohot for patch of exploit
 
 sakurai07 for patching it further with a user friendly ui
 
 # Disclaimer
 I am not responsible for your device errors
