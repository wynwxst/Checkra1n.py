import sys
import os
from tkinter import *

window=Tk()

window.title("Checkra1n.py")
window.geometry('1800x1800')

def run():
    os.system('python ipwndfu -p')

def term():
    os.system('python term.py')

btn = Button(window, text="Jailbreak", bg="black", fg="white",command=run)
btn.place(x=0, y=0)

class Lol(Frame):
    def __init__(self, master=None):
        Frame.__init__(self, master)
        self.master = master

        menu = Menu(self.master)
        self.master.config(menu=menu)

        fileMenu = Menu(menu)
        fileMenu.add_command(label="Terminal", command=term)
        fileMenu.add_command(label="Exit", command="")
        menu.add_cascade(label="Open", menu=fileMenu)

        editMenu = Menu(menu)
        editMenu.add_command(label="Undo")
        editMenu.add_command(label="Redo")
        menu.add_cascade(label="Edit", menu=editMenu)

app = Lol(window)
window.mainloop()