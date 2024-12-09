miniSQL

## Architecture

The project adopts a modular design, consisting of the following components:

- **Parser**: Parses user-input SQL commands and converts them into executable operations.
- **Database Management**: Manages multiple databases, including creating, deleting, and selecting databases.
- **Table Management**: Handles the creation, deletion, and CRUD (Create, Read, Update, Delete) operations on tables.
- **Storage Engine**: Responsible for data persistence, saving and loading database and table data to and from files.
- **Executor**: Executes operations based on the parsed commands, such as data insertion, querying, and updating.

## Usage Examples

### Creating a Database and Table

CREATE DATABASE testDB;
USE DATABASE testDB;
CREATE TABLE users (id INTEGER, name TEXT, age INTEGER);

INSERT INTO users VALUES (1, 'Alice', 30);
INSERT INTO users VALUES (2, 'Bob', 25);

SELECT * FROM users;

UPDATE users SET age = 31 WHERE id = 1;
DELETE FROM users WHERE id = 2;
