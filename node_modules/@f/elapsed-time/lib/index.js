/**
 * Modules
 */

var timestamp = require('@f/timestamp')

/**
 * Expose elapsedTime
 */

module.exports = elapsedTime

/**
 * elapsedTime
 */

function elapsedTime (start) {
  var t = start === undefined ? timestamp() : start
  return function () {
    return timestamp() - t
  }
}
