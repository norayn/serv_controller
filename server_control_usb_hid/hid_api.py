#!/usr/bin/python
 
import os
import sys
import time

import usb.core
import usb.util
 
import ctypes
import ctypes.util

packet_len = 64

VID = 0x0483
PID = 0x5711
 
# define HID constants
 
REQ_HID_GET_REPORT   = 0x01
REQ_HID_GET_IDLE     = 0x02
REQ_HID_GET_PROTOCOL = 0x03
 
REQ_HID_SET_REPORT   = 0x09
REQ_HID_SET_IDLE     = 0x0A
REQ_HID_SET_PROTOCOL = 0x0B
 
HID_REPORT_TYPE_INPUT   = 1<<8
HID_REPORT_TYPE_OUTPUT  = 2<<8
HID_REPORT_TYPE_FEATURE = 3<<8


TC_R0 = 0
TC_R1 = 1
TC_R2 = 2	
TC_IS_RUN = 3		
TC_PWR_OFF = 4
TC_DEBUG_PAUSE = 5
TC_SET_LED_SEGMENT_RGB = 6
TC_SET_FAN_POWER = 7
TC_TO_OFF = 8

LED_LEFT = 0
LED_RIGHT = 1
LED_UP = 2
LED_DOWN = 3
LED_ALL = 4

FAN = [0] * 5
FAN[ 0 ] = 0
FAN[ 1 ] = 1
FAN[ 2 ] = 2
FAN[ 3 ] = 3
FAN[ 4 ] = 4

FAN_PWR = FAN[ 0 ]
FAN_CPU = FAN[ 1 ]
FAN_OUT = FAN[ 2 ]
FAN_HDD = FAN[ 3 ]
FAN_ALL = FAN[ 4 ]

TERM = [0] * 4
TERM[ 0 ] = 5
TERM[ 1 ] = 2
TERM[ 2 ] = 3
TERM[ 3 ] = 4

TERM_PWR = TERM[ 0 ]
TERM_CPU = TERM[ 1 ]
TERM_OUT = TERM[ 2 ]
TERM_HDD = TERM[ 3 ]

TERM_START = [0] * 4
TERM_START[ 0 ] = 13
TERM_START[ 1 ] = 10
TERM_START[ 2 ] = 11
TERM_START[ 3 ] = 12

FAN_POWER = [0] * 4
FAN_POWER[ 0 ] = 6
FAN_POWER[ 1 ] = 7
FAN_POWER[ 2 ] = 8
FAN_POWER[ 3 ] = 9

FAN_POWER_PWR = FAN_POWER[ 0 ]
FAN_POWER_CPU = FAN_POWER[ 1 ]
FAN_POWER_OUT = FAN_POWER[ 2 ]
FAN_POWER_HDD = FAN_POWER[ 3 ]



class color:
    R = 0
    G = 0
    B = 0

    def __init__( self, R, G, B ):
        self.R = int( max(0, min( R, 255 ) ) )
        self.G = int( max(0, min( G, 255 ) ) )
        self.B = int( max(0, min( B, 255 ) ) )

class term_buffer:

    TermBufferCount = 5  
    TermBuff = [ [ 0 for i in range( 16 ) ] for j in range( TermBufferCount ) ]
   
    def Add( e, Arr ):
        term_buffer.TermBuff.append( Arr )
        del term_buffer.TermBuff[ 0 ]

    def GetTerm( e, TermIndex ):
        TermSumm = 0
        for I in range(0, term_buffer.TermBufferCount ):            
            TermSumm += term_buffer.TermBuff[ I ][ TermIndex ]
            
        return TermSumm / term_buffer.TermBufferCount

m_HidDev = usb.core.find(idVendor=VID, idProduct=PID)
m_ControllerData = [ 0 ] * 16
m_TermBuffer = term_buffer()


def HidInit():
    print "HidInit"

    #m_HidDev = usb.core.find(idVendor=VID, idProduct=PID)

    if m_HidDev is None:
        raise ValueError('Device not found')
 
    try:
        m_HidDev.detach_kernel_driver(0)
    except: # this usually mean that kernel driver has already been dettached
        pass
  
def SendDataToController( Data ):
    report_id = 0x02	
    bmRequestTypeOut = usb.util.build_request_type(usb.util.CTRL_OUT, usb.util.CTRL_TYPE_CLASS, usb.util.CTRL_RECIPIENT_INTERFACE)
    m_HidDev.ctrl_transfer( bmRequestTypeOut,
                       REQ_HID_SET_REPORT, # == HID set_report
                       HID_REPORT_TYPE_FEATURE | report_id, # (3==HID feat.report)
                       0,
                       Data) 
    
    global m_ControllerData
    m_ControllerData = m_HidDev.read(0x81, packet_len, 0)
    m_TermBuffer.Add( m_ControllerData )  
    time.sleep(0.3)
    #print "SendDataToController read"

def SetFanPower( FanIndex, Power ):    
    Power = int( max(0, min( Power, 255 ) ) )

    data1 = [0x0] * 16
    data1[0] = TC_SET_FAN_POWER
    data1[1] = FanIndex
    data1[2] = Power
     
    SendDataToController( data1 )

def SetLedColor( LedIndex, Color ):
    data1 = [0x0] * 16
    data1[0] = TC_SET_LED_SEGMENT_RGB
    data1[1] = LedIndex
    data1[2] = Color.R
    data1[3] = Color.G
    data1[4] = Color.B       
    SendDataToController( data1 )

def SetCommand( TcComand ):
    data1 = [0x0] * 16
    data1[0] = TcComand      
    SendDataToController( data1 )

def GetTerm( TermIndex, IsNeedUpdate ):
    global m_ControllerData
    if IsNeedUpdate != None:	    
        m_ControllerData = m_HidDev.read(0x81, packet_len, 0)
        m_TermBuffer.Add( m_ControllerData )
        #print "GetTerm read"

    #return 255 - m_ControllerData[ TermIndex ]
    return 255 - m_TermBuffer.GetTerm( TermIndex )

def GetTermStart( TermStartIndex ):
    global m_ControllerData
    return 255 - m_ControllerData[ TermStartIndex ]

def GetFanPower( FanPowerIndex, IsNeedUpdate ):
    global m_ControllerData
    if IsNeedUpdate != None:	    
        m_ControllerData = m_HidDev.read(0x81, packet_len, 0)
        m_TermBuffer.Add( m_ControllerData )
        #print "GetFanPower read"

    return m_ControllerData[ FanPowerIndex ]




