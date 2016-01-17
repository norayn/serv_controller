#!/usr/bin/python
 
import os
import sys
import datetime
import time
import os.path
import ConfigParser
import copy
import hid_api as sc
import puppy_serv_api as serv

#from random import randint

CONFIG_PATCH = "/raid/shared/prog/PythonCode/hid_serv/config.cfg"

TermLo =        [  10, 140, 150, 155, 160 ]
TermHi =        [ 140, 150, 155, 160, 255 ]
FanSpeedLo =    [  80, 120, 130, 180, 220 ]
FanSpeedHi =    [ 120, 160, 180, 220, 255 ]
TermSpeedCount = 5

DefaultColor = sc.color( 111, 111, 111 )
temp_add = [0] * 4
fan_power_add = [0] * 4
manual_fan_power_hdd = [0] * 4

   
g_Config = ConfigParser.RawConfigParser 
Log_Path = "/raid/sustem/logs/hid_serv/"
Status_File = "hidserv"

LogicIsChange = 0
PrevServerState = serv.server_state()
#-----------------------------------------------

def ReloadConfig():
    try:         
        g_Config = ConfigParser.RawConfigParser()
        g_Config.read( CONFIG_PATCH )

        global Log_Path
        Log_Path = g_Config.get('main', 'log_path')
        global Status_File
        Status_File = g_Config.get('main', 'status_file')
        
        global DefaultColor
        DefaultColor.R = g_Config.getint('mode_normal_color', 'r')
        DefaultColor.G = g_Config.getint('mode_normal_color', 'g')
        DefaultColor.B = g_Config.getint('mode_normal_color', 'b')
        sc.SetLedColor( sc.LED_ALL, DefaultColor )

        manual_fan_power_hdd[ 0 ] = g_Config.getint('main', 'manual_fan_power_pwr')
        manual_fan_power_hdd[ 1 ] = g_Config.getint('main', 'manual_fan_power_cpu')
        manual_fan_power_hdd[ 2 ] = g_Config.getint('main', 'manual_fan_power_out')
        manual_fan_power_hdd[ 3 ] = g_Config.getint('main', 'manual_fan_power_hdd')

        for I in range( 0, 4 ):
            if manual_fan_power_hdd[ I ] != -1 :
                sc.SetFanPower( sc.FAN[ I ], manual_fan_power_hdd[ I ] )

        temp_add[ 0 ] = g_Config.getint('main', 'temp_add_pwr')
        temp_add[ 1 ] = g_Config.getint('main', 'temp_add_cpu')
        temp_add[ 2 ] = g_Config.getint('main', 'temp_add_out')
        temp_add[ 3 ] = g_Config.getint('main', 'temp_add_hdd')

        fan_power_add[ 0 ] = g_Config.getint('main', 'fan_power_add_pwr')
        fan_power_add[ 1 ] = g_Config.getint('main', 'fan_power_add_cpu')
        fan_power_add[ 2 ] = g_Config.getint('main', 'fan_power_add_out')
        fan_power_add[ 3 ] = g_Config.getint('main', 'fan_power_add_hdd')

        return os.path.getmtime( CONFIG_PATCH )
    except:
        #todo log error
        sc.SetLedColor( sc.LED_ALL, sc.color( 99, 99, 99 ) )
        return os.path.getmtime( CONFIG_PATCH )


def TermControl():

    for I in range( 0, 4 ):
        if manual_fan_power_hdd[ I ] != -1 :
            continue

        Term = sc.GetTerm( sc.TERM[ I ], None ) + temp_add[ I ]
        FanSpeed = sc.GetFanPower( sc.FAN_POWER[ I ], None )

        for J in range(0, TermSpeedCount ):
            if Term > TermLo[ J ] and Term <= TermHi[ J ]:
                TermBorder = TermHi[ J ] - TermLo[ J ]
                TermDiff = Term - TermLo[ J ]
                PowerPersent = float( TermDiff ) / float( TermBorder )
                SpeedBorder = FanSpeedHi[ J ] - FanSpeedLo[ J ]
                ResultSpeed = int( float( SpeedBorder ) * PowerPersent ) + FanSpeedLo[ J ] + fan_power_add[ I ]

                if FanSpeed != ResultSpeed:
                    sc.SetFanPower( sc.FAN[ I ], ResultSpeed )
                    #print "I", I, "TermBorder", TermBorder, "TermDiff", TermDiff
                    #print "PowerPersent", PowerPersent, "SpeedBorder", SpeedBorder, "ResultSpeed", ResultSpeed

                continue


def Log( Text ):
    global Log_Path
    #print "Log" + "\t"  + Log_Path + "hid_serv_log.txt"
    LogFile = open( Log_Path + "hid_serv_log.txt", 'a' )
    LogFile.write( datetime.datetime.now().strftime( "%d_%m_%y %H_%M: " ) + Text + "\n" )
    LogFile.close()


def LogTerm():
    #print "LogAll" + "\t"  + Log_Path
    LogFile = open( Log_Path + datetime.datetime.now().strftime( "serv_term_log_%d_%m_%y.txt" ), 'a' )
    for I in range(0, 4):
        LogFile.write( str( sc.GetTerm( sc.TERM[ I ], None ) + temp_add[ I ] ) + "\t"  + str( sc.GetFanPower( sc.FAN_POWER[ I ], None ) ) + "\t")

    LogFile.write( datetime.datetime.now().strftime( "%H:%M" ) )
    LogFile.write( "\n" )
    LogFile.close()


def LogStatus():
    #print "LogStatus" + "\t"  + Status_File
    LogFile = open( Status_File, 'w' )

    LogFile.write( "TERM_PWR" + "\t"  + str( sc.GetTerm( sc.TERM_PWR, None ) + temp_add[ 0 ] ) + "\t"  + "start" + "\t"  + str( sc.GetTermStart( sc.TERM_START[ 0 ] ) ) + "\t"  +  "fan" + "\t"  + str( sc.GetFanPower( sc.FAN_POWER[ 0 ], None ) ) )
    LogFile.write( "\n" )             
    LogFile.write( "TERM_CPU" + "\t"  + str( sc.GetTerm( sc.TERM_CPU, None ) + temp_add[ 1 ] ) + "\t"  + "start" + "\t"  + str( sc.GetTermStart( sc.TERM_START[ 1 ] ) ) + "\t"  +  "fan" + "\t"  + str( sc.GetFanPower( sc.FAN_POWER[ 1 ], None ) ) )
    LogFile.write( "\n" )             
    LogFile.write( "TERM_OUT" + "\t"  + str( sc.GetTerm( sc.TERM_OUT, None ) + temp_add[ 2 ] ) + "\t"  + "start" + "\t"  + str( sc.GetTermStart( sc.TERM_START[ 2 ] ) ) + "\t"  +  "fan" + "\t"  + str( sc.GetFanPower( sc.FAN_POWER[ 2 ], None ) ) )
    LogFile.write( "\n" )            
    LogFile.write( "TERM_HDD" + "\t"  + str( sc.GetTerm( sc.TERM_HDD, None ) + temp_add[ 3 ] ) + "\t"  + "start" + "\t"  + str( sc.GetTermStart( sc.TERM_START[ 3 ] ) ) + "\t"  +  "fan" + "\t"  + str( sc.GetFanPower( sc.FAN_POWER[ 3 ], None ) ) )
    LogFile.write( "\n" )
    global PrevServerState
    LogFile.write( "RaidState = " + str( PrevServerState.Raid ) + "\n" )
    LogFile.write( "CpuTermState = " + str( PrevServerState.CpuTerm ) + "\n" )
    LogFile.write( "FreeMemoryState = " + str( PrevServerState.Memory ) + "\n" )
    LogFile.write( "HddTorrentSpaseState = " + str( PrevServerState.HddTorrent ) + "\n" )
    LogFile.write( "HddRaidSpaseState = " + str( PrevServerState.HddRaid ) + "\n" )
    LogFile.write( "HddSystemSpaseState = " + str( PrevServerState.HddSystem ) + "\n" )

    LogFile.close()


def UpdateLogic():

    ServerState = serv.server_state()
    ServerState.UpdateState()

    global PrevServerState

    if ServerState.IsDiffState( PrevServerState ) == False:       

        if ServerState.CpuTerm != PrevServerState.CpuTerm:
            if ServerState.CpuTerm == False:
                sc.SetLedColor( sc.LED_UP, sc.color( 222, 222, 0 ) )
                Log( "WARNING cpu term"	)
            else:
                sc.SetLedColor( sc.LED_UP, DefaultColor )
                Log( "OK cpu term"	)
        
        if ServerState.Raid != PrevServerState.Raid:
            if ServerState.Raid == False:
                sc.SetLedColor( sc.LED_ALL, sc.color( 222, 0, 0 ) )
                Log( "WARNING Raid"	)
            else:
                sc.SetLedColor( sc.LED_ALL, DefaultColor )
                Log( "OK Raid"	)
        
        if ServerState.Memory != PrevServerState.Memory:
            if ServerState.Memory == False:
                sc.SetLedColor( sc.LED_DOWN, sc.color( 0, 222, 222 ) )
                Log( "WARNING Memory"	)
            else:
                sc.SetLedColor( sc.LED_DOWN, DefaultColor )
                Log( "OK Memory"	)
        
        if ServerState.HddRaid != PrevServerState.HddRaid:
            if ServerState.HddRaid == False:
                sc.SetLedColor( sc.LED_RIGHT, sc.color( 0, 0, 222 ) )
                Log( "WARNING HddRaid"	)
            else:
                sc.SetLedColor( sc.LED_RIGHT, DefaultColor )
                Log( "OK HddRaid"	)

        if ServerState.HddTorrent != PrevServerState.HddTorrent:
            if ServerState.HddTorrent == False:
                sc.SetLedColor( sc.LED_LEFT, sc.color( 0, 0, 222 ) )
                Log( "WARNING HddTorrent"	)
            else:
                sc.SetLedColor( sc.LED_LEFT, DefaultColor )
                Log( "OK HddTorrent"	)

        if ServerState.HddSystem != PrevServerState.HddSystem:
            if ServerState.HddSystem == False:
                sc.SetLedColor( sc.LED_LEFT, sc.color( 0, 0, 222 ) )
                sc.SetLedColor( sc.LED_RIGHT, sc.color( 0, 0, 222 ) )
                Log( "WARNING HddSystem"	)
            else:
                sc.SetLedColor( sc.LED_LEFT, DefaultColor )
                sc.SetLedColor( sc.LED_RIGHT, DefaultColor )
                Log( "OK HddSystem"	)

    PrevServerState = copy.deepcopy( ServerState )


def PrintStat():
    os.system(['clear','cls'][os.name == 'nt'])
    #print sc.m_ControllerData
    print "TERM_PWR", sc.GetTerm( sc.TERM_PWR, True ) + temp_add[ 0 ], " |  start", sc.GetTermStart( sc.TERM_START[ 0 ] ),  " |  fan", sc.GetFanPower( sc.FAN_POWER[ 0 ], None )
    print "TERM_CPU", sc.GetTerm( sc.TERM_CPU, None ) + temp_add[ 1 ], " |  start", sc.GetTermStart( sc.TERM_START[ 1 ] ),  " |  fan", sc.GetFanPower( sc.FAN_POWER[ 1 ], None )
    print "TERM_OUT", sc.GetTerm( sc.TERM_OUT, None ) + temp_add[ 2 ], " |  start", sc.GetTermStart( sc.TERM_START[ 2 ] ),  " |  fan", sc.GetFanPower( sc.FAN_POWER[ 2 ], None )
    print "TERM_HDD", sc.GetTerm( sc.TERM_HDD, None ) + temp_add[ 3 ], " |  start", sc.GetTermStart( sc.TERM_START[ 3 ] ),  " |  fan", sc.GetFanPower( sc.FAN_POWER[ 3 ], None )
    print "\n"
    global PrevServerState

    if PrevServerState.Raid == False:
        print "___WARNING___     RAID ERROR"
    if PrevServerState.Memory == False:
        print "WARNING!     free memory=" , PrevServerState.RawFreeMemory, "of", PrevServerState.RawAllMemory, "Mb"
    else:
        print "free memory=" , PrevServerState.RawFreeMemory, "of", PrevServerState.RawAllMemory, "Mb"
    if PrevServerState.CpuTerm == False:
        print "WARNING!     cpu term=" , PrevServerState.RawCpuTerm, "C"
    else:
        print "cpu term=" , PrevServerState.RawCpuTerm, "C"
    if PrevServerState.HddTorrent == False:
        print "WARNING!     Hdd Torrent free space persent= %.2f" % PrevServerState.RawHddTorrentFreePersent
    else:
        print "Hdd Torrent free space persent= %.2f" % PrevServerState.RawHddTorrentFreePersent
    if PrevServerState.HddRaid == False:
        print "WARNING!     Hdd    Raid free space persent= %.2f" % PrevServerState.RawHddRaidFreePersent
    else:
        print "Hdd    Raid free space persent= %.2f" % PrevServerState.RawHddRaidFreePersent
    if PrevServerState.HddSystem == False:
        print "WARNING!     Hdd  System free space persent= %.2f" % PrevServerState.RawHddSystemFreePersent
    else:
        print "Hdd  System free space persent= %.2f" % PrevServerState.RawHddSystemFreePersent
 
    
def main():
	Log( "start----------------------------------------" )

	LogCounter = 0
	sc.HidInit()

	sc.SetCommand( sc.TC_IS_RUN )
	#sc.SetCommand( sc.TC_DEBUG_PAUSE )

	sc.SetLedColor( sc.LED_ALL, sc.color( 0, 30, 0 ) )

	print sc.m_ControllerData
	time.sleep(1)

	g_LastChangeConfig = None 
	UpdateLogicPreckallerCounter = 0

	while True:
		try:
		    if g_LastChangeConfig != os.path.getmtime( CONFIG_PATCH ):
		    	g_LastChangeConfig = ReloadConfig()
		except:
		    Log( "error config" )
		    time.sleep(2)

		PrintStat()
		TermControl()

		time.sleep(1)
		sc.SetCommand( sc.TC_IS_RUN )
		try:		
		    LogStatus()
		except:
		    Log( "error status" )
		    time.sleep(2)

		if LogCounter == 0:
			LogCounter = 60 #log
			LogTerm()

		LogCounter -= 1

		if UpdateLogicPreckallerCounter == 0:
			try:
			    UpdateLogic()
			except:
			    Log( "error logic" )
			    time.sleep(2)

			UpdateLogicPreckallerCounter = 5

		UpdateLogicPreckallerCounter -= 1

		time.sleep(1)
	print "fin"


main()



