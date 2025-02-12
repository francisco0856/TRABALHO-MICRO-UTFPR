import tkinter as tk
from tkinter import ttk
import serial
import threading
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import time

# Configurações da porta serial
PORT = 'COM3'
BAUD_RATE = 230400
TIMEOUT = 0
MAX_VALUE = 2100
MIN_VALUE = 2000
BUFFER_SIZE = 2048
UPDATE_INTERVAL_MS = 16  # Atualização do gráfico (~60 Hz)

# Inicializa a porta serial
ser = None
try:
    ser = serial.Serial(PORT, BAUD_RATE, timeout=TIMEOUT)
    print(f"Conectado à porta {PORT}")
except serial.SerialException as e:
    print(f"Erro ao abrir a porta serial: {e}")

# Buffer para armazenar valores lidos da serial
value_buffer = [0] * BUFFER_SIZE
serial_lock = threading.Lock()
stop_thread = threading.Event()  # Para encerrar a thread com segurança


def read_serial():
    """Lê dados da porta serial em uma thread separada."""
    global value_buffer
    if ser is None:
        return

    buffer = ""
    while not stop_thread.is_set():
        try:
            if ser.in_waiting > 0:
                data = ser.read(ser.in_waiting).decode('utf-8', errors='ignore')
                buffer += data

                while ';' in buffer:
                    with serial_lock:
                        value, buffer = buffer.split(';', 1)
                        value = value.strip()

                        if value.isdigit():
                            value = int(value)
                            if MIN_VALUE <= value <= MAX_VALUE:
                                value_buffer.append(value)
                                if len(value_buffer) > BUFFER_SIZE:
                                    value_buffer.pop(0)

            time.sleep(0.001)  # Pequeno delay para reduzir uso de CPU
        except Exception as e:
            print(f"Erro na leitura serial: {e}")
            break


class RealTimePlotApp:
    """Interface gráfica para exibir um gráfico de dados em tempo real."""
    def __init__(self, root):
        self.root = root
        self.root.title("Gráfico de Amostras UART em Tempo Real")

        # Criar figura do matplotlib
        self.fig, self.ax = plt.subplots(figsize=(8, 4))
        self.line, = self.ax.plot(range(BUFFER_SIZE), value_buffer, 'b-', lw=1)
        self.ax.set_ylim(MIN_VALUE - 10, MAX_VALUE + 10)
        self.ax.set_xlim(0, BUFFER_SIZE)
        self.ax.set_title("Gráfico em Tempo Real")
        self.ax.set_xlabel("Amostras")
        self.ax.set_ylabel("Valores")
        self.ax.grid(True, linestyle="--", linewidth=0.5)

        # Integrar o gráfico ao Tkinter
        self.canvas = FigureCanvasTkAgg(self.fig, master=self.root)
        self.canvas.get_tk_widget().pack(side=tk.TOP, fill=tk.BOTH, expand=True)

        # Botão para fechar a aplicação
        self.quit_button = ttk.Button(self.root, text="Sair", command=self.quit_app)
        self.quit_button.pack(side=tk.BOTTOM, pady=10)

        # Atualizar o gráfico periodicamente
        self.update_plot()

    def update_plot(self):
        """Atualiza o gráfico com os novos valores lidos da serial."""
        with serial_lock:
            self.line.set_ydata(value_buffer)  # Atualiza os valores do gráfico sem recriar a figura
        self.canvas.draw()
        self.root.after(UPDATE_INTERVAL_MS, self.update_plot)

    def quit_app(self):
        """Finaliza o programa de maneira segura."""
        global ser
        print("Encerrando programa...")
        stop_thread.set()  # Sinaliza para encerrar a thread

        if ser and ser.is_open:
            ser.close()
            print("Porta serial fechada.")

        self.root.quit()
        self.root.destroy()
# Criar a thread de leitura da serial
if ser is not None:
    serial_thread = threading.Thread(target=read_serial, daemon=True)
    serial_thread.start()

# Criar a interface gráfica
root = tk.Tk()
app = RealTimePlotApp(root)
root.mainloop()

# Finalizar a thread ao encerrar o Tkinter
stop_thread.set()
if ser and ser.is_open:
    ser.close()
    print("Porta serial fechada.")
