from flask import Flask, redirect, Response
app = Flask(__name__)

@app.route('/redirect')
def redirect_url():
    return redirect("http://www.domain.com/", code=302) #insecure

@app.route('/url/<url>')
def change_location(url):
    response = Response()
    headers = response.headers
    headers["location"] = url # insecure
    return response.headers["location"]


if __name__ == ' __main__ ':
	app.run(debug = True)
