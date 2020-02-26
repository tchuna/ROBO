import serial
import time
import tkinter as tk
from tkinter import *
WIDTH = 400
HEIGHT = 300	
RESET_DURATION = 3
serialPort = []
window = []

def setHighSpeed():
	data = 'h'.encode()
	serialPort.write(data)
	print(data)

def setLowSpeed():
	data = 'l'.encode()
	serialPort.write(data)
	print(data)

def stopMotor():
	data = 's'.encode()
	serialPort.write(data)
	print(data)

def toggleDirection():
	data = 't'.encode()
	serialPort.write(data)
	print(data)		

def initSerialPort():
	serialPort = serial.Serial('COM6')
	serialPort.baudrate = 9600
	serialPort.bytesize = serial.EIGHTBITS
	serialPort.stopbits = serial.STOPBITS_ONE
	serialPort.parity = serial.PARITY_NONE
	time.sleep(RESET_DURATION)
	return serialPort

def addWidget(t,row,col,text,columnspan,bw):
	if t=='Button':
		widget=Button(window,text=text,borderwidth=bw)
		widget.grid(row=row,column=col,columnspan=columnspan)
	if t=='Label':
		widget=Label(window,text=text,borderwidth=bw)
		widget.grid(row=row,column=col,columnspan=columnspan)
	return widget	


def initGUI():
	window = tk.Tk()
	window.title('Robot - Change Speed App')

	#SPEED label
	speedLabel = addWidget('Label',0,0,'SPEED CONTROL',2,2)
	speedLabel.config(font='Helvetica 16 bold')

	#button1
	btn1Label = addWidget('Label',1,0,'HIGH SPEED',1,2)
	btn1 = addWidget('Button',1,1,'Set',1,2)
	btn1.config(padx=20,pady=20,command=setHighSpeed)

	#button2
	btn2Label = addWidget('Label',2,0,'LOW SPEED',1,2)
	btn2 = addWidget('Button',2,1,'Set',1,2)
	btn2.config(padx=20,pady=20,command=setLowSpeed)

	#button3
	btn3Label = addWidget('Label',3,0,'STOP',1,2)
	btn3 = addWidget('Button',3,1,'Set',1,2)
	btn3.config(padx=20,pady=20,command=stopMotor)

	#DIRECTION label
	directionLabel = addWidget('Label',4,0,'DIRECTION CONTROL',2,2)
	directionLabel.config(font='Helvetica 16 bold')

	btn4 = addWidget('Button',5,0,'Toggle',2,2)
	btn4.config(padx=20,pady=20,command=toggleDirection)	

	return window


serialPort = initSerialPort()
window = initGUI()
window.mainloop()