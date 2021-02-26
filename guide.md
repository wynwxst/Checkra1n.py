To begin, you'll want to download this version of the checkm8 exploit (huge thanks to Geohot for rewriting the script to run on Windows). Extract the .zip and make a note of the extracted location.

Next, you'll need to grab the latest version of libusb-win32. Extract the .zip.

Plug your Apple device into your PC and put it into DFU mode. Make sure your PC recognises your device.

Because we're using a Python script to communicate with your device, we need to install a dependency that will let our script send data to and from our device. Navigate to where you extracted libusb-win32. Open up /bin/amd64/. Go ahead and run the install-filter-win.exe file. Select "Install a device filter" and click next. In the list, find your device in DFU mode. It should say "Apple Mobile Device (DFU Mode). If it does not say DFU mode, do not continue. Click on it and then press install. After it completes, close the window.

To check if it successfully installed the filter, open testlibusb-win.exe. It should show your device's information. Close this window.

Go back to /bin/ and open up inf-wizard.exe. On the window that opens, click next. Select "Apple Mobile Device (DFU Mode) and then click next. Check that you've chosen the right device, then click next. On the new window that opens, choose your desktop to save this .inf file. (Note that for whatever reason, the default save location 'Documents' didn't work on future steps for me.) After saving it, a new window will open. Do not click "Install now". Simply click done and the window will close automatically.

Now here comes the most tedious part. Due to Windows not allowing unsigned third party drivers to be installed while not in safe mode, we'll have to boot into it. Bring up your power down options, and while holding shift, click restart. Keep holding shift until a blue screen comes up. Click "Troubleshoot", then click "Advanced options". Click "Startup Settings", then click restart. When a list of options comes up, press '7' and let your PC boot. Sign in as normal.

Open up Device Manager, and find your Apple device (it's usually down the bottom in one of the USB categories). Right click on it, and choose "Update Driver". Choose "Browse my computer for driver software". Click "Let me choose from a list of available drives on my computer". On the bottom right, click "Have Disk...". In the new window, click "Browse". Navigate to your desktop, and select the .inf file you made earlier. Click "Open", then "Okay". Click "Next". On the window that pops up, simply confirm your choice. Once it's done, go back to Device Manager.

You may have to reconnect your Apple device here. Do so if necessary. Once done, look for "libusb-win32 devices", and open the category. If you see "Apple Mobile Device (DFU Mode), then you were successful.

With that completed, we can now finally test the script. Navigate to where you extracted Geohot's version of checkm8. Open up a CMD with administrative privileges, and run the following commands:
