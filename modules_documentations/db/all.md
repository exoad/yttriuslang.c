## All Method

The all method is used to return all data saved in the database. It has some [extra options](#extra-options) that are used to manage the data return.

```javascript
db.all();
db.all({options});
```

This method works with all types of data that can be saved.
* The first parameter can only be an `Object`.

### Example

```javascript
const db = require('secure-db');

db.set('my_money', 800);
db.set('user', 'David');
db.set('afk-mode', false);

db.all();
/*
[
  { ID: 'my_money', data: 800 },
  { ID: 'user', data: 'David' },
  { ID: 'afk-mode', data: false }
]
*/

db.all({ startsWith: 'm' }); // [ { ID: 'my_money', data: 800 } ]
db.all({ endsWith: 'mode' }); // [ { ID: 'afk-mode', data: false } ]

db.all({ type: 'number' }); // [ { ID: 'my_money: data: 800 } ]
db.all({ type: 'string' }); // [ { ID: 'user', data: 'David' } ]
db.all({ type: 'boolean' }); // [ { ID: 'afk-mode', data: false } ]

db.set("my_friend's_money", 450);
db.all({ sort: 'a-z', type: 'number' });
/*
[
  { ID: "my_friend's_money", data: 450 },
  { ID: 'my_money', data: 800 }
]
*/
db.all({ sort: 'z-a', type: 'number'});
/*
  { ID: 'my_money', data: 800 },
  { ID: "my_friend's_money", data: 450 }
*/

db.set('Felipe', { money: 2080, age: 25 });
db.set('Lia', { money: 40300, age: 19 });
db.all({ sort: 'z-a', sortSource: 'money', type: 'object' });
/*
[
  { ID: 'Lia', data: { money: 40300, age: 19 } },
  { ID: 'Felipe', data: { money: 2080, age: 25 } }
]
*/
db.all({ sort: 'z-a', sortSource: 'age', type: 'object' });
/*
[
  { ID: 'Felipe', data: { money: 2080, age: 25 } },
  { ID: 'Lia', data: { money: 40300, age: 19 } }
]
*/
```
### Extra Options

All of the following options must be within and object.

* startsWith - Returns all data that the name starts with the entered value.
* endsWith - Returns all data that the name ends with the entered value.
* type - Returns all data that the type is equal to the entered value.
* sort - Returns all data in an organized way, whether alphabetical or the other way around. 
* sortSource - Defines the depth of research for the organization. _This option will only work if the sort option is set._
