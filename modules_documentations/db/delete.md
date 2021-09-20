## Delete Method

This method is used to delete some data from the database.

```javascript
db.delete('data-name');
```

This method works with all types of saved data.
* The first parameter can only be a `String`.

### Example

```javascript
const db = require('secure-db');

db.set('name', 'Jhon');
db.set('my_money', 700);
db.set('cooldown', 4000)

db.delete('name');
db.all(); // [ { ID: 'my_money", data: 700 } ]

db.delete(['cooldown', 'my_money']);
db.all(); // []
```
