'use strict';
let allElements = require('./element-info.js');

module.exports = {
  image: function(elementName) {
    if (typeof elementName === 'string' && elementName.toLowerCase() in allElements) {
      return allElements[elementName.toLowerCase()]['image'];
    } else {
      throw new Error('element name invalid');
    }
  },

  country: function(elementName) {
    if (typeof elementName === 'string' && elementName.toLowerCase() in allElements) {
      return allElements[elementName.toLowerCase()]['country'];
    } else {
      throw new Error('element name invalid');
    }
  },

  story: function(elementName) {
    if (typeof elementName === 'string' && elementName.toLowerCase() in allElements) {
      return allElements[elementName.toLowerCase()]['story'];
    } else {
      throw new Error('element name invalid');
    }
  },

  fullInfo: function(elementName) {
    if (typeof elementName === 'string' && elementName.toLowerCase() in allElements) {
      return allElements[elementName.toLowerCase()];
    } else {
      throw new Error('element name invalid');
    }
  },

  allNames: function() {
    return Object.keys(allElements);
  }
}
