#####################################################################
# This module handles the compilation of a program to machine code  #
#####################################################################

# Addresses of the PP2 registers
PP2_REG_COMMAND = 0x50
PP2_REG_STATUS  = 0x50
PP2_REG_INSTR   = 0x51
PP2_REG_TRANSF  = 0x52

#                  command ->       (opCode, register,        usual_args)
                   # Do a Full Reset
PP2_COMMAND_KW = { 'reset':         ('R',    PP2_REG_COMMAND, [0x02]),
                   # Change the working mode to charge
                   'charge_mode':   ('c',    PP2_REG_COMMAND, [0x03]),
                   # Change the working mode to micro
                   'micro_mode':    ('M',    PP2_REG_COMMAND, [0x00]),
                   # transfer
                   'transfer':      ('T',    PP2_REG_TRANSF , [0x00]),
                   # Trigger the pulse sequence
                   'launch':        ('D',    PP2_REG_COMMAND, [0x00, 0x80]),
                   # Save an instruction on the PP2 memory
                   'save':          ('A',    PP2_REG_INSTR  , [])
                 }
PP2_COMMANDS   = PP2_COMMAND_KW.keys ()

class PP2 (object):
    def __init__ (self, usb):
        '''
        Create a new PP2 manager instance. It need to be used as a Singleton.
        usb: usb_interface.Interface
        '''
        self.usb = usb
        self.program_loaded = True

    def __execute_command__ (self, cmnd, args=None, now=True):
        '''
        TODO: Comment
        '''
        if cmnd not in PP2_COMMANDS: raise PP2InvalidCommand (cmnd)
        op, reg, argList = PP2_COMMAND_KW [cmnd]
        if args: # Overload arguments
            argList = args
        self.usb.set_command (op)
        self.usb.set_address (reg)
        for p in argList:
            self.usb.add_param (p)
        if now:
            self.usb.send_and_receive ()

    def load (self, instructions):
        self.__execute_command__ ('reset')
        self.__execute_command__ ('charge_mode')
        for i in instructions:
            b = i.encode ()
            self.__execute_command__ ('save', b)
            self.__execute_command__ ('transfer')
        self.program_loaded = True

    def launch (self):
        if not self.program_loaded:
            raise PP2ProgramNotLoaded ()
        self.__execute_command__ ('launch')
        self.program_loaded = False


# PP2 Errors
class PP2ERROR (Exception): pass
class PP2InvalidCommand (PP2ERROR):
    def __init__ (self, cmnd):
        PP2ERROR.__init__ (self, "%s is not a valid command. Command should be one of [%s]" % (cmnd, ','.join (PP2_COMMANDS))
class PP2ProgramNotLoaded (PP2ERROR):
        PP2ERROR.__init__ (self, "You need to load a program before launching it")