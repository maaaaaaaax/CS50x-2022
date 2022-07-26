-- write a SQL query to list the titles of all movies in which both Johnny Depp and Helena Bonham Carter starred

select title from movies
where id in (select movie_id from stars where person_id in (select id from people where name = "Helena Bonham Carter"))
and id in (select movie_id from stars where person_id in (select id from people where name = "Johnny Depp"));