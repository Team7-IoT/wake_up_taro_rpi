var bleno = require('bleno');
var PrimaryService = bleno.PrimaryService;
var Characteristic = bleno.Characteristic;
var Descriptor = bleno.Descriptor;

var primaryService = new PrimaryService({
    //uuid: '7e1bf98cbfa34a8fb6a782c3a851e0d0',
    uuid : '180f',
        characteristics: [new Characteristic({
        uuid : 'f9a318d8a1784ee18d47d92cdae9c907',
        properties : ['read'],
                descriptors: [
                                                new Descriptor({
                                                        uuid : '2901',
                                                        value : 'test descriptor'
                                                })
                                        ],
                onReadRequest : function(offset, callback) {
          var result = Characteristic.RESULT_SUCCESS;
          var data = new Buffer( [1, 2, 3] );
          callback(result, data);
        }
    })]
});

bleno.on('stateChange', function(state) {
  console.log('on -> stateChange: ' + state);

  if (state === 'poweredOn') {
    bleno.startAdvertising('bleno_test', [primaryService.uuid]);
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