#!/usr/bin/python

# Pizzica.py
# In Circuit Programming utility for FOX Board and Netus G20  
#
# Author: Sergio Tanzilli - tanzilli@acmesystems.it
#
# Copyright 2010 Acme Systems srl. Ladispoli (Rome) Italy
# http://www.acmesystems.it
# All Rights Reserved.
# 
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

import serial
import time
import sys
import getopt
import string 
from xmodem import XMODEM

try:
	opts, args = getopt.getopt(sys.argv[1:], "f:d:m:")
except getopt.GetoptError, err:
	print str(err) 
	sys.exit(2)


for argument, value in opts:
	if argument == "-f":
		filename = value
	elif argument == "-d":
		serialdevice = value
	else:
		assert False, "Unhandled option"

print "ISP utility for FOX Board and Netus G20"
print "Version: 0.01"

try:
	print "--- Filename: " + filename
	print "--- Serial device: " + serialdevice
except:
	print "--- Example of use:"
	print "./isp.py -f filename -d /dev/ttyUSB0"
	sys.exit(1)


#Open the serial port 
ser = serial.Serial(
	port=serialdevice, 
	baudrate=115200, 
	timeout=1,
	parity=serial.PARITY_NONE,
	stopbits=serial.STOPBITS_ONE,
	bytesize=serial.EIGHTBITS
)  
ser.flushInput()

#Define the getc and putc function required from the
#xmodem module
def getc(size, timeout=1):
	data = ser.read(size)
	return data

def putc(data, timeout=1):
	ser.write(data)
	return len(data)

x = XMODEM(getc, putc)

print "Close the NetusG20 boot pin and turn-on the board..."

while True:
	ser.flushInput()
	ser.write("#")
	time.sleep(1)
	if ser.read(1)=='>':
	    break

ser.flushInput()

address=0x200000
cmdstring = "S%06X,#" % (address)
print "Send: [" + cmdstring + "]"
ser.write(cmdstring)


stream = open(filename, 'rb')
x.send(stream)
stream.close()

while ser.read(1)!='>':
    pass

cmdstring = "G200000#"
print "Send: [" + cmdstring + "]"
ser.write(cmdstring)

print "Characters received from the FOX Board G20 (type Ctrl-C to exit)"

while True:
    s = ser.read(1) 
    sys.stdout.write(s)


ser.close()


