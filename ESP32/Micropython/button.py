from machine import Pin
from time import sleep

led = Pin(5,Pin.OUT)
btn = Pin(2,Pin.IN, Pin.PULL_UP)

while True :
    btn_val = btn.value()
    print(btn_val)
    sleep(0.5)