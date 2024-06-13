const updateData = () => {
  console.log("Update data...");
  var xhr = new XMLHttpRequest();
  xhr.open("GET", 'https://api.thingspeak.com/channels/2571668/feeds.json?', true);
  xhr.send();
  var lslChecked = false;
  var lseChecked = false;
  var uidChecked = false;
  var dooChecked = false;
  var bliChecked = false;
  var almChecked = false;
  xhr.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      var feed = JSON.parse(xhr.responseText).channel.last_entry_id;
      if(feed>100) var rest = feed - 100;
      else         var rest = 0;
      console.log("Num. Feeds: ", feed);
      for(var i = feed-rest-1; i >= 0; i--) {
        var lightSlider = JSON.parse(xhr.responseText).feeds[i].field1;
        var lightSensor = JSON.parse(xhr.responseText).feeds[i].field2;
        var uid = JSON.parse(xhr.responseText).feeds[i].field5;
        var time = JSON.parse(xhr.responseText).feeds[i].created_at;
        var door = JSON.parse(xhr.responseText).feeds[i].field6;
        var blind = JSON.parse(xhr.responseText).feeds[i].field4;
        var alarm = JSON.parse(xhr.responseText).feeds[i].field8;
        if (lightSlider != null && !lslChecked) {
          var slider = document.getElementById("myRange");
          var output = document.getElementById("sent-light");
          slider.value = lightSlider;
          output.innerHTML = lightSlider;
          console.log(i, lightSlider);
          lslChecked = true;
        }
        if (lightSensor != null && !lseChecked) {
          var light = document.getElementById("rec-light");
          light.innerHTML = lightSensor;
          console.log(i, lightSensor);
          lseChecked = true;
        }
        if (uid != null && !uidChecked) {
          var user = document.getElementById("usr-uid");
          var access = document.getElementById("usr-accss-cncd");
          var hour = document.getElementById("usr-time");
          var date = document.getElementById("usr-date");
          const acc_uid = uid.split(",");
          var indicator = document.getElementById("intruder-alarm");
          user.innerHTML = acc_uid[1];
          if(acc_uid[0] == "1"){
            access.innerHTML = "Granted";
            indicator.style.backgroundColor = "green";
          }
          else{
            indicator.style.backgroundColor = "red";
            access.innerHTML = "Denied";
          }
          const aux = time.split("T");
          date.innerHTML = aux[0];
          var hourMod = parseInt(aux[1].split("Z")[0].split(":")[0]) - 5;
          hour.innerHTML = hourMod + ":" + aux[1].split("Z")[0].split(":")[1] + ":" + aux[1].split("Z")[0].split(":")[2];
          console.log(i, uid);
          uidChecked = true;
        }
        if (door != null && !dooChecked) {
          console.log("Door value", door);
          var doorSt = document.getElementById("door-status");
          var indicator = document.getElementById("door-open");
          if(door == "1"){
            indicator.style.backgroundColor = "green";
            doorSt.innerHTML = "open";
          } else if (door == "0") {
            indicator.style.backgroundColor = "red";
            doorSt.innerHTML = "closed";
          }
          dooChecked = true;
        }
        if (blind != null && !bliChecked) {
          var indicator = document.getElementById("blinds-open");
          if(blind == "1")       indicator.style.backgroundColor = "green";
          else if (blind == "0") indicator.style.backgroundColor = "red";
          bliChecked = true;
        }
        if (alarm != null && !almChecked) {
          var button = document.getElementById("deact-alarm");
          if(alarm == "1")       button.disabled = false;
          else                   button.disabled = true;
          almChecked = true;
        }

        if (lslChecked && lseChecked && uidChecked && dooChecked && bliChecked && almChecked) {
          break;
        }
      }
    } else {
      console.log('Error onreadystatechange');
    }
  };
};

const postData = (field, value) => {
  var xhr = new XMLHttpRequest();
  xhr.open("GET", 'https://api.thingspeak.com/update?api_key=I00H45MBL35AXYKW&'+field+'='+value, false);
  xhr.send();
  updateData();
};

updateData();

// Button for updating data
document.getElementById("get-button").addEventListener("click", updateData);

// Buttons for lights control
document.getElementById("lights-on").addEventListener("click", () => postData("field1", "100"));

document.getElementById("lights-off").addEventListener("click", () => postData("field1", "0"));

var slider = document.getElementById("myRange");
var output = document.getElementById("sent-light");

slider.onchange = function() {
  output.innerHTML = this.value;
  postData("field1", this.value);
};

// Buttons for blinds
document.getElementById("open-blinds").addEventListener("click", () => postData("field3", "1"));

document.getElementById("close-blinds").addEventListener("click", () => postData("field3", "0"));

// Button for deactivating alarm
document.getElementById("deact-alarm").addEventListener("click", () => {
  postData("field7", "1")
});