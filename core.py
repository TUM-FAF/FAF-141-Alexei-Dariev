from tkinter import *
from math import *

def __init__(self):


        self.error=False

def clearTxt(self):
    self.string.set("")
    self.error=False

def equals(self):
    result=""
    try:
        result=float(eval(self.string.get()))
    except:
        result="Error"
    self.string.set(result)


def delete(self):
    if(not self.error):
        self.string.set(self.string.get()[0:-1])

def squareroot(core):
    try:
        core.string.set(( sqrt(float(core.string.get()))))
    except ValueError:
        if result() == '':
            showerror('Entry error', 'You have to put a number in the blank.')
        else:
            showerror('Square root error',
                              "Can't find the square root of '%s'" % core.string.get())
        core.string.set('')
        result=('Waiting for a number...')

def power(self):
    try:
        self.string.set((pow(float(self.string.get()),2)))
    except ValueError:
        if result() == '':
            showerror('Entry error', 'You have to put a number in the blank.')
        else:
            showerror('error')
        self.string.set('')
        result=('Waiting for a number...')

def Plus_Minus(self):
    result=""
    try:
        result=-float(eval(self.string.get()))
    except:
        result="Error"
    self.string.set(result)