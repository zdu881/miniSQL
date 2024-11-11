CREATE DATABASE db_university;
USE DATABASE db_university;
CREATE TABLE students (
    ID INTEGER,
    Name TEXT,
    GPA FLOAT
);
CREATE TABLE courses (
    CourseID INTEGER,
    CourseName TEXT,
    StudentID INTEGER
);

INSERT INTO courses VALUES (1, "Math", 1000);
INSERT INTO courses VALUES (2, "Science", 1003);
INSERT INTO courses VALUES (3, "History", 1000);


INSERT INTO students VALUES ( 1000 , "Jay Chou" , 3.0 );
INSERT INTO students VALUES ( 1002 , "David Tao" , 3.8 );
INSERT INTO students VALUES ( 1003 , "JJ Lin" , 3.9 );
UPDATE students SET GPA = 3.5 WHERE ID = 1000;
DELETE FROM students WHERE ID = 1002;
UPDATE students SET GPA = GPA + 1 WHERE ID = 1003;
SELECT ID , Name , GPA FROM students;
SELECT * FROM students;
SELECT ID , Name , GPA FROM students WHERE GPA > 3.5;
SELECT students.ID, students.Name, students.GPA, courses.CourseName
FROM students
INNER JOIN courses ON students.ID = courses.StudentID;