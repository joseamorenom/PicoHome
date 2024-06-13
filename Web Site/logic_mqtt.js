const host = "18.207.44.162";
const port = 80; // Websocket port
var mqtt;
var field;
var reconnectTimeout = 2000;
const username = "Ky0rJgAkBwgfIy8fGgkQPQ8";
const password = "Iid+7moLn+TDnnAclpGk3J/E";
const clientId = "Ky0rJgAkBwgfIy8fGgkQPQ8";

const MQTT_TOPIC_PUB_LIGHT = "channels/2571668/publish/fields/field1";
const MQTT_TOPIC_SUB_LIGHT = "channels/2571668/subscribe/fields/field2";
const MQTT_TOPIC_PUB_BLIND = "channels/2571668/publish/fields/field3";
const MQTT_TOPIC_SUB_BLIND = "channels/2571668/subscribe/fields/field4";
const MQTT_TOPIC_SUB_NFC = "channels/2571668/subscribe/fields/field5";
const MQTT_TOPIC_SUB_DOOR = "channels/2571668/subscribe/fields/field6";
const MQTT_TOPIC_PUB_ALARM = "channels/2571668/publish/fields/field7";
const MQTT_TOPIC_SUB_ALARM = "channels/2571668/subscribe/fields/field8";


function onConnect() {
  console.log("Connected to "+ host);

  mqtt.subscribe("channels/2571668/subscribe/fields/field2");
  mqtt.subscribe("channels/2571668/subscribe/fields/field4");
  mqtt.subscribe("channels/2571668/subscribe/fields/field5");
  mqtt.subscribe("channels/2571668/subscribe/fields/field6");
  mqtt.subscribe("channels/2571668/subscribe/fields/field8");
}

function onFailure(message) {
  console.log("Connection Attempt to Host "+host+": --Failed--");
  setTimeout(MQTTconnect, reconnectTimeout);
}

function onMessageArrived(msg){
  field = msg.destinationName.split("/")[4];
  console.log("Message received at topic "+msg.destinationName);
  // console.log(field);
  if(field == "field2"){
    console.log("Light sensor information received");
    var light = document.getElementById("rec-light");
    light.innerHTML = msg.payloadString;
    console.log("New light sensor info: "+msg.payloadString);
  } else if(field == "field4"){
    console.log("Blind sensor information received");
    var indicator = document.getElementById("blinds-open");
    if(msg.payloadString == "1")       indicator.style.backgroundColor = "green";
    else if (msg.payloadString == "0") indicator.style.backgroundColor = "red";
  } else if(field == "field5"){
    console.log("NFC information received");
    var user = document.getElementById("usr-uid");
    var access = document.getElementById("usr-accss-cncd");
    var hour = document.getElementById("usr-time");
    var date = document.getElementById("usr-date");
    var acc_uid = (msg.payloadString).split(",");
    var indicator = document.getElementById("intruder-alarm");
    user.innerHTML = acc_uid[1];
    if(acc_uid[0] == "1"){
      access.innerHTML = "Granted";
      indicator.style.backgroundColor = "green";
    }
    else if (acc_uid[0] == "0"){
      indicator.style.backgroundColor = "red";
      access.innerHTML = "Denied";
    }
    var time = new Date();
    time = time.toString();
    console.log(time);
    const aux = time.split(" ");
    date.innerHTML = aux[2]+"-"+aux[1]+"-"+aux[3];
    hour.innerHTML = aux[4];
  } else if(field == "field6"){
    console.log("Door sensor information received");
    var doorSt = document.getElementById("door-status");
    var indicator = document.getElementById("door-open");
    if(msg.payloadString == "1"){
      indicator.style.backgroundColor = "green";
      doorSt.innerHTML = "open";
    } else if (msg.payloadString == "0") {
      indicator.style.backgroundColor = "red";
      doorSt.innerHTML = "closed";
    }
  } else if(field == "field8"){
    console.log("Alarm sensor information received");
    var button = document.getElementById("deact-alarm");
    if(msg.payloadString == "1")       button.disabled = false;
    else                               button.disabled = true;
  }
}

function MQTTconnect() {
  console.log("Connecting to "+ host +" "+ port);
  mqtt = new Paho.Client(host, port, clientId);
  var options = {
    timeout: 3,
    userName : username,
    password : password,
    onSuccess: onConnect,
    onFailure: onFailure,
  };
  mqtt.onMessageArrived = onMessageArrived;
  mqtt.connect(options); //connect
}

function publishData(topic, message) {
  mqtt.send(topic, message);
}

MQTTconnect();

// Buttons for lights control
var slider = document.getElementById("myRange");
var output = document.getElementById("sent-light");

slider.onchange = function() {
  output.innerHTML = this.value;
  publishData("channels/2571668/publish/fields/field1", this.value);
};

document.getElementById("lights-on")
        .addEventListener("click", () => {
          slider.value = 100;
          output.innerHTML = 100;
          publishData("channels/2571668/publish/fields/field1", "100")
        });

document.getElementById("lights-off")
        .addEventListener("click", () => {
          slider.value = 0;
          output.innerHTML = 0;
          publishData("channels/2571668/publish/fields/field1", "0")
        });

// Buttons for blinds
document.getElementById("open-blinds")
        .addEventListener("click", () => publishData("channels/2571668/publish/fields/field3", "1"));

document.getElementById("close-blinds")
        .addEventListener("click", () => publishData("channels/2571668/publish/fields/field3", "0"));

// Button for deactivating alarm
document.getElementById("deact-alarm")
        .addEventListener("click", () => {
          publishData("channels/2571668/publish/fields/field7", "0");
          this.disabled = true;
        });