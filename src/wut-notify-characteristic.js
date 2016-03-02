var util = require('util');
var bleno = require('bleno');

var Descriptor = bleno.Descriptor;
var Characteristic = bleno.Characteristic;
var ChildProcess = require('child_process');
var driver = ChildProcess.spawn('/home/rpi/wake_up_taro_rpi/src/okoshi_driver', ['-s']);

var WUTNotifyCharacteristic = function() {
  WUTNotifyCharacteristic.super_.call(this, {
    uuid: '7BD98C73E66148B5B09396C4EC897B9C',
    properties: ['notify']
  });
};

util.inherits(WUTNotifyCharacteristic, Characteristic);

WUTNotifyCharacteristic.prototype.onSubscribe = function(maxValueSize, updateValueCallback) {
  console.log('on -> subscribe');
  this._updateValueCallback = updateValueCallback;
  var self = this;
  driver.stdout.removeAllListeners('data');
  driver.stdout.on('data', (data) => {
    console.log('stdout: ' +  data);
    self._updateValueCallback(new Buffer(data));
  });
};

WUTNotifyCharacteristic.prototype.onUnsubscribe = function() {
  console.log('on -> unsubscribe');
  driver.stdout.removeAllListeners('data');
  console.log('on -> removeListeners');
  this._updateValueCallback = null;
};

module.exports = WUTNotifyCharacteristic;

