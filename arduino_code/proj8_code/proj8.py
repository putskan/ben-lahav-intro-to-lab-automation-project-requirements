import threading
import queue
import serial
import FreeSimpleGUI as sg

def serial_worker(port, baudrate, write_queue, read_queue, stop_event):
    try:
        with serial.Serial(port, baudrate, timeout=1) as ser:
            while not stop_event.is_set():
                try:
                    # Write if there's a message
                    message = write_queue.get_nowait()
                    ser.write(message.encode())
                except queue.Empty:
                    pass
                # Always try to read
                if ser.in_waiting:
                    response = ser.readline().decode(errors='ignore').strip()
                    if response:
                        read_queue.put(response)
    except Exception as e:
        read_queue.put(f"Error: {e}")

# GUI layout
layout = [
    [sg.Text("Serial Port:"), sg.InputText(key="PORT", size=(20, 1), default_text="COM5")],
    [sg.Text("Baud Rate:"), sg.InputText(key="BAUDRATE", size=(20, 1), default_text="9600")],
    [sg.Text("LED Time (ms):"), sg.InputText(key="TIME", size=(20, 1))],
    [sg.Button("Send"), sg.Button("Exit")],
    [sg.Text("Response:"), sg.Multiline(size=(40, 10), key="RESPONSE", disabled=True)]
]

window = sg.Window("Arduino Serial Communication", layout)

write_queue = queue.Queue()
read_queue = queue.Queue()
stop_event = threading.Event()
serial_thread = None

while True:
    event, values = window.read(timeout=100)
    if event == sg.WINDOW_CLOSED or event == "Exit":
        stop_event.set()
        if serial_thread and serial_thread.is_alive():
            serial_thread.join()
        break
    elif event == "Send":
        port = values["PORT"]
        baudrate = int(values["BAUDRATE"])
        message = values["TIME"]
        # Start serial thread if not running
        if not serial_thread or not serial_thread.is_alive():
            stop_event.clear()
            serial_thread = threading.Thread(
                target=serial_worker,
                args=(port, baudrate, write_queue, read_queue, stop_event),
                daemon=True,
            )
            serial_thread.start()

        print('sending')
        write_queue.put(message)
        
    # Always check for responses from Arduino
    try:
        while True:
            response = read_queue.get_nowait()
            window["RESPONSE"].update(response + "\n", append=True)
    except queue.Empty:
        pass
