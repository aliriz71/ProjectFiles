DROP DATABASE IF EXISTS question_one;
CREATE DATABASE question_one;
USE question_one;

CREATE TABLE Instructor 
(
	email VARCHAR(255) NOT NULL PRIMARY KEY,
    affiliation VARCHAR(255),
    name VARCHAR(255) NOT NULL
);


CREATE TABLE Course
(
	course_ID INT AUTO_INCREMENT PRIMARY KEY,
    course_code VARCHAR(255) NOT NULL,
    term VARCHAR(50),
    year INT,
    instructor_email VARCHAR(255),
    FOREIGN KEY (instructor_email) REFERENCES Instructor(email)
);


CREATE TABLE Student
(
	email VARCHAR(255),
    student_ID INT NOT NULL PRIMARY KEY
);

CREATE TABLE Assignment
(
	assignment_ID INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
    course_ID INT NOT NULL,
    due_date DATE NOT NULL, 
    lateness_penalty DECIMAL NOT NULL,
    FOREIGN KEY (course_ID) REFERENCES Course(course_ID)
);
CREATE TABLE Task
(
	task_ID INT AUTO_INCREMENT PRIMARY KEY
);

CREATE TABLE AssignmentTask
(
	assignment_ID INT,
    task_ID INT,
    max_grade DECIMAL,
    PRIMARY KEY (assignment_ID, task_ID),
    FOREIGN KEY (assignment_ID) REFERENCES Assignment(assignment_ID),
    FOREIGN KEY (task_ID) REFERENCES Task(task_ID)
);

CREATE TABLE QuestionPool
(
	instructor_email VARCHAR(255),
    task_ID INT PRIMARY KEY,
    FOREIGN KEY (instructor_email) REFERENCES Instructor(email),
    FOREIGN KEY (task_ID) REFERENCES Task(task_ID)
    
);

CREATE TABLE ENROLLED_IN
(
	student_ID INT, 
    course_ID INT,
    PRIMARY KEY(student_ID, course_ID),
    FOREIGN KEY (student_ID) REFERENCES Student(student_ID),
    FOREIGN KEY (course_ID) REFERENCES Course(course_ID)
);

CREATE TABLE ATTEMPT
(
	student_ID INT,
    task_ID INT, 
    task_time DATE,
    score DECIMAL DEFAULT 0,
    PRIMARY KEY (student_ID, task_ID),
    FOREIGN KEY (student_ID) REFERENCES Student(student_ID),
    FOREIGN KEY (task_ID) REFERENCES Task(task_ID)
);

CREATE TABLE Submission
(
	assignment_ID INT,
    student_ID INT,
    mark DECIMAL DEFAULT 0,
	submission_time DATE, /*latest task_time*/
    PRIMARY KEY (assignment_ID, mark, submission_time),
    FOREIGN KEY (assignment_ID) REFERENCES Assignment(assignment_ID),
    FOREIGN KEY (student_ID) REFERENCES Student(student_ID)
);


