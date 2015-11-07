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

chrome.serial.onReceive.addListener(function(info) {
  if (page !== null) {
    page.postMessage({'data': BufferToString(info.data)});
  }
});

chrome.serial.onReceiveError.addListener(function(info) {
  chrome.serial.disconnect(info.connectionId, function() {});
  serial = null;
});

setInterval(function() {
  chrome.serial.getDevices(function(ports) {
    if (serial !== null) {
      return;
    }
    for (var i = 0; i < ports.length; i++) {
      if (ports[i].path.search('tty.usbserial') >= 0) {
        chrome.serial.connect(
            ports[i].path,
            {bitrate: 9600, persistent: true}, function(c) {
          serial = c;
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
