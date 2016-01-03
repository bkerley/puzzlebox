require 'sinatra'

get '/' do
  haml :index
end

get '/default.css' do
  sass :default
end
