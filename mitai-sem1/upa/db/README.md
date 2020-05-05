# Project database

This README provides some basic info that may come handy during implementation process.

## Scripts

Folder `db` contains useful scripts for manipulation with database.

`db-create-tables.sql`: Creates all required tables.

`db-create-others.sql`: Creates triggers, functions, etc..

`db-drop.sql`: Drops all tables and thus removes all data.

`db-fill-data.sql`: Initilizes database with preview data.

Created for SQLPlus.

## Database initialization with `SQLPlus`

1. Connect to the database server.
  
    Make sure you are in the directory containing required scripts. In this step
    I will describe how to connect to the school server `gort.fit.vutbr.cz`.

    To connect to the remote database server using `sqlplus` use following:
    ```php
    $ sqlplus "$USER"/"$PASS"@gort.fit.vutbr.cz:1521/orclpdb
    ```

    Where:
     * USER - FIT login
     * PASS - generated password
 
2. Load required scripts:

 * If you want to create tables for the first time use:
   ```php
   SQL> @db-create-tables
   SQL> @db-create-others
   ```
 * If you want to remove all data from database and create new tables use:
   ```php
   SQL> @db-drop
   SQL> @db-create-tables
   SQL> @db-create-others
   ```

### Usage from merlin

You can initialize database by ssh on `merlin`:

```
$ cat db/db-drop.sql db/db-create.sql  db/db-init.sql | ssh merlin "sqlplus $USER/$PASS@gort.fit.vutbr.cz:1521/orclpdb"
```
