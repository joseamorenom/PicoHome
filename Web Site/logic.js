const updateData = () => {
  console.log("Update data...");
  var xhr = new XMLHttpRequest();
  xhr.open("GET", 'https://api.thingspeak.com/channels/2553412/feeds.json?api_key=TLF9XT501C4E6B4C', true);
  xhr.send();
  var lslChecked = false;
  var lseChecked = false;
  var tseChecked = false;
  xhr.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      var feed = JSON.parse(xhr.responseText).channel.last_entry_id;
      if(feed>100) var rest = feed - 100;
      else         var rest = 0;
      console.log("Num. Feeds: ", feed);
      for(var i = feed-rest-1; i >= 0; i--) {
        console.log("Feed: ", i);
        var lightSlider = JSON.parse(xhr.responseText).feeds[i].field2;
        var lightSensor = JSON.parse(xhr.responseText).feeds[i].field2;
        var tempSensor = JSON.parse(xhr.responseText).feeds[i].field1;
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
        if (lslChecked && lseChecked && tseChecked) {
          break;
        }
      }
    } else {
      console.log('Error onreadystatechange');
    }
  };
};

const getData = (field) => {};

const postData = (field, value) => {
  var xhr = new XMLHttpRequest();
  xhr.open("GET", 'https://api.thingspeak.com/update?api_key=BDW9UM21H3OTQ4QN&'+field+'='+value, false);
  xhr.send();
  updateData();
};

updateData();

document.getElementById("get-button").addEventListener("click", updateData);

document.getElementById("lights-on").addEventListener("click", () => postData("field2", "100"));

//document.getElementById("lights-off").addEventListener("click", () => postData("field2", "0"));
document.getElementById("lights-off").addEventListener("click", () => postData("field2", "0"));

var slider = document.getElementById("myRange");
var output = document.getElementById("sent-light");

slider.onchange = function() {
  output.innerHTML = this.value;
  postData("field2", this.value);
}