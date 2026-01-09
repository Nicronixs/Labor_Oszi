import serial
import pyqtgraph as pg
from pyqtgraph.Qt import QtCore, QtWidgets

PORT = "COM7"
BAUD = 115200

ser = serial.Serial(PORT, BAUD, timeout=0.01)  # kleiner Timeout verhindert Hänger

app = QtWidgets.QApplication([])

win = pg.GraphicsLayoutWidget(show=True, title="STM32 Live uint8_t Plot")
plot = win.addPlot(title="Kanal 1")
curve = plot.plot(pen='y')
plot.setYRange(0, 255)

data = []

def update():
    global data, ser
    try:
        n = ser.in_waiting  # ← kann Fehler werfen, daher im try-block
        if n > 0:
            raw = ser.read(n)
            data.extend(raw)

            if len(data) > 2000:
                data = data[-2000:]

        curve.setData(data)

    except serial.SerialException as e:
        print("⚠ SerialException:", e)
        # Optional: Auto-Reconnect
        try:
            ser.close()
        except:
            pass
        QtCore.QTimer.singleShot(1000, reconnect)

def reconnect():
    global ser
    try:
        ser = serial.Serial(PORT, BAUD, timeout=0.01)
        print("✔ COM-Port wieder verbunden")
    except:
        print("❌ Reconnect fehlgeschlagen, versuche erneut…")
        QtCore.QTimer.singleShot(1000, reconnect)

timer = QtCore.QTimer()
timer.timeout.connect(update)
timer.start(5)  # 5 ms statt 1 → stabiler

app.exec_()
