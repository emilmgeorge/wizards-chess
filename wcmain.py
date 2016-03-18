#!/usr/bin/env python2

import chess
from wchardware import *
from wcspeech import *

import time


class WizardsChess:
	def __init__(self):

		print "Initialising Game..."
		self.board=chess.Board()	#initialise and reset board
		self.board.reset()

		print "Connecting to Chessboard Hardware..."
		self.hwobj=WCHardware(self.board)

		print "Initialising Speech Recognition..."
		self.speechobj=WCSpeech()
				

	def play(self):
		f=True
		claim=""
		while f:
			print "Current Game Status: "
			if self.board.is_check():
				print "Warning: Check!!"

			if self.board.can_claim_threefold_repetition():
				print "Draw by Threefold Repetition claim possible!"
				claim="Threefold Repetition"

			if self.board.can_claim_fifty_moves():
				print "Draw by Fifty Moves claim possible!"
				claim="Fifty Moves"

			if self.board.can_claim_draw():
				if raw_input("Do you want to claim draw?(y/n):")=='y':
					break

			self.printGame()
			self.move()
			#@TODO: Wait for x secs and show option to resign
			f=not self.board.is_game_over()

		print "Game Over!"
		if claim!="":
			print "Draw by",claim,"Claim"
		elif self.board.is_fivefold_repetition():
			print "Draw by FiveFold Repetition Rule!"
		elif board.is_seventyfive_moves():
			print "Draw by Seventyfive Moves Rule!"
		elif self.board.is_insufficient_material():
			print "Draw by Insufficient Material Rule!"
		elif self.board.is_stalemate():
			print "Draw by Stalemate!"
		elif self.board.is_checkmate():
			print "CHECKMATE!"
	
		print "RESULT: ",self.board.result(claim!="")
		print "kthxbye :)" 


	def move(self):
		try:
			mv=self.getMoveFromUser()
			if mv=='undo':
				print self.board.pop()
			else:
				self.board.push_san(mv)
		except ValueError:
			print "Invalid Move! Please give a valid move!"
			return 1
		self.hwobj.setBoard(self.board)
		self.hwobj.move(str(self.board.peek()))
		return 0

	def getMoveFromUser(self):
		print "Speak your move: "

		sst=(self.speechobj.recognize()).split(' ')		# get move in english from speech

		#if sst==['undo']:
		#	return 'undo'

		while True:
			x=sst.pop(0)
			if x!='please' and x!='move':	#ignore "please", "move" ... words from command
				break

		mv=''
		frm=''
		to=''

		if sst==['side','castle']:
			if x=="king":
				mv='O-O'
			else:
				mv='O-O-O'
			return mv


		#Speech command to chess notation
		if x=='knight':		
			mv='N'
		else:
			if x=='pawn':
				mv=''
			else:
				mv=x[0].upper()

		if sst[0]=='from':
			del sst[0]
			frm=sst.pop(0)[0]
			frm=''.join((frm,sst.pop(0)))
		
		if sst[0]=='to':
			del sst[0]
			to=sst.pop(0)[0]
			to=''.join((to,sst.pop(0)))

		mv= ''.join((mv,frm,to))
		return mv


	def printGame(self):
		print "________________"
		print "  "
		print(self.board)
		print "________________"
		print "  "

		if self.board.turn:
			print "White to move."
		else:
			print "Black to move."

		return 0



if __name__ == "__main__":
	game=WizardsChess()
	if raw_input("Are you ready to play?(y/n):")!='y':
		exit()
	game.play()
	
