# Library Management System (Command-Line Application)
A C++ CLI app with admin/user login, book management, substring search, issue/return, search history and persistent storage.

## Features
- User authentication (username + password)
- Role-based menus for Admin and User
- Add/update books (ID, Title, Author, Publisher, Year, Status)
- Case-insensitive substring search (ID/Title/Author/Publisher/Status)
- Issue & return books (limit: 5 per user)
- Per-user search history (last 5 queries, session)
- Simple statistics dashboard
- Data persistence via `books.txt`, `users.txt`, `issued.txt`

## Default Admin
On first run the app creates a default admin:
- Username: `admin`
- Password: `admin123`



## File Structure
- `library_system.cpp` — source
- `books.txt` — persisted book records
- `users.txt` — usernames|passwords|roles
- `issued.txt` — issued book entries
- `README.md`
