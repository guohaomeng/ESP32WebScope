var consoleText = document.getElementById("console");
var angleData = document.getElementById("angleData");
let ws_connected = false;
let angle_value = 0.0;  // 摆杆角度
let velocity_value = 0.0; // 电机速度
function ws_connect() {
  webSocket = new WebSocket('ws://192.168.8.1:80/');
  webSocket.onmessage = function (event) {
    //console.log(event.data);
    if (event.data[0] == 'a' && event.data[1] == ':') {
      var value1 = event.data.split(',')[0];
      var value2 = event.data.split(',')[1];
      angle_value = parseFloat((value1).match(/\d+(\.\d+)?/g)); // 提取浮点数
      velocity_value = parseFloat((value2).match(/\d+(\.\d+)?/g)); // 提取浮点数
      angleData.innerHTML = angle_value.toFixed(3) / 10;
      Plotter.refresh();
      //console.log(angle_value);
    }
    else {
      if (consoleText.innerHTML.length > 200)
        consoleText.innerHTML = "";
      consoleText.innerHTML += event.data;
      consoleText.innerHTML += "<br>";
    }

  }
  webSocket.onopen = function (event) {
    ws_connected = true;
    console.log("websocket连接成功");
    sendData("ID1");

  }
  webSocket.onclose = function (event) {
    ws_connected = false;
    //settings.form2.controllerConnected = false;
    console.log("websocket断开连接");
  }
}

function sendData(data) {
  if (ws_connected) {
    webSocket.send(data);
  }
  else {
    console.log("请先建立websocket连接");
    console.log(data);
  }
}

