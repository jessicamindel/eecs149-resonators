from flask import Flask
from flask import request
from flask_cors import CORS
from backend/ble_utils import parse_ble_args
from bleak import BleakClient, BleakError

app = Flask(__name__)
CORS(app)

midi = None

ble = {
    "service": "314b2cb7-d379-474f-832f-6f833657e7e2",
    "start": "10f4c060-fdd1-49a5-898e-ab924709a558",
    "stop": "10f4c060-fdd1-49a5-898e-bb924709a558",
    "volume": "10f4c060-fdd1-49a5-898e-db924709a558",
    "tempo": "10f4c060-fdd1-49a5-898e-eb924709a558"
}

ports = {
    # Webserver (React): 3000
    # Webserver OSC send: 3001
    # Webserver OSC receive: 3002
    # Webserver (Flask): 5000
    "react": 3000,
    "osc_send": 3001,
    "osc_receive": 3002,
    "flask": 5000
}

async def send(address, uuid, data):
    print(f"searching for device {address} ({timeout}s timeout)")
    try:
        async with BleakClient(address, timeout = timeout) as client:
            try:
                await client.write_gatt_char(uuid, bytes(data, 'utf-8'))
            except Exception as e:
                print(f"\t{e}")
    except BleakError as e:
        print("not found")

@app.route("/", methods=['GET', 'POST'])
def hello_world():
    if request.method == 'POST':
        return "<img src='https://miro.medium.com/focal/87/87/50/50/1*XBktsimICRx2AbHA3vRNYQ.jpeg' />"
    return "Successfully connected to backend."

@app.route('/tempo/<int:tempo>', methods=['POST'])
def send_tempo(tempo):
    return f'Tempo sent: {tempo}'

@app.route('/midi', methods=['POST'])
def send_midi():
    global midi
    midi = request.data         # The contents of the MIDI file; not a file object.
    # client.write_gatt_char()
    return f'MIDI sent.'

@app.route('/play', methods=['POST'])
def play():
    global midi
    if midi is None:
        return "Can't play."
    # client.write_gatt_char(ble["start"])
    return f'Playing.'

@app.route('/stop', methods=['POST'])
def stop():
    # client.write_gatt_char(ble["stop"])
    return f'Stopping.'