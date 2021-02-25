import os
#import mydearpygui
import time

print("Welcome to checkra1n.py")
print("by Sakurai07")
print("#########################")
print("[1]. Jailbreak")
print("[2]. Github")
print("[3]. Guide")
print("type help for help")
choice = input("$ ")
print(choice)
if choice == "1":
	print("Connect your I-device which is compatible")
	print("If the jailbreak fails run the guide")
	time.sleep(2)
	print("When it asks to install drives select the usb port with the i-device connection")
	asktime = input("How much time do you think it will take you?(Numbers only) ")
	print(asktime)
	intime = int(asktime)
	os.system("install-filter-win.exe")	
	while intime > 0:
		intime-=1
		print(intime)
		time.sleep(1)
	#time.sleep(asktime)
	os.system("python confirm.py")
	#os.system("dir")
	#os.system("python ipwndfu")
elif choice == "2":
	print("https://github.com/sakurai07/checkra1n.py")
elif choice == "3":
	print("Go to the github repository or check README.md")