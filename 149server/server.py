from flask import Flask
from flask import request
from flask_cors import CORS
#from backend/ble_utils import parse_ble_args    don't think we need to use here - Jet
from bleak import BleakClient, BleakError


#from . import _winrt
#_winrt.init_apartment(_winrt.MTA) #keeping these two lines here because something is sus about windows

app = Flask(__name__)
CORS(app)

midi = None
devices = []
clients = []

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

async def connect_to_device(address):
    global devices
    global clients
    async with BleakClient(address, timeout=60.0, use_cached=False) as client:
        print(f"Connected: {address} {client.is_connected}")
        clients += [client] #adds this client into the list so we can send to all simultaneously
        while True:
            await asyncio.sleep(1.0) #this keeps it connected hopefully forever

# async def send(address, uuid, data):
#     print(f"searching for device {address} ({timeout}s timeout)")
#     try:
#         async with BleakClient(address, timeout = timeout) as client:
#             print("Successfully connected to device with address: " + str(address))
#             try:
#                 await client.write_gatt_char(uuid, bytes(data, 'utf-8'))
#             except Exception as e:
#                 print(f"\t{e}")
#     except BleakError as e:
#         print("not found")

@app.route("/", methods=['GET', 'POST'])
def hello_world():
    if request.method == 'POST':
        return "<img src='https://miro.medium.com/focal/87/87/50/50/1*XBktsimICRx2AbHA3vRNYQ.jpeg' />"
    return "Successfully connected to backend."

#adds the individual addresses to the queue list. This is so we can connect to them concurrently.
@app.route('/add/<string:address>', methods=['POST'])
def add(address):
    global devices
    devices += [address]
    return f'Added address: {address}'

#connects to all devices simultaneously
@app.route('/connect', methods=['POST'])
def connect():
    global devices
    asyncio.gather(*(connect_to_device(address) for address in devices))
    return f'Connected to devices.'

@app.route('/play', methods=['POST'])
def play():
    global clients
    global midi
    if midi is None:
        return "Can't play."
    #currently no tick seek, just gonna write it at 0
    asyncio.gather(*(client.write_gatt_char("10f4c060-fdd1-49a5-898e-ab924709a558", bytes("0", 'utf-8')) for client in clients))
    return f'Playing.'

@app.route('/stop', methods=['POST'])
def stop():
    # client.write_gatt_char(ble["stop"])
    asyncio.gather(*(client.write_gatt_char("10f4c060-fdd1-49a5-898e-bb924709a558", bytes("0", 'utf-8')) for client in clients))
    return f'Stopping.'

@app.route('/vol/<float:vol>', methods=['POST'])
def send_vol(vol):
    global clients
    asyncio.gather(*(client.write_gatt_char("10f4c060-fdd1-49a5-898e-db924709a558", bytes(vol, 'utf-8')) for client in clients))
    return f'Vol sent: {vol}'

@app.route('/tempo/<int:tempo>', methods=['POST'])
def send_tempo(tempo):
    global clients
    asyncio.gather(*(client.write_gatt_char("10f4c060-fdd1-49a5-898e-eb924709a558", bytes(tempo, 'utf-8')) for client in clients))
    return f'Tempo sent: {tempo}'

@app.route('/midi', methods=['POST'])
def send_midi():
    global midi
    midi = request.data         # The contents of the MIDI file; not a file object.
    # client.write_gatt_char()
    return f'MIDI sent.'

