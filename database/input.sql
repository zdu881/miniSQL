
CREATE DATABASE db_university;
USE DATABASE db_university;
CREATE TABLE students (
    ID INTEGER,
    Name TEXT,
    GPA FLOAT
);

INSERT INTO students VALUES ( 1000 , "Jay Chou" , 3.0 );
INSERT INTO students VALUES ( 1002 , "David Tao" , 3.8 );
INSERT INTO students VALUES ( 1003 , "JJ Lin" , 3.9 );

SELECT * FROM students WHERE Name = Jay Chou;
SELECT * FROM students WHERE Name = "Jay Chou";
SELECT * FROM students WHERE Name = 'Jay Chou';
SELECT * FROM students WHERE Name = 'Jay Chou' or GPA = 3.0;
SELECT * FROM students WHERE Name = 'Jay Chou' and GPA = 3.0;
SELECT * FROM students WHERE GPA = 3.0;