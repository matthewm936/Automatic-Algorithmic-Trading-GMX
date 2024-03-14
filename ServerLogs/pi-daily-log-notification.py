import os
import smtplib
import socket
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

def get_log_contents():
    with open('/home/johnsmith/Trading/Algorithmic-Trading/ServerLogs/log.txt', 'r') as file:
        return file.read()

if __name__ == "__main__":
    ip_address = get_ip_address()
    log_contents = get_log_contents()
    send_email("Raspberry Pi Daily log file", f"The current IP address is {ip_address}.\n\nLog contents:\n{log_contents}")