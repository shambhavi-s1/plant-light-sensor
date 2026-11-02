# Plant Light Sensor 🌱

An Arduino light monitor that tells you whether your plant is getting the right amount of light.

You type in the name of a plant. A Python script looks up how much sunlight that plant needs using the [Perenual plant API](https://perenual.com/docs/api) and sends the ideal light range to the Arduino. The Arduino reads a light sensor and shows the current reading on a small OLED screen, along with one of:

- **Too dark!**
- **Too bright!**
- **IDEAL**

## How it works

1. `index.py` asks for a plant name and searches the Perenual API for it.
2. It reads the plant's sunlight needs (e.g. "full sun", "part shade") and converts them into a numeric light range.
3. It sends that range to the Arduino over USB serial as `min,max`.
4. `arduino_light_sensor.ino` stores the new range, keeps reading the light sensor, and updates the display.

If the plant isn't found or has no sunlight data, a default range is used.

| Sunlight condition    | Range used |
|-----------------------|------------|
| full shade            | 50–200     |
| part shade            | 200–400    |
| part sun/part shade   | 400–600    |
| full sun              | 600–1000   |

> **Note:** these ranges are compared against the Arduino's raw analog reading (0–1023), not a calibrated lux measurement, so they're approximate.

## Hardware

- Arduino board with an analog pin `A6` (e.g. Arduino Nano)
- Light sensor (e.g. photoresistor module) connected to `A6`
- 128×64 SSD1306 OLED display (I2C)
- USB cable to connect the Arduino to your computer

## Setup

### 1. Arduino

1. Install the **U8g2** library in the Arduino IDE (Tools → Manage Libraries → search "U8g2").
2. Open `arduino_light_sensor/arduino_light_sensor.ino` and upload it to your board.

### 2. Python

Install the dependencies:

```bash
pip install requests pyserial
```

Get a free API key from [Perenual](https://perenual.com/docs/api), then create a file called `.env` in the project folder:

```
PERENUAL_KEY=your_api_key_here
```

The `.env` file is listed in `.gitignore`, so your key won't be uploaded to GitHub.

### 3. Serial port

`index.py` connects to `/dev/cu.usbserial-0001`. Your port may be different. To find it, look under Tools → Port in the Arduino IDE, then change this line in `index.py` to match:

```python
arduino = serial.Serial('/dev/cu.usbserial-0001', 9600, timeout=1)
```

Close the Arduino IDE's Serial Monitor before running the script, because only one program can use the port at a time.

## Usage

```bash
python index.py
```

```
Enter plant name: monstera
['part shade']
monstera: part shade (200-400 lux)
Sent to Arduino: 200-400 lux
```

The OLED briefly shows the new range, then goes back to the live reading.

## Project structure

```
plant_light_sensor/
├── arduino_light_sensor/
│   └── arduino_light_sensor.ino   # Arduino code: reads sensor, drives display
├── index.py                       # Python: plant lookup + sends range to Arduino
├── .env                           # your API key (not committed)
└── .gitignore
```
