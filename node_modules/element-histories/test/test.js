const expect = require('chai').expect;
const elementHistories = require('../index.js');
const image = elementHistories.image;
const country = elementHistories.country;
const story = elementHistories.story;
const fullInfo = elementHistories.fullInfo;
const allNames = elementHistories.allNames;

describe('elementHistories', function() {
  describe('#image', function() {
    it('returns a string', function () {
      expect(image('silver')).to.be.a('string');
      expect(image('copper')).to.be.a('string');
      expect(image('helium')).to.be.a('string');
    });

    it('returns a URL link', function() {
      expect(image('fermium')).to.match(/^http/);
      expect(image('lead')).to.match(/^http/);
      expect(image('lithium')).to.match(/^http/);
    });

    it('throws with specific error message when element is misspelled or invalid', function() {
      let fn1 = function() { image('ironn') };
      let fn2 = function() { image('') };
      let fn3 = function() { image(1000) };
      expect(fn1).to.throw(Error, 'element name invalid');
      expect(fn2).to.throw(Error, 'element name invalid');
      expect(fn3).to.throw(Error, 'element name invalid');
    });

    it('is not case sensitive', function() {
      let elementImageLink1 = image('fLuOrInE');
      let elementImageLink2 = image('FLUORINE');
      expect(elementImageLink1).to.equal(elementImageLink2);
    });
  });

  describe('#country', function() {
    it('returns a country as a string if country is known', function() {
      expect(country('berkelium')).to.equal('USA');
      expect(country('cobalt')).to.equal('Sweden');
      expect(country('palladium')).to.equal('UK');
    });

    it('returns \'ancient\' as a string if element was known to ancient peoples and exact country is hard to identify', function() {
      expect(country('silver')).to.equal('ancient');
      expect(country('tin')).to.equal('ancient');
      expect(country('antimony')).to.equal('ancient');
    });

    it('throws with specific error message when element is misspelled or invalid', function() {
      let fn1 = function() { country('oxyygen') };
      let fn2 = function() { country('') };
      let fn3 = function() { country(2000) };
      expect(fn1).to.throw(Error, 'element name invalid');
      expect(fn2).to.throw(Error, 'element name invalid');
      expect(fn3).to.throw(Error, 'element name invalid');
    });

    it('is not case sensitive', function() {
      let discoveryCountry1 = country('Argon');
      let discoveryCountry2 = country('argon');
      expect(discoveryCountry1).to.equal(discoveryCountry2);
    });
  });

  describe('#story', function() {
    it('returns a string with a brief story behind each element', function() {
      expect(story('polonium')).to.be.a('string');
      expect(story('nobelium')).to.be.a('string');
      expect(story('rhodium')).to.be.a('string');
    });

    it('throws with specific error message when element is misspelled or invalid', function() {
      let fn1 = function() { story('neeon') };
      let fn2 = function() { story('') };
      let fn3 = function() { story(3000) };
      expect(fn1).to.throw(Error, 'element name invalid');
      expect(fn2).to.throw(Error, 'element name invalid');
      expect(fn3).to.throw(Error, 'element name invalid');
    });

    it('is not case sensitive', function() {
      let discoveryStory1 = story('cadmium');
      let discoveryStory2 = story('CADMIUM');
      expect(discoveryStory1).to.equal(discoveryStory2);
    });
  });

  describe('#fullInfo', function() {
    it('returns an object', function() {
      expect(fullInfo('silicon')).to.be.a('object');
      expect(fullInfo('yttrium')).to.be.a('object');
      expect(fullInfo('technetium')).to.be.a('object');
    });

    it('returns an object with properties of \'image\', \'country\', and \'story\'', function() {
      let fI = fullInfo('cerium');
      expect(fI).to.include.keys('image', 'country', 'story');
    });

    it('throws with specific error message when element is misspelled or invalid', function() {
      let fn1 = function() { fullInfo('led') };
      let fn2 = function() { fullInfo('') };
      let fn3 = function() { fullInfo(4000) };
      expect(fn1).to.throw(Error, 'element name invalid');
      expect(fn2).to.throw(Error, 'element name invalid');
      expect(fn3).to.throw(Error, 'element name invalid');
    });

    it('is not case sensitive', function() {
      let fullInfo1 = fullInfo('tellURIUM');
      let fullInfo2 = fullInfo('TELLurium');
      expect(fullInfo1).to.equal(fullInfo2);
    });
  });

  describe('#allNames', function() {
    let names = allNames();

    it('returns an array', function() {
      expect(names).to.be.a('array');
    });

    it('returns all 118 element names', function() {
      expect(names).to.have.lengthOf(118);
    });

    it('provides names in order of atomic number', function() {
      expect(names[0]).to.equal('hydrogen');
      expect(names[1]).to.equal('helium');
      expect(names[2]).to.equal('lithium');
      expect(names[names.length - 3]).to.equal('livermorium');
      expect(names[names.length - 2]).to.equal('ununseptium');
      expect(names[names.length - 1]).to.equal('ununoctium');
    })
  });
});
