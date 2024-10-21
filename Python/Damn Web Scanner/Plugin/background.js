var config_server = "http://127.0.0.1:8000";

/**
 * Extract the domain from an URL
 * @param string(url) - url of the target which will be shorten
 */
function extract_domain(url){
    var domain;
    if (url.indexOf("://") > -1) {
        domain = url.split('/')[2];
    }
    else {
        domain = url.split('/')[0];
    }

    //find & remove port number
    return domain.split(':')[0];
}

/**
 * Inspect the source code and send a request to the server for every url
 *
 * @param string(url)    - url of the target
 * @param string(deep)   - deep of the target, 0 is the actual page
 * @param string(impact) - aggressivity of the scan from 0 to 5
 *
 */
function send_target(server, url, deep, impact, cookies, method, data){
    var http = new XMLHttpRequest();
    infos = server + "/?url=" + url + "&deep="+ deep + "&impact=" + impact + "&cookies=" + cookies + "&useragent=" + navigator.userAgent + "&method=" + method + "&data="+ data;

    // Display the informations sent by the scanner
    http.onreadystatechange = function() {
      if (http.readyState == XMLHttpRequest.DONE) {
        http_data = JSON.parse(http.responseText);

        // Update vulns count
        chrome.storage.sync.get(['rce','xss','lfi','sql'], function(items) {
            chrome.storage.sync.set({'rce': items['rce']+parseInt(http_data.rce)});
            chrome.storage.sync.set({'xss': items['xss']+parseInt(http_data.xss)});
            chrome.storage.sync.set({'lfi': items['lfi']+parseInt(http_data.lfi)});
            chrome.storage.sync.set({'sql': items['sql']+parseInt(http_data.sql)});
        });

        // Update vulnerabilities URL list
        chrome.storage.sync.get(['list'], function(items) {
           chrome.storage.sync.set({'list': items['list']+http_data.list})
        });

        // Notifications
        if (http_data.xss != '0'){
          new Notification('New vulnerability detected !', {
            icon: 'icon.png',
            body: 'XSS on '+extract_domain(unescape(url))
          })();
        }

        if (http_data.sql != '0'){
          new Notification('New vulnerability detected !', {
            icon: 'icon.png',
            body: 'SQLi on '+extract_domain(unescape(url))
          })();
        }

        if (http_data.lfi != '0'){
          new Notification('New vulnerability detected !', {
            icon: 'icon.png',
            body: 'LFI on '+extract_domain(unescape(url))
          })();
        }

        if (http_data.rce != '0'){
          new Notification('New vulnerability detected !', {
            icon: 'icon.png',
            body: 'RCE on '+extract_domain(unescape(url))
          })();
        }

      }
    }
    http.open("GET", infos, true);
    http.send();
}

// Set a clean local storage
chrome.storage.sync.set({'rce':0, 'xss': 0, 'sql': 0, 'lfi': 0, 'work': 0, 'list':'' })


// Handle POST scan
chrome.runtime.onMessage.addListener(
  function(request, sender, sendResponse) {
    if (request.type == "scan_plz" && request.data != ''){

      chrome.storage.sync.get(['work'], function(items) {
        if(items['work'] == 1){

          // Start a POST scan with the url and the cookies
          send_target(config_server, escape(request.url), 0, 0, escape(request.cookie), 'POST', escape(request.data));

        }
      });

    }
});


// Launch a scan when the tab change - Submit a form / Open new URL from bar
chrome.tabs.onUpdated.addListener(function(tabId,changeInfo, tab) {
  if(changeInfo.status == 'complete'){

    // Inject this function into the page to catch a submit event for every forms
    function inject_onsubmit(){
      for (var i = 0; i < document.forms.length ; i++) {
          document.forms[i].addEventListener('submit', function(){

            // Detect value of inputs of the form
            post_data = '';
            for (var j = 0; j < document.forms[i-1].elements.length; j++) {
              post_data += (document.forms[i-1].elements[j].name+":"+document.forms[i-1].elements[j].value+"|");
              console.log(post_data);
            }


            // Send data to this plugin (POST Scan) - check the method, GET is already handle with onUpdated
            if(post_data != '' && document.forms[i-1].method.toUpperCase() == 'POST'){
              chrome.runtime.sendMessage({type: "scan_plz", data:post_data, url:document.location.href, cookie:document.cookie}, function() {});
            }
        });
      }
    }
    chrome.tabs.executeScript({code: '(' + inject_onsubmit + ')();'}, (results) => {});


    // Get the information of the updated tab
    chrome.tabs.get(tabId, function(tab){

      // Handle start/stop button
      chrome.storage.sync.get(['work'], function(items) {
        if(items['work'] == 1){

          // Extract cookies from the domain
          var cookies_string = "";
          chrome.cookies.getAll({ 'domain': extract_domain(tab.url)}, function(cookies) {

            // Custom cookie string with all cookies from the domain
            for (var i = 0; i < cookies.length; i++) {
             cookies_string += ("name:" + cookies[i].name + "|value:" + cookies[i].value+"\n");
            }

            // Start a GET scan with the url and the cookies
            send_target(config_server, escape(tab.url), 0, 0, escape(cookies_string), 'GET', '');

          });
        }
      });

    });
  }
});
