from network import STA_IF, WLAN
from simple import MQTTClient
ssid = 'Erufu'
pwd = 'Delta_006'

wlan = WLAN(STA_IF)
wlan.active(True)
wlan.connect(ssid,pwd)
print('WiFi Connecting...')
while not wlan.isconnected() :
    pass
print('WiFi connected')
print(wlan.ifconfig())

client_id = '53e60d85-62b9-4a8a-80f1-b62db48ac5d8'
token = '2oT9VP7K3CLgJnU4cs5yKu3cvmpGbatS'
secret = 'ZD!FF(hfA9uSL8s~CqjI$~4sQTm*DcUT'
broker = 'mqtt.netpie.io'

client = MQTTClient(client_id,broker,user=token,password=secret)
print('MQTT connecting....')
client.connect()
print('MQTT connected')