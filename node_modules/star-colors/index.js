const data = require('./data');
const keys = Object.keys(data);

const chromaticity = {
  get (type) {
    return data[type] || null;
  },

  randomRgb () {
    const stellar = data[keys[Math.round(Math.random() * (keys.length - 1))]];
    return {
      r: stellar.r,
      g: stellar.g,
      b: stellar.b
    };
  },

  randomHex () {
    return data[keys[Math.round(Math.random() * (keys.length - 1))]].hex;
  }
};

module.exports = chromaticity;
