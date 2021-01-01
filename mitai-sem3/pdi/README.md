# PDI 2020/2021

<!----------------------------------------------------------------------------->

## Zadání

- Varianta termínu: [Jeřábek] Zprovozněte a vyzkoušejte Packet Beat s Elastic search a reportováním do Kibana

### Popis varianty
- Vyzkoušejte a zprovozněte nástroj pro práci se síťovými daty Packet Beat v kombinaci s Elastic Search a reportováním a vizualizací v Kibana.
- Prostudujte si jednotlivé nástroje a pokuste se vše nastavit a lokálně spustit pomocí Docker kontejnerů.
- Váš výsledný správný postup pro nastavení zaznamenejte v krátké dokumentaci (spíše postup) a výsledné řešení demonstrujte.
- Primárně se zaměřte na získání informací z flow a TLS.

**Požadované výstupy:**
- Připravte docker-compose konfigurační soubor(y) pro zprovoznění celého systému.
- Krátká dokumentace s postupem nasazení.
- Demonstrace funkčnosti.

**Reference:**
- https://www.elastic.co/guide/en/beats/packetbeat/current/index.html

<!----------------------------------------------------------------------------->

## Popis

```
$ sudo sysctl --all | grep vm.max_map_count
```

```
$ sudo sysctl --write vm.max_map_count=262144
```

## Docker commands

- [stackoverflow.com/questions/40801772](https://stackoverflow.com/questions/40801772/what-is-the-difference-between-docker-compose-ports-vs-expose)

### Ports

- Expose ports.
- Either specify both ports (HOST:CONTAINER), or just the container port (a random host port will be chosen).

```
mysql:
  image: mysql:5.7
  ports:
    - "3306"
```

```
$ docker-compose ps
Name                     Command               State            Ports
-------------------------------------------------------------------------------------
  mysql_1       docker-entrypoint.sh mysqld      Up      0.0.0.0:32769->3306/tcp
```

### Expose

- Expose ports without publishing them to the host machine - they’ll only be accessible to linked services.
- Only the internal port can be specified.

```
mysql:
  image: mysql:5.7
  expose:
    - "3306"
```

```
$ docker-compose ps
  Name                  Command             State    Ports
---------------------------------------------------------------
 mysql_1      docker-entrypoint.sh mysqld   Up      3306/tcp
```

### NET_ADMIN

- [man7.org/linux/man-pages/man7/capabilities.7.html](https://man7.org/linux/man-pages/man7/capabilities.7.html)

```
cap_add:
  - NET_ADMIN
```

```
CAP_NET_ADMIN
    Perform various network-related operations:
    * interface configuration;
    * administration of IP firewall, masquerading, and accounting;
    * modify routing tables;
    * bind to any address for transparent proxying;
    * set type-of-service (TOS);
    * clear driver statistics;
    * set promiscuous mode;
    * enabling multicasting;
    * use setsockopt(2) to set the following socket options:
    SO_DEBUG, SO_MARK, SO_PRIORITY (for a priority outside the
    range 0 to 6), SO_RCVBUFFORCE, and SO_SNDBUFFORCE.
```
