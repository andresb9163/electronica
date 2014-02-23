#####################################################################
# This module handles the compilation of a program to machine code  #
#####################################################################
import statement
from   ctypes import c_uint64 as uint64

# Instruction codes for the pp2
_PP2_CONTINUE = 0x01  # Update and keep an output pattern
_PP2_LOOP     = 0x02  # Loop the next instructions
_PP2_RETL     = 0x03  # End loop
_PP2_END      = 0x07  # End a pp2 program
_PP2_INSTRUCTIONS = [_PP2_CONTINUE, _PP2_LOOP, _PP2_RETL, _PP2_END]

def compile (program, vars, phase=None):
    '''
    Compiles a program
    Returns a list of 8-bit hex words
    =================================
    program: statement.Program
    vars:    dict
    phase:   UNKNOWN
    '''
    pass

def __unroll_loop (self, loop):
    '''
    Unrolls a loop
    Returns: List of statements
    ================================
    loop: statement.For
    '''
    pass

class Instruction (object):
    def __init__ (self, time, icode, epat=[False] * 16, loopl=0, data=0):
        '''
        Creates a new instruction
        =========================
        time:  time to keep the pattern. 16 bits.
        icode: instrucion code. Element of _PP2_INSTRUCTIONS
        loopl: loop level. int between 0 and 3
        data:  field to store extra parameters for an instruction. 11 bit int.
        epat:  exit pattern, set each of the 16 pulses to high or low. List
               of 16 booleans
        '''
        self.time  = time
        if time < 0 or time >= 2 ** 16:
            raise CompilerTimeError ()
        self.icode = icode
        if icode not in _PP2_INSTRUCTIONS:
            raise CompilerInternalError ("icode")
        self.loopl = loopl
        if loopl not in range (4):
            raise CompilerLoopError ("loopl")
        self.data  = data
        if data < 0 or data >= 2 ** 11:
            raise CompilerInternalError ("data")
        self.epat  = epat
        if len (epat) != 16:
            raise CompilerInternalError ("epat")

    def encode (self):
        '''
        Returns a 8-len int list representing the instruction
        '''
        time  = (format (self.time,  'b')).rjust (32, '0')
        icode = (format (self.icode, 'b')).rjust (3,  '0')
        loopl = (format (self.loopl, 'b')).rjust (2,  '0')
        data  = (format (self.data,  'b')).rjust (11, '0')
        epat  = ''.join (map (lambda x: '1' if x else '0', self.epat))
        # Binary word representing the instruction
        word = epat + data + loopl + icode + time
        if len (word) != 64:
            raise CompilerInternalError ("word")
        # TODO: Make this return a list of 8 elements
        return [int (word [i * 8: (i+1) * 8]) for i in range (8)]

# Compiler Errors
class CompilerError (Exception): pass

class CompilerTimeError (CompilerError):
    def __init__ (self):
        CompilerError.__init__ (self, "Time for instruction is out of bounds")

class CompilerInternalError (CompilerError):
    def __init__ (self, var):
        CompilerError.__init__ (self, "This should not be happening. Var %s out of bounds" % var)

class CompilerLoopError (CompilerError):
    def __init__ (self):
        CompilerError.__init__ (self, "Only 3 loop folds are allowed")