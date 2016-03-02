var assert = require('assert');
var WUTSensorStateCharacteristic  = require('../wut-sensor-state-characteristic');
var Characteristic = require('bleno').Characteristic;
var target = new WUTSensorStateCharacteristic();


describe('MODULE: wut-sensor-state-characteristic', function() {
  describe('Constructor: "properties"', function() {
    it('"properties" is set correctly', function() {
      assert.equal(target.properties[0], 'read');
    });
    it('"uuid" is set correctly', function() {
      assert.equal(target.uuid, 'F9A318D8A1784EE18D47D92CDAE9C907');
    });
  });
  describe('METHOD: onReadRequest', function() {
    it('return RESULT_SUCCESS(0x00)', function() {
      target.onReadRequest(1,function(value, buf){
        assert.equal(value, Characteristic.RESULT_SUCCESS);
      });
    });
  });
});
