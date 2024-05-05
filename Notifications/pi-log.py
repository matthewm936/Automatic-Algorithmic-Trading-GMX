import smtplib
import socket
import os
from email.mime.multipart import MIMEMultipart
from email.mime.base import MIMEBase
from email import encoders
from datetime import datetime

def send_email(subject, filenames):
	try:
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
		print("pi-log email send successfully!")
	except (smtplib.SMTPException, socket.error, OSError) as e:
		print(f"An error occurred while sending the email: {e}")

def get_ip_address():
	try:
		s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
		s.connect(("8.8.8.8", 80))
		ip_address = s.getsockname()[0]
		s.close()
		return ip_address
	except socket.error as e:
		print(f"An error occurred while getting the IP address: {e}")
		return None

def get_log_filename():
	try:
		current_date = datetime.now().strftime('%Y-%m-%d')
		filename = '/home/johnsmith/Trading/Algorithmic-Trading/log' + current_date
		return filename
	except Exception as e:
		print(f"An error occurred while getting the log filename: {e}")
		return None

def get_filenames():
	try:
		log_filename = get_log_filename()
		error_log_filename = '/home/johnsmith/Trading/Algorithmic-Trading/error_log.txt'
		positions_filename = '/home/johnsmith/Trading/Algorithmic-Trading/positions.txt'
		trade_log_filename = '/home/johnsmith/Trading/Algorithmic-Trading/trade_log.txt'
		
		return log_filename, error_log_filename, positions_filename, trade_log_filename
	except Exception as e:
		print(f"An error occurred while getting the filenames: {e}")
		return None

if __name__ == "__main__":
	ip_address = get_ip_address()
	if ip_address:
		filenames = get_filenames()
		if filenames:
			send_email("Raspberry Pi log file", filenames)
