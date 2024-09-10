var targetUrl = `ws://${window.location.hostname}/ws`;
var websocket;
window.addEventListener('load', onLoad);


function onLoad() {
  initializeSocket();
}

function initializeSocket() {
  console.log('Opening WebSocket connection to ESP32...');
  websocket = new WebSocket(targetUrl);
  websocket.onopen = onOpen;
  websocket.onclose = onClose;
  websocket.onmessage = onMessage;
}
function onOpen(event) {
  console.log('Starting connection to server..');
}
function onClose(event) {
  console.log('Closing connection to server..');
  setTimeout(initializeSocket, 2000);
}
function onMessage(event) {
  console.log("WebSocket message received:", event)
}

function sendMessage(message) {
  websocket.send(message);
}

/*
Speed Settings Handler
*/
var speedSettings = document.querySelectorAll(
  'input[type=radio][name="speed-settings"]'
);
speedSettings.forEach((radio) =>
  radio.addEventListener("change", () => {
    var speedSettings = radio.value;
    console.log('Speed Settings :: ' + speedSettings)
    sendMessage(speedSettings);
  }
  ));

var pose = document.getElementById("pose");
pose.addEventListener("change", () => {
  console.log('Pose :: ' + pose.checked);
  if (pose.checked) {
    sendMessage("contraction");
  }
  else {
    sendMessage("relaxation");
  }
})


/*
O-Pad/ D-Pad Controller and Javascript Code
*/
// Prevent scrolling on every click!
// super sweet vanilla JS delegated event handling!
document.body.addEventListener("click", function (e) {
  if (e.target && e.target.nodeName == "A") {
    e.preventDefault();
  }
});

function touchStartHandler(event) {
  var direction = event.target.dataset.direction;
  console.log('Touch Start :: ' + direction)
  sendMessage(direction);
}

function touchEndHandler(event) {
  const stop_command = 'stop';
  var direction = event.target.dataset.direction;
  console.log('Touch End :: ' + direction)
  sendMessage(stop_command);
}


document.querySelectorAll('.control').forEach(item => {
  item.addEventListener('touchstart', touchStartHandler)
})

document.querySelectorAll('.control').forEach(item => {
  item.addEventListener('touchend', touchEndHandler)
})




setInterval(function () {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperature").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}, 1000);

setInterval(function () {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("angle").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/angle", true);
  xhttp.send();
}, 50);

function toggleCheckbox(element) {
  var xhr = new XMLHttpRequest();
  if (element.checked) { xhr.open("GET", "/update?state=0", true); }
  else { xhr.open("GET", "/update?state=1", true); }
  xhr.send();
}

setInterval(function () {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
      var inputChecked;
      var outputStateM;
      //sendMessage(this.responseText);
      if (this.responseText == '1') {
        inputChecked = true;
        outputStateM = "Contraction";

      }
      else {
        inputChecked = false;
        outputStateM = "Relaxation";
      }
      //document.getElementById("pose").checked = inputChecked;
      document.getElementById("output").checked = inputChecked;
      document.getElementById("outputState").innerHTML = outputStateM;

    }
  };
  xhttp.open("GET", "/state", true);
  xhttp.send();
}, 50);