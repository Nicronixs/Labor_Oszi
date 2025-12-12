import serial
import pyqtgraph as pg
from pyqtgraph.Qt import QtCore, QtWidgets  # QtWidgets statt QtGui

PORT = "COM7"      # anpassen
BAUD = 115200

ser = serial.Serial(PORT, BAUD)

# QApplication verwenden
app = QtWidgets.QApplication([])

win = pg.GraphicsLayoutWidget(show=True, title="STM32 Live uint8_t Plot")
plot = win.addPlot(title="Kanal 1")
curve = plot.plot(pen='y')  # gelbe Linie
plot.setYRange(0, 255)

data = []

def update():
    global data
    while ser.in_waiting:
        value = ser.read(1)[0]
        data.append(value)

    if len(data) > 2000:
        data = data[-2000:]

    curve.setData(data)

timer = QtCore.QTimer()
timer.timeout.connect(update)
timer.start(1)

app.exec_()   # QtWidgets verwendet exec_() statt exec()
