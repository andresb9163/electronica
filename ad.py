MODULATED  = 0x00
CONTINUOUS = 0x01
ON         = 0x00
OFF        = 0x01
RESETED    = 0x02


class ad (object):
    def __init__ (self):
        self.conf  = None                # Configuración actual AD
        self.mode  = MODE_AD["MODULATE"] # Modo de AD {Modulated, Continuous}
        self.ndata = 0                   # Número de datos adquiridos
        self.kbpc  = 0                   # Kb de adquicición por ciclo
        self.channels = (None, None)

    def acquire ():
        raise NotImplementedError ()

    def configure ():
        raise NotImplementedError ()

