FROM ruby:2.3-onbuild
CMD ["ruby", "setup_db.rb"]
EXPOSE 8080
