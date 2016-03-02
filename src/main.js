var bleno = require('bleno');
var WUTService = require('./wut-service');
var primaryService = new WUTService();

bleno.on('stateChange', function(state) {
  console.log('on -> stateChange: ' + state);

  if (state === 'poweredOn') {
    bleno.startAdvertising('wake_up_taro', [primaryService.uuid]);
  } else {
    bleno.stopAdvertising();
  }
});

bleno.on('advertisingStart', function(error) {
  console.log('on -> advertisingStart: ' + (error ? 'error ' + error : 'success'));

  if (!error) {
    bleno.setServices([primaryService], function(error){
      console.log('setServices: '  + (error ? 'error ' + error : 'success'));
    });
  }
});

module.exports = bleno;
