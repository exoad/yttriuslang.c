## Remove method

This method is used to remove a database/child.

```javascript
<database/child>.remove();
```

This method will remove your database.

> Once used, the command will delete a database/child forever without reverting.

* No parameters.

### Exemple

```javascript
const db = require('secure-db');

db.set('name', 'Jhon');
db.set('my_money', 700);
db.set('cooldown', 4000);

db.remove();

db.get('name'); // null
db.get('my_money') // null
db.all(); // null
```