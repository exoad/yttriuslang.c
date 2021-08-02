
# elapsed-time

[![Build status][travis-image]][travis-url]
[![Git tag][git-image]][git-url]
[![NPM version][npm-image]][npm-url]
[![Code style][standard-image]][standard-url]

Maintain a running counter of time elapsed from a certain point

## Installation

    $ npm install @f/elapsed-time

## Usage

```js
var elapsed = require('@f/elapsed-time')

var time = elapsed()

time() // ~0
setTimeout(function () {
  time() // ~20
  setTimeout(function () {
    time() // ~50
  }, 30)
}, 20)

```

## API

### elapsedTime(start)

- `start` - Optional starting point in milliseconds. If not passed, defaults to the current time.

**Returns:** A function that, when called, returns the elapsed time since `start`.

## License

MIT

[travis-image]: https://img.shields.io/travis/micro-js/elapsed-time.svg?style=flat-square
[travis-url]: https://travis-ci.org/micro-js/elapsed-time
[git-image]: https://img.shields.io/github/tag/micro-js/elapsed-time.svg
[git-url]: https://github.com/micro-js/elapsed-time
[standard-image]: https://img.shields.io/badge/code%20style-standard-brightgreen.svg?style=flat
[standard-url]: https://github.com/feross/standard
[npm-image]: https://img.shields.io/npm/v/@f/elapsed-time.svg?style=flat-square
[npm-url]: https://npmjs.org/package/@f/elapsed-time
