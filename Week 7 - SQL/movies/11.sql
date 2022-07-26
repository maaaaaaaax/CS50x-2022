-- write a SQL query to list the titles of the five highest rated movies (in order) that Chadwick Boseman starred in,
-- starting with the highest rated

-- select id from people where name = "Chadwick Boseman";

-- select movie_id from stars where person_id in (select id from people where name = "Chadwick Boseman");

-- select movies.title, ratings.rating from movies join ratings on movies.id = ratings.movie_id where movie_id in (
--     select movie_id from stars where person_id in (select id from people where name = "Chadwick Boseman")
-- ) order by ratings.rating desc limit 5;

select movies.title from movies join ratings on movies.id = ratings.movie_id where movie_id in (
    select movie_id from stars where person_id in (select id from people where name = "Chadwick Boseman")
) order by ratings.rating desc limit 5;