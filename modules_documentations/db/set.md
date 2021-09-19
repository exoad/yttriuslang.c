## Set Method

This method is used to define data in the database.

```javascript
db.set('data-name', 'data-value');
```

This method overwrites data that is already saved.
* The first parameter can only be a `String`.
* The second parameter cannot be a `Function` or `Class`.

### Example

```javascript
const db = require('secure-db');

db.set('money', 40);
db.get('money'); // 40

db.set('money', 5000);
db.get('money'); // 5000
```
