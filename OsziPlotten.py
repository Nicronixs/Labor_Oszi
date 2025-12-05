import serial
import pyqtgraph as pg
from pyqtgraph.Qt import QtGui, QtCore

PORT = "COM3"
BAUD = 115200

ser = serial.Serial(PORT, BAUD)

app = QtGui.QApplication([])
win = pg.GraphicsLayoutWidget(show=True, title="STM32 Live Plot")
plot = win.addPlot(title="Signal")
curve = plot.plot()

data = []

def update():
    global data
    if ser.in_waiting:
        line = ser.readline().decode().strip()
        try:
            value = float(line)
            data.append(value)
            if len(data) > 2000:
                data = data[-2000:]   # nur letztes Fenster anzeigen
            curve.setData(data)
        except:
            pass

timer = QtCore.QTimer()
timer.timeout.connect(update)
timer.start(1)

QtGui.QApplication.instance().exec_()
