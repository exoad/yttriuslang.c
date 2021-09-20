## Sub Method

This method is used to subtract a value from a number.

```javascript
db.sub('data-name', number);
db.subtract('data-name', number);
```

This method also removes text from a string.
* The first parameter can only be a `String`.
* The second parameter can only be a `Number` or a `String`.

### Example

```javascript
const db = require('secure-db');

db.set('my_money', 500);
db.sub('my_money', 20); // 480
```
