var assert = require('assert');
var WUTNotifyCharacteristic  = require('../wut-notify-characteristic');
var target = new WUTNotifyCharacteristic();


describe('MODULE: wut-notify-characteristic', function() {
  describe('Constructor: "properties"', function() {
    it('"properties" is set correctly', function() {
      assert.equal(target.properties[0], 'notify');
    });
    it('"uuid" is set correctly', function() {
      assert.equal(target.uuid, '7BD98C73E66148B5B09396C4EC897B9C');
    });
  });
  describe('METHOD: onSubscribe', function() {
    it('"_updateValueCallback" is not null', function() {
      var value = 1;
      target.onSubscribe(0,value);
      assert.equal(target._updateValueCallback, value);
    });
  });
  describe('METHOD: onUnsubscribe', function() {
    it('"_updateValueCallback" is null', function() {
      target.onUnsubscribe();
      assert.equal(target._updateValueCallback, null);
    });
  });
});

