var assert = require('assert');
var target = require('../main');
var BLENO_CONSTANT = require('../bleno-constant');

console.log('--- Module Check ---');
assert.ok(target);

console.log('------ test ---- start --------');

target.onStateChange(BLENO_CONSTANT.POWERED_ON);
assert.equal(target.state, BLENO_CONSTANT.POWERED_ON);

target.onStateChange('DUMMY');
assert.notEqual(target.state, BLENO_CONSTANT.POWERED_ON);
assert.equal(target.state, 'DUMMY');

// koko wo dou assert suruka kentou suru
console.log(target.onAdvertisingStart(true));
console.log(target.onAdvertisingStart(false));


console.log('------ test ---- end --------');

