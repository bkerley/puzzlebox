require 'sqlite3'
require 'bcrypt'

db = SQLite3::Database.new(
  File.expand_path('../data/puzzlebox.sqlite3',
                   __FILE__))

db.execute <<-SQL
CREATE TABLE IF NOT EXISTS users (
  id INTEGER PRIMARY KEY,
  username VARCHAR UNIQUE ON CONFLICT REPLACE,
  password_digest VARCHAR);
SQL


{
  'admin' => 'x',
  'user' => BCrypt::Password.create('password')
}.each do |pair|
  db.execute(<<-SQL, pair)
    INSERT INTO users (username, password_digest) VALUES
    (?, ?);
SQL
end
