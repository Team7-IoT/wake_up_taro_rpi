var assert = require('assert');
var target = require('../main');
var BLENO_CONSTANT = require('../bleno-constant');

describe('MODULE: main.js', function() {
  describe('EVENT: stateChange', function() {
    it('PARAM: powered_on -> state : powered_on', function() {
      target.onStateChange(BLENO_CONSTANT.POWERED_ON);
      assert.equal(target.state, BLENO_CONSTANT.POWERED_ON);
    });
    it('PARAM: dummy -> state is OFF', function() {
      target.onStateChange('DUMMY');
      assert.notEqual(target.state, BLENO_CONSTANT.POWERED_ON);
      assert.equal(target.state, 'DUMMY');
    });
  });
});

