#!/usr/bin/env python

from threading import Thread
import serial
import time
import collections
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import pandas as pd
import queue
import sys
import getopt

class serialPlot:
	def __init__(self, serialPort='/dev/ttyACM0', serialBaud=115200, plotLength=256):
		self.port = serialPort
		self.baud = serialBaud
		self.plotMaxLength = plotLength
		self.q = queue.Queue(plotLength * 4)

		self.data = []

		self.data.append(collections.deque([0] * plotLength, maxlen=plotLength))
		self.isRun = True
		self.isReceiving = False
		self.thread = None
		self.plotTimer = 0
		self.previousTimer = 0


		print('Trying to connect to: ' + str(serialPort) + ' at ' + str(serialBaud) + ' BAUD.')
		try:
			self.serialConnection = serial.Serial(serialPort, serialBaud, timeout=4)
			print('Connected to ' + str(serialPort) + ' at ' + str(serialBaud) + ' BAUD.')
		except:
			print("Failed to connect with " + str(serialPort) + ' at ' + str(serialBaud) + ' BAUD.')

	def readSerialStart(self):
		if self.thread == None:
			self.thread = Thread(target=self.backgroundThread)
			self.thread.start()
			while self.isReceiving != True:
				time.sleep(1)

	def getSerialData(self, frame, lines, lineValueText, lineLabel, timeText, ax):
		while self.q.qsize() > 0 :
			self.data[0].append(self.q.get())
			lines[0].set_data(range(self.plotMaxLength), self.data[0])
		min_val = min(min(self.data))
		max_val =  max(max(self.data))
		ax.set_ylim(min_val*0.95, max_val*1.05)

	def backgroundThread(self):
		time.sleep(1.0)
		self.serialConnection.reset_input_buffer()
		while (self.isRun):
			line = self.serialConnection.readline()
			for val in  line.split() :
				self.q.put(int(val))
			self.isReceiving = True


	def close(self):
		self.isRun = False
		self.thread.join()
		self.serialConnection.close()
		print('Disconnected...')


def main(argv):

	FILE_NAME = argv[0]
	portName = ""
	numPlots = 1

	try:
		opts,etc_args = getopt.getopt(argv[1:], "h:p:n:t:", ["help", "port="])

	except getopt.GetoptError:
		print(FILE_NAME, '-p <uart name>')
		sys.exit(2)

	for opt, arg in opts:
		if opt in ("-h", "--help"):
			print(FILE_NAME, '-p <uart name>')
			sys.exit()
		elif opt in ("-p"):
			portName = arg


	baudRate = 115200
	maxPlotLength = 512
	s = serialPlot(portName, baudRate, maxPlotLength)
	s.readSerialStart()


	pltInterval = 1000
	xmin = 0
	xmax = maxPlotLength
	ymin = 5000
	ymax = 10000
	fig = plt.figure(figsize=(10, 8))
	ax = plt.axes(xlim=(xmin, xmax), ylim=(float(ymin - (ymax - ymin) / 10), float(ymax + (ymax - ymin) / 10)))
	ax.set_title('BP6A Data')
	ax.set_xlabel("Sample #")
	ax.set_ylabel("ADC Value")

	lineLabel = ''
	style = 'r-'
	timeText = ax.text(0.70, 0.95, '', transform=ax.transAxes)
	lines = []
	lineValueText = []

	lines.append(ax.plot([], [], style, label=lineLabel)[0])
	lineValueText.append(ax.text(0.70, 0.90, '', transform=ax.transAxes))

	anim = animation.FuncAnimation(fig, s.getSerialData, fargs=(lines, lineValueText, lineLabel, timeText, ax), interval=pltInterval)

	plt.legend(loc="upper left")
	plt.show()

	s.close()


if __name__ == '__main__':
	main(sys.argv)
