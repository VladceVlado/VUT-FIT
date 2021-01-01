# Exercise 1

- Apache Pig
- Apache Hadoop
- Apache Spark

## Priprava

### Oracle VM VirtualBox
- VirtualBox import Appliance
- Network settings viz screenshots
- Pomoci vychozi konzole z VirtualBoxu zjistit ip adresu pomoci "ip adddr"
- Expose porty viz screenshots

### Putty
- Pomoci SSH se pripojit na zjistenou ip adresu
- Credentials:
    user: demo
    password: demo

### WinSCP
- Prihlaseni stejne jako u Putty
- Alternativne nastavit sdilenou volume do virtualky, pak FTP klient neni treba

### IDEA
- Java/Scala projekt

## Resources
- https://rychly-edu.gitlab.io/dist-app-environment/howtos/nixos-distapp-vm/
- https://rychly-edu.gitlab.io/dist-app-environment/lab/

## Other

- UIs:
    - http://localhost:4040/
    - http://localhost:9870/
    - http://localhost:9864/

HDFS prikazy:
```
$ hdfs dfs -ls
```

```
$ hdfs dfs -cat ...
```

```
$ hdfs dfs -get ...
```

```
$ hdfs dfs -rm -r -f output
```

- Embedden Hadoop vs Hadoop Cluster
