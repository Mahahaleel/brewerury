import sqlite3

connection = sqlite3.connect('database.db')


with open('schema.sql') as f:
    connection.executescript(f.read())

cur = connection.cursor()

cur.execute("INSERT INTO brew_parameters (brew_temperature, measure_interval, fan_cycle_delay) VALUES (?, ?, ?)",
            (20.0, 5000, 20)
            )

cur.execute("INSERT INTO arduino_power (parameter_name, description, value) VALUES (?, ?, ?)",
            ('arduino_power', 'Is the Arduino turned on or off', 'ON')
            )            

connection.commit()
connection.close()
