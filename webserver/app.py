from flask import Flask, render_template
import datetime
import socket

HOST = '192.168.1.2'
PORT = 8080

app = Flask(__name__)
@app.route('/')

def hello():
   now = datetime.datetime.now()
   timeString = now.strftime("%Y-%m-%d %H:%M::%S")
   templateData = {
      'title' : 'HELLO!',
      'time': timeString
      }
   return render_template('index.html', **templateData)

@app.route("/show/", methods=['POST'])

def show():
    data = ""
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((HOST, PORT))
        data = s.makefile().readline()
        s.sendall(b"gpio0:show\n")
        data = "show:" + s.makefile().readline()
        s.close()

    return render_template('index.html', data=data)

@app.route("/off/", methods=['POST'])

def off():
    data = ""
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((HOST, PORT))
        data = s.makefile().readline()
        s.sendall(b"gpio0:off\n")
        data = s.makefile().readline()
        s.close()

    return render_template('index.html', data=data)

@app.route("/on/", methods=['POST'])

def on():
    data = ""
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((HOST, PORT))
        data = s.makefile().readline()
        s.sendall(b"gpio0:on\n")
        data = s.makefile().readline()
        s.close()

    return render_template('index.html', data=data)

if __name__ == '__main__':
    app.run(debug=True, port=80, host='localhost')