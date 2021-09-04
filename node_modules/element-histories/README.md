Chemical Element Histories
=========

NPM package that provides some historical info for each chemical element (backstory, country of discovery, and even an image of the element!)


<img src="http://smart-learning.co.uk/teachers-club/wp-content/uploads/cache/2014/12/elements-hero-image/3281568215.jpg" height="195px" width="620px"/>

## Installation

`npm install element-histories`

## Usage

Use simple, clean functions `story`, `country`, and `image` to gather the respective information about each element.

Gathering the story:
```js
var elementHistories = require('element-histories');

// provides fascinating backstory information for each chemical element,
// from the ancient elements to the most recently discovered
elementHistories.story('tin');
// "Tin had a direct impact on human history mainly on account of bronze, although it could be used in its own right, witness a tin ring and pilgrim bottle found in an Egyptian tomb of the eighteenth dynasty (1580–1350 BC). The Chinese were mining tin around 700 BC in the province of Yunnan. Pure tin has also been found at Machu Picchu, the mountain citadel of the Incas. When copper was alloyed with around 5 per cent of tin it produced bronze, which not only melted at a lower temperature, so making it easier to work, but produced a metal that was much harder, and ideal for tools and weapons. The Bronze Age is now a recognised stage in the development of civilisation. How bronze was discovered we do not know, but the peoples of Egypt, Mesopotamia, and the Indus valley started using it around 3000 BC."

elementHistories.story('einsteinium');
// "Einsteinium was discovered in the debris of the first thermonuclear explosion which took place on a Pacific atoll, on 1 November 1952. Fall-out material, gathered from a neighbouring atoll, was sent to Berkeley, California, for analysis. There it was examined by Gregory Choppin, Stanley Thompson, Albert Ghiorso, and Bernard Harvey. Within a month they had discovered and identified 200 atoms of a new element, einsteinium, but it was not revealed until 1955. The einsteinium had formed when some uranium atoms had captured several neutrons and gone through a series of capture and decay steps resulting in einsteinium-253, which has a half-life of 20.5 days. By 1961, enough einsteinium had been collected to be visible to the naked eye, and weighed, although it amounted to mere 10 millionths of a gram."
```

Gathering the country of discovery (Some elements have a more complex history involving multiple countries; one country is chosen and the story attempts to elucidate the history more in-depth. Also, a few elements have been known since ancient times and it's hard to pinpoint an exact location. In these instances, 'ancient' is returned):
```js
elementHistories.country('lithium');
// 'Sweden'
elementHistories.country('boron');
// 'France'
elementHistories.country('thallium');
// 'UK'
elementHistories.country('gold');
// 'ancient'
elementHistories.country('lead');
// 'ancient'
```

Gathering image information. This functions returns a string with a URL link to a cloud-hosted .jpg image of the element (In some cases, the chemical element may be so unstable or occur in such small quantities that a picture is impossible. The best replacement is chosen :) )
```js
elementHistories.image('thorium');
// 'http://res.cloudinary.com/dc2o3efbz/image/upload/v1480906354/elements/thorium.jpg'
elementHistories.image('uranium');
// http://res.cloudinary.com/dc2o3efbz/image/upload/v1480906424/elements/uranium.jpg
elementHistories.image('magnesium');
// http://res.cloudinary.com/dc2o3efbz/image/upload/v1480904391/elements/magnesium.jpg
```

Also, some useful functions have been defined to gather all the names of the elements in an array, and all the info for each element:
```js
elementHistories.allNames();
// returns an array of all names in order of atomic number: ['hydrogen', 'helium', etc.]

elementHistories.fullInfo('aluminum');
// returns {image: _, country: _, story: _} with the info populated
```

## Tests

`npm test`

## Credits

Royal Society of Chemistry for the history of each element:
(http://www.rsc.org/periodic-table)
