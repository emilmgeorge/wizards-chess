import serial
import time
from wcarduino import *

HIGH="HIGH"
LOW="LOW"
x='x'
y='y'


def correctStep(step, multiple=12.5):
	d=step%multiple
	if d>multiple/2:
		return step-d+multiple
	else:
		return step-d


class WCHardware:
	def __init__(self, board=""):
		self.board=board
		self.initArduino() 
		self.width=200		#width in steps
		self.height=200
		self.squarewidth=self.width/8	#width, height of individual squares in the chessboard
		self.squareheight=self.height/8
		self.curstepx=0
		self.curstepy=0


	def setBoard(self, board):
		self.board=board

	def initArduino(self):
		"""
		Connects to the arduino via pyserial
		"""
		print "Connecting to uC via USB-Serial..."
		try:
			self.arduino=Arduino(9600)
			#self.arduino=serial.Serial(ARDUINO,9600,timeout=None)
		except:
			print "Error: Could not connect to arduino!"
			return 1
		return 0

			
	def moveToPoint(self, ptx=0, pty=0,mag=0):
		"""
		Sends the correct number of steps to move
		"""
		#Calculate number of steps to move
		nxsteps=ptx-self.curstepx		
		nysteps=pty-self.curstepy

		#Correct the number of steps to move to multiples of square[width or height]/2 eg: 12.5, 25, 37.5 ....
		nxsteps=correctStep(nxsteps)
		nysteps=correctStep(nysteps)

		self.curstepy=correctStep(pty)
		self.curstepx=correctStep(ptx)

		#give the move command to arduino via serial
		self.arduino.do("xy",(nxsteps,nysteps,mag,))


	def move(self, move):
		"""
		Calculates the sequence of x,y points to move to in order to make the move
		"""
		print "Moving",move,".. Please wait.."
		frmindex={}	#index
		toindex={}
		frmstep={}	#step
		tostep={}
		tostepcorner={}
		frmindex[x]=ord(move[0])-97
		frmindex[y]=int(move[1])-1
		toindex[x]=ord(move[2])-97
		toindex[y]=int(move[3])-1

		"""
		how a move is made:
		1. move the piece to the one of the 4 corners of its current cell which is in the direction of the 'to' cell.
		2. move the piece towards the 'to' cell through the border, in the Y direction
		3. move the piece as step 2, in the X direction
		4. move the piece from corner to centre of the 'to' cell. 

		@TODO !!! code to move captured pieces outside board
		@TODO get better variable names? 
		"""

		#No. of steps to move to the corner of the cell from the centre of the cell
		tostepcorner[x]=(0.5)*self.squarewidth 
		if toindex[x]<frmindex[x]:
			tostepcorner[x]=-tostepcorner[x]

		tostepcorner[y]=(0.5)*self.squareheight 
		if toindex[y]<frmindex[y]:
			tostepcorner[y]=-tostepcorner[y]


		#No. of steps to move to the 'from' cell from the initial 0,0 position
		frmstep[x]=(frmindex[x]+0.5)*self.squarewidth
		frmstep[y]=(frmindex[y]+0.5)*self.squareheight


		#first move to 'from' cell to get the piece
		self.moveToPoint(frmstep[x],frmstep[y],0)
		#start moving

		#move to corner with piece
		self.moveToPoint(self.curstepx+tostepcorner[x],self.curstepy+tostepcorner[y],1)

		#no. of steps to move from corner to the center of the 'to' cell
		if toindex[x]<frmindex[x]:
			# the plus 1 is for selecting the corner closer to 'from' cell, in case from > to from origin.
			tostep[x]=(toindex[x]+1)*self.squarewidth
		elif toindex[x]==frmindex[x]:
			tostepcorner[x]=-tostepcorner[x]
			tostep[x]=self.curstepx
		else:
			tostep[x]=(toindex[x])*self.squarewidth

 
		if toindex[y]<frmindex[y]:
			tostep[y]=(toindex[y]+1)*self.squareheight
		elif toindex[y]==frmindex[y]:
			tostep[y]=self.curstepy
			tostepcorner[y]=-tostepcorner[y]
		else:
			tostep[y]=(toindex[y])*self.squareheight


		#move y thru border
		self.moveToPoint(self.curstepx,tostep[y])

		#move x thru border
		self.moveToPoint(tostep[x],self.curstepy)			

		#move to center with piece
		self.moveToPoint(self.curstepx+tostepcorner[x],self.curstepy+tostepcorner[y])


	


if __name__ == "__main__":
	hwobj=ChessHardware()
	print "Hardware test. Moving b1c3"
	hwobj.move("b1c3")