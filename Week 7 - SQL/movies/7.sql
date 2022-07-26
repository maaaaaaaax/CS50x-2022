-- write a SQL query to list all movies released in 2010 and their ratings, in descending order by rating.
-- For movies with the same rating, order them alphabetically by title.

select movies.title, ratings.rating from movies join ratings on movies.id = ratings.movie_id where movies.year = 2010 order by ratings.rating desc, title asc;