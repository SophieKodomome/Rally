CREATE USER rally WITH PASSWORD 'rally';
ALTER USER rally WITH SUPERUSER; 

CREATE DATABASE rally;
\c rally;

CREATE TABLE speciale(
    id SERIAL PRIMARY KEY,
    name VARCHAR(20)
);

CREATE TABLE groupe(
    id SERIAL PRIMARY KEY,
    name VARCHAR(20)
);
CREATE TABLE racers(
    id SERIAL PRIMARY KEY,
    id_group INT,
    name VARCHAR(20),
    time TIMESTAMP,
    FOREIGN KEY (id_group) REFERENCES groupe(id)
);

INSERT INTO speciale(name) VALUES ('S1');
INSERT INTO speciale(name) VALUES ('S2');
INSERT INTO speciale(name) VALUES ('S3');
INSERT INTO speciale(name) VALUES ('S4');
INSERT INTO speciale(name) VALUES ('S5');
INSERT INTO groupe(name) VALUES ('A');
INSERT INTO groupe(name) VALUES ('B');
INSERT INTO groupe(name) VALUES ('C');
INSERT INTO groupe(name) VALUES ('D');
INSERT INTO racers(name,id_group,time) VALUES ('Racer 1',1,NULL);
INSERT INTO racers(name,id_group,time) VALUES ('Racer 2',1,NULL);
INSERT INTO racers(name,id_group,time) VALUES ('Racer 3',1,NULL);
INSERT INTO racers(name,id_group,time) VALUES ('Racer 4',2,NULL);
INSERT INTO racers(name,id_group,time) VALUES ('Racer 5',3,NULL);
INSERT INTO racers(name,id_group,time) VALUES ('Racer 6',3,NULL);
INSERT INTO racers(name,id_group,time) VALUES ('Racer 7',4,NULL);
INSERT INTO racers(name,id_group,time) VALUES ('Racer 8',4,NULL);

