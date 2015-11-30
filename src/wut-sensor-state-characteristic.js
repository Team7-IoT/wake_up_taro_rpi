var util = require('util');
var bleno = require('bleno');

var Descriptor = bleno.Descriptor;
var Characteristic = bleno.Characteristic;


var WUTSensorStateCharacteristic = function() {
  WUTSensorStateCharacteristic.super_.call(this, {
    uuid: 'F9A318D8A1784EE18D47D92CDAE9C907',
    properties: ['read']
  });
};

util.inherits(WUTSensorStateCharacteristic, Characteristic);

WUTSensorStateCharacteristic.prototype.onReadRequest = function(offset, callback) {
  // return hardcoded value
  callback(this.RESULT_SUCCESS, new Buffer([50]));
};

module.exports = WUTSensorStateCharacteristic;