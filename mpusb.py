#########################################################
# This module is a python api for the windows mpusb dll #
#########################################################

from ctypes.util import find_library
from ctypes import WinDLL, c_ulong, c_char_p, c_void_p, create_string_buffer,\
                   byref, c_char, POINTER

# Constants
MPUSB_FAIL    = 0
MPUSB_SUCCESS = 1
MP_WRITE      = 0
MP_READ       = 1
MAX_NUM_MPUSB_DEV  = 127
INVALID_HANDLE_VALUE = c_void_p(-1).value

class mpusb (object):
    def __init__ (self):
        p = find_library ("mpusbapi")
        if not p:
            raise LookupError ("mpusbapi.dll not found")
        self.lib = WinDLL (p)
        self.__set_function_types__ ()


    def getDLLVersion (self):
        '''
        TODO: Comment
        ====
        '''
        return self.lib._MPUSBGetDLLVersion ()

    def deviceCount (self, vid_pid):
        '''
        TODO: Comment
        ====
        '''
        return self.lib._MPUSBGetDeviceCount (vid_pid)

    def open (self, instance, vid_pid, ep, dir, reserved):
        '''
        TODO: Comment
        ====
        '''
        handle = self.lib._MPUSBOpen (instance, vid_pid, ep, dir, reserved)
        if handle.value == INVALID_HANDLE_VALUE:
            raise RuntimeError ("Invalid handle returned")
        return Connection (self.lib, handle)

    def __set_function_types__ (self):
        self.lib._MPUSBGetDLLVersion.argtypes  = []
        self.lib._MPUSBGetDLLVersion.restype   = c_ulong
        ##
        self.lib._MPUSBGetDeviceCount.argtypes = [c_char_p]
        self.lib._MPUSBGetDeviceCount.restype  = c_ulong
        ##
        self.lib._MPUSBOpen.argtypes    = [c_ulong, c_char_p, c_char_p, c_ulong, c_ulong]
        self.lib._MPUSBOpen.restype     = c_void_p
        ##
        self.lib._MPUSBRead.argtypes    = [c_void_p, c_void_p, c_ulong, POINTER(c_ulong), c_ulong]
        self.lib._MPUSBRead.restype     = c_ulong
        ##
        self.lib._MPUSBWrite.argtypes   = [c_void_p, c_void_p, c_ulong, POINTER(c_ulong), c_ulong]
        self.lib._MPUSBWrite.restype    = c_ulong
        ##
        self.lib._MPUSBReadInt.argtypes = [c_void_p, c_void_p, c_ulong, POINTER(c_ulong), c_ulong]
        self.lib._MPUSBReadInt.restype  = c_ulong
        ##
        self.lib._MPUSBClose.argtypes   = [c_void_p]
        self.lib._MPUSBClose.restype    = c_ulong


class Connection (object):
    def __init__ (lib, handle):
        self.lib = lib
        self.handle  = handle

    def read (self, l, ms):
        '''
        TODO: Comment
        ====
        l     : number of bytes to read
        ms    : specifies the time-out interval, in milliseconds
        '''
        data   = create_string_buffer (l)
        length = c_ulong
        r = self.lib._MPUSBRead (self.handle, data, l, byref (length), ms)
        return (r, data.raw[:length])

    def write (self, data, ms):
        '''
        TODO: Comment
        ====
        data  : list of bytes (python ints) to be written
        ms    : specifies the time-out interval, in milliseconds
        '''
        array = ''.join (map (chr, data))
        r = self.lib._MPUSBWrite (self.handle, array, len (data), byref (length), ms)
        return (r, length)

    def readInt (self, len, ms):
        '''
        TODO: Comment
        ====
        l     : number of bytes to read
        ms    : specifies the time-out interval, in milliseconds
        '''
        data   = create_string_buffer (len)
        length = c_ulong
        r = self.lib._MPUSBRead (self.handle, data, len, byref (length), ms)
        return (r, data.raw[:length])

    def close (self):
        '''
        TODO: Comment
        ====
        '''
        return (self.lib._MPUSBClose (self.handle) != 0)

    def __del__ (self):
        if self.opened:
            self.close()