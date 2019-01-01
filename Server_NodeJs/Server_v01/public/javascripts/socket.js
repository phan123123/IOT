// var socket  = io.connect('ws://localhost:4000');
// // socket.on('news', function (data) {
// //     console.log('Nhan data');
// // });
// socket.on('message',(data)=>{
//     console.log('data here '+data);
// });
// socket.send("this is message");
var webSocket = new WebSocket('ws://localhost:4000/');
webSocket.onopen = function () {
    alert('Opened connection ');
    webSocket.send("123");
};