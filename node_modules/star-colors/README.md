# star-colors

[![Build Status](https://travis-ci.com/fralonra/star-colors.svg?branch=master)](https://travis-ci.com/fralonra/star-colors)
[![npm version](https://img.shields.io/npm/v/star-colors.svg)](https://www.npmjs.com/package/star-colors)

Get realistic colors of stars in rgb or hex. Datas come from [Mitchell Charity's](http://www.vendian.org/mncharity/dir3/starcolor/UnstableURLs/starcolors.txt).

## Install

```bash
npm install --save star-colors
```

## Usage

```javascript
const starColors = require('star-colors');

const O5V = starColors.get('O5V')；
// O5V
// {
//   "class": "O5V",
//   "r": "155",
//   "g": "176",
//   "b": "255",
//   "hex": "#9bb0ff"
// }
```

Or in **Html**:
```html
<script src="/path/to/star-colors.min.js"></script>
```

## API

### get (type)
Return the entire data object of a specific [stellar class](https://en.wikipedia.org/wiki/Stellar_classification#Spectral_types). Including the following properities: `class`, `r`, `g`, `b`, `hex`.

### randomRgb ()
Return a random color in rgb code from data.

### randomHex ()
Return a random color in hex code from data.
