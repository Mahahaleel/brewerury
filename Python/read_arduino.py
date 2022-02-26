#!/usr/bin/env python3
import serial

if __name__ == '__main__':
    try:
      ser = serial.Serial('/dev/ttyACM1', 9600, timeout=1)
      ser.flush()
    except:
      ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
      ser.flush()

    while True:
        if ser.in_waiting > 0:
            line = ser.readline().rstrip()
            print(line)
#        except:
          #if __name__ == '__main__':
          #  try:
          #    ser = serial.Serial('/dev/ttyACM1', 9600, timeout=1)
          #    ser.flush()
          #  except:
          #    ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
          #    ser.flush()
#          pass
