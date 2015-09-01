var express = require('express');
var exec = require('child_process').exec;

var app = express();
var count = 1;

function getDateTime() {

    var date = new Date();

    var hour = date.getHours();
    hour = (hour < 10 ? "0" : "") + hour;

    var min  = date.getMinutes();
    min = (min < 10 ? "0" : "") + min;

    var sec  = date.getSeconds();
    sec = (sec < 10 ? "0" : "") + sec;

    var year = date.getFullYear();

    var month = date.getMonth() + 1;
    month = (month < 10 ? "0" : "") + month;

    var day  = date.getDate();
    day = (day < 10 ? "0" : "") + day;

    return year + ":" + month + ":" + day + " " + hour + ":" + min + ":" + sec;

}

app.get('/input/motion', function(req, res){
  var espid = req.query.espid;
  var gpio = req.query.gpio;

  if(gpio==1) { 
    console.log(getDateTime() + " - count["+count+"] espId("+espid+"): gpio: " + gpio);
    exec('afplay dog_bark6.wav', function(error, stdout, stderr) {
      if (error !== null) {
          console.log('exec error: ' + error);
      }
    });
    count = count + 1;
  }

});

app.listen(8080);
