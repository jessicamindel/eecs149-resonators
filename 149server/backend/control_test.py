import asyncio
import sys
from getpass import getpass

from bleak import BleakClient, BleakError

from ble_utils import parse_ble_args, handle_sigint
args = parse_ble_args('Print advertisement data from a BLE device')
addr = args.addr.lower()
timeout = args.timeout
handle_sigint()

SERVICE_UUID = "314b2cb7-d379-474f-832f-6f833657e7e2"
START_UUID = "10f4c060-fdd1-49a5-898e-ab924709a558"
STOP_UUID = "10f4c060-fdd1-49a5-898e-bb924709a558"
VOL_UUID = "10f4c060-fdd1-49a5-898e-db924709a558"
TEMPO_UUID = "10f4c060-fdd1-49a5-898e-eb924709a558"

async def main(address):
    print(f"searching for device {address} ({timeout}s timeout)")
    try:
        async with BleakClient(address,timeout=timeout) as client:
            print("connected")
            try:
                print("Type message and send with Enter key")
                while True:
                    display = input("")
                    await client.write_gatt_char(START_UUID, bytes(display, 'utf-8'))
            except Exception as e:
                print(f"\t{e}")
    except BleakError as e:
        print("not found")

if __name__ == "__main__":
    while True:
        asyncio.run(main(addr))