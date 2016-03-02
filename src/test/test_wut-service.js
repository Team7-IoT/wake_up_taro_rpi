var assert = require('assert');
var WUTService  = require('../wut-service');
var WUTSensorStateCharacteristic = require('../wut-sensor-state-characteristic');
var WUTSensorControlCharacteristic = require('../wut-sensor-control-characteristic');
var WUTNotifyCharacteristic = require('../wut-notify-characteristic');

var target = new WUTService();
var WUTSensorStateCharacteristic = new WUTSensorStateCharacteristic();
var WUTSensorControlCharacteristic = new WUTSensorControlCharacteristic();
var WUTNotifyCharacteristic = new WUTNotifyCharacteristic();


describe('MODULE: wut-service', function() {
  describe('Constructor: "properties"', function() {
    it('"characteristics" is set correctly', function() {
      assert.equal(target.characteristics.length, 3);
      assert.equal(target.characteristics[0].uuid, WUTSensorStateCharacteristic.uuid);
      assert.equal(target.characteristics[1].uuid, WUTSensorControlCharacteristic.uuid);
      assert.equal(target.characteristics[2].uuid, WUTNotifyCharacteristic.uuid);
    });
    it('"uuid" is set correctly', function() {
      assert.equal(target.uuid, '7E1BF98CBFA34A8FB6A782C3A851E0D0');
    });
  });
});

