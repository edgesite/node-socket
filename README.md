# Node-socket
expose socket to nodejs
## Preview only
This module is preview only and need to refactor in near future, I'll complete when I'm free. Use as you own risk.
```js
const fd = require('socket').socket({
  ifname: 'ens160',
  localAddress: '224.224.224.224'
});
```
will be refactored to
```js
const socket = require('socket');
const fd = socket.socket(socket.AF_INET, socket.SOCK_STREAM);
// setsockopt(fd, SOL_SOCKET, SO_BINDTODEVICE, 'ens160', strlen('ens160')+1);
socket.setsockopt(fd, socket.SOL_SOCKET, socket.SO_BINDTODEVICE, 'ens160');
socket.bind(fd, '224.224.224.224:0');
```
## Usage
```js
const net = require('net');
const fd = require('socket').socket({
  ifname: 'ens160',
  localAddress: '224.224.224.224'
});

const s = new net.Socket({fd});
s.on('data', d => {
  console.log('> ', d.toString())
});
s.on('close', () => console.log('closed'))
s.connect({
  host: 'ip.sb',
  port: 80,
}, () => {
  console.log('connected');
  s.write('GET / HTTP/1.1\r\nHost: ip.sb\r\nUser-Agent: curl/7.64.0\r\nAccept: */*\r\n\r\n');
});

```
