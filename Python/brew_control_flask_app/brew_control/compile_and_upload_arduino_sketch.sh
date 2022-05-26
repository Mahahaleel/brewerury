########## Compile & Upload Arduino commands:

# arduino-cli compile --fbqn <fbqn> <sketch>
# arduino-cli compile --fbqn arduino:avr:uno arduino_brewerury

arduino-cli upload -p <port> --fqbn <fqbn> <sketch>
arduino-cli upload -p /dev/ttyACM0 --fqbn arduino:avr:uno arduino_brewerury