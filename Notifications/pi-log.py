import os
import smtplib
import socket
from email.mime.multipart import MIMEMultipart
from email.mime.base import MIMEBase
from email import encoders
from datetime import datetime

os.chdir(os.path.dirname(os.path.abspath(__file__)))

def send_email(subject, filenames):
	msg = MIMEMultipart()
	msg['Subject'] = subject
	msg['From'] = 'the.matthew.maloney@gmail.com'
	msg['To'] = ', '.join(['the.matthew.maloney@gmail.com', 'mrtennizman@gmail.com'])

	for filename in filenames:
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

def get_log_filename():
	current_date = datetime.now().strftime('%Y-%m-%d')
	filename = '/home/johnsmith/Trading/Algorithmic-Trading/log' + current_date
	return filename

if __name__ == "__main__": #TODO this is untested (the file path and emailing with the new error log and candlestick log)
	ip_address = get_ip_address()
	filenames = [get_log_filename(), '../error_log.txt', '../candlesticks.txt']
	send_email("Raspberry Pi log file", filenames)