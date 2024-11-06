CREATE DATABASE db_university;
USE DATABASE db_university;
CREATE TABLE students (
    ID INTEGER,
    Name TEXT,
    GPA FLOAT
);
INSERT INTO student VALUES (1000, ’Jay Chou’, 3.0);
INSERT INTO student VALUES (1001, ’JJ Lin’, 3.5);
INSERT INTO student VALUES (1002, ’David Tao’, 3.8);

SELECT ID, Name, GPA FROM students;
SELECT * FROM students;
SELECT ID, Name FROM students WHERE GPA > 3.5;