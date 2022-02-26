from datetime import datetime
from influxdb import InfluxDBClient
from datetime import datetime, timedelta

influxhost = '192.168.2.24'
client = InfluxDBClient(host=influxhost, port=8086, username='brewmaster', password='Brewerury')
client.switch_database('brewerury')
json_body = [] 

print(client.get_list_database())

client.switch_database('brewerury')

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
                            "counter": 1.00,
                            "thermistorTemp": 18.10,
                            "temperature": 20.55,
                            "heatIndex": 18.31,
                            "humidity": 24.45,
                            "movingAverageTemp": 18.44,

                        }
                    }
                ]
    client.write_points(json_body)  
except:
    print(json_body)
