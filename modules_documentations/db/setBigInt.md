## SetBigInt Method

This method is used to save numbers above 1e + 20 (100000000000000000000).

```javascript
db.setBigInt('data-name', 'big-number');
```

With this method, the numbers have no size limit.
* The first parameter can only be a `String`.
* The second parameter can only be a `Number`.

### Example

```javascript
const db = require('secure-db');

db.set('my_big_number', 50000000000000000000000000); // my_big_number = null
db.setBigInt('my_big_number', 50000000000000000000000000); // my_big_number = 50000000000000000000000000
```
