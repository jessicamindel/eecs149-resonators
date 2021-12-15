from flask import Flask
from flask import request
from flask_cors import CORS
import asyncio
import sys
import serial
import time
from getpass import getpass
from threading import Thread
from pythonosc.udp_client import SimpleUDPClient
from pythonosc.osc_server import BlockingOSCUDPServer
from pythonosc.dispatcher import Dispatcher
from backend.thread_utils import launch_thread
import subprocess


#from . import _winrt
#_winrt.init_apartment(_winrt.MTA) #keeping these two lines here because something is sus about windows

app = Flask(__name__)
CORS(app)

midi = None
devices = []
proc = None

ble = {
    "service": "314b2cb7-d379-474f-832f-6f833657e7e2",
    "start": "10f4c060-fdd1-49a5-898e-ab924709a558",
    "stop": "10f4c060-fdd1-49a5-898e-bb924709a558",
    "volume": "10f4c060-fdd1-49a5-898e-db924709a558",
    "tempo": "10f4c060-fdd1-49a5-898e-eb924709a558",
}

ip = "127.0.0.1"

ports = {
    # Webserver (React): 3000
    # Webserver OSC send: 3001
    # Webserver OSC receive: 3003 (temp modification, may need to change again)
    # Webserver (Flask): 5000
    "react": 3000,
    "osc_send": 3001,
    "osc_receive": 3003,
    "flask": 5000,
}

# TODO: SET PORT HERE
serialport = 'COM5'
baudrate = 115200

ser = serial.Serial()
ser.baudrate = baudrate
ser.port = serialport
ser.open()

# async def connect_to_device(address):
#     global devices
#     global clients
#     async with BleakClient(address, timeout=60.0, use_cached=False) as client:
#         print(f"Connected: {address} {client.is_connected}")
#         clients += [client] #adds this client into the list so we can send to all simultaneously
#         while True:
#             await asyncio.sleep(1.0) #this keeps it connected hopefully forever

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

def receive_osc_input(address, *args):
    print("Received OSC: " + address + " " + " ".join(args))
    if (address == "/resume"):
        play(send_osc=False)
    elif (address == "/pause"):
        stop(send_osc=False)
    # Ignore all other messages

def read_serial():
    line = ser.read()
    return line

osc_out = SimpleUDPClient(ip, ports['osc_send'])
osc_dispatcher = Dispatcher()
osc_dispatcher.map('/*', receive_osc_input)
osc_in = BlockingOSCUDPServer((ip, ports['osc_receive']), osc_dispatcher)
osc_in_stop_listening = launch_thread(lambda: osc_in.handle_request())
# TODO: Call this function at some point. Alternatively, don't run this in a 
# separate thread; this might not work with bluetooth because it's on the main
# thread, but unsure (can't test from where I am).

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
    #loop = asyncio.get_event_loop()
    global devices
    global proc

    proc = subprocess.Popen("python bleakout.py", stdin=subprocess.PIPE,shell=True)
    return f'Connected to devices.'

@app.route('/play', methods=['POST'])
def play(send_osc=True):
    global midi
    global proc
    if midi is None:
       return "Can't play."
    #currently no tick seek, just gonna write it at 0
    if send_osc: osc_out.send_message('/resume', 1)
    outs, errs = proc.communicate(input = bytes('0 0\n', "utf-8"))
    return f'Playing.'

@app.route('/stop', methods=['POST'])
def stop(send_osc=True):
    global proc
    # client.write_gatt_char(ble["stop"])
    if send_osc: osc_out.send_message('/pause', 1)
    outs, errs = proc.communicate(input = bytes('1 0\n', "utf-8"))
    return f'Stopping.'

@app.route('/incvol', methods=['POST'])
def incvol():
    global proc
    outs, errs = proc.communicate(input = bytes('2 1\n', "utf-8"))
    return f'Volume increased.'

@app.route('/decvol', methods=['POST'])
def decvol():
    global proc
    outs, errs = proc.communicate(input = bytes('2 0\n', "utf-8"))
    return f'Volume decreased.'

@app.route('/inctempo', methods=['POST'])
def inctempo():
    global proc
    outs, errs = proc.communicate(input = bytes('3 1\n', "utf-8"))
    return f'Increasing tempo.'

@app.route('/dectempo', methods=['POST'])
def dectempo():
    global proc
    outs, errs = proc.communicate(input = bytes('3 0\n', "utf-8"))
    return f'Decreasing tempo.'

@app.route('/midi', methods=['POST'])
def send_midi():
    global midi
    midi = request.data         # The contents of the MIDI file; not a file object.
    # client.write_gatt_char()
    # NOTE: No longer sending this over OSC because file must be manually changed
    # in Ableton Live. Not the cleanest, but given that we're not supporting changing
    # the MIDI file in realtime over bluetooth anymore, this is fine.
    return f'MIDI sent.'

def query_cereal():
    while True:
        print("querying")
        action = read_serial()
        if action == 0:
            decvol()
            dectempo()
        elif action == 1:
            decvol()
        elif action == 2:
            decvol()
            inctempo()
        elif action == 3:
            dectempo()
        elif action == 5:
            inctempo()
        elif action == 6:
            incvol()
            dectempo()
        elif action == 7:
            incvol()
        elif action == 8:
            incvol()
            incTempo()
        time.sleep(1)
        if action != 4:
            return f'adjusting'
        return f'no adjust'
    

# # TODO TEMP READ LOOP
# while 1:
#     print(read_serial())


