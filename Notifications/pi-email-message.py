from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart
import smtplib
import sys

def send_email(subject, message):
	msg = MIMEMultipart()
	msg['Subject'] = subject
	msg['From'] = 'the.matthew.maloney@gmail.com'
	msg['To'] = ', '.join(['the.matthew.maloney@gmail.com', 'mrtennizman@gmail.com'])

	# Attach the message to the email
	msg.attach(MIMEText(message, 'plain'))

	server = smtplib.SMTP('smtp.gmail.com', 587)
	server.starttls()
	server.login('the.matthew.maloney@gmail.com', 'ofag lqqg wwzh yhax')
	server.send_message(msg)
	server.quit()

if __name__ == "__main__":
	if len(sys.argv) > 1:
		header = sys.argv[1]
		message = sys.argv[2]
		send_email(header, message)
	else:
		print("Please provide a message as a command-line argument.")