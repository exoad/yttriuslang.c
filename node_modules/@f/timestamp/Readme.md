
# timestamp

[![Build status][travis-image]][travis-url]
[![Git tag][git-image]][git-url]
[![NPM version][npm-image]][npm-url]
[![Code style][standard-image]][standard-url]

Get the current timestamp in milliseconds. Provides a more semantic and consistent interface than `+new Date()` and other weird js idioms.

## Installation

    $ npm install @f/timestamp

## Usage

```js
var timestamp = require('@f/timestamp')

timestamp() === new Date().getTime()
timestamp() === +new Date
```

## API

### timestamp()

**Returns:** The current timestamp in milliseconds.

## License

MIT

[travis-image]: https://img.shields.io/travis/micro-js/timestamp.svg?style=flat-square
[travis-url]: https://travis-ci.org/micro-js/timestamp
[git-image]: https://img.shields.io/github/tag/micro-js/timestamp.svg
[git-url]: https://github.com/micro-js/timestamp
[standard-image]: https://img.shields.io/badge/code%20style-standard-brightgreen.svg?style=flat
[standard-url]: https://github.com/feross/standard
[npm-image]: https://img.shields.io/npm/v/@f/timestamp.svg?style=flat-square
[npm-url]: https://npmjs.org/package/@f/timestamp
