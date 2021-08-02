/**
 * Imports
 */

var elapsed = require('..')
var test = require('tape')

/**
 * Tests
 */

test('should work', function (t) {
  var time = elapsed()

  t.ok(time() < 5)
  setTimeout(function () {
    t.ok(time() < 40)
    t.ok(time() > 10)
    t.end()
  }, 20)
})
