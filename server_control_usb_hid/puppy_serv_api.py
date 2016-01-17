#!/usr/bin/python

import subprocess


MIN_FREE_MEMORY_PERSENT = 2
MIN_FREE_HDD_SPACE_TORRENT_PERSENT = 5
MIN_FREE_HDD_SPACE_SYSTEM_PERSENT = 15
MIN_FREE_HDD_SPACE_RAID_PERSENT = 15

def FindLine( File, Text ):
    try:
        for String in File:
            if Text in String:
                return String
        return None 
    except:
        return None


def FindNumberInLine( Line ):
    Result = 0
    Mode = 0
    for Char in Line:
        if Char.isdigit():
            Mode = 1
            Result = Result * 10 + int( Char )
        else:
            if Mode == 1:
                return Result

    return Result

     
def GetMemAllMb():
    MemFile = open( "/proc/meminfo" )
    Mem = FindLine( MemFile, "MemTotal" )
    MemFile.close()    
    return FindNumberInLine( Mem ) / 1024
    
     
def GetMemFreeMb():
    MemFile = open( "/proc/meminfo" )
    Mem = FindLine( MemFile, "MemFree" )
    MemFile.close()    
    return FindNumberInLine( Mem ) / 1024
 
     
def IsRaidNominal():
    File = open( "/proc/mdstat" )
    Line = FindLine( File, "[UU]" )
    File.close() 
    if Line == None:
        return False
    else:
        return True
 
    
def GetCpuTerm():
    File = open( "/sys/devices/pci0000:00/0000:00:18.3/temp1_input" )
    String = File.readline() 
    return float( String ) / 1000
    File.close() 
 
    
def GetCpuTermCrit():
    File = open( "/sys/devices/pci0000:00/0000:00:18.3/temp1_crit" )
    String = File.readline() 
    return float( String ) / 1000
    File.close() 
 
    
def GetCpuTermCritHyst():
    File = open( "/sys/devices/pci0000:00/0000:00:18.3/temp1_crit_hyst" )
    String = File.readline() 
    return float( String ) / 1000
    File.close() 
 
    
def GetCpuTermMax():
    File = open( "/sys/devices/pci0000:00/0000:00:18.3/temp1_max" )
    String = File.readline() 
    return float( String ) / 1000
    File.close() 


def GetUsageHddFreePersent( Path ):
    df = subprocess.Popen( [ "df", Path ], stdout=subprocess.PIPE )
    output = df.communicate()[0]
    device, size, used, available, percent, mountpoint = \
        output.split("\n")[1].split()
    return float( available ) / float( size ) * 100  
        

class server_state:
    Memory = False    
    Raid = False
    CpuTerm = False
    HddTorrent = False
    HddRaid = False
    HddSystem = False

    RawFreeMemory = 0
    RawAllMemory = 0
    RawCpuTerm = 0
    RawCpuTermCrit = 0
    RawHddTorrentFreePersent = 0
    RawHddRaidFreePersent = 0
    RawHddSystemFreePersent = 0

    def UpdateRawData( self ):
        self.RawFreeMemory = GetMemFreeMb()
        self.RawAllMemory = GetMemAllMb()
        self.RawCpuTerm = GetCpuTerm()
        self.RawCpuTermCrit = GetCpuTermCrit()
        self.RawHddTorrentFreePersent = GetUsageHddFreePersent( "/raid/shared/noraid" )
        self.RawHddRaidFreePersent = GetUsageHddFreePersent( "/raid" )
        self.RawHddSystemFreePersent = GetUsageHddFreePersent( "/initrd/mnt/dev_save" )
        
    def UpdateState( self ):
        self.UpdateRawData()
        self.Memory = float( self.RawFreeMemory ) / float( self.RawAllMemory ) * 100 > MIN_FREE_MEMORY_PERSENT
        self.Raid = IsRaidNominal()
        self.CpuTerm = self.RawCpuTerm < self.RawCpuTermCrit
        self.HddTorrent = self.RawHddTorrentFreePersent > MIN_FREE_HDD_SPACE_TORRENT_PERSENT
        self.HddRaid = self.RawHddRaidFreePersent > MIN_FREE_HDD_SPACE_SYSTEM_PERSENT
        self.HddSystem = self.RawHddSystemFreePersent > MIN_FREE_HDD_SPACE_RAID_PERSENT

    def IsDiffState( self, State ):
        if self.Memory == State.Memory and self.HddRaid == State.HddRaid and \
          self.CpuTerm == State.CpuTerm and self.HddTorrent == State.HddTorrent and \
          self.HddRaid == State.HddRaid and self.HddSystem == State.HddSystem:
            return True
        else:
            return False

           
