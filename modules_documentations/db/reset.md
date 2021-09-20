## Reset Method

This method is used to reset the database.

```javascript
db.reset()
db.reset({options});
```

You can choose what data you want to keep.You can choose what data you want to keep.
* The first parameter can only be a `Object`.

### Example

```javascript
const db = require('secure-db');

db.set('lucas', { age: 40 });
db.set('my_money', 210);
db.set('shop', { items: [] });
db.set('users', [ 'Pedro', 'Alfred' ]);

db.reset();

db.get('lucas'); // null
db.get('shop'); // null...
```
Keep option: Serves to keep the defined data saved, and will not be deleted.
```javascript
const db = require('secure-db');

db.set('lucas', { age: 40 });
db.set('my_money', 210);
db.set('shop', { items: [] });
db.set('users', [ 'Pedro', 'Alfred' ]);

db.reset({ keep: ['lucas', 'shop']});

db.get('my_money'); // null
db.get('users'); // null
db.get('lucas'); // { age: 40 }
db.get('shop'); // { items: [] }
```
