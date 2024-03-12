import os
import smtplib
from email.message import EmailMessage
import socket

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


if __name__ == "__main__":
    send_email("Prices from PI", f"The current prices are: ")