var assert = require('assert');
var WUTNotifyCharacteristic  = require('../wut-notify-characteristic');

console.log('--- Module Check ---');
assert.ok(WUTNotifyCharacteristic);

console.log('------ test ---- start --------');

var target = new WUTNotifyCharacteristic();
target.onUnsubscribe();



console.log('------ test ---- end --------');
