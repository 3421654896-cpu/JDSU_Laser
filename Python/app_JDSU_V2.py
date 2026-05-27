import pyvisa
import time

rm = pyvisa.ResourceManager()

print(rm.list_resources())

inst = rm.open_resource("GPIB0::7::INSTR")

inst.timeout = 10000
inst.write("*CLS")

inst.write(":CONF:ARR:POW")

inst.write(":INIT")
inst.write("*WAI")

print(inst.query(":FETCh:ARRay:POWer?"))

time.sleep(1)
print(inst.query(":FETCh:ARRay:POWer:WAVelength?"))

print(inst.query("*IDN?"))