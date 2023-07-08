import machine
button = machine.Pin(23, machine.Pin.IN, machine.Pin.PULL_UP)
while True:
    if not button.value():
        print('Button pressed!')