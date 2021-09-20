## Child Method

This method is used to create a database within another database and is separated by files.

```javascript
new db.Child('database-name', 'child-name');
```

This method completely separates one database from another, making it individual.
* The first parameter can only be a `String`.
* The second parameter can only be a `String`.

```javascript
const { Database, Child } = require('secure-db');

const users = new Database('users');
const Felipe = new Child('users', 'Felipe');
const Lucas = new Child('users', 'Lucas');

Felipe.set('age', 62);

users.get('age'); // null
Lucas.get('age'); // null
Felipe.get('age'); // 62
```
