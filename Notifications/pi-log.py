import os
import smtplib
import socket
from email.mime.multipart import MIMEMultipart
from email.mime.base import MIMEBase
from email import encoders

def send_email(subject, filename):
	msg = MIMEMultipart()
	msg['Subject'] = subject
	msg['From'] = 'the.matthew.maloney@gmail.com'
	msg['To'] = ', '.join(['the.matthew.maloney@gmail.com', 'mrtennizman@gmail.com'])

	# Attach the file
	with open(filename, 'rb') as file:
		part = MIMEBase('application', 'octet-stream')
		part.set_payload(file.read())
	encoders.encode_base64(part)
	part.add_header('Content-Disposition', 'attachment; filename= {}'.format(os.path.basename(filename)))
	msg.attach(part)

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
	filename = '/home/johnsmith/Trading/Algorithmic-Trading/log.txt'
	send_email("Raspberry Pi log file", filename)