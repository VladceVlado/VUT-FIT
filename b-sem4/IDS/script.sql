/**
 * VUT FIT 2BIT
 * IDS 2017/2018
 * Semester project
 * Authors: Vladimir Dusek (xdusek27), Tomas Kukan (xkukan00)
 * Date: 2/5/2018
 * File: skript.sql
 */

/*********************************************************************************************************
 * Odstraneni vsech objektu v databazi, pokud nektere objekty v databazi nejsou, nastane chyba
 */

DROP MATERIALIZED VIEW LOG ON Spolecnost;
DROP MATERIALIZED VIEW LOG ON Vlak;
DROP MATERIALIZED VIEW mv_vlak_spolecnost;
DROP INDEX idx_uz_email;
DROP PROCEDURE sp_kontrola_email;
DROP PROCEDURE sp_kontrola_vlak_id;
DROP TRIGGER jizdenka_id_auto_inc;
DROP TRIGGER check_spolecnost_ico;
DROP SEQUENCE sq_last_jizdenka_id;
DROP TABLE vz_Jizdenka_Sluzba;
DROP TABLE vz_Spoj_Trasa;
DROP TABLE vz_Spoj_Vlak;
DROP TABLE vz_Spoj_Spolecnost;
DROP TABLE vz_Vlak_Sluzba;
DROP TABLE Jizdenka;
DROP TABLE Spoj;
DROP TABLE Trasa;
DROP TABLE Vlak;
DROP TABLE Sluzba;
DROP TABLE Spolecnost;
DROP TABLE Uzivatel;

/*********************************************************************************************************
 * Vytvoreni vsech tabulek
 */

CREATE TABLE Uzivatel (
    email VARCHAR(50) NOT NULL,
    jmeno VARCHAR(50) NOT NULL,
    prijmeni VARCHAR(50) NOT NULL,
    datum_narozeni TIMESTAMP,
    telefon VARCHAR(13) NOT NULL, -- format '+420777888999'
    adresa VARCHAR(50) NOT NULL
);

CREATE TABLE Jizdenka (
    id INTEGER NOT NULL,
    priplatek_kc INTEGER, -- priplatek za sluzbu nebo vlak
    sleva_prc INTEGER,
    celkova_cena_kc INTEGER NOT NULL,
    uzivatel_email VARCHAR(50) NOT NULL,
    spoj_id INTEGER NOT NULL
);

CREATE TABLE Spoj (
    id INTEGER NOT NULL,
    odkud VARCHAR(50) NOT NULL,
    kam VARCHAR(50) NOT NULL,
    doba_prepravy_min INTEGER NOT NULL,
    cas TIMESTAMP NOT NULL, -- kdy vyjizdi ze stanice odkud
    prumerne_zpozdeni_min INTEGER NOT NULL,
    trasa_id INTEGER NOT NULL
);

CREATE TABLE Trasa (
    id INTEGER NOT NULL,
    odkud VARCHAR(50) NOT NULL,
    kam VARCHAR(50) NOT NULL,
    pres VARCHAR(500),
    delka_trasy_km INTEGER NOT NULL
);

CREATE TABLE Vlak (
    id INTEGER NOT NULL,
    jmeno VARCHAR(50) NOT NULL,
    typ VARCHAR(50) NOT NULL,
    priplatek_kc INTEGER,
    prumerna_rychlost_km INTEGER NOT NULL,
    spolecnost_id INTEGER NOT NULL
);

CREATE TABLE Sluzba (
    id INTEGER NOT NULL,
    typ VARCHAR(50) NOT NULL,
    cena_kc INTEGER NOT NULL
);

CREATE TABLE Spolecnost (
    id INTEGER NOT NULL,
    nazev VARCHAR(50) NOT NULL,
    jmeno_vlastnika VARCHAR(50),
    ico VARCHAR(8)
);

CREATE TABLE vz_Jizdenka_Sluzba (
    jizdenka_id INTEGER NOT NULL,
    sluzba_id INTEGER NOT NULL
);

CREATE TABLE vz_Spoj_Trasa (
    spoj_id INTEGER NOT NULL,
    trasa_id INTEGER NOT NULL
);

CREATE TABLE vz_Spoj_Vlak (
    spoj_id INTEGER NOT NULL,
    vlak_id INTEGER NOT NULL
);

CREATE TABLE vz_Spoj_Spolecnost (
    spoj_id INTEGER NOT NULL,
    spolecnost_id INTEGER NOT NULL
);

CREATE TABLE vz_Vlak_Sluzba (
    vlak_id INTEGER NOT NULL,
    sluzba_id INTEGER NOT NULL
);

/*********************************************************************************************************
 * Nastaveni primarnich a cizich klicu
 */

ALTER TABLE Uzivatel ADD CONSTRAINT pk_uzivatel PRIMARY KEY (email);

ALTER TABLE Jizdenka ADD CONSTRAINT pk_jizdenka PRIMARY KEY (id);

ALTER TABLE Spoj ADD CONSTRAINT pk_spoj PRIMARY KEY (id);

ALTER TABLE Trasa ADD CONSTRAINT pk_trasa PRIMARY KEY (id);

ALTER TABLE Vlak ADD CONSTRAINT pk_vlak PRIMARY KEY (id);

ALTER TABLE Sluzba ADD CONSTRAINT pk_sluzba PRIMARY KEY (id);

ALTER TABLE Spolecnost ADD CONSTRAINT pk_spolecnost PRIMARY KEY (id);

ALTER TABLE vz_Jizdenka_Sluzba ADD CONSTRAINT pk_js_id PRIMARY KEY (jizdenka_id, sluzba_id);

ALTER TABLE vz_Spoj_Trasa ADD CONSTRAINT pk_st_id PRIMARY KEY (spoj_id, trasa_id);

ALTER TABLE vz_Spoj_Vlak ADD CONSTRAINT pk_sv_id PRIMARY KEY (spoj_id, vlak_id);

ALTER TABLE vz_Spoj_Spolecnost ADD CONSTRAINT pk_ss_id PRIMARY KEY (spoj_id, spolecnost_id);

ALTER TABLE vz_Vlak_Sluzba ADD CONSTRAINT pk_vs_id PRIMARY KEY (vlak_id, sluzba_id);


ALTER TABLE Jizdenka ADD CONSTRAINT fk_j_spoj_id FOREIGN KEY (spoj_id) REFERENCES Spoj (id);

ALTER TABLE Jizdenka ADD CONSTRAINT fk_j_uzivatel_email FOREIGN KEY (uzivatel_email) REFERENCES Uzivatel (email);

ALTER TABLE Spoj ADD CONSTRAINT fk_s_trasa_id FOREIGN KEY (trasa_id) REFERENCES Trasa (id);

ALTER TABLE Vlak ADD CONSTRAINT fk_v_spolecnost_id FOREIGN KEY (spolecnost_id) REFERENCES Spolecnost (id);

ALTER TABLE vz_Jizdenka_Sluzba ADD CONSTRAINT fk_js_jizdenka_id FOREIGN KEY (jizdenka_id) REFERENCES Jizdenka (id);

ALTER TABLE vz_Jizdenka_Sluzba ADD CONSTRAINT fk_js_sluzba_id FOREIGN KEY (sluzba_id) REFERENCES Sluzba (id);

ALTER TABLE vz_Spoj_Trasa ADD CONSTRAINT fk_st_spoj_id FOREIGN KEY (spoj_id) REFERENCES Spoj (id);

ALTER TABLE vz_Spoj_Trasa ADD CONSTRAINT fk_st_trasa_id FOREIGN KEY (trasa_id) REFERENCES Trasa (id);

ALTER TABLE vz_Spoj_Vlak ADD CONSTRAINT fk_sv_spoj_id FOREIGN KEY (spoj_id) REFERENCES Spoj (id);

ALTER TABLE vz_Spoj_Vlak ADD CONSTRAINT fk_sv_vlak_id FOREIGN KEY (vlak_id) REFERENCES Vlak (id);

ALTER TABLE vz_Spoj_Spolecnost ADD CONSTRAINT fk_ss_spoj_id FOREIGN KEY (spoj_id) REFERENCES Spoj (id);

ALTER TABLE vz_Spoj_Spolecnost ADD CONSTRAINT fk_ss_spolecnost_id FOREIGN KEY (spolecnost_id) REFERENCES Spolecnost (id);

ALTER TABLE vz_Vlak_Sluzba ADD CONSTRAINT fk_vs_vlak_id FOREIGN KEY (vlak_id) REFERENCES Vlak (id);

ALTER TABLE vz_Vlak_Sluzba ADD CONSTRAINT fk_vs_sluzba_id FOREIGN KEY (sluzba_id) REFERENCES Sluzba (id);

/*********************************************************************************************************
 * Vlozeni dat do tabulek pro dalsi praci s databazi
 */

INSERT INTO Uzivatel (email, jmeno, prijmeni, telefon, adresa)
VALUES ('josef.novak@gmail.com', 'Josef', 'Novak', '+420999888777', 'nam. A. Jiraska 1, 563 01, Lanskroun');

INSERT INTO Uzivatel (email, jmeno, prijmeni, telefon, adresa)
VALUES ('petr.maly@gmail.com', 'Petr', 'Maly', '+420888777666', 'Zamek 2, 530 02, Pardubice I');

INSERT INTO Uzivatel (email, jmeno, prijmeni, telefon, adresa)
VALUES ('tomas.studeny@seznam.cz', 'Tomas', 'Studeny', '+420777666555', 'Gajdosova 3, 615 00, Brno-Zidenice');


INSERT INTO Spolecnost (id, nazev, jmeno_vlastnika, ico)
VALUES (1, 'Dobre Drahy', 'Maxmilian Vevoda', '13693671');

INSERT INTO Spolecnost (id, nazev, jmeno_vlastnika, ico)
VALUES (2, 'StarkJet', 'Ferdinand Stark', '00030996');


INSERT INTO Trasa (id, odkud, kam, pres, delka_trasy_km)
VALUES (1, 'Graz Hbf', 'Praha hl.n.', 'Wien Hbf - Breclav - Brno hl.n. - Blansko - Ceska Trebova - Pardubice hl.n.', 520);

INSERT INTO Trasa (id, odkud, kam, pres, delka_trasy_km)
VALUES (2, 'Praha hl.n.', 'Havirov', 'Pardubice hl.n. - Ceska Trebova - Zabreh na Morave - Olomouc hl.n. - Hranice na Morave - Ostrava hl.n.', 390);

INSERT INTO Trasa (id, odkud, kam, pres, delka_trasy_km)
VALUES (3, 'Lanskroun', 'Ceska Trebova', 'Rudoltice v Cechach - Trebovice v Cechach', 30);

INSERT INTO Trasa (id, odkud, kam, pres, delka_trasy_km)
VALUES (4, 'Praha hl.n.', 'Cheb', 'Plzen - Stribro - Plana u Mar. Lazni - Marianske Lazne', 210);


INSERT INTO Spoj (id, odkud, kam, doba_prepravy_min, cas, prumerne_zpozdeni_min, trasa_id)
VALUES (1, 'Brno', 'Blansko', 30, TO_TIMESTAMP('14:45', 'HH24:MI'), 3, 1);

INSERT INTO Spoj (id, odkud, kam, doba_prepravy_min, cas, prumerne_zpozdeni_min, trasa_id)
VALUES (2, 'Pardubice', 'Ceska Trebova', 45, TO_TIMESTAMP('18:20', 'HH24:MI'), 6, 2);

INSERT INTO Spoj (id, odkud, kam, doba_prepravy_min, cas, prumerne_zpozdeni_min, trasa_id)
VALUES (3, 'Lanskroun', 'Ceska Trebova', 15, TO_TIMESTAMP('7:37', 'HH24:MI'), 3, 3);

INSERT INTO Spoj (id, odkud, kam, doba_prepravy_min, cas, prumerne_zpozdeni_min, trasa_id)
VALUES (4, 'Breclav', 'Pardubice hl.n.', 90, TO_TIMESTAMP('13:43', 'HH24:MI'), 10, 1);

INSERT INTO Spoj (id, odkud, kam, doba_prepravy_min, cas, prumerne_zpozdeni_min, trasa_id)
VALUES (5, 'Plzen', 'Cheb', 40, TO_TIMESTAMP('15:06', 'HH24:MI'), 0, 4);

INSERT INTO Spoj (id, odkud, kam, doba_prepravy_min, cas, prumerne_zpozdeni_min, trasa_id)
VALUES (6, 'Praha', 'Brno', 120, TO_TIMESTAMP('6:32', 'HH24:MI'), 7, 1);

INSERT INTO Spoj (id, odkud, kam, doba_prepravy_min, cas, prumerne_zpozdeni_min, trasa_id)
VALUES (7, 'Lanskroun', 'Rudoltice', 15, TO_TIMESTAMP('14:30', 'HH24:MI'), 0, 1);


INSERT INTO Jizdenka (id, priplatek_kc, sleva_prc, celkova_cena_kc, uzivatel_email, spoj_id)
VALUES (1, 20, 25, 67, 'josef.novak@gmail.com', 1);

INSERT INTO Jizdenka (id, priplatek_kc, sleva_prc, celkova_cena_kc, uzivatel_email, spoj_id)
VALUES (2, NULL, 25, 50, 'josef.novak@gmail.com', 3);

INSERT INTO Jizdenka (id, priplatek_kc, sleva_prc, celkova_cena_kc, uzivatel_email, spoj_id)
VALUES (3, NULL, 50, 64, 'petr.maly@gmail.com', 1);

INSERT INTO Jizdenka (id, priplatek_kc, sleva_prc, celkova_cena_kc, uzivatel_email, spoj_id)
VALUES (4, NULL, 50, 23, 'petr.maly@gmail.com', 3);

INSERT INTO Jizdenka (id, priplatek_kc, sleva_prc, celkova_cena_kc, uzivatel_email, spoj_id)
VALUES (5, 30, NULL, 104, 'tomas.studeny@seznam.cz', 3);

INSERT INTO Jizdenka (id, priplatek_kc, sleva_prc, celkova_cena_kc, uzivatel_email, spoj_id)
VALUES (6, NULL, NULL, 135, 'tomas.studeny@seznam.cz', 6);


INSERT INTO Vlak (id, jmeno, typ, priplatek_kc, prumerna_rychlost_km, spolecnost_id)
VALUES (1, 'Rx Pardubicky Porter', 'Rychlik vyssi kvality', 5, 90, 1);

INSERT INTO Vlak (id, jmeno, typ, priplatek_kc, prumerna_rychlost_km, spolecnost_id)
VALUES (2, 'rj Antonin Dvorak', 'RailJet', 10, 100, 1);

INSERT INTO Vlak (id, jmeno, typ, priplatek_kc, prumerna_rychlost_km, spolecnost_id)
VALUES (3, 'IC Johann Strauss', 'InterCity', 20, 85, 1);

INSERT INTO Vlak (id, jmeno, typ, priplatek_kc, prumerna_rychlost_km, spolecnost_id)
VALUES (4, 'Sp 1972', 'Spesny vlak', 0, 75, 2);

INSERT INTO Vlak (id, jmeno, typ, priplatek_kc, prumerna_rychlost_km, spolecnost_id)
VALUES (5, 'Os 12724', 'Osobni vlak', 0, 65, 2);

INSERT INTO Vlak (id, jmeno, typ, priplatek_kc, prumerna_rychlost_km, spolecnost_id)
VALUES (6, 'rj Antonin Conka', 'RailJet', 15, 100, 1);


INSERT INTO Sluzba (id, typ, cena_kc)
VALUES (1, 'Jidelni vuz', 20);

INSERT INTO Sluzba (id, typ, cena_kc)
VALUES (2, 'Spaci vuz', 30);

INSERT INTO Sluzba (id, typ, cena_kc)
VALUES (3, 'Preprava kol', 30);


INSERT INTO vz_Jizdenka_Sluzba (jizdenka_id, sluzba_id)
VALUES (1, 1);

INSERT INTO vz_Jizdenka_Sluzba (jizdenka_id, sluzba_id)
VALUES (5, 3);


INSERT INTO vz_Spoj_Trasa (spoj_id, trasa_id)
VALUES (1, 1);

INSERT INTO vz_Spoj_Trasa (spoj_id, trasa_id)
VALUES (2, 2);

INSERT INTO vz_Spoj_Trasa (spoj_id, trasa_id)
VALUES (3, 3);

INSERT INTO vz_Spoj_Trasa (spoj_id, trasa_id)
VALUES (4, 1);

INSERT INTO vz_Spoj_Trasa (spoj_id, trasa_id)
VALUES (5, 4);

INSERT INTO vz_Spoj_Trasa (spoj_id, trasa_id)
VALUES (6, 1);


INSERT INTO vz_Spoj_Vlak (spoj_id, vlak_id)
VALUES (1, 2);

INSERT INTO vz_Spoj_Vlak (spoj_id, vlak_id)
VALUES (2, 4);

INSERT INTO vz_Spoj_Vlak (spoj_id, vlak_id)
VALUES (2, 5);

INSERT INTO vz_Spoj_Vlak (spoj_id, vlak_id)
VALUES (2, 6);

INSERT INTO vz_Spoj_Vlak (spoj_id, vlak_id)
VALUES (2, 3);

INSERT INTO vz_Spoj_Vlak (spoj_id, vlak_id)
VALUES (2, 1);


INSERT INTO vz_Spoj_Spolecnost (spoj_id, spolecnost_id)
VALUES (1, 1);

INSERT INTO vz_Spoj_Spolecnost (spoj_id, spolecnost_id)
VALUES (2, 1);

INSERT INTO vz_Spoj_Spolecnost (spoj_id, spolecnost_id)
VALUES (3, 2);

INSERT INTO vz_Spoj_Spolecnost (spoj_id, spolecnost_id)
VALUES (4, 1);

INSERT INTO vz_Spoj_Spolecnost (spoj_id, spolecnost_id)
VALUES (5, 2);

INSERT INTO vz_Spoj_Spolecnost (spoj_id, spolecnost_id)
VALUES (6, 1);


INSERT INTO vz_Vlak_Sluzba (vlak_id, sluzba_id)
VALUES (1, 1);

INSERT INTO vz_Vlak_Sluzba (vlak_id, sluzba_id)
VALUES (1, 2);

INSERT INTO vz_Vlak_Sluzba (vlak_id, sluzba_id)
VALUES (2, 3);

/*********************************************************************************************************
 * Vytvoreni triggeru a ukazky jejich pouziti
 */

---------------
-- TRIGGER 1 --
---------------

-- Pomocna sekvence, uchovava posledni id jizdenky
CREATE SEQUENCE sq_last_jizdenka_id;

-- Trigger pro automatickou inkrementaci id jizdenky
CREATE OR REPLACE TRIGGER jizdenka_id_auto_inc
    BEFORE INSERT ON Jizdenka
    FOR EACH ROW
BEGIN
    :new.id := sq_last_jizdenka_id.nextval; -- do id hodnota ze sekvence +1
END jizdenka_id_auto_inc;
/

-- Testovani triggeru jizdenka_id_auto_inc:

-- Je nutne smazat aktualni zaznamy v nasledujicich tabulkach
DELETE FROM vz_Jizdenka_Sluzba; -- referencuje jizdenku
DELETE FROM Jizdenka;

-- Neni nutne uvadet id jizdenky
INSERT INTO Jizdenka (priplatek_kc, sleva_prc, celkova_cena_kc, uzivatel_email, spoj_id)
VALUES (20, 25, 67, 'josef.novak@gmail.com', 1);

INSERT INTO Jizdenka (priplatek_kc, sleva_prc, celkova_cena_kc, uzivatel_email, spoj_id)
VALUES (NULL, 25, 50, 'josef.novak@gmail.com', 3);

INSERT INTO Jizdenka (priplatek_kc, sleva_prc, celkova_cena_kc, uzivatel_email, spoj_id)
VALUES (NULL, 50, 64, 'petr.maly@gmail.com', 1);

INSERT INTO Jizdenka (priplatek_kc, sleva_prc, celkova_cena_kc, uzivatel_email, spoj_id)
VALUES (NULL, 50, 23, 'petr.maly@gmail.com', 3);

INSERT INTO Jizdenka (priplatek_kc, sleva_prc, celkova_cena_kc, uzivatel_email, spoj_id)
VALUES (30, NULL, 104, 'tomas.studeny@seznam.cz', 3);

INSERT INTO Jizdenka (priplatek_kc, sleva_prc, celkova_cena_kc, uzivatel_email, spoj_id)
VALUES (NULL, NULL, 135, 'tomas.studeny@seznam.cz', 6);

-- Vraceni databaze do puvodniho stavu
INSERT INTO vz_Jizdenka_Sluzba (jizdenka_id, sluzba_id)
VALUES (1, 1);

INSERT INTO vz_Jizdenka_Sluzba (jizdenka_id, sluzba_id)
VALUES (5, 3);

---------------
-- TRIGGER 2 --
---------------

-- Trigger pro test spravnosti ICO spolecnosti
CREATE OR REPLACE TRIGGER check_spolecnost_ico
    BEFORE INSERT OR UPDATE OF ico ON spolecnost
    FOR EACH ROW
DECLARE
    m_ico VARCHAR(8);
    control_num NUMBER;
    real_control_num NUMBER;
    m_sum NUMBER;
BEGIN
    m_ico := :NEW.ico;
    IF (LENGTH(m_ico) != 8) THEN
        Raise_Application_Error (-20010, 'ICO nema spravnou delku.');
    END IF;

    IF (LENGTH(TRIM(TRANSLATE(m_ico, 'ABCDEFGHIJKLMNOPQRSTUVWXYZ', ' '))) != null) THEN
        Raise_Application_Error (-20010, 'ICO se nesklada pouze z cisel.');
    END IF;
    real_control_num := TO_NUMBER(SUBSTR(m_ico, 8, 1));
    m_sum := TO_NUMBER(SUBSTR(m_ico, 1, 1)) * 8 + TO_NUMBER(SUBSTR(m_ico, 2, 1)) * 7 + TO_NUMBER(SUBSTR(m_ico, 3, 1)) * 6 + TO_NUMBER(SUBSTR(m_ico, 4, 1)) * 5
            + TO_NUMBER(SUBSTR(m_ico, 5, 1)) * 4 + TO_NUMBER(SUBSTR(m_ico, 6, 1)) * 3 + TO_NUMBER(SUBSTR(m_ico, 7, 1)) * 2;
    control_num := MOD(11 - MOD(m_sum, 11), 10);

    IF (control_num != real_control_num) THEN
        Raise_Application_Error (-20010, 'ICO je neplatne.');
    END IF;
END check_spolecnost_ico;
/

-- Testovani triggeru check_spolecnost_ico:

-- Insert spolecnosti s platnym ICO
INSERT INTO Spolecnost (id, nazev, jmeno_vlastnika, ico)
VALUES (3, 'DustTrains', 'Josef Velky', '25596641');

-- Smazani zaznamu
DELETE FROM Spolecnost WHERE Spolecnost.id = 3;

-- Insert spolecnosti s neplatnym ICO - VYHODI CHYBU
INSERT INTO Spolecnost (id, nazev, jmeno_vlastnika, ico)
VALUES (3, 'DustTrains', 'Josef Velky', '12345678');

-- Smazani zaznamu
DELETE FROM Spolecnost WHERE Spolecnost.id = 3;

/*********************************************************************************************************
 * Vytvoreni ulozenych procedur a ukazky jejich pouziti
 * Pozn: Toto neni optimalni, kontroly spravneho tvaru atributu se daji resit
 * integritnim omezenim. Ale nenapadlo me zadne lepsi vyuziti procedur.
 */

-----------------
-- PROCEDURA 1 --
-----------------

-- Vytvoreni procedury sp_kontrola_email
CREATE OR REPLACE PROCEDURE sp_kontrola_email
AS
    CURSOR cr_uzivatel IS SELECT email FROM Uzivatel;
    ptr_column_email Uzivatel.email%TYPE;
    ex_invalid_email EXCEPTION;
BEGIN
    OPEN cr_uzivatel;
    LOOP
    FETCH cr_uzivatel INTO ptr_column_email;
        EXIT WHEN cr_uzivatel%NOTFOUND;
        IF (ptr_column_email NOT LIKE '%@%.%')
        THEN
            RAISE ex_invalid_email;
        END IF;
    END LOOP;
    CLOSE cr_uzivatel;
EXCEPTION
    WHEN ex_invalid_email THEN
        dbms_output.put_line('Chyba! Neplatna emailova adresa.');
        raise_application_error(-20020, 'Chyba! Neplatna emailova adresa.');
END;
/

-- Testovani procedury sp_kontrola_email:

-- Insert uzivatele s platnou emailovou adresou
INSERT INTO Uzivatel (email, jmeno, prijmeni, telefon, adresa)
VALUES ('tomas.rosicky@gmail.com', 'Tomas', 'Rosicky', '+420999888777', 'London, Emirates Stadium');

-- Zavolani procedury - vse v poradku
EXEC sp_kontrola_email();

-- Smazani zaznamu
DELETE FROM Uzivatel WHERE email = 'tomas.rosicky@gmail.com';

-- Insert uzivatele s neplatnou emailovou adresou
INSERT INTO Uzivatel (email, jmeno, prijmeni, telefon, adresa)
VALUES ('tomas.rosickygmail.com', 'Tomas', 'Rosicky', '+420999888777', 'London, Emirates Stadium');

-- Zavolani procedury - VYHODI CHYBU
EXEC sp_kontrola_email();

-- Smazani zaznamu
DELETE FROM Uzivatel WHERE email = 'tomas.rosicky@gmail.com';

-----------------
-- PROCEDURA 2 --
-----------------

-- Vytvoreni procedury sp_kontrola_vlak_id
CREATE OR REPLACE PROCEDURE sp_kontrola_vlak_id
AS
    CURSOR cr_vlaky IS SELECT id FROM Vlak;
    ptr_column_id Vlak.id%TYPE;
    ex_invalid_id  EXCEPTION;
BEGIN
    OPEN cr_vlaky;
    LOOP
    FETCH cr_vlaky INTO ptr_column_id;
        EXIT WHEN cr_vlaky%NOTFOUND;
        IF ptr_column_id <= 0
        THEN
            RAISE ex_invalid_id;
        END IF;
    END LOOP;
    CLOSE cr_vlaky;
EXCEPTION
    WHEN ex_invalid_id THEN
        dbms_output.put_line('Chyba! Neplatne ID vlaku.');
        raise_application_error(-20030, 'Chyba! Neplatne ID vlaku.');
END;
/

-- Testovani procedury sp_kontrola_vlak_id:

-- Insert vlaku s platnym ID
INSERT INTO Vlak (id, jmeno, typ, priplatek_kc, prumerna_rychlost_km, spolecnost_id)
VALUES (100, 'Rx Pardubicky Porter', 'Rychlik vyssi kvality', 5, 90, 1);

-- Zavolani procedury - vse v poradku
EXEC sp_kontrola_vlak_id();

-- Smazani zaznamu
DELETE FROM Vlak WHERE Vlak.id = 100;

-- Insert vlaku s neplatnym ID
INSERT INTO Vlak (id, jmeno, typ, priplatek_kc, prumerna_rychlost_km, spolecnost_id)
VALUES (-1, 'Rx Pardubicky Porter', 'Rychlik vyssi kvality', 5, 90, 1);

-- Zavolani procedury - VYHODI CHYBU
EXEC sp_kontrola_vlak_id();

-- Smazani zaznamu
DELETE FROM Vlak WHERE Vlak.id = -1;

/*********************************************************************************************************
 * Nekolik dotazu select
 */

-- vypise nazev spolecnosti a jmeno vlaku jenz vlastni
SELECT Spolecnost.nazev, Vlak.jmeno
FROM Spolecnost RIGHT JOIN Vlak ON Vlak.spolecnost_id = Spolecnost.id;

-- vypise jmeno uzivatele, id a celkovou cenu jizdenky zakoupene uzivatelem
SELECT Uzivatel.jmeno, Jizdenka.id AS jizdenka_id, Jizdenka.celkova_cena_kc
FROM Uzivatel RIGHT JOIN Jizdenka ON Uzivatel.email = Jizdenka.uzivatel_email;

-- vypise nazev vlaku a sluzby k nemu nalezici s cenou
SELECT Vlak.jmeno, Sluzba.typ, Sluzba.cena_kc
FROM (( vz_Vlak_Sluzba INNER JOIN Vlak ON Vlak.id = vz_Vlak_Sluzba.vlak_id)
INNER JOIN Sluzba ON Sluzba.id = vz_Vlak_Sluzba.sluzba_id);

-- vypise prumernou cenu typu vlaku
SELECT typ, AVG(priplatek_kc)
FROM Vlak
GROUP BY typ;

-- vypise pocet spoju kde kombinace destinace a vyjezdove misto je unikatni, seradi od nejvetsiho poctu spoju
SELECT odkud, kam, COUNT(id) AS pocet_spoju
FROM Spoj
GROUP BY odkud, kam
ORDER BY pocet_spoju DESC;

-- vypise uzivatele kteri zakoupili jizdenku s cenou 50 CZK
SELECT jmeno, prijmeni
FROM Uzivatel
WHERE EXISTS (
    SELECT * FROM Jizdenka WHERE uzivatel_email = Uzivatel.email AND celkova_cena_kc = 50
);

-- vypise vsechny trasy, ktere zacinaji nejaky spoj. (tzn prvni cast spoje, je tato trasa)
SELECT * FROM Trasa
WHERE odkud IN (
    SELECT odkud FROM Spoj
);

/*********************************************************************************************************
 * Zobrazeni zpracovani dotazu pomoci explain plan a nasledna optimalizace indexem
 */

-- EXPLAIN PLAN pro nasledujici SELECT
EXPLAIN PLAN SET STATEMENT_ID 'my_explained_query1' FOR
SELECT Uzivatel.prijmeni, Uzivatel.jmeno, SUM(Jizdenka.celkova_cena_kc) AS utraceno_kc
FROM Uzivatel NATURAL JOIN Jizdenka WHERE Uzivatel.email = Jizdenka.uzivatel_email
GROUP BY Uzivatel.prijmeni, Uzivatel.jmeno;

-- Vypsani EXPLAIN PLAN
SELECT plan_table_output FROM TABLE (dbms_xplan.display('plan_table', 'my_explained_query1', 'typical'));

-- Optimalizace zpracovani dotazu pomoci indexu
CREATE INDEX idx_uz_email ON Jizdenka(uzivatel_email);

-- EXPLAIN PLAN pro stejny SELECT
EXPLAIN PLAN SET STATEMENT_ID 'my_explained_query2' FOR
SELECT /*+ INDEX(email index_uz_email) */ Uzivatel.prijmeni, Uzivatel.jmeno, SUM(Jizdenka.celkova_cena_kc) AS utraceno_kc
FROM Uzivatel NATURAL JOIN Jizdenka WHERE Uzivatel.email = Jizdenka.uzivatel_email
GROUP BY Uzivatel.prijmeni, Uzivatel.jmeno;

-- Vypsani EXPLAIN PLAN po optimalizaci
-- => Snizeni ceny (mensi vyuziti CPU)
SELECT plan_table_output FROM TABLE (dbms_xplan.display('plan_table', 'my_explained_query2', 'typical'));

/*********************************************************************************************************
 * Ukazka prace s materializovanym pohledem
 */

-- Vytvoreni logu pro Spolecnost
CREATE MATERIALIZED VIEW LOG ON Spolecnost
WITH PRIMARY KEY, ROWID;

-- Vytvoreni logu pro Vlak
CREATE MATERIALIZED VIEW LOG ON Vlak
WITH PRIMARY KEY, ROWID;

-- Vytvoreni materializovaneho pohledu mv_vlak_spolecnost
CREATE MATERIALIZED VIEW mv_vlak_spolecnost
    CACHE
    BUILD IMMEDIATE
    REFRESH FAST ON COMMIT
    AS
        SELECT jmeno AS vlak, nazev AS spolecnost, Vlak.ROWID AS vlak_rowid, Spolecnost.ROWID AS spol_rowid
        FROM Vlak JOIN Spolecnost ON Vlak.spolecnost_id = Spolecnost.id;

-- Vypis vsech polozek v mv_vlak_spolecnost
SELECT * FROM mv_vlak_spolecnost;

-- Pridani noveho vlaku do tabulky Vlak
INSERT INTO Vlak (id, jmeno, typ, priplatek_kc, prumerna_rychlost_km, spolecnost_id)
VALUES (7, 'IC Hell', 'InterCity', NULL, 85, 2);

-- Vypis vsech polozek v mv_vlak_spolecnost - neni aktualni, novy vlak chyby
SELECT * FROM mv_vlak_spolecnost;

COMMIT;

-- Vypis vsech polozek v mv_vlak_spolecnost - po commitu se data aktualizovala
SELECT * FROM mv_vlak_spolecnost;

-- Smazani noveho vlaku
DELETE FROM Vlak WHERE id = 7;

COMMIT;

-- Vypis vsech polozek v mv_vlak_spolecnost - po commitu se data aktualizovala
-- a novy vlak v materializovanem pohledu opet neni
SELECT * FROM mv_vlak_spolecnost;

-- Explain plan pro SELECT dotaz pouzity v materializovanem pohledu
EXPLAIN PLAN SET STATEMENT_ID 'my_explained_query3' FOR
SELECT jmeno AS vlak, nazev AS spolecnost, Vlak.ROWID AS vlak_rowid, Spolecnost.ROWID AS spol_rowid
FROM Vlak JOIN Spolecnost ON Vlak.spolecnost_id = Spolecnost.id;

SELECT plan_table_output FROM TABLE (dbms_xplan.display('plan_table', 'my_explained_query3', 'typical'));

-- Explain plan pro materializovany pohled stejneho SELECT dotazu
EXPLAIN PLAN SET STATEMENT_ID 'my_explained_query4' FOR SELECT * FROM mv_vlak_spolecnost;

SELECT plan_table_output FROM TABLE (dbms_xplan.display('plan_table', 'my_explained_query4', 'typical'));

-- => Snizeni ceny (mensi vyuziti CPU) diky materializovanemu pohledu

/*********************************************************************************************************
 * Prideleni pristupovych prav druhemu clenovi tymu ke vsem databazovym objektum
 * Druhy clen tymu se k databazi prvniho clena tymu pripoji pomoci prikazu:
 * ALTER SESSION SET CURRENT_SCHEMA = xdusek27;
 * Nasledne muze libovolne pracovat s databazovymi objekty, napr:
 * SELECT * FROM mv_vlak_spolecnost;
 */

GRANT ALL ON Uzivatel TO xkukan00;
GRANT ALL ON Jizdenka TO xkukan00;
GRANT ALL ON Spoj TO xkukan00;
GRANT ALL ON Trasa TO xkukan00;
GRANT ALL ON Vlak TO xkukan00;
GRANT ALL ON Sluzba TO xkukan00;
GRANT ALL ON Spolecnost TO xkukan00;
GRANT ALL ON vz_Jizdenka_Sluzba TO xkukan00;
GRANT ALL ON vz_Spoj_Trasa TO xkukan00;
GRANT ALL ON vz_Spoj_Vlak TO xkukan00;
GRANT ALL ON vz_Spoj_Spolecnost TO xkukan00;
GRANT ALL ON vz_Vlak_Sluzba TO xkukan00;
GRANT EXECUTE ON sp_kontrola_email TO xkukan00;
GRANT EXECUTE ON sp_kontrola_vlak_id TO xkukan00;
GRANT ALL ON mv_vlak_spolecnost TO xkukan00;

-- Pozn:
-- Nemam prava pridelit prava vytvoreni cehokoliv druhemu clenovi tymu, viz.
-- GRANT CREATE MATERIALIZED VIEW TO xkukan00;
--
-- Kdyz se druhy clen tymu pokusi vytvorit materializovany pohled hlasi chybu:
-- Error report -
-- ORA-01031: insufficient privileges
-- 01031. 00000 -  "insufficient privileges"
-- *Cause:    An attempt was made to perform a database operation without
--           the necessary privileges.
--
-- Tedy posledni ukol tedy nebylo mozne kompletne splnit.
