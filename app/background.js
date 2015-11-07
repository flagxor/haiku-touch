'use strict';

var serial = null;
var page = null;

function BufferToString(b) {
  return String.fromCharCode.apply(null, new Uint8Array(b));
}

function StringToBuffer(s) {
  var buf = new ArrayBuffer(s.length);
  var view = new Uint8Array(buf);
  for (var i = 0; i < s.length; i++) {
    view[i] = s.charCodeAt(i);
  }
  return buf;
}

setInterval(function() {
  chrome.serial.getDevices(function(ports) {
    if (serial !== null) {
      return;
    }
    for (var i = 0; i < ports.length; i++) {
      if (ports[i].path.search('tty.usbserial') >= 0) {
        console.log(ports[i].path);
        chrome.serial.connect(
            ports[i].path,
            {bitrate: 9600, persistent: true}, function(c) {
          serial = c;
          chrome.serial.onReceive.addListener(function(info) {
            if (page !== null) {
              page.postMessage({'data': BufferToString(info.data)});
              console.log(info.data.byteLength);
            }
          });
          chrome.serial.onReceiveError.addListener(function() {
            chrome.serial.disconnect(c.connectionId, function() {});
            serial = null;
          });
        });
      }
    }
  });
}, 500);

chrome.runtime.onConnectExternal.addListener(function(port) {
  page = port;
  port.onMessage.addListener(function(m) {
    var data = StringToBuffer(m.data);
    if (serial !== null) {
      chrome.serial.send(serial.connectionId, data, function() {});
    }
  });
  port.onDisconnect.addListener(function() {
    page = null;
  });
});

chrome.power.requestKeepAwake('display');
chrome.app.runtime.onLaunched.addListener(function() {
  chrome.app.window.create('haiku.html', {
    'id': 'haiku',
    'state': 'maximized',
    'bounds': {
      'width': 800,
      'height': 600,
    },
  });
});
