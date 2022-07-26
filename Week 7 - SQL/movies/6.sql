-- write a SQL query to determine the average rating of all movies released in 2012
select avg(rating) from ratings where movie_id in (select id from movies where year = 2012);