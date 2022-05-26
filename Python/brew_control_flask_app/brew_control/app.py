import sqlite3
from flask import Flask, render_template, request, url_for, flash, redirect
import os
from arduino_writer import rewrite_arduino_sketch
import subprocess
import hashlib

def get_db_connection():
    conn = sqlite3.connect('database.db')
    conn.row_factory = sqlite3.Row
    return conn

app = Flask(__name__, static_folder='static')
app.config['SECRET_KEY'] = '6819A8F8C296D6E0D3530C0E12CFE9DF8B4080819F3CA281186A0BD39093B8DB'

MagicHash = '20d1311c58b6e869daff7af9afc1e12a'

# @app.route('/')
# def hello_world():
#     # return "Hello World!"
#     return render_template("index.html")
#     # return """hoi"""

@app.route('/', methods=('GET', 'POST'))
def home():
    conn = get_db_connection()
    values = conn.execute('SELECT * FROM brew_parameters WHERE id = (SELECT MAX(id) FROM brew_parameters)').fetchone()
    arduino_power = conn.execute("SELECT * FROM arduino_power WHERE parameter_name = 'arduino_power' ").fetchone()
    conn.close()    
    success = ''

    if request.method == 'POST':
      if request.form['btn_identifier'] == 'powerbutton':
          if arduino_power['value'] == 'OFF':
            # Set new value
            conn = get_db_connection()
            conn.execute("UPDATE arduino_power SET value = 'ON' WHERE parameter_name = 'arduino_power'")
            conn.commit()
            conn.close() 
            #Fetch new value
            conn = get_db_connection()
            arduino_power = conn.execute("SELECT * FROM arduino_power WHERE parameter_name = 'arduino_power' ").fetchone()
            conn.close()  
            return render_template("index_brew.html", values=values, success=success, arduino_power=arduino_power)
          if arduino_power['value'] == 'ON':
            # Set new value
            conn = get_db_connection()
            conn.execute("UPDATE arduino_power SET value = 'OFF' WHERE parameter_name = 'arduino_power'")
            conn.commit()
            conn.close()        
            #Fetch new value
            conn = get_db_connection()
            arduino_power = conn.execute("SELECT * FROM arduino_power WHERE parameter_name = 'arduino_power' ").fetchone()
            conn.close()              
            return render_template("index_brew.html", values=values, success=success, arduino_power=arduino_power)
      elif request.form['btn_identifier'] == 'upload&compile':
        Temperature = request.form['Temperature']
        Tinterval = request.form['Tinterval']
        TDelay = request.form['TDelay']
        Magic = request.form['Magic']

        if not Temperature or not Tinterval or not TDelay or not Magic:
          success = 'All parameters are required!'
          return render_template("index_brew.html", values=values, success=success, arduino_power=arduino_power) 
        else:
          # First Check Magic
          if MagicHash == hashlib.md5(Magic.encode()).hexdigest():
            # Insert New Values
            conn = get_db_connection()
            conn.execute('INSERT INTO brew_parameters (brew_temperature, measure_interval, fan_cycle_delay) VALUES (?, ?, ?)',(Temperature, Tinterval, TDelay))
            conn.commit()
            conn.close()
            # Fetch recent values
            conn = get_db_connection()
            values = conn.execute('SELECT * FROM brew_parameters WHERE id = (SELECT MAX(id) FROM brew_parameters)').fetchone()
            conn.close() 
            success = 'Succesfully processed the new parameters'

            rewrite_arduino_sketch(Temperature, Tinterval, TDelay)
            # exit_code = subprocess.call('./compile_and_upload_arduino_sketch.sh')

            # upload = 'upload returned exit code: ' + str(exit_code)

            # return redirect(url_for('home'))
            return render_template("index_brew.html", values=values, success=success, arduino_power=arduino_power) #, upload=upload) 
            # flash('All parameters were processed correctly!') 
          else:
            success = 'What is the Magic word?'
            return render_template("index_brew.html", values=values, success=success, arduino_power=arduino_power) #, upload=upload) 

    return render_template("index_brew.html", values=values, arduino_power=arduino_power) 

if __name__ == '__main__':
    app.run(debug=True,host='0.0.0.0')
