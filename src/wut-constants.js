var WutConstants = {
  UUID : {
    WUTNotifyCharacteristic : '7BD98C73E66148B5B09396C4EC897B9C',
    WUTSensorControlCharacteristic : '04BDBB401F5C453FA9E1333BC8930826',
    WUTSensorStateCharacteristic : 'F9A318D8A1784EE18D47D92CDAE9C907',
    WUTService : '7E1BF98CBFA34A8FB6A782C3A851E0D0'
  },
  PROPERTIES : {
    WUTNotifyCharacteristic : ['notify'],
    WUTSensorControlCharacteristic : ['writeWithoutResponse'],
    WUTSensorStateCharacteristic : ['read']
  },
}

module.exports = WutConstants;
