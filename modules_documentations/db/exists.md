## Exists method

This method is to check if a database/child exists or not.

```javascript
db.exists('database-name');
// or
db.exists('database-name', 'child-name'); // this will only delete the child.
```
This method returns boolean.
* The first parameter must necessarily be a `String`.
* The second parameter must necessarily be a `String`.

```javascript
const { Database, Child, exists } = require('secure-db');

const db = new Database('my-database');
const cd = new Child('my-database', 'my-child');

exists('my-database'); // true
exists('games'); // false

exists('my-database', 'my-child'); // true
exists('my-database', 'my-second-child'); // false
```