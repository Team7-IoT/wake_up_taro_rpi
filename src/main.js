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


bleno.on(BLENO_CONSTANT.SERVICE_SET, function(error) {
    console.log('on -> servicesSet: ' + (error ? 'error ' + error : 'success'));
});

bleno.on(BLENO_CONSTANT.SERVICE_SET_ERROR, function(error) {
    console.log('on -> servicesSetError: ' + (error ? 'error ' + error : 'success'));
});


bleno.on(BLENO_CONSTANT.ACCEPT, function(clientAddress) {
    console.log('on -> accept : ' + clientAddress);
});

bleno.on(BLENO_CONSTANT.DISCONNECT, function(clientAddress) {
    console.log('on -> disconnect : ' + clientAddress);
});

module.exports = bleno;
