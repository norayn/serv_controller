#!/usr/bin/python
 
import os
import sys
import time
import hid_api as sc


def main():
	print "start"
	LogCounter = 0
	sc.HidInit()

	sc.SetCommand( sc.TC_TO_OFF )

	print "fin"

main()



