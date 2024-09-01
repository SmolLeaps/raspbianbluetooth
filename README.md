# raspbianmonitor
bluetooth classic/BLE collection of code snippets + keyboard emulation via C 

**btferret modification**
```
//on rpi: modify keyboard.txt to include RPi-specific MAC address
gcc btlib.c keyboard.c -o keyboard
sudo ./keyboard
```

**Libusb keyboard emulator**
```
g++ oma.c `pkg-config --libs --cflags libusb-1.0`
./a.out
```
note: `sudo ./a.out` if `LIBUSB_ERROR_ACCESS`

**Bluetooth classic server example using BlueZ software stack + DBus (IPC method)**
```
cd dbus_libbluetooth
make all
sudo ./bluetooth
```
note: BlueZ software stack is linux-specific stack(API+values) of developing bluetooth applications. 

**dependencies**
g++ (C compiler, preinstalled on Raspbian OS)
```
sudo apt-get install libusb (C library for writing drivers for USB devices)
sudo apt-get install bluez
sudo apt-get install libdbus-1-dev
```

**connection**
```
//on rpi: ngrok tcp 22
ssh -p [dynamic port no on ngrok instance running on rpi] 0.tcp.ap.ngrok.io
```

**useful terminal commands**
see VID, PID and Keyboard Manufacturer Name
```
lsusb
```
bluetooth(both LE and classic) pairing via commands 
```
bluetoothctl
power on
discoverable on
scan on
exit
```