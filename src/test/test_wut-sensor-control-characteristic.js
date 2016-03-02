var assert = require('assert');
var WUTSensorControlCharacteristic  = require('../wut-sensor-control-characteristic');
var Characteristic = require('bleno').Characteristic;
var target = new WUTSensorControlCharacteristic();


describe('MODULE: wut-sensor-control-characteristic', function() {
  describe('Constructor: "properties"', function() {
    it('"properties" is set correctly', function() {
      assert.equal(target.properties[0], 'writeWithoutResponse');
    });
    it('"uuid" is set correctly', function() {
      assert.equal(target.uuid, '04BDBB401F5C453FA9E1333BC8930826');
    });
  });
  describe('METHOD: onSubscribe', function() {
    it('return RESULT_SUCCESS(0x00)', function() {
      target.onWriteRequest(1,1,1,function(value){
        assert.equal(value, Characteristic.RESULT_SUCCESS);
      });
    });
  });
});
