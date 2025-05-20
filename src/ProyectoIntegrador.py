import sys
from PyQt5 import uic, QtWidgets
from PyQt5.QtCore import QTimer
import serial

qtCreatorFile = "../Interfaz/ProyectoIntegrador.ui"
Ui_MainWindow, QtBaseClass = uic.loadUiType(qtCreatorFile)

class MyMainWindow(QtWidgets.QMainWindow, Ui_MainWindow):
    def __init__(self):
        super().__init__()
        self.setupUi(self)

        self.sensor_temp_activado = False
        self.sensor_ultra_activado = False
        self.arduino = None

        try:
            self.arduino = serial.Serial('COM3', baudrate=9600, timeout=1)
            print("Conectado a COM3")
        except Exception as e:
            print(f"No se pudo conectar al Arduino: {e}")


        self.btn_temp.clicked.connect(self.toggle_sensor_temp)
        self.btn_ultra.clicked.connect(self.toggle_sensor_ultra)


        self.timer = QTimer()
        self.timer.timeout.connect(self.leer_serial)
        self.timer.start(500)

    def toggle_sensor_temp(self):
        if self.arduino:
            if not self.sensor_temp_activado:
                self.arduino.write(b'T1\n')
                self.btn_temp.setText("Desactivar Temperatura")
            else:
                self.arduino.write(b'T0\n')
                self.arduino.write(b'B0\n')  # 🔇 Apagar buzzer
                self.btn_temp.setText("Activar Temperatura")
            self.sensor_temp_activado = not self.sensor_temp_activado

    def toggle_sensor_ultra(self):
        if self.arduino:
            if not self.sensor_ultra_activado:
                self.arduino.write(b'U1\n')
                self.btn_ultra.setText("Desactivar Ultrasonido")
            else:
                self.arduino.write(b'U0\n')
                self.arduino.write(b'B0\n')  # 🔇 Apagar buzzer
                self.btn_ultra.setText("Activar Ultrasonido")
            self.sensor_ultra_activado = not self.sensor_ultra_activado

    def leer_serial(self):
        if self.arduino and self.arduino.inWaiting():
            try:
                linea = self.arduino.readline().decode().strip()
                if linea.startswith("TEMP:"):
                    valor = linea.replace("TEMP:", "")
                    temperatura_real = float(valor) / 10
                    self.temperatura.setText(f"{temperatura_real} °C")
                elif linea.startswith("DIST:"):
                    valor = linea.replace("DIST:", "")
                    self.distancia.setText(f"{valor} cm")
            except:
                pass

if __name__ == "__main__":
    app = QtWidgets.QApplication(sys.argv)
    window = MyMainWindow()
    window.show()
    sys.exit(app.exec_())