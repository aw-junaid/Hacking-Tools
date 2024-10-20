from flask import Flask

app = Flask(__name__)

class MyException(Exception):
    status_code = 400

    def __init__(self, message, status_code):
        Exception.__init__(self)
        
@app.route('/showException')
def main():
    raise MyException('MyException', status_code=500)

if __name__ == ' __main__ ':
	app.run(debug = True) #insecure
