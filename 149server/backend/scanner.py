import asyncio
from bleak import BleakScanner
from bleak import BleakClient

async def main():
    devices = await BleakScanner.discover()
    for d in devices:
        print(d)

asyncio.run(main())