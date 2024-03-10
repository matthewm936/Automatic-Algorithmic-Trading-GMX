import os
import smtplib
import socket
import argparse
from email.message import EmailMessage

def send_email(subject, content):
    msg = EmailMessage()
    msg.set_content(content)

    msg['Subject'] = subject
    msg['From'] = 'the.matthew.maloney@gmail.com'
    msg['To'] = ['the.matthew.maloney@gmail.com', 'mrtennizman@gmail.com']

    server = smtplib.SMTP('smtp.gmail.com', 587)
    server.starttls()
    server.login('the.matthew.maloney@gmail.com', 'ofag lqqg wwzh yhax')
    server.send_message(msg)
    server.quit()

def get_ip_address():
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.connect(("8.8.8.8", 80))
    ip_address = s.getsockname()[0]
    s.close()
    return ip_address

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('--type', help='The connection type (LAN or Wireless)')
    parser.add_argument('--ssid', help='The SSID of the WiFi network')
    args = parser.parse_args()

    ip_address = get_ip_address()
    subject = "Raspberry Pi network change"
    content = f"The Raspberry Pi has connected to a new network. The current IP address is {ip_address}.\n\nConnection type: {args.type}\nSSID: {args.ssid}"

    send_email(subject, content)