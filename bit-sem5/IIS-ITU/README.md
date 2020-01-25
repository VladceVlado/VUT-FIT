# Informacni system OAFL



## Manage project

**Make migrations**  
```
$ python manage.py makemigrations
```

**Migrate**  
```
$ python manage.py migrate
```

**Run server**  
```
$ python manage.py runserver
```

**Create super user**  
```
$ python manage.py createsuperuser
```

**Command for fill database**  
```
$ python manage.py fill_db
```

**Command for clean database**  
```
$ python manage.py clean_db
```

**Fill database with fixtures/data.json by fixture**  
```
$ python manage.py loaddata data.json
```

**Dumpdata from database**  
```
$ python manage.py dumpdata
```



## Development setup


### Virtual enviroment

**Initialization**  
```
$ virtualenv env
```

**Activate**  
```
$ source env/bin/activate
```

**Deactivate**  
```
(env) $ deactivate
```

**Install python dependencies to enviroment**  
```
(env) # pip install django
(env) # pip install psycopg2
```


### PostgreSQL database

**Install, initialize, run**  
- According to official website - [postgresql.org/download/](https://www.postgresql.org/download/)

**Set local connections as trust**  
- Restart daemon for apply changes  
```
# vim /var/lib/pgsql/11/data/pg_hba.conf
```
```
# TYPE  DATABASE        USER            ADDRESS                 METHOD
# "local" is for Unix domain socket connections only
local   all             all                                     trust
# IPv4 local connections:
host    all             all             127.0.0.1/32            trust
# IPv6 local connections:
host    all             all             ::1/128                 trust
```

**Manage the database**  
```
$ psql -d postgres -U postgres
```

**Drop database**  
```
$ dropdb -U postgres -W postgres
```

**Create database**  
```
$ createdb
```



### Manage system services

**Check service status**  
```
$ systemctl status postgresql-11.service
```

**Start service**  
```
# systemctl start postgresql-11.service
```

**Enable start service after start OS**  
```
# systemctl enable postgresql-11.service
```

**Restart service**  
```
# systemctl restart postgresql-11.service
```



### Resources
- https://fedoraproject.org/wiki/PostgreSQL  
