import asyncio

from bleak import BleakClient, BleakError

serviceUUID = "314b2cb7-d379-474f-832f-6f833657e7e2"
START_UUID = "10f4c060-fdd1-49a5-898e-ab924709a558"
STOP_UUID = "10f4c060-fdd1-49a5-898e-bb924709a558"
VOL_UUID = "10f4c060-fdd1-49a5-898e-db924709a558"
TEMPO_UUID = "10f4c060-fdd1-49a5-898e-eb924709a558"

async def connect_to_device(address):
    print(f"searching for device {address} (60s timeout)")
    async with BleakClient(address,timeout=60.0) as client:
        print("connected")
        try:
            print("Type message and send with Enter key")
            while True:
                display = input("")
                if display[0] == "0":
                    await client.write_gatt_char(START_UUID, bytes(display[2:], 'utf-8'))
                if display[0] == "1":
                    await client.write_gatt_char(STOP_UUID, bytes(display[2:], 'utf-8'))
                if display[0] == "2":
                    await client.write_gatt_char(VOL_UUID, bytes(display[2:], 'utf-8'))
                if display[0] == "3":
                    await client.write_gatt_char(TEMPO_UUID, bytes(display[2:], 'utf-8'))
        except Exception as e:
            print(f"\t{e}")


async def main(addresses):
    await asyncio.gather(*(connect_to_device(address) for address in addresses))


if __name__ == "__main__":
    asyncio.run(
        main(
            [
                "DC:A6:32:72:88:9B",
                "DC:A6:32:72:88:9A",
            ]
        )
    )