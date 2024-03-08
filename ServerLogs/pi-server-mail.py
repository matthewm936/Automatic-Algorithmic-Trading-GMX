import os
import smtplib
from email.message import EmailMessage
import socket

def send_email(subject, content):
    msg = EmailMessage()
    msg.set_content(content)

    msg['Subject'] = subject
    msg['From'] = 'the.matthew.maloney@gmail.com'
    msg['To'] = 'the.matthew.maloney@gmail.com'

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
    ip_address = get_ip_address()
    send_email("Raspberry Pi rebooted", f"The Raspberry Pi has rebooted. The current IP address is {ip_address}.")