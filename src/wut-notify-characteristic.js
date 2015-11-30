var util = require('util');
var bleno = require('bleno');

var Descriptor = bleno.Descriptor;
var Characteristic = bleno.Characteristic;


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
  setTimeout(
  	function(self) {
 		console.log('on -> timer');
  		self._updateValueCallback(new Buffer([0, 1, 2, 3]));
  	}, 2000, this);
};

WUTNotifyCharacteristic.prototype.onUnsubscribe = function() {
  console.log('on -> unsubscribe');

  this._updateValueCallback = null;
};
module.exports = WUTNotifyCharacteristic;