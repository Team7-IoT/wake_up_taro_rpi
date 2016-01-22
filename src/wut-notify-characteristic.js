var util = require('util');
var bleno = require('bleno');

var Descriptor = bleno.Descriptor;
var Characteristic = bleno.Characteristic;
var ChildProcess = require('child_process');
var driver = ChildProcess.spawn('./okoshi_driver', ['-s']);

var WUTNotifyCharacteristic = function() {
  WUTNotifyCharacteristic.super_.call(this, {
    uuid: '7BD98C73E66148B5B09396C4EC897B9C',
    properties: ['notify']
  });
};

util.inherits(WUTNotifyCharacteristic, Characteristic);

WUTNotifyCharacteristic.prototype.onSubscribe = function(maxValueSize, updateValueCallback) {
  this._updateValueCallback = updateValueCallback;
  function(self) {
    driver.stdout.on('data', (data) => {
      if (data.length === 1) {
        console.log('stdout: ' +  data);
        self._updateValueCallback(new Buffer(data));
      }
    });
  }
};

WUTNotifyCharacteristic.prototype.onUnsubscribe = function() {
  console.log('on -> unsubscribe');

  this._updateValueCallback = null;
};
module.exports = WUTNotifyCharacteristic;
