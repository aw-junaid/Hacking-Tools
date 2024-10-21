#!/usr/bin/python
# -*- coding: utf-8 -*-
from flask import Flask, request, jsonify
from ghost import Ghost
from scans import *
import requests
import datetime
import re

app     = Flask(__name__)

""" Route /ping
Description: Simple ping implementation to check if the server is up via the extension
"""
@app.route('/ping',methods=['GET'])
def ping():
	return "pong"


""" Template
Description: Basic template, will be used in the next features
"""
@app.route('/template', methods=['GET', 'POST'])
def template():
        return render_template('index.html')


""" Route /
Description: main route for the flask application, every scan is launched from here
"""
@app.route('/',methods=['GET'])
def index():
	vulns = {'rce': 0, 'xss': 0, 'sql': 0, 'lfi': 0, 'list':''}

	# Parse requests - extract arguments
	args          = request.args
	url           = args['url']
	useragent     = args['useragent']
	methods       = args['method']
	data          = args['data']
	method        = ''
	matches       = []
	data_requests = {}

	# Parse args for GET
	if "?" in url:
		method  = 'GET'

		# Parse cookies strings - string like name:username|value:admin
		cookies_requests = {}
		cookies_ghost    = ""
		for cookie in args['cookies'].split('\n'):

			c = cookie.split('|')
			if c != '' and c != None:
				if len(c) != 1:
					name  = str(c[0]).replace('name:','')
					value = str(c[1]).replace('value:','')
					cookies_requests[name] = value
					cookies_ghost += " "+cookie.replace('name:','').replace('value:','=').replace('|','') + ";"

		# Parse GET data (in url)
		params  = url.split('?')[1]
		regex   = re.compile('([a-zA-Z0-9\-_]*?)=')
		matches = regex.findall(params)


	# Parse args for POST
	if data != '':
		method = 'POST'

		# Parse document.cookie for Ghost and Requests
		cookies_requests = {} #dict
		cookies_ghost    = "" #string header
		for cookie in args['cookies'].split(';'):
			c = cookie.split('=')
			if c != '' and c != None:
				if len(c) != 1:
					name  = c[0]
					value = c[1]
					cookies_requests[name] = value
					cookies_ghost += " "+cookie.replace('name:','').replace('value:','=').replace('|','') + ";"


		# Parse POST data (in data parameter)
		data_requests = {}
		for post_data in data.split('|'):
			d = post_data.split(':')
			if d != '' and d != None:
				if len(d) != 1:
					name  = str(d[0])
					value = str(d[1])
					data_requests[name] = value

		# Convert dict(data_requests) to list(matches)
		matches = data_requests.keys()


	# Launch scans - iterate through all parameters
	for fuzz in matches:
		print ("\n---[ " + method + " - New parameter " + fuzz + " for url: " + url + " ]---")
		scan_xss(method, vulns, url, fuzz, cookies_ghost, useragent, data_requests)
		scan_lfi(method, vulns, url, fuzz, cookies_requests, useragent, data_requests)
		scan_sql_error(method, vulns, url, fuzz, cookies_requests, useragent, data_requests)
		scan_sql_blind_time(method, vulns, url, fuzz, cookies_requests, useragent, data_requests)
		scan_rce(method, vulns, url, fuzz, cookies_requests, useragent, data_requests)


	# Display results as a json
	return jsonify(vulns)

if __name__ == '__main__':
	app.run(host='0.0.0.0', port=8000, threaded=True, passthrough_errors=True) # Seems to crash the server with the following options, threaded=True, passthrough_errors=False)
