from pyparsing import Word, alphas, alphanums, nums, Forward, Group, Combine,\
                      Literal, delimitedList, OneOrMore, Optional
#Literals
equal  = Literal ("=").suppress ()
colon  = Literal (":").suppress ()
lparen = Literal ("(").suppress ()
rparen = Literal (")").suppress ()

# Keywords
phase_entry_kw = Literal ("ph").suppress ()
phase_kw       = Literal ("phases").suppress ()
program_kw     = Literal ("program").suppress ()
loop_kw        = Literal ("loop").suppress ()
times_kw       = Literal ("times").suppress ()
end_loop_kw    = Literal ("endloop").suppress ()
pshift_kw      = Literal ("phase_shift")
pulse_kw       = Literal ("p").suppress ()
delay_kw       = Literal ("d").suppress ()
acquire_kw    = Literal ("acquire").suppress ()

# Syntax Rules
#     > Callback Functions
_callbackPhase = lambda tok: PhaseDecl (int (tok[0]), [int (x) for x in tok[1]])
_callbackLoop  = lambda tok: For (int (tok[0]), tok [-1].asList (), len (tok) == 3)
_callbackPulse = lambda tok: Pulse (int (tok[0]), int (tok[1]))
_callbackDelay = lambda tok: Delay (int (tok[0]))
_callbackAcq   = lambda tok: Acquire ()

#     > Parsing Phases Section
phase_entry = (Combine (phase_entry_kw + Word (nums)) + equal +\
              Group (OneOrMore (Word (nums)))).setParseAction (_callbackPhase)
phases_section = phase_kw + colon + OneOrMore (phase_entry)

#     > Parsing Program Section
statement = Forward ()
loop      = (loop_kw + Word (nums) + times_kw +\
            Optional (pshift_kw) + Group (OneOrMore (statement)) +\
            end_loop_kw).setParseAction (_callbackLoop)
pulse     = (Combine (pulse_kw + Word (nums)) + lparen + \
             Combine (phase_entry_kw + Word (nums)) + \
             rparen).setParseAction (_callbackPulse)
delay     = Combine (delay_kw + Word (nums)).setParseAction (_callbackDelay)
acquire   = acquire_kw.setParseAction (_callbackAcq)
statement << (loop | delay | pulse | acquire)
program_section  = program_kw + colon + OneOrMore (statement)
program = Group (phases_section) + Group (program_section)

# Main functions
def parseString (string):
    pr      = program.parseString (string)
    phases  = pr[0].asList ()
    prog    = pr[1].asList ()
    return Program (phases, prog)

def parseFromFile (filename):
    string = open (filename, "r").read ()
    return parseString (string)

# Classes to hold the info
class Statement (object):
    def __str__ (self):
        return self.__pp__ (0)
    def __repr__ (self):
        return self.__pp__ (0)
    def __indent__ (self, il):
        return '    ' * il

class PhaseDecl (Statement):
    def __init__ (self, number, ph_list):
        self.number  = number
        self.ph_list = ph_list
    def __pp__ (self, il):
        return self.__indent__ (il) + "ph%d =%s" % \
               (self.number, "".join ([" %d" % x for x in self.ph_list]))

class For (Statement):
    def __init__ (self, times, statements, phase_shift=False):
        self.times       = times
        self.statements  = statements
        self.phase_shift = phase_shift
    def __pp__ (self, il):
        return self.__indent__ (il) + "loop %d times%s\n" % (self.times,\
               " phase_shift" if self.phase_shift else "") +\
               ''.join ([x.__pp__ (il + 1) + '\n' for x in self.statements]) +\
               self.__indent__ (il) + "endloop"

class Acquire (Statement):
    def __pp__ (self, il):
        return self.__indent__ (il) + "acquire"

class Pulse (Statement):
    def __init__ (self, pulse, phase):
        self.pulse = pulse
        self.phase = phase
    def __pp__ (self, il):
        return self.__indent__ (il) + "p%d (ph%d)" % (self.pulse, self.phase)

class Delay (Statement):
    def __init__ (self, delay):
        self.delay = delay
    def __pp__ (self, il):
        return self.__indent__ (il) + "d%d" % (self.delay)

class Program (object):
    def __init__ (self, phases, program):
        self.phases   = phases
        self.program  = program
    def __str__ (self):
        return "".join (["phases:\n"] + [x.__pp__ (1) + '\n' for x in self.phases] +\
               ["program:"] + ['\n' + x.__pp__ (1) for x in self.program])
    def __repr__ (self):
        return self.__str__ ()

