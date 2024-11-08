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
DELETE FROM students WHERE ID = 1003;
UPDATE students SET GPA = 3.0 WHERE ID = 1000;

SELECT ID , Name , GPA FROM students;
SELECT * FROM students;
SELECT ID , Name , GPA FROM students WHERE GPA > 2.5;