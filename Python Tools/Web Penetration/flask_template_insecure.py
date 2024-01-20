from flask import Flask , request , make_response
app = Flask(__name__)

@app.route ('/info',methods =['GET'])
def getInfo():
	parameter = request.args.get('parameter','')#insecure
	html = open('templates/template.html').read()
	response = make_response(html.replace('{{ data }}',parameter))
	return response

if __name__ == ' __main__ ':
	app.run(debug = True)
