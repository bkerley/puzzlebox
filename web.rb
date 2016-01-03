require 'sinatra'
require 'sqlite3'
require 'bcrypt'

enable :sessions
enable :show_exceptions

helpers do
  def db
    return @db if defined? @db
    path = File.expand_path('../data/puzzlebox.sqlite3',
                            __FILE__)
    @db = SQLite3::Database.new(path)
  end
end

get '/' do
  @title = 'welcome to the puzzle box'
  haml :index
end

post '/step1' do
  rows = db.execute <<-SQL
SELECT
  id, username, password_digest
FROM users
WHERE
  username = '#{params[:username]}'
SQL
  if rows.empty?
    @comment = "wrong username"
    return haml :login_fail
  end
  first_found_user = rows.first
  id, username, password_digest = first_found_user
  begin
    password_comparator = BCrypt::Password.new password_digest
  rescue BCrypt::Errors::InvalidHash => e
    @error = e
    @comment = "#{password_digest} doesn't seem to be a bcrypt"
    return haml :login_fail
  end
  password_candidate = params[:password] || ''
  password_ok = password_comparator == password_candidate
  unless password_ok
    @comment = "wrong password"
    return haml :login_fail
  end


  session[:username] = username

  return redirect to '/admin' if username == 'admin'

  redirect to '/muggle'
end

get '/muggle' do
  return redirect to '/admin' if session[:username] == 'admin'
  return redirect to '/' unless session[:username]

  @title = 'basic ass puzzle box muggle'
  haml :muggle
end

get '/admin' do
  return redirect to '/' unless session[:username]
  return redirect to '/muggle' unless session[:username] == 'admin'

  @title = 'you kingshit'
  erb :admin
end

get '/default.css' do
  sass :default
end
