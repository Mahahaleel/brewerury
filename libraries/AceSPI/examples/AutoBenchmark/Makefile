# See https://github.com/bxparks/EpoxyDuino for documentation about this
# Makefile to compile and run Arduino programs natively on Linux or MacOS.

APP_NAME := AutoBenchmark
ARDUINO_LIBS := EpoxyMockDigitalWriteFast AceCommon AceSPI
MORE_CLEAN := more_clean
include ../../../EpoxyDuino/EpoxyDuino.mk

.PHONY: benchmarks

AUNITER_DIR := ../../../AUniter/tools

TARGETS := nano.txt micro.txt samd.txt stm32.txt esp8266.txt esp32.txt \
teensy32.txt

README.md: generate_readme.py generate_table.awk $(TARGETS)
	./generate_readme.py > $@

benchmarks: $(TARGETS)

# The USB/ACM ports can change dynamically. Make sure that the microcontroller
# is on the correct port before using these Make targets.
#
# When benchmarking the speed of Hardware I2C of the TwoWireInterface class
# which uses the <Wire.h> library using hardware I2C, a real I2C device (e.g.
# HT16K33) must exist on the bus so that the proper ACK comes back from the
# device. Otherwise, the <Wire.h> library seems to terminate the transfer which
# results in an abnormally small durations.

nano.txt:
	$(AUNITER_DIR)/auniter.sh upmon -o $@ --eof END nano:USB0

micro.txt:
	$(AUNITER_DIR)/auniter.sh upmon -o $@ --eof END micro:ACM0

samd.txt:
	$(AUNITER_DIR)/auniter.sh upmon -o $@ --eof END samd:ACM0

stm32.txt:
	$(AUNITER_DIR)/auniter.sh upmon -o $@ --eof END stm32:ACM0

esp8266.txt:
	$(AUNITER_DIR)/auniter.sh upmon -o $@ --eof END nodemcu:USB0

esp32.txt:
	$(AUNITER_DIR)/auniter.sh upmon -o $@ --eof END esp32:USB0

# Teensy requires manual capture of teensy32.txt. This Makefile rule is an
# aspirational hope for consistency.
teensy32.txt:
	$(AUNITER_DIR)/auniter.sh upmon -o $@ --eof END teensy32:ACM0

more_clean:
	echo "Use 'make clean_benchmarks' to remove *.txt files"

clean_benchmarks:
	rm -f $(TARGETS)
