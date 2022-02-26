#!/usr/bin/env python3
import serial
from datetime import datetime
from influxdb import InfluxDBClient
from datetime import datetime, timedelta
import json

influxhost = '192.168.2.24'
client = InfluxDBClient(host=influxhost, port=8086, username='brewmaster', password='Brewerury')
print(client.get_list_database())
client.switch_database('brewerury')

json_body = [] 

if __name__ == '__main__':
    try:
        ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
    except:
        ser = serial.Serial('/dev/ttyACM1', 9600, timeout=1)

    ser.flush()

    while True:
        # try:
            if ser.in_waiting > 0:
                line = ser.readline().decode('utf-8').rstrip()
                print(line)
                print(type(line))

                thermistorTemp = line.split("Temperature thermistor:")[1][1:6]
                # print('thermistor Temperature: ' + thermistorTemp)

                # Humidity
                humidity = line.split('Humidity: ')[1][0:5]
                # print('Humidity: ' + humidity)

                # Temperature
                temperature = line.split(" Temperature: ")[1][0:5]
                # print('Temperature: ' + temperature)

                # Heat Index
                heatIndex = line.split(' Heat index: ')[1][0:5]
                # print('Heat Index: ' + heatIndex)

                # Moving Average
                movingAverageTemp = line.split(' Origin moving average: ')[1][0:5]
                # print('Moving Average: ' + movingAverageTemp)

                # Counter
                counter = line.split(' Counter: ')[1][0:20].split('\'')[0]
                # print('Counter: ' + counter)

                # Fetch Current Datetime
                now = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
                # print('Timestamp: ' + now)
                print('Extracted following values; temperature - ' + temperature + ' humidity - ' + humidity)
                try:
                    json_body = [
                        {
                            "measurement": "gistkast",
                            "tags": {
                                "host": "brewerury"
                            },
                            "time": now,
                            "fields": {
                                "counter": counter,
                                "thermistorTemp": thermistorTemp,
                                "temperature": temperature,
                                "heatIndex": heatIndex,
                                "humidity": humidity,
                                "movingAverageTemp": movingAverageTemp,

                            }
                        }
                    ]

                    client.write_points(json_body)

                    print('succesfully wrote measurements to influxDB')

                except:
                    print('failed to insert line: ')
                    print(now + ': ' + line)
                    pass

