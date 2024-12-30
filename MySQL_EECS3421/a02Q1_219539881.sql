create schema if not exists a02;

use a02;

drop table if exists enrollment;
drop table if exists officehour;
drop table if exists class;
drop table if exists course;
drop table if exists student;
drop table if exists professor;

create table professor ( 
   pnum     integer not null, 
   pname    varchar(30) not null, 
   office   char(6) not null, 
   dept     char(2) not null, 
   primary key (pnum));

insert into professor values (1, 'Weddell', 'DC3346', 'CS');
insert into professor values (2, 'Marmalade', 'DC3346', 'CS');
insert into professor values (3, 'Strawman', 'DC3346', 'CS');
create table student ( 
   snum      integer not null, 
   sname     varchar(30) not null, 
   year      integer not null,  
   primary key (snum));

insert into student values (1, 'Mary', 3);
insert into student values (2, 'Fred', 2);

create table course ( 
   cnum      char(5) not null, 
   cname     varchar(50) not null, 
   primary key (cnum));

insert into course values ('CS348', 'Intro to Databases');

create table class ( 
   cnum      char(5) not null, 
   term      char(5) not null, 
   section   integer not null, 
   pnum      integer not null, 
   primary key (cnum,term,section), 
   foreign key (cnum) references course(cnum), 
   foreign key (pnum) references professor(pnum));

insert into class values ('CS348', 'F2018', 1, 1);
insert into class values ('CS348', 'F2018', 2, 1);
insert into class values ('CS348', 'F2023', 1, 1);
insert into class values ('CS348', 'F2023', 2, 1);
insert into class values ('CS348', 'F2024', 1, 2);
insert into class values ('CS348', 'F2019', 1, 3);
create table officehour ( 
   cnum      char(5) not null, 
   term      char(5) not null, 
   pnum      integer not null, 
   day       varchar(10) not null, 
   time      char(5) not null, 
   primary key (cnum,term,pnum,day,time), 
   foreign key (cnum) references course(cnum), 
   foreign key (pnum) references professor(pnum));

insert into officehour values ('CS348', 'F2023', 1, 'Tuesday', '02:00');
insert into officehour values ('CS348', 'F2023', 1, 'Thursday', '02:00');

create table enrollment ( 
   snum      integer not null, 
   cnum      char(5) not null, 
   term      char(5) not null, 
   section   integer not null, 
   grade     integer, 
   primary key (snum,cnum,term,section), 
   foreign key (snum) references student(snum), 
   foreign key (cnum,term,section) references class(cnum,term,section));

insert into enrollment values (1, 'CS348', 'F2018', 1, 91);
insert into enrollment values (2, 'CS348', 'F2023', 1, 55);

SELECT * FROM class;

/* Syed Ali Raza Rizvi - [student_number] */
use a02;

-- Query1 code after this
SELECT DISTINCT professor.pnum, professor.pname FROM professor
JOIN class 
ON professor.pnum = class.pnum

JOIN enrollment 
ON class.cnum = enrollment.cnum AND class.term = enrollment.term
WHERE professor.dept = "CS" AND enrollment.cnum = "CS348" AND enrollment.grade < 60
ORDER BY professor.pnum, professor.pname ASC;

-- Query2 code after this
SELECT professor.pnum, professor.pname FROM professor
WHERE professor.dept = "CS"
AND professor.pnum NOT IN (
    SELECT DISTINCT class.pnum FROM class
    WHERE class.cnum IN ("CS348", "CS234")
)
ORDER BY professor.pnum, professor.pname ASC;

-- Query3 code after this
SELECT student.snum, student.sname, student.year FROM student
WHERE student.year = 4 AND NOT EXISTS(
    SELECT * FROM enrollment 
    JOIN course 
    ON enrollment.cnum = course.cnum
    WHERE enrollment.snum = student.snum AND enrollment.grade < 90 AND course.cnum LIKE "C%" /* AND course number LIKE C% insures only results of the computing dept */
)
ORDER BY student.snum, student.sname, student.year ASC;

-- Query4 code after this
SELECT professor.pnum, professor.pname FROM professor
JOIN class 
ON professor.pnum = class.pnum
JOIN enrollment 
ON class.cnum = enrollment.cnum AND class.section = enrollment.section
WHERE class.cnum = "CS245" AND enrollment.grade = (
    -- lowest grade recorded
    SELECT MIN(enrollmenti.grade)
    FROM enrollment enrollmenti
    WHERE enrollmentI.cnum = "CS245"
)
ORDER BY professor.pnum, professor.pname ASC;

-- Query5 code after this
SELECT ROUND(100.0 * COUNT(DISTINCT professor.pnum) / (SELECT COUNT(*) FROM professor professor), 2) AS percentage
FROM professor 
JOIN class ON class.pnum = class.pnum
GROUP BY professor.pnum
HAVING COUNT(DISTINCT class.cnum, class.term, class.section) >= 2; /* having taught two distinct sections of the same course in one term*/
