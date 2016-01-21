var bleno = require('bleno');
var WUTService = require('./wut-service');
var primaryService = new WUTService();
var BLENO_CONSTANT = require('./bleno-constant');

bleno.on(BLENO_CONSTANT.POWERED_ON, function(state) {
  console.log('on -> stateChange: ' + state);

  if (state === BLENO_CONSTANT.POWERED_ON) {
    bleno.startAdvertising('wake_up_taro', [primaryService.uuid]);
  } else {
    bleno.stopAdvertising();
  }
});

bleno.on(BLENO_CONSTANT.ADDRESS_CHANGE, function(error) {
  console.log('on -> advertisingStart: ' + (error ? 'error ' + error : 'success'));

  if (!error) {
    bleno.setServices([primaryService], function(error){
      console.log('setServices: '  + (error ? 'error ' + error : 'success'));
    });
  }
});

module.exports = bleno;
