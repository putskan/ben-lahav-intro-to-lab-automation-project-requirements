import time
import threading
from pymata4 import pymata4
import tkinter as tk
from tkinter import messagebox

# --- Constants ---
BUTTON_PIN = 6
LED_PIN = 4
LIGHT_TIME_SEC = 2  # default, can be changed via GUI

# --- Hardware Setup ---
board = pymata4.Pymata4(com_port="COM5")
board.set_pin_mode_digital_output(LED_PIN)
board.set_pin_mode_digital_input_pullup(BUTTON_PIN)


# --- GUI Setup ---
class App:
    def __init__(self, root):
        self.root = root
        self.root.title("Button and LED Controller")
        self.light_time = LIGHT_TIME_SEC
        self.led_on = False

        # Status Labels
        self.button_state_label = tk.Label(root, text="Button State: Released", font=("Arial", 14))
        self.button_state_label.pack(pady=10)

        self.led_state_label = tk.Label(root, text="LED State: OFF", font=("Arial", 14))
        self.led_state_label.pack(pady=10)

        # Input for changing timer interval
        self.entry_label = tk.Label(root, text="LED ON Time (sec):")
        self.entry_label.pack()
        self.timer_entry = tk.Entry(root)
        self.timer_entry.insert(0, str(self.light_time))
        self.timer_entry.pack()

        self.set_timer_button = tk.Button(root, text="Set Time", command=self.update_timer)
        self.set_timer_button.pack(pady=5)

        board.set_pin_mode_digital_input_pullup(BUTTON_PIN, callback=self.button_callback)


    def update_timer(self):
        try:
            new_time = float(self.timer_entry.get())
            if new_time <= 0:
                raise ValueError
            self.light_time = new_time
            messagebox.showinfo("Success", f"Timer updated to {self.light_time} seconds.")
        except ValueError:
            messagebox.showerror("Error", "Please enter a valid positive number.")

    def turn_led_off(self):
        board.digital_write(LED_PIN, 0)
        self.led_on = False
        self.led_state_label.config(text="LED State: OFF")

    def button_callback(self, data):
        if data[2] == 1:
            self.button_state_label.config(text="Button State: Pressed")
            board.digital_write(LED_PIN, 1)
            self.led_on = True
            self.led_state_label.config(text="LED State: ON")
            threading.Timer(self.light_time, self.turn_led_off).start()
        else:
            self.button_state_label.config(text="Button State: Released")


if __name__ == "__main__":
    root = tk.Tk()
    app = App(root)
    root.mainloop()
