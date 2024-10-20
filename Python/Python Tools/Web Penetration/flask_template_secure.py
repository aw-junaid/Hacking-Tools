from flask import Flask , request , make_response
from flask import escape

app = Flask(__name__)

@app.route ('/info',methods =['GET'])
def getInfo():
	parameter = escape(request.args.get('parameter',''))#secure
	html = open('templates/template.html').read()
	response = make_response(html.replace('{{ data }}',parameter))
	return response

if __name__ == ' __main__ ':
	app.run(debug = True)
