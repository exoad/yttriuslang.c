## GetChilds Method

This method serves to return the name of all [`Child Databases`](https://github.com/DablioZe/SecureDB/tree/main/docs/Child.md) that are saved in a `Database Folder`.

```javascript
db.getChilds('database-name', callback function);
```

If there is no child database, the returned value will be an empty Array.
* The first parameter can only be a `String`.
* The second parameter can only be a `Function`.

### Example

```javascript
const db = require('secure-db');

const Felipe = new db.Child('users', 'Felipe');
const Lucas = new db.Child('users', 'Lucas');

Felipe.set('age', 45);
Lucas.set('age', 19);

db.getChilds('users', function( user_list ){
  // user_list = [ 'Felipe', 'Lucas']
})
```
