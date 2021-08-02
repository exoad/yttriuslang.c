/**
 * Imports
 */

var timestamp = require('..')
var test = require('tape')

/**
 * Tests
 */

test('should work', function (t) {
  t.ok(timestamp() <= new Date().getTime())
  t.ok(typeof timestamp() === 'number')
  t.end()
})
