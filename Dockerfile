FROM ruby:2.3-onbuild
RUN ["bundle", "exec", "ruby", "setup_db.rb"]
CMD ["bundle", "exec", "puma"]
EXPOSE 8080
