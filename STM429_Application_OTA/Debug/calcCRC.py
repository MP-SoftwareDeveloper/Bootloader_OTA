import zlib
# In PC side for application binary file and be writen in application Header in build time it will become part of firmware image
APP_BIN = "application.bin"

with open(APP_BIN, "rb") as f:
	data = f.read()
	
crc = zlib.crc32(data) & 0xFFFFFFFF

print(f"CRC32 = 0x{crc:08X}")
print(f"Size = {len(data)} bytes")

input("Press Enter to exit...")