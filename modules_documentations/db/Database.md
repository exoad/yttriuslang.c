## Database Method

This method is used to create a new database separated by folder.

```javascript
new db.Database('database-name', Encrypt?);
```

If you enter a name that already exists, a new database will not be created, it will save the data in the existing database.
* The first parameter can only be a `String`.
* The second parameter can only be a `Boolean`.

### Example

```javascript
const db = require('secure-db');

const Felipe = new db.Database('Felipe');
Felipe.set('age', 30);

db.get('age'); // null
Felipe.get('age'); // 30
```
