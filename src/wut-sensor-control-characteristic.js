var util = require('util');
var bleno = require('bleno');

var Descriptor = bleno.Descriptor;
var Characteristic = bleno.Characteristic;


var WUTSensorControlCharacteristic = function() {
  WUTSensorControlCharacteristic.super_.call(this, {
    uuid: '04BDBB401F5C453FA9E1333BC8930826',
    properties: ['writeWithoutResponse']
  });
};

util.inherits(WUTSensorControlCharacteristic, Characteristic);

WUTSensorControlCharacteristic.prototype.onWriteRequest = function(data, offset, withoutResponse, callback) {
  console.log(data);
  callback(this.RESULT_SUCCESS);
};

module.exports = WUTSensorControlCharacteristic;