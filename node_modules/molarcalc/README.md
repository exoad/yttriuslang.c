# molarcalc

Very simple npm package, just give it a valid chemical formula and get its molar mass.

## How to use it
Install with:
`npm i molarcalc`

Then include this in your code:

`const molarcalc = require('molarcalc');`

`console.log(molarcalc.calc('NaCl'));`

Response should be:
`{ atoms: [ 'Na', 'Cl' ], mass: '58.4425' }`

`molarcalc.calc()` function returns a JSON object with an array of all the atoms found in the formula, and the calculated molar mass.

## What it can and cannot do
It recognises:
- Elements (written as their periodic table symbols).
- Frequent o-chem abbreviations such as 'Me' for methyl or 'Ph' for 'phenyl' (will probably add some more in the future).
- Numbers.
- Parentheses surrounding several elements. It will read everything inside and add it to the calculation a number of times indicated by the number after the closing parenthesis. Just a complicated way of saying that it knows how to read parentheses.

It does NOT recognise:
- Words. Won't recognise 'tri-' or 'acetyl'.
- 'Ac' for 'acetyl', as it creates a conflict with element Actinium. You will need to write 'CH3CO', 'C2H3O', 'COMe', etc.
