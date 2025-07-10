import threading
import queue
import serial
import FreeSimpleGUI as sg
import matplotlib.pyplot as plt
import matplotlib
import csv
import time
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg

matplotlib.use("TkAgg")

def parse_arduino_line(line):
    # Example: "timeElapsed=107642.00;angle=87.30;buzzerOn=1"
    try:
        parts = line.strip().split(';')
        data = {}
        for part in parts:
            if '=' in part:
                k, v = part.split('=')
                data[k.strip()] = v.strip()
        return float(data['timeElapsed']), float(data['angle']), int(data['buzzerOn'])
    except Exception:
        return None

def serial_worker(port, baudrate, read_queue, stop_event):
    try:
        with serial.Serial(port, baudrate, timeout=1) as ser:
            while not stop_event.is_set():
                if ser.in_waiting:
                    line = ser.readline().decode(errors='ignore').strip()
                    if line:
                        read_queue.put(line)
    except Exception as e:
        read_queue.put(f"Error: {e}")

def draw_compass(angle, buzzer_on):
    fig, ax = plt.subplots(figsize=(2.5,2.5))
    ax.set_xlim(-1.2, 1.2)
    ax.set_ylim(-1.2, 1.2)
    ax.axis('off')
    # Draw circle
    circle = plt.Circle((0,0), 1, fill=False, linewidth=2)
    ax.add_artist(circle)
    # Draw needle
    x = 0.9 * np.sin(np.radians(angle))
    y = 0.9 * np.cos(np.radians(angle))
    ax.arrow(0, 0, x, y, head_width=0.1, head_length=0.1, fc='r', ec='r')
    # Draw center
    ax.plot(0, 0, 'ko')
    # Draw buzzer LED
    led_color = 'red' if buzzer_on else 'green'
    led = plt.Circle((0, -1.1), 0.08, color=led_color)
    ax.add_artist(led)
    ax.text(0, -1.25, "Buzzer", ha='center', va='center', fontsize=10)
    return fig

import numpy as np

# --- CSV Logging ---
csv_file = "log.csv"
csv_header = ["timeElapsed", "angle", "buzzerOn"]
csv_fp = open(csv_file, "w", newline="")
csv_writer = csv.writer(csv_fp)
csv_writer.writerow(csv_header)

# --- GUI Layout ---
layout = [
    [sg.Text("Serial Port:"), sg.InputText("COM5", key="PORT", size=(10,1)),
     sg.Text("Baud Rate:"), sg.InputText("9600", key="BAUDRATE", size=(8,1)),
     sg.Button("Connect"), sg.Button("Exit")],
    [sg.Canvas(key="CANVAS")],
    [sg.Text("Raw Data:"), sg.Multiline(size=(50,5), key="RESPONSE", disabled=True)]
]

window = sg.Window("Arduino Angle Logger", layout, finalize=True)

# --- Matplotlib Canvas Setup ---
fig = draw_compass(0, 0)
canvas_elem = window["CANVAS"].TKCanvas
fig_canvas_agg = FigureCanvasTkAgg(fig, master=canvas_elem)
fig_canvas_agg.draw()
fig_canvas_agg.get_tk_widget().pack(side='top', fill='both', expand=1)

# --- Threading Setup ---
read_queue = queue.Queue()
stop_event = threading.Event()
serial_thread = None

while True:
    event, values = window.read(timeout=100)
    if event in (sg.WINDOW_CLOSED, "Exit"):
        stop_event.set()
        if serial_thread and serial_thread.is_alive():
            serial_thread.join()
        break
    elif event == "Connect":

        print('connecting')
        port = values["PORT"]
        baudrate = int(values["BAUDRATE"])
        if not serial_thread or not serial_thread.is_alive():
            stop_event.clear()
            serial_thread = threading.Thread(
                target=serial_worker,
                args=(port, baudrate, read_queue, stop_event),
                daemon=True,
            )
            serial_thread.start()

    # Handle incoming serial data
    try:
        while True:
            line = read_queue.get_nowait()
            print(line)
            window["RESPONSE"].update(line + "\n", append=True)
            parsed = parse_arduino_line(line)
            if parsed:
                t, angle, buzzer = parsed
                # Log to CSV
                csv_writer.writerow([t, angle, buzzer])
                csv_fp.flush()
                # Update compass
                plt.close(fig)
                fig = draw_compass(angle, buzzer)
                fig_canvas_agg.figure = fig
                fig_canvas_agg.draw()
    except queue.Empty:
        pass

csv_fp.close()
