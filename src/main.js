var bleno = require('bleno');
var WUTService = require('./wut-service');

var primaryService = new WUTService();

bleno.on('stateChange', function(state) {
  console.log('on -> stateChange: ' + state);

  if (state === 'poweredOn') {
    bleno.startAdvertising('Battery', [primaryService.uuid]);
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


bleno.on('servicesSet', function(error) {
	console.log('on -> servicesSet: ' + (error ? 'error ' + error : 'success'));
});

bleno.on('servicesSetError', function(error) {
    console.log('on -> servicesSetError: ' + (error ? 'error ' + error : 'success'));
});


bleno.on('accept', function(clientAddress) {
	console.log('on -> accept : ' + clientAddress);
});

bleno.on('disconnect', function(clientAddress) {
    console.log('on -> disconnect : ' + clientAddress);
});

