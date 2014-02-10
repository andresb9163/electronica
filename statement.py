############################################
# TODO: Comment main module functionality  #
############################################

class Statement (object):
    def vars (self):
        """
        TODO: Comment method
        """
        raise NotImplementedError ()
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
    icode = [0, 4]
    def vars (self):
        return set.union (*[x.vars () for x in self.statements])
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
    icode = 1
    def vars (self):
        return set ()
    def __pp__ (self, il):
        return self.__indent__ (il) + "acquire"

class Pulse (Statement):
    icode = 2
    def __init__ (self, pulse, phase):
        self.pulse = pulse
        self.phase = phase
    def vars (self):
        return set (['p%d' % self.pulse])
    def __pp__ (self, il):
        return self.__indent__ (il) + "p%d (ph%d)" % (self.pulse, self.phase)

class Delay (Statement):
    icode = 3
    def __init__ (self, delay):
        self.delay = delay
    def vars (self):
        return set (["d%d" % self.delay])
    def __pp__ (self, il):
        return self.__indent__ (il) + "d%d" % (self.delay)

class Program (object):
    def __init__ (self, phases, program):
        self.phases   = phases
        self.program  = program
    def vars (self):
        return set.union (*[x.vars () for x in self.program])
    def __str__ (self):
        return "".join (["phases:\n"] + [x.__pp__ (1) + '\n' for x in self.phases] +\
               ["program:"] + ['\n' + x.__pp__ (1) for x in self.program])
    def __repr__ (self):
        return self.__str__ ()
