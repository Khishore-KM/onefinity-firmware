import errno

try:
    try:
        import smbus
    except:
        import smbus2 as smbus
except:
    smbus = None


class I2C(object):

    def __init__(self, port, disabled):
        self.port = port
        self.i2c_bus = None
        self.disabled = disabled or smbus is None

    def connect(self):
        if self.disabled: return
        if self.i2c_bus is None:
            try:
                self.i2c_bus = smbus.SMBus(self.port)

            except OSError as e:
                self.i2c_bus = None
                if e.errno == errno.ENOENT: self.disabled = True
                else: raise type(e)('I2C failed to open device: %s' % e)

    def read_word(self, addr):
        self.connect()
        if self.disabled: return

        try:
            return self.i2c_bus.read_word_data(addr, 0)

        except IOError as e:
            self.i2c_bus.close()
            self.i2c_bus = None
            raise type(e)('I2C read word failed: %s' % e)

    def write(self, addr, cmd, byte=None, word=None, block=None):
        self.connect()
        if self.disabled: return

        try:
            if byte is not None:
                self.i2c_bus.write_byte_data(addr, cmd, byte)

            elif word is not None:
                self.i2c_bus.write_word_data(addr, cmd, word)

            elif block is not None:
                if isinstance(block, str): block = list(map(ord, block))
                self.i2c_bus.write_i2c_block_data(addr, cmd, block)

            else:
                self.i2c_bus.write_byte(addr, cmd)

        except IOError as e:
            self.i2c_bus.close()
            self.i2c_bus = None
            raise type(e)('I2C write failed: %s' % e)
