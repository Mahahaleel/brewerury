#!/usr/bin/env python3
import serial
from datetime import datetime
from influxdb import InfluxDBClient
from datetime import datetime, timedelta
import json

influxhost = '192.168.2.4'
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
    
    # Set line-counter 
    line_counter = 0

    # Set empty variables
    thermistorTemp  = ""
    humidity = ""
    temperature = ""
    heatIndex =""
    movingAverageTemp = ""
    counter = ""

    while True:
        # try:
            if ser.in_waiting > 0:
                line = ser.readline().decode('utf-8').rstrip()
                print(line)
                print(type(line))

                if "Temperature from thermistor: " in line:
                    line_counter = 0

                    thermistorTemp = line.split("Temperature from thermistor: ")[1][0:5].replace("°","")
                    humidity = line.split('Humidity: ')[1][0:5]
                    temperature = line.split(" Temperature: ")[1][0:5].replace("°","")
                    heatIndex = line.split(' Heat index: ')[1][0:5].replace("°","")
                    

                    line_counter += 1
                

                if "Moving average Check: " in line:
                    movingAverageTemp = line.split(' Origin moving average: ')[1][0:5].replace("°","")

                    line_counter += 1
                

                if "Writing to file..." in line:
                    counter = line.split(' Counter: ')[1][0:20].split('\'')[0]

                    line_counter +=1
                

                if line_counter == 3: 
                    now = datetime.now().strftime('%Y-%m-%d %H:%M:%S')

                    try:
                        json_body = [
                            {
                                "measurement": "gistkast",
                                "tags": {
                                    "host": "brewerury"
                                },
                                "time": now,
                                "fields": {
                                    "counter": float(counter),
                                    "thermistorTemp": float(thermistorTemp),
                                    "temperature": float(temperature),
                                    "heatIndex": float(heatIndex),
                                    "humidity": float(humidity),
                                    "movingAverageTemp": float(movingAverageTemp)
                                }
                            }
                        ]

                        client.write_points(json_body)

                        print('succesfully wrote measurements to influxDB')

                        line_counter = 0

                    except Exception as e:
                        print('failed to insert line: ')
                        print('time = ' + now + ' of type: ' + str(type(now)))
                        print('counter = ' + counter + ' of type: ' + str(type(counter)))
                        print('thermistorTemp = ' + thermistorTemp + ' of type: ' + str(type(thermistorTemp)))
                        print('temperature = ' + temperature + ' of type: ' + str(type(temperature)))
                        print('heatIndex = ' + heatIndex + ' of type: ' + str(type(heatIndex)))
                        print('humidity = ' + humidity + ' of type: ' + str(type(humidity)))
                        print('movingAverageTemp = ' + movingAverageTemp + ' of type: ' + str(type(movingAverageTemp)))

                        print('because of error: ')
                        print(str(e))


                        pass
            



                # thermistorTemp = line.split("Temperature thermistor:")[1][1:6]
                # # print('thermistor Temperature: ' + thermistorTemp)

                # # Humidity
                # humidity = line.split('Humidity: ')[1][0:5]
                # # print('Humidity: ' + humidity)

                # # Temperature
                # temperature = line.split(" Temperature: ")[1][0:5]
                # # print('Temperature: ' + temperature)

                # # Heat Index
                # heatIndex = line.split(' Heat index: ')[1][0:5]
                # # print('Heat Index: ' + heatIndex)

                # # Moving Average
                # movingAverageTemp = line.split(' Origin moving average: ')[1][0:5]
                # # print('Moving Average: ' + movingAverageTemp)

                # # Counter
                # counter = line.split(' Counter: ')[1][0:20].split('\'')[0]
                # # print('Counter: ' + counter)

                # # Fetch Current Datetime
                # now = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
                # # print('Timestamp: ' + now)
                # print('Extracted following values; temperature - ' + temperature + ' humidity - ' + humidity)
                # try:
                #     json_body = [
                #         {
                #             "measurement": "gistkast",
                #             "tags": {
                #                 "host": "brewerury"
                #             },
                #             "time": now,
                #             "fields": {
                #                 "counter": counter,
                #                 "thermistorTemp": thermistorTemp,
                #                 "temperature": temperature,
                #                 "heatIndex": heatIndex,
                #                 "humidity": humidity,
                #                 "movingAverageTemp": movingAverageTemp,

                #             }
                #         }
                #     ]

                #     client.write_points(json_body)

                #     print('succesfully wrote measurements to influxDB')

                # except:
                #     print('failed to insert line: ')
                #     print(now + ': ' + line)
                #     pass


