import sys
import os
from tkinter import *

window=Tk()

window.title("Checkra1n.py")
window.geometry('1800x1800')

def run():
    os.system('python ipwndfu -p')

btn = Button(window, text="Jailbreak", bg="black", fg="white",command=run)
btn.grid(column=1, row=0)

window.mainloop()