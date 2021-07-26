from flask import Flask, render_template
import datetime
import threading
import serial

data = ""
dict = {}
def listen():
    ser = serial.Serial('/dev/ttyACM0', 9600)
    print("Starting listen function")
    while True:
        read_serial = ser.readline().decode()
        data = read_serial.split('\n')[0]
        location = data.split(', ')[0]
        temp = data.split(', ')[1]
        dict[location] = temp

app = Flask(__name__)

@app.route("/")
def index():
    print("Starting index function")
    now = datetime.datetime.now()
    timeString = now.strftime("%m/%d/%Y %H:%M")
    templateData = {
        'title' : 'LOLIOT',
        'data' : dict,
        'time' : timeString
    }
    return render_template('index.html', **templateData)

if __name__ == "__main__":
    radio_thread = threading.Thread(target=listen, daemon=True)
    radio_thread.start()
app.run(debug=True, port=80, host='0.0.0.0')
