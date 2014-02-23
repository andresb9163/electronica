from array   import array
from mpusb   import mpusb, MP_READ, MP_WRITE, MAX_NUM_MPUSB_DEV

class Interface (object):
    vid_pid   = "vid_04d8&pid_000c"
    out_pipe  = "\\MCHP_EP1"
    in_pipe   = "\\MCHP_EP1"

    def __init__ (self):
        self.lib = mpusb ()
        self.rxbuffer = ""
        # The transmitions are always 64 bytes long
        self.txbuffer = array('B', [0x00] * 64)
        self.payload  = 2
        self.paq = 0

    def set_address (self, addr):
        '''
        TODO: Comment
        ====
        '''
        self.txbuffer [0] = addr

    def set_command (self, cmnd):
        '''
        TODO: Comment
        ====
        '''
        self.txbuffer [1] = cmnd

    def add_param (self, param):
        '''
        Add a new param to the command
        '''
        assert (payload < len (self.txbuffer))
        self.txbuffer [payload] = param
        payload += 1

    def send (self, send_delay=10000):
        '''
        TODO: Comment
        ====
        '''
        outp = self.lib.open (0, self.vid_pid, self.out_pipe, MP_WRITE, 0)
        outp.write (self.txbuffer, send_delay)
        outp.close ()

    def receive (self, lenght, receive_delay=30000):
        '''
        TODO: Comment
        ====
        '''
        inp  = self.lib.open (0, self.vid_pid, self.in_pipe, MP_READ, 0)
        r, d = inp.read (lenght, receive_delay)
        inp.close ()
        return d

    def send_and_receive (self, send_delay=10000, receive_delay=15000):
        '''
        TODO: Comment
        ====
        '''
        inp  = self.lib.open (0, self.vid_pid, self.in_pipe, MP_READ, 0)
        outp = self.lib.open (0, self.vid_pid, self.out_pipe, MP_WRITE, 0)
        outp.write (self.txbuffer, send_delay)
        r, d = inp.read (len (self.txbuffer), receive_delay)
        self.rxbuffer = d

    def get_summary (self):
        '''
        Todo: Comment
        '''
        max_count = self.lib.deviceCount (self.vid_pid)
        count = 0
        retvalue = 256
        for i in xrange (MAX_NUM_MPUSB_DEV):
            try:
                tmp_conn = self.lib.open (i, self.vid_pid, None, MP_READ, 0)
                count += 1
                retvalue = i
            except:
                pass
            if count == max_count:
                break
            return retvalue
