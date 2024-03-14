import os
import smtplib
import json
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

def read_prices():
    with open('/home/johnsmith/Trading/Algorithmic-Trading/data/prices.json', 'r') as f:
        data = json.load(f)
    return data

if __name__ == "__main__":
    prices = read_prices()
    formatted_prices = json.dumps(prices, indent=4)
    send_email("prices.json: dump ", f"The current prices are: {formatted_prices}")