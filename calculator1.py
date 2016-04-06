from core import *
import threading
import time
exitFlag = 0

class calculator(threading.Thread):
    core = __import__('core')

    def __init__(self, threadID, name, counter):
        threading.Thread.__init__(self)
        self.threadID=threadID
        self.name = name
        self.counter = counter
        self.error=False
        window = Tk()
        window.title("Calculator")
        window.configure(background='white')

        self.string = StringVar()
        entry = Entry(window, textvariable=self.string, font="Helvetica 15 bold")
        entry.grid(row= 0, column= 0, columnspan=6)
        entry.bind('<KeyPress>',self.keyPress)
        entry.focus()

        values = ["7", "8", "9", "/", "Clear", "<-",
                  "4", "5", "6", "*", "(",")", "1", "2", "3", "-", "=","0", ".", "%", "+",
                  "sqrt", "pow", "+/-"]


        i=0
        row=1
        col=0
        for txt in values:
            padx=10
            pady=10

            if(i==6):
                row=2
                col=0
            if(i==12):
                row=3
                col=0
            if(i==17):
                col=0
                row=4
            if(i==21):
                col=0
                row=5


            if(txt=="="):
                btn = Button(window, height=2, width =4, padx=23, pady=23, text=txt,
                             command = lambda :self.core.equals(self))
                btn.grid(row=row, column=col, columnspan=2, rowspan=2, padx=1, pady=1)

            elif(txt=="sqrt"):
                btn = Button(window, height=2, width =4, padx=pady, pady=pady, text=txt,
                             command = lambda :self.core.squareroot(self))
                btn.grid(row=row, column=col, padx=1, pady=1)

            elif(txt=="pow"):
                btn = Button(window, height=2, width =4, padx=pady, pady=pady, text=txt,
                             command = lambda :self.core.power(self))
                btn.grid(row=row, column=col, padx=1, pady=1)

            elif(txt=="+/-"):
                btn = Button(window, height=2, width =4, padx=pady, pady=pady, text=txt,
                             command = lambda :self.core.Plus_Minus(self))
                btn.grid(row=row, column=col, padx=1, pady=1)

            elif(txt=="Clear"):
                btn = Button(window, height=2, width =4, padx=pady, pady=pady, text=txt,
                             command = lambda :self.core.clearTxt(self))
                btn.grid(row=row, column=col, padx=1, pady=1)
            elif(txt=="<-"):
                btn = Button(window, height=2, width =4, padx=pady, pady=pady, text=txt,
                             command = lambda :self.core.delete(self))
                btn.grid(row=row, column=col, padx=1, pady=1)
            else:
                btn = Button(window, height=2, width =4, padx=pady, pady=pady, text=txt,
                             command = lambda txt=txt:self.addChar(txt))
                btn.grid(row=row, column=col, padx=1, pady=1)

            col=col+1
            i=i+1

        window.mainloop()
    def run(self):
        print ("Starting" + self.name)
        print_time(self.name, self.counter, 5)
        print ("Exiting" + self.name)



    def keyPress(self, event):
        allowedValues=["KP_0","KP_1","KP_2","KP_3","KP_4","KP_5","KP_6","KP_7","KP_8",
                       "KP_9","7","8","9","KP_Divide","slash","4","5","6","KP_Multiply",
                       "parenleft","parenright","1","2","3","KP_Subtract","minus","equal","0",
                       "period","percent","KP_Add", "plus","Backspace","asterisk","Right",
                       "Left", "KP_Decimal"]
        if(not core.error):
            if event.keysym in("Return", "KP_Enter"):
                core.equals()
            elif event.keysym not in allowedValues:
                return 'break'
        else:
            return 'break'

        core.clearTxt(self)
        core.equals(self)
        core.delete(self)
        core.squareroot(self)
        core.power(self)
        core.Plus_Minus(self)

    def addChar(self, char):
        if(not self.error):
            self.string.set(self.string.get()+(str(char)))

def print_time(threadName, delay, counter):
    while counter:
        if exitFlag:
            threadName.exit()
        time.sleep(delay)
        print  (threadName, time.ctime(time.time()))
        counter -=1


thread1=calculator(1, "Thread-1", 1)
thread2=calculator(2, "Thread-2", 2)

thread1.start()
thread2.start()

print ("exiting main thread")




calculator(threadID, name, counter)

