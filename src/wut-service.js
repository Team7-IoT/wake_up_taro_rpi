var util = require('util');
var bleno = require('bleno');

var BlenoPrimaryService = bleno.PrimaryService;

var WUTSensorStateCharacteristic = require('./wut-sensor-state-characteristic');
var WUTSensorControlCharacteristic = require('./wut-sensor-control-characteristic');
var WUTNotifyCharacteristic = require('./wut-notify-characteristic');

function WUTService() {
  WUTService.super_.call(this, {
      uuid: '7E1BF98CBFA34A8FB6A782C3A851E0D0',
      characteristics: [
          new WUTSensorStateCharacteristic(),
          new WUTSensorControlCharacteristic(),
          new WUTNotifyCharacteristic()
      ]
  });
}

util.inherits(WUTService, BlenoPrimaryService);

module.exports = WUTService;
